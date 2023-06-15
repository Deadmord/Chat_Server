#include "RoomController.h"

RoomController::RoomController(QObject* parent) : QObject(parent)
{
}

RoomController::~RoomController() = default;

//void RoomController::initRoom()
//{
//    loadMsgHistory(msg_history_path);
//}

void RoomController::userEntry(QSharedPointer<SrvRoom> shp_room_, UserConnection* user)
{
    shp_room_->getConnectedUsers().append(user);
}

//---!!!---это от сюда нужно убрать
void RoomController::jsonReceived(QSharedPointer<SrvRoom> shp_room_, UserConnection* sender, const QJsonObject& docObj)
{
    Q_ASSERT(sender);
    const QJsonValue typeVal = docObj.value(QLatin1String("type"));
    if (typeVal.isNull() || !typeVal.isString())
        return;
    if (typeVal.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) != 0)
        return;
    const QJsonValue textVal = docObj.value(QLatin1String("text"));
    if (textVal.isNull() || !textVal.isString())
        return;
    const QString text = textVal.toString().trimmed();
    if (text.isEmpty())
        return;
    QJsonObject message;
    message[QStringLiteral("type")] = QStringLiteral("message");
    message[QStringLiteral("text")] = text;
    message[QStringLiteral("sender")] = sender->getUserName();
    broadcastSend(shp_room_, message, sender);
}

//---!!!---это от сюда нужно убрать
void RoomController::broadcastSend(QSharedPointer<SrvRoom> shp_room_, const QJsonObject& message, UserConnection* exclude)
{
    for (UserConnection* user : shp_room_->getConnectedUsers()) {
        Q_ASSERT(user);
        if (user == exclude)
            continue;
        sendJson(user, message);
    }
}

//---!!!---это от сюда нужно убрать
void RoomController::sendJson(UserConnection* destination, const QJsonObject& message)
{
    Q_ASSERT(destination);
    destination->sendJson(message);
}



