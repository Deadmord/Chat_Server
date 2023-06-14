#pragma once
#ifndef MESSAGEHISTORY_SERVICE_H
#define MESSAGEHISTORY_SERVICE_H
#include <qobject.h>
#include "SrvRoom.h"


class RoomStorage_Service : public QObject
{
    Q_OBJECT

public:

    static RoomStorage_Service* getInstance();
    static void init();
    QList<QSharedPointer<SrvRoom>> getRooms();
    QSharedPointer<SrvRoom> getRoom(qint32 room_id_);

public slots:


    void createRoom(QSharedPointer<SrvRoom> new_room_);
    void addMessageToRoom(qint32 room_id_, User_Message* message_);
    void addConnecntedUserToRoom(qint32 room_id_, UserConnection* user_);
       
private:

    explicit RoomStorage_Service(QObject* parent_ = nullptr);
    static QSharedPointer<RoomStorage_Service> shp_instance;
    void downloadRoomsFromDB();
    void uploadRoomToDB(QSharedPointer<SrvRoom> new_room_);


    QSet<QSharedPointer<SrvRoom>> rooms_set;
    
    

};

#endif //MESSAGEHISTORY_SERVICE_H
