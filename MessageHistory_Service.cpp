#include "MessageHistory_Service.h"


void MessageHistory_Service::init()
{
}

QList<QSharedPointer<SrvRoom>> MessageHistory_Service::getRooms()
{
    return QList<QSharedPointer<SrvRoom>>();
}

QList<QSharedPointer<SrvRoom>> MessageHistory_Service::getRoom(qint32 room_id_)
{
    return QList<QSharedPointer<SrvRoom>>();
}

void MessageHistory_Service::createRoom(QSharedPointer<SrvRoom> new_room_)
{
}

void MessageHistory_Service::addMessageToRoom(qint32 room_id_, User_Message* message_)
{
}

void MessageHistory_Service::addConnecntedUserToRoom(qint32 room_id_, UserConnection* user_)
{
}

void MessageHistory_Service::downloadRoomsFromDB()
{
}

void MessageHistory_Service::uploadRoomToDB(QSharedPointer<SrvRoom> new_room_)
{
}
