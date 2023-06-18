#include "MessageController.h"
#include "DBUser.h"
#include "MediaSaver_Service.h"
#include "UserRepository.h"


MessageController::MessageController(QObject* object_) : QObject(object_) {}

MessageController::~MessageController() {}

QSharedPointer<MessageController> MessageController::instance()
{
    if (!shp_instance)
    {
        shp_instance = QSharedPointer<MessageController>(new MessageController());
        connect(shp_instance.get(), &MessageController::userEntrySignal, RoomController::instance().get(), &RoomController::userEntry);
        connect(shp_instance.get(), &MessageController::userLeaveSignal, RoomController::instance().get(), &RoomController::userLeave);
        connect(shp_instance.get(), &MessageController::messageToRoom, RoomController::instance().get(), &RoomController::jsonMsgReceived);
        connect(shp_instance.get(), &MessageController::roomListRequestSignal, RoomController::instance().get(), &RoomController::roomListRequest);
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
bool MessageController::toDTOMessageFromJson(DTOModel::DTOMessage& user_masg_dto_, const QJsonObject& user_msg_)
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
    if (date_time.isEmpty())
        return false;

    const QJsonValue nickname_val = user_msg_.value(QLatin1String("nickname"));
    if (nickname_val.isNull() || !nickname_val.isString())
        return false;
    const QString nickname = nickname_val.toString().trimmed();
    if (nickname.isEmpty())
        return false;

    const QJsonValue text_val = user_msg_.value(QLatin1String("text"));
    if (text_val.isNull() || !text_val.isString())
        return false;
    const QString text = text_val.toString().trimmed();
    if (text.isEmpty())
        return false;

    const QJsonValue mediaid_val = user_msg_.value(QLatin1String("mediaid"));
    if (mediaid_val.isNull() || !mediaid_val.isString())
        return false;
    const QString mediaid = mediaid_val.toString().trimmed();

    const QJsonValue rtl_val = user_msg_.value(QLatin1String("rtl"));
    if (rtl_val.isNull() || !mediaid_val.isBool())
        return false;
    const bool rtl = rtl_val.toBool();

    
    const auto likes_val = user_msg_.value(QLatin1String("likes")).toObject().toVariantMap();
    QMap<QString, bool> likes;
    for (auto it = likes_val.constBegin(); it != likes_val.constEnd(); ++it)
    {
        likes.insert(it.key(), it.value().toBool());
    }

    user_masg_dto_ = DTOModel::DTOMessage(QUuid(id), QDateTime::fromString(date_time), nickname,
        text, rtl, mediaid, parentid);
    user_masg_dto_.setLikes(likes);

    return true;
}

//void MessageController::broadcastSend(QSharedPointer<User_Message> spr_srv_msg, const QSharedPointer<SrvRoom> shp_room_, const QSharedPointer<SrvUser> exclude_)
//{
//    for (UserConnection* user : shp_room_->getConnectedUsers()) {
//        Q_ASSERT(user);
//        if (user == exclude_)
//            continue;
//        sendJson(user, message_);
//    }
//}

void MessageController::sendJson(QSharedPointer<SrvUser> destination_, const QJsonObject& message_)
{
    Q_ASSERT(destination_);
    destination_->sendJson(message_);
}

//void MessageController::sendMedia(QSharedPointer<SrvUser> destination_, const QJsonObject& message_)
//{
//    Q_ASSERT(destination_);
//    destination_->sendJson(message_);
//}

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

void MessageController::jsonWMediaReceived(QSharedPointer<SrvUser> sender_, const QJsonObject& doc_, const QByteArray& data_)
{
    Q_ASSERT(sender_);
    PLOGI << QLatin1String("JSON received: ") + QJsonDocument(doc_).toJson(QJsonDocument::Compact);
    if (sender_->getUserName().isEmpty())
        return jsonFromLoggedOut(sender_, doc_);
    if (sender_->getRoomId() == 0)
        return jsonFromLoggedWoRoom(sender_, doc_);
    jsonFromLoggedIn(sender_, doc_, data_);
    //-------------- проверять принадлежность комнате ------------------
    // -----------------если принадлежит то отправлять сообщение в конкретную комнату---------------
    //вместо этого

}

void MessageController::jsonMediaRequest(QSharedPointer<SrvUser> sender_, const QJsonObject& doc_)
{
    /*Q_ASSERT(sender_);
    const auto media_id = QUuid::fromString(doc_.value(QLatin1String("mediaid")).toString());
    auto future = MediaSaver_Service::getInstance()->getMedia(sender_->getUserName(), media_id, sender_->getRoomId());
    QFutureWatcher<decltype(future.result())> watcher;
    connect(&watcher, &QFutureWatcher<decltype(future.result())>::finished, [&]() {
        sendJson(sender_, messageList);
        });*/
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

    //QFuture<QSharedPointer<DBEntity::DBUser>> future_user_info = QtConcurrent::run(&DBService::UserRepository::getUserByLogin, new_user_name); //ask to db for user info
    QFuture<QSharedPointer<DBEntity::DBUser>> future_user_info = DBService::UserRepository::getUserByLogin(new_user_name); //ask to DB for user info

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

    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(password_str.toUtf8());

    future_user_info.waitForFinished(); //wait for DB responce
    QSharedPointer<DBEntity::DBUser> user_info = future_user_info.result();

    //check login and password
    if (new_user_name != user_info->getLogin() || QString(hash.result().toHex()) != user_info->getPassword())
    //if (hash.result().toHex().compare(QByteArray::(user_info->getPassword())) != 0)
    //if(false)
    {
        PLOGI << "wrong loggin or password" + new_user_name;
        QJsonObject message;
        message[QStringLiteral("type")] = QStringLiteral("login");
        message[QStringLiteral("success")] = false;
        message[QStringLiteral("reason")] = QStringLiteral("wrong loggin or password");
        sendJson(sender_, message);
        return;
    }

    //{   // Query to DB
    //    QSet<QString> username = { "User01","User02","User03" };
    //    QSet<QString> password = { "Pass01","Pass02","Pass03" };

    //    if (!username.contains(new_user_name)|| !password.contains(password_str)) {
    //        PLOGI << "wrong loggin or password" + new_user_name;
    //        QJsonObject message;
    //        message[QStringLiteral("type")] = QStringLiteral("login");
    //        message[QStringLiteral("success")] = false;
    //        message[QStringLiteral("reason")] = QStringLiteral("wrong loggin or password");
    //        sendJson(sender_, message);
    //        return;
    //    }
    //}

    //sinchronise with DB user
    sender_->setUserName(user_info->getLogin());
    sender_->setPassword(user_info->getPassword());
    sender_->setUserpic(user_info->getUserpic());
    sender_->setRating(user_info->getRating());

    QByteArray userpicData = user_info->getUserpic();
    QString base64Userpic = QString::fromLatin1(userpicData.toBase64());

    //send loggin success + user info
    QJsonObject userinfo;
    userinfo[QStringLiteral("username")] = sender_->getUserName();
    userinfo[QStringLiteral("userpic")] = base64Userpic;
    userinfo[QStringLiteral("rating")] = double(sender_->getRating());
    QJsonObject success_message;
    success_message[QStringLiteral("type")] = QStringLiteral("login");
    success_message[QStringLiteral("success")] = true;
    success_message[QStringLiteral("userinfo")] = userinfo;    //Send DTO User
    sendJson(sender_, success_message);
    return;
}

void MessageController::jsonFromLoggedIn(QSharedPointer<SrvUser> sender_, const QJsonObject& doc_obj_)
{
    Q_ASSERT(sender_);
    const QJsonValue type_val = doc_obj_.value(QLatin1String("type"));
    if (type_val.isNull() || !type_val.isString())
        return;
    if (type_val.toString().compare(QLatin1String("roomLeave"), Qt::CaseInsensitive) == 0)
    {
        emit userLeaveSignal(sender_);
    }
    if (type_val.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) == 0)
    {
        if (sender_->isFloodLimit())                 //implementation of flood protection mechanism
        {
            PLOGD << "flood protection, wait...";    //notify the server of invalid data
            return;
        }
        sender_->setFloodLimit();

        const QJsonObject messagebody_val = doc_obj_.value(QLatin1String("messagebody")).toObject();
        //if (messagebody_val.isEmpty())
           // return;

        DTOModel::DTOMessage tempDTO;
        if (!toDTOMessageFromJson(tempDTO, messagebody_val))
            return;
        QSharedPointer<User_Message> spr_srv_msg = DTOModel::DTOMessage::createSrvFromDTO(QSharedPointer<DTOModel::DTOMessage>(&tempDTO));
        RoomStorage_Service::getInstance()->addMessageToRoom(sender_->getRoomId(), spr_srv_msg);    //archive message

        emit messageToRoom(sender_->getRoomId(), sender_, messagebody_val);     //Send message to all members in room
        return;
    }
}

void MessageController::jsonFromLoggedIn(QSharedPointer<SrvUser> sender_, const QJsonObject& doc_obj_, const QByteArray& data_)
{
    Q_ASSERT(sender_);
    const QJsonValue type_val = doc_obj_.value(QLatin1String("type"));
    if (type_val.isNull() || !type_val.isString())
        return;
    if (type_val.toString().compare(QLatin1String("roomLeave"), Qt::CaseInsensitive) == 0)
    {
        emit userLeaveSignal(sender_);
    }
    if (type_val.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) == 0)
    {
        if (sender_->isFloodLimit())                 //implementation of flood protection mechanism
        {
            PLOGD << "flood protection, wait...";    //notify the server of invalid data
            return;
        }
        sender_->setFloodLimit();
        QUuid media_id = QUuid::createUuid();
        QJsonObject messagebody_val = doc_obj_.value(QLatin1String("messagebody")).toObject();
        messagebody_val["mediaid"] = media_id.toString();
        //if (messagebody_val.isEmpty())
           // return;

        DTOModel::DTOMessage tempDTO;
        if (!toDTOMessageFromJson(tempDTO, messagebody_val))
            return;
        MediaSaver_Service::getInstance()->saveMedia(sender_->getRoomId(), sender_->getUserName(), media_id, data_);
        QSharedPointer<User_Message> spr_srv_msg = DTOModel::DTOMessage::createSrvFromDTO(QSharedPointer<DTOModel::DTOMessage>(&tempDTO));
        RoomStorage_Service::getInstance()->addMessageToRoom(sender_->getRoomId(), spr_srv_msg);    //archive message

        emit messageToRoom(sender_->getRoomId(), sender_, messagebody_val);     //Send message to all members in room
        return;
    }
}

void MessageController::jsonFromLoggedWoRoom(QSharedPointer<SrvUser> sender_, const QJsonObject& doc_obj_)
{
    Q_ASSERT(sender_);
    const QJsonValue type_val = doc_obj_.value(QLatin1String("type"));
    if (type_val.isNull() || !type_val.isString())
        return;
    if (type_val.toString().compare(QLatin1String("roomListRequest"), Qt::CaseInsensitive) == 0)
    {
        emit roomListRequestSignal(sender_);
    }
    if (type_val.toString().compare(QLatin1String("roomEntry"), Qt::CaseInsensitive) == 0)
    {
        const QJsonValue room_val = doc_obj_.value(QLatin1String("room"));
        if (room_val.isNull() || !room_val.isDouble())
            return;
        const quint32 room_id = room_val.toInt();
        if (!room_id)
            return;
        emit userEntrySignal(room_id, sender_);
    }
}

