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
    //проверить что комната с таким номером вообще существует
    //назначить комнату юзеру
    user_->setRoomId(room_id);
    //Отправить юзера в нужную комнату
    //уже в комнате по сигналу вхождения юзера сделать рассылку
    
}

void RoomController::userLeave(QSharedPointer<SrvUser> user_)
{
    quint32 room_id = user_->getRoomId();
    if (!room_id)
    {
        PLOGW << "Room leave request wo room number.";
        return;
    }
    RoomStorage_Service::getInstance()->deleteConnecntedUserFromRoom(room_id, user_);
    //Проверять что успешно удален
    QJsonObject disconnectedMessage;
    disconnectedMessage[QStringLiteral("type")] = QStringLiteral("userdisconnected");
    disconnectedMessage[QStringLiteral("username")] = user_->getUserName();
    broadcastSend(disconnectedMessage, room_id, nullptr);
    //проверить что комната с таким номером вообще существует
    //обнулить номер
    user_->setRoomId(0);
}

void RoomController::jsonMsgReceived(const quint32& room_id_, QSharedPointer<SrvUser> sender_, const QJsonObject& message_)
{
    QJsonObject userMessage;
    userMessage[QStringLiteral("type")] = QStringLiteral("message");
    userMessage[QStringLiteral("sender")] = sender_->getUserName();
    userMessage[QStringLiteral("text")] = message_;
    broadcastSend(userMessage, room_id_, sender_);
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
            room_json[QStringLiteral("id")] = QString::number(room->getId());
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
    QJsonObject messageList;
    QJsonArray messages;
    //Тут запрос истории
    //Тут преобразование в JSON
    sendJson(user_, messageList);
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



