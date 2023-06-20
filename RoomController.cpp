﻿#include "RoomController.h"
#include "RoomStorage_Service.h"

RoomController::RoomController(QObject* parent) : QObject(parent)
{
}

RoomController::~RoomController() = default;

QSharedPointer<RoomController> RoomController::instance()
{
    if (!shp_instance)
    {
        shp_instance = QSharedPointer<RoomController>(new RoomController());
    }
    return shp_instance;
}

//void RoomController::initRoom()
//{
//    loadMsgHistory(msg_history_path);
//}

void RoomController::userEntry(const quint32& room_id, QSharedPointer<SrvUser> user_)
{
    RoomStorage_Service::getInstance()->addConnecntedUserToRoom(room_id, user_);
    
    QJsonObject connected_message;
    connected_message[QStringLiteral("type")] = QStringLiteral("newuser");
    connected_message[QStringLiteral("username")] = user_->getUserName();
    broadcastSend(connected_message, room_id, user_);
    user_->setRoomId(room_id);
    
}

void RoomController::userLeave(QSharedPointer<SrvUser> user_)
{
    quint32 room_id = user_->getRoomId();
    if (!room_id)
    {
        PLOGW << "Room leave request wo room number.";
        return;
    }
    if(RoomStorage_Service::getInstance()->deleteConnecntedUserFromRoom(room_id, user_))
    {
	    QJsonObject disconnectedMessage;
	    disconnectedMessage[QStringLiteral("type")] = QStringLiteral("userdisconnected");
	    disconnectedMessage[QStringLiteral("username")] = user_->getUserName();
	    broadcastSend(disconnectedMessage, room_id, nullptr);

        QJsonObject disconnectedMessageforUser;
        disconnectedMessageforUser[QStringLiteral("type")] = QStringLiteral("exitRoom");
        disconnectedMessageforUser[QStringLiteral("success")] = "success";
        sendJson(user_, disconnectedMessageforUser);

	    user_->setRoomId(0);
    }
    else
    {
        PLOGE << "User was not deleted";
    }
    
}

void RoomController::jsonMsgReceived(const quint32& room_id_, QSharedPointer<SrvUser> sender_, const QJsonObject& message_)
{
    QJsonObject userMessage;
    userMessage[QStringLiteral("type")] = QStringLiteral("message");
    userMessage[QStringLiteral("sender")] = sender_->getUserName();
    userMessage[QStringLiteral("messagebody")] = message_;
    broadcastSend(userMessage, room_id_, nullptr);
}

void RoomController::roomListRequest(QSharedPointer<SrvUser> user_)
{
        auto rooms_ptr_list = RoomStorage_Service::getInstance()->getRooms();

        QJsonObject roomList;
        QJsonArray rooms;

        //id name description topic is_private
        foreach(const auto room, rooms_ptr_list)
        {
            QJsonObject room_json;
            room_json[QStringLiteral("id")] = int(room->getId());
            room_json[QStringLiteral("name")] = room->getName();
            room_json[QStringLiteral("description")] = room->getDescription();
            room_json[QStringLiteral("topic")] = room->getTopicName();
            room_json[QStringLiteral("is_private")] = room->isPrivate();
            rooms.append(std::move(room_json));

        }
        roomList[QStringLiteral("type")] = QStringLiteral("roomList");
        roomList[QStringLiteral("rooms")] = rooms;
        sendJson(user_, roomList);
}

void RoomController::messageHystoryRequest(quint32 room_id_, QSharedPointer<SrvUser> user_, QDateTime message_time_, quint32 pool_size_)
{
    auto future = RoomStorage_Service::getInstance()->getMessages(room_id_, message_time_, pool_size_);
    QFutureWatcher<decltype(future.result())> watcher;

    QObject::connect(&watcher, &QFutureWatcher<decltype(future.result())>::finished, [&, user_]() {
        QJsonObject messageList;
        QJsonArray messages;
        auto messages_set = std::move(future.result());
        foreach (auto message, messages_set)
        {
            QJsonObject message_json;
            QJsonObject jsonObject;

    
            
            if (message->getLikes().size() != 0) {

                QMapIterator<QString, bool> it(message->getLikes());
                while (it.hasNext()) {
                    it.next();
                    jsonObject.insert(it.key(), QJsonValue(it.value()));
                }
            }
            message_json[QStringLiteral("id")] = message->getId().toString();
            message_json[QStringLiteral("dateTime")] = message->getDateTime().toString();
            message_json[QStringLiteral("text")] = message->getText();
            message_json[QStringLiteral("parentid")] = message->getParentId();
            message_json[QStringLiteral("nickname")] = message->getNickname();
            message_json[QStringLiteral("mediaid")] = message->getMedia();
            message_json[QStringLiteral("rtl")] = message->isRtl();
            message_json[QStringLiteral("likes")] = jsonObject;
            messages.append(message_json);
        }
        sendJson(user_, messageList);
    });
}

void RoomController::createRoom(QSharedPointer<SrvUser> sender_, const QJsonObject& room_)
{

    auto room = QSharedPointer<SrvRoom>::create(
        room_[QStringLiteral("name")].toString(),
        room_[QStringLiteral("description")].toString(),
        room_[QStringLiteral("topic")].toString(),
        room_[QStringLiteral("is_private")].toBool(),
        room_[QStringLiteral("password")].toString()
    );

    auto future = RoomStorage_Service::getInstance()->createRoom(room);

    QFutureWatcher<decltype(future.result())> watcher;
    connect(&watcher, &QFutureWatcher<decltype(future.result())>::finished, [&, sender_]() {
        QJsonObject res;
        res[QStringLiteral("createChat")] = "success";
        sendJson(sender_, res);
    });

}

void RoomController::broadcastSend(const QJsonObject& message_,const quint32& room_id_, const QSharedPointer<SrvUser>& exclude_)
{
    for (const auto& user : RoomStorage_Service::getInstance()->getRoom(room_id_)->getConnectedUsers()) {
        Q_ASSERT(user);
        if (user == exclude_)
            continue;
        sendJson(user, message_);
    }
}

void RoomController::sendJson(const QSharedPointer<SrvUser>& destination, const QJsonObject& message)
{
    Q_ASSERT(destination);
    destination->sendJson(message);
}



