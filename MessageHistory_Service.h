#pragma once
#ifndef MESSAGEHISTORY_SERVICE_H
#define MESSAGEHISTORY_SERVICE_H
#include <qobject.h>
#include "SrvRoom.h"


class MessageHistory_Service : public QObject
{
    Q_OBJECT

public:

    static MessageHistory_Service* getInstance();
    static void init();
    QList<QSharedPointer<SrvRoom>> getRooms();
    QList <QSharedPointer<SrvRoom>> getRoom(qint32 room_id_);

public slots:

    void createRoom(QSharedPointer<SrvRoom> new_room_);
    void addMessageToRoom(qint32 room_id_, User_Message* message_);
    void addConnecntedUserToRoom(qint32 room_id_, UserConnection* user_);
       
private:

    explicit MessageHistory_Service(QObject* parent_ = nullptr);
    static QSharedPointer<MessageHistory_Service> shp_instance;
    void downloadRoomsFromDB();
    void uploadRoomToDB(QSharedPointer<SrvRoom> new_room_);


    QSet<QSharedPointer<SrvRoom>> rooms_set;
    
    

};

#endif //MESSAGEHISTORY_SERVICE_H
