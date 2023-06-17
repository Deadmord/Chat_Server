#include "RoomController.h"
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
    
}

void RoomController::userLeave(const quint32& room_id, QSharedPointer<SrvUser> user_)
{
    RoomStorage_Service::getInstance()->deleteConnecntedUserFromRoom(room_id, user_);
    //Проверять что успешно удален
    QJsonObject disconnectedMessage;
    disconnectedMessage[QStringLiteral("type")] = QStringLiteral("userdisconnected");
    disconnectedMessage[QStringLiteral("username")] = user_->getUserName();
    broadcastSend(disconnectedMessage, room_id, nullptr);



}

void RoomController::jsonMsgReceived(const quint32& room_id_, QSharedPointer<SrvUser> sender_, const QJsonObject& message_)
{
    QJsonObject userMessage;
    userMessage[QStringLiteral("type")] = QStringLiteral("message");
    userMessage[QStringLiteral("sender")] = sender_->getUserName();
    userMessage[QStringLiteral("text")] = message_;
    broadcastSend(userMessage, room_id_, sender_);
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



