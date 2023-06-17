#pragma once
#ifndef MESSAGEHISTORY_SERVICE_H
#define MESSAGEHISTORY_SERVICE_H
#include <qobject.h>
#include "SrvRoom.h"
#include "DBRoom.h"
#include "DBMessage.h"
#include "RoomRepository.h"
#include "LocalStorage_Service.h"


class RoomStorage_Service : public QObject
{
    Q_OBJECT

public:

    [[nodiscard]] bool is_started();
    void make_started();

    [[nodiscard]] static RoomStorage_Service* getInstance();

    static void init();


    [[nodiscard]] QList<QSharedPointer<SrvRoom>> getRooms();
    [[nodiscard]] QSharedPointer<SrvRoom> getRoom(const qint32& room_id_);

public slots:

    void createRoom(const QSharedPointer<SrvRoom>& shp_new_room_);
    void addMessageToRoom(const qint32& room_id_, const QSharedPointer<User_Message>& shp_user_);
    void addMessagesToRoom(const qint32& room_id_, const QSet<QSharedPointer<User_Message>>& messages_);
    [[nodiscard]] bool addConnecntedUserToRoom(const qint32& room_id_, const QSharedPointer<SrvUser>& shp_user_);
    [[nodiscard]] bool deleteConnecntedUserFromRoom(const qint32& room_id_, const QSharedPointer<SrvUser>& shp_user_);
       
private:

    explicit RoomStorage_Service(QObject* parent_ = nullptr);
    static inline QSharedPointer<RoomStorage_Service> shp_instance = nullptr;

    static QSharedPointer<DBEntity::DBMessage> fromUserMessageToDBMessage(const QSharedPointer<User_Message>& shp_user_message_);
    static QSharedPointer<User_Message> fromDBMessageToUserMessage(const QSharedPointer<DBEntity::DBMessage>& shp_user_message_);

    QList<QSharedPointer<User_Message>> getMessages(const quint32& room_id_, const QDateTime& from_, const QDateTime& to_);

    QList<QString> searchForFiles(const QDateTime& from_, const QDateTime& to_, const quint32& room_);
    
    void addLikeToMessage(const quint32& room_id_, const QUuid& message_id_, const QString& user_login_, const bool like_dislike_);
    void downloadRoomsFromDB();
    void uploadRoomToDB(const QSharedPointer<SrvRoom>& shp_new_room_) const;
    void getMessagesFromDB(const quint32& room_id_, const QDateTime& from_, const QDateTime& to_);
    void getMessagesFromLocalStorage(const quint32& room_id_, const QDateTime& from_, const QDateTime& to_);

    QMap<qint32, QSharedPointer<SrvRoom>> rooms_storage;
    bool started = false;
    
signals:

    void messageRetrieved(QList<QSharedPointer<SrvRoom>>);

};

#endif //MESSAGEHISTORY_SERVICE_H
