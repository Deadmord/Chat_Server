#pragma once
#ifndef MESSAGEHISTORY_SERVICE_H
#define MESSAGEHISTORY_SERVICE_H
#include <qobject.h>
#include "SrvRoom.h"
#include "DBRoom.h"


class RoomStorage_Service : public QObject
{
    Q_OBJECT

public:

    [[nodiscard]] bool is_started();
    void make_started();

    [[nodiscard]] static RoomStorage_Service* getInstance();

    static void init();


    [[nodiscard]] QList<QSharedPointer<SrvRoom>> getRooms();
    [[nodiscard]] QSharedPointer<SrvRoom> getRoom(qint32 room_id_);

public slots:
    void createRoom(QSharedPointer<SrvRoom> new_room_);
    void addMessageToRoom(qint32 room_id_, User_Message* message_);
    void addConnecntedUserToRoom(qint32 room_id_, UserConnection* user_);
       
private:

    

    explicit RoomStorage_Service(QObject* parent_ = nullptr);
    static QSharedPointer<RoomStorage_Service> shp_instance;
    void downloadRoomsFromDB();
    void uploadRoomToDB(QSharedPointer<SrvRoom> new_room_);

    QMap<qint32, QSharedPointer<SrvRoom>> rooms_set;
    bool started;
    

};

#endif //MESSAGEHISTORY_SERVICE_H
