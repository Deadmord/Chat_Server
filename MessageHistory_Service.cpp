#include "MessageHistory_Service.h"


void RoomStorage_Service::init()
{
}

QList<QSharedPointer<SrvRoom>> RoomStorage_Service::getRooms()
{
    return QList<QSharedPointer<SrvRoom>>();
}

QSharedPointer<SrvRoom> RoomStorage_Service::getRoom(qint32 room_id_)
{
    return QSharedPointer<SrvRoom>();
}

void RoomStorage_Service::createRoom(QSharedPointer<SrvRoom> new_room_)
{
}

void RoomStorage_Service::addMessageToRoom(qint32 room_id_, User_Message* message_)
{
}

void RoomStorage_Service::addConnecntedUserToRoom(qint32 room_id_, UserConnection* user_)
{
}

void RoomStorage_Service::downloadRoomsFromDB()
{
}

void RoomStorage_Service::uploadRoomToDB(QSharedPointer<SrvRoom> new_room_)
{
}
