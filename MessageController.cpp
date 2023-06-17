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

//User_Message MessageController::createMessage(const QString& nickname_, const QString& text_)
//{
//    Message msg;
//    msg.nickname = nickname_;
//    msg.text = text_;
//    msg.room_id = 0;
//    msg.deleted = false;
//    return User_Message(msg);
//    //return User_Message(QUuid::createUuid().toString(), 0, QDateTime::currentDateTime(), _nickname, _text);
//}
bool MessageController::createDTOMessage(const QJsonObject& user_msg_)
{
    const QJsonValue id_val = user_msg_.value(QLatin1String("id"));
    if (id_val.isNull() || !id_val.isString())
        return false;
    const QString id = id_val.toString().trimmed();
    if (id.isEmpty())
        return false;

    const QJsonValue parentid_val = user_msg_.value(QLatin1String("parentid"));
    if (parentid_val.isNull() || !parentid_val.isString())
        return false;
    const QString parentid = parentid_val.toString().trimmed();

    const QJsonValue date_time_val = user_msg_.value(QLatin1String("datetime"));
    if (date_time_val.isNull() || !date_time_val.isString())
        return false;
    const QString date_time = date_time_val.toString().trimmed();
    if (id.isEmpty())
        return false;

    const QJsonValue nickname_val = user_msg_.value(QLatin1String("nickname"));
    if (nickname_val.isNull() || !nickname_val.isString())
        return false;
    const QString nickname = nickname_val.toString().trimmed();
    if (id.isEmpty())
        return false;

    const QJsonValue text_val = user_msg_.value(QLatin1String("text"));
    if (text_val.isNull() || !text_val.isString())
        return false;
    const QString text = text_val.toString().trimmed();
    if (text.isEmpty())
        return false;


    QString _id;
    //QUuid _id;
    QDateTime _date_time;
    QString _nickname;
    QString _text;
    QString _media_id;
    QString _parent_id;
    //QUuid _parent_id;
    bool _deleted{ false };
    QMap<QUuid, bool> _likes;

    //Message msg;
    //msg.nickname = nickname_;
    //msg.text = text_;
    //msg.room_id = 0;
    //msg.deleted = false;
    //return User_Message(msg);
    //return User_Message(QUuid::createUuid().toString(), 0, QDateTime::currentDateTime(), _nickname, _text);
}

void MessageController::broadcastSend(const QJsonObject& message_, const QSharedPointer<SrvRoom> shp_room_, const QSharedPointer<SrvUser> exclude_)
{
    /*for (UserConnection* user : shp_room_->getConnectedUsers()) {
        Q_ASSERT(user);
        if (user == exclude_)
            continue;
        sendJson(user, message_);
    }*/
}

void MessageController::sendJson(QSharedPointer<SrvUser> destination_, const QJsonObject& message_)
{
    Q_ASSERT(destination_);
    destination_->sendJson(message_);
}

void MessageController::jsonReceived(QSharedPointer<SrvUser> sender_, const QJsonObject& doc_)
{
    Q_ASSERT(sender_);
    PLOGI << QLatin1String("JSON received: ") + QJsonDocument(doc_).toJson(QJsonDocument::Compact);
    if (sender_->getUserName().isEmpty())
        return jsonFromLoggedOut(sender_, doc_);
    if (sender_->getRoomId() == 0)
        return jsonFromLoggedWoRoom(sender_, doc_);
    jsonFromLoggedIn(sender_, doc_);
    //-------------- проверять принадлежность комнате ------------------
    // -----------------если принадлежит то отправлять сообщение в конкретную комнату---------------
    //вместо этого

}

void MessageController::jsonFromLoggedOut(QSharedPointer<SrvUser> sender_, const QJsonObject& doc_obj_)
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

    const QJsonValue password_val = doc_obj_.value(QLatin1String("password"));
    if (password_val.isNull() || !password_val.isString())
        return;
    const QString password_str = password_val.toString().simplified();
    if (password_str.isEmpty())
        return;

    for (const QSharedPointer<SrvUser> user : UserController::instance()->getUsersList()) {     //Find duplicat username //qAsConst(server.getUsersList()))
        if (user == sender_)                             
            continue;
        if (user->getUserName().compare(new_user_name, Qt::CaseInsensitive) == 0) {
            PLOGI << "duplicate username or allrady loggin" + new_user_name;
            QJsonObject message;
            message[QStringLiteral("type")] = QStringLiteral("login");
            message[QStringLiteral("success")] = false;
            message[QStringLiteral("reason")] = QStringLiteral("duplicate username or allrady loggin");
            sendJson(sender_, message);
            return;
        }
    }
    //QConcurent::run()

    {   // Query to DB
        QSet<QString> username = { "User01","User02","User03" };
        QSet<QString> password = { "Pass01","Pass02","Pass03" };

        if (!username.contains(new_user_name)|| !password.contains(password_str)) {
            PLOGI << "wrong loggin or password" + new_user_name;
            QJsonObject message;
            message[QStringLiteral("type")] = QStringLiteral("login");
            message[QStringLiteral("success")] = false;
            message[QStringLiteral("reason")] = QStringLiteral("wrong loggin or password");
            sendJson(sender_, message);
            return;
        }
    }
    //User-repository. get user by ID
    sender_->setUserName(new_user_name);
    {   // Upload user from DB, (check for name equals db?, socket steal the same)  
        //sender_->setUserPicId("011");
        //sender_->setRatingLikes(100);
    }

    QJsonObject success_message;
    success_message[QStringLiteral("type")] = QStringLiteral("login");
    success_message[QStringLiteral("success")] = true;
    success_message[QStringLiteral("userinfo")] = QJsonObject();    //Send DTO User
    sendJson(sender_, success_message);

    QJsonObject connected_message;
    connected_message[QStringLiteral("type")] = QStringLiteral("newuser");
    connected_message[QStringLiteral("username")] = new_user_name;

    broadcastSend(connected_message, RoomStorage_Service::getInstance()->getRoom(sender_->getRoomId()), sender_);
}

void MessageController::jsonFromLoggedIn(QSharedPointer<SrvUser> sender_, const QJsonObject& doc_obj_)
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
        if (sender_->isFloodLimit())                 //implementation of flood protection mechanism
        {
            PLOGD << "flood protection, wait...";    //notify the server of invalid data
            return;
        }
        sender_->setFloodLimit();

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
        return;
    }


}

void MessageController::jsonFromLoggedWoRoom(QSharedPointer<SrvUser> sender_, const QJsonObject& doc_obj_)
{

}

//void MessageController::jsonFromLoggedInMsg(const QSharedPointer<SrvUser> sender_, const QJsonObject& doc_obj_)
//{
//    Q_ASSERT(sender_);
//    const QJsonValue type_val = doc_obj_.value(QLatin1String("type"));
//    if (type_val.isNull() || !type_val.isString())
//        return;
//    if (type_val.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) != 0)
//        return;
//    const QJsonValue text_val = doc_obj_.value(QLatin1String("text"));
//    if (text_val.isNull() || !text_val.isString())
//        return;
//    const QString text = text_val.toString().trimmed();
//    if (text.isEmpty())
//        return;
//    QJsonObject message;
//    message[QStringLiteral("type")] = QStringLiteral("message");
//    message[QStringLiteral("text")] = text;
//    message[QStringLiteral("sender")] = sender_->getUserName();
//    broadcastSend(message, RoomStorage_Service::getInstance()->getRoom(sender_->getRoomId()), sender_);
//}
