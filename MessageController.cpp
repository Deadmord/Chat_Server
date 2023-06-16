#include "MessageController.h"

MessageController::MessageController(QObject* object_) : QObject(object_) {}

MessageController::~MessageController() {}

QSharedPointer<MessageController> MessageController::instance()
{
    if (!shp_instance)
    {
        shp_instance = QSharedPointer<MessageController>(new MessageController());
    }
    return shp_instance;
}

//MessageController* MessageController::instance(QObject* parent)
//{
//    static MessageController inst(parent);
//    return &inst;
//}

User_Message MessageController::createMessage(const QString& nickname_, const QString& text_)
{
    Message msg;
    msg.nickname = nickname_;
    msg.text = text_;
    msg.room_id = 0;
    msg.deleted = false;
    return User_Message(msg);
    //return User_Message(QUuid::createUuid().toString(), 0, QDateTime::currentDateTime(), _nickname, _text);
}

void MessageController::broadcastSend(const QJsonObject& message_, const QSharedPointer<SrvRoom> shp_room_, const UserConnection* exclude_)
{
    for (UserConnection* user : shp_room_->getConnectedUsers()) {
        Q_ASSERT(user);
        if (user == exclude_)
            continue;
        sendJson(user, message_);
    }
}

void MessageController::sendJson(UserConnection* destination_, const QJsonObject& message_)
{
    Q_ASSERT(destination_);
    destination_->sendJson(message_);
}

void MessageController::jsonReceived(UserConnection* sender_, const QJsonObject& doc_)
{
    Q_ASSERT(sender_);
    PLOGI << QLatin1String("JSON received: ") + QJsonDocument(doc_).toJson(QJsonDocument::Compact);
    if (sender_->getUserName().isEmpty())
        return jsonFromLoggedOut(sender_, doc_);
    //-------------- проверять принадлежность комнате ------------------
    // -----------------если принадлежит то отправлять сообщение в конкретную комнату---------------
    //вместо этого
    jsonFromLoggedInCmd(sender_, doc_);
}

void MessageController::jsonFromLoggedOut(UserConnection* sender_, const QJsonObject& doc_obj_)
{
    Q_ASSERT(sender_);
    const QJsonValue type_val = doc_obj_.value(QLatin1String("type"));
    if (type_val.isNull() || !type_val.isString())
        return;
    if (type_val.toString().compare(QLatin1String("login"), Qt::CaseInsensitive) != 0)
        return;
    const QJsonValue username_val = doc_obj_.value(QLatin1String("username"));
    if (username_val.isNull() || !username_val.isString())
        return;
    const QString new_user_name = username_val.toString().simplified();
    if (new_user_name.isEmpty())
        return;
    for (const UserConnection* user : QList<UserConnection*>{}) {    //Find duplicat username //qAsConst(server.getUsersList()))
        if (user == sender_)
            continue;
        if (user->getUserName().compare(new_user_name, Qt::CaseInsensitive) == 0) {
            QJsonObject message;
            message[QStringLiteral("type")] = QStringLiteral("login");
            message[QStringLiteral("success")] = false;
            message[QStringLiteral("reason")] = QStringLiteral("duplicate username");
            sendJson(sender_, message);
            return;
        }
    }
    sender_->setUserName(new_user_name);
    QJsonObject success_message;
    success_message[QStringLiteral("type")] = QStringLiteral("login");
    success_message[QStringLiteral("success")] = true;
    sendJson(sender_, success_message);
    QJsonObject connected_message;
    connected_message[QStringLiteral("type")] = QStringLiteral("newuser");
    connected_message[QStringLiteral("username")] = new_user_name;
    broadcastSend(connected_message, RoomStorage_Service::getInstance()->getRoom(sender_->getRoomId()), sender_);
}

void MessageController::jsonFromLoggedInCmd(UserConnection* sender_, const QJsonObject& doc_obj_)
{
    Q_ASSERT(sender_);
    const QJsonValue type_val = doc_obj_.value(QLatin1String("type"));
    if (type_val.isNull() || !type_val.isString())
        return;
    if (type_val.toString().compare(QLatin1String("roomListRequest"), Qt::CaseInsensitive) == 0)
    {

    }
    if (type_val.toString().compare(QLatin1String("roomEntry"), Qt::CaseInsensitive) == 0)
    {
        const QJsonValue room_val = doc_obj_.value(QLatin1String("room"));
        if (room_val.isNull() || !room_val.isDouble())
            return;
        const quint32 room_id = room_val.toInt();
        if (!room_id)
            return;
        //проверить что комната с таким номером вообще существует
        sender_->setRoomId(room_id);
        //Отправить юзера в нужную комнату
        //уже в комнате по сигналу вхождения юзера сделать рассылку
    }
    if (type_val.toString().compare(QLatin1String("roomLeave"), Qt::CaseInsensitive) == 0)
    {

    }
    if (type_val.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) == 0)
    {

        

        const QJsonValue text_val = doc_obj_.value(QLatin1String("text"));
        if (text_val.isNull() || !text_val.isString())
            return;
        const QString text = text_val.toString().trimmed();
        if (sender_->isFloodLimit())                                //implementation of flood protection mechanism
        {
            PLOGD << "flood protection, wait...";    //notify the server of invalid data
            return;
        }
        sender_->setFloodLimit();
        if (text.isEmpty())
            return;
        QJsonObject message;
        message[QStringLiteral("type")] = QStringLiteral("message");
        message[QStringLiteral("text")] = text;
        message[QStringLiteral("sender")] = sender_->getUserName();
        broadcastSend(message, RoomStorage_Service::getInstance()->getRoom(sender_->getRoomId()), sender_);
        return;
    }


}

void MessageController::jsonFromLoggedInMsg(const UserConnection* sender_, const QJsonObject& doc_obj_)
{
    Q_ASSERT(sender_);
    const QJsonValue type_val = doc_obj_.value(QLatin1String("type"));
    if (type_val.isNull() || !type_val.isString())
        return;
    if (type_val.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) != 0)
        return;
    const QJsonValue text_val = doc_obj_.value(QLatin1String("text"));
    if (text_val.isNull() || !text_val.isString())
        return;
    const QString text = text_val.toString().trimmed();
    if (text.isEmpty())
        return;
    QJsonObject message;
    message[QStringLiteral("type")] = QStringLiteral("message");
    message[QStringLiteral("text")] = text;
    message[QStringLiteral("sender")] = sender_->getUserName();
    broadcastSend(message, RoomStorage_Service::getInstance()->getRoom(sender_->getRoomId()), sender_);
}
