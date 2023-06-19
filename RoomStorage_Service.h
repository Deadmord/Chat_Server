#pragma once
#ifndef MESSAGEHISTORY_SERVICE_H
#define MESSAGEHISTORY_SERVICE_H
#include <qobject.h>
#include "SrvRoom.h"
#include "DBRoom.h"
#include "DBMessage.h"
#include "RoomRepository.h"
#include "LocalStorage_Service.h"
#include "RatingCounter_Service.h"
#include <QtCore>


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
    QSet<QSharedPointer<User_Message>> getMessages(const quint32& room_id_, const QDateTime& from_, const QDateTime& to_);
    // If from_to_ == false - search for older than time_
    QFuture<QSet<QSharedPointer<User_Message>>> getMessages(const quint32& room_id_, const QDateTime& time_, const quint32& pool_size_ = 20, const bool from_to_ = false);
    QFuture<QSharedPointer<SrvRoom>> createRoom(const QSharedPointer<SrvRoom>& shp_new_room_);

public slots:

    void addMessageToRoom(const qint32& room_id_, const QSharedPointer<User_Message>& shp_user_);
    void addMessagesToRoom(const qint32& room_id_, const QSet<QSharedPointer<User_Message>>& messages_);
    [[nodiscard]] bool addConnecntedUserToRoom(const qint32& room_id_, const QSharedPointer<SrvUser>& shp_user_);
    [[nodiscard]] bool deleteConnecntedUserFromRoom(const qint32& room_id_, const QSharedPointer<SrvUser>& shp_user_);
       
private:

    void addRoom(const QSharedPointer<SrvRoom>& shp_room_);
    explicit RoomStorage_Service(QObject* parent_ = nullptr);
    static inline QSharedPointer<RoomStorage_Service> shp_instance = nullptr;

    

    
    
    void addLikeToMessage(const quint32& room_id_, const QUuid& message_id_, const QDateTime& message_datetime_, const QString& user_login_, const bool like_dislike_);
    void downloadRoomsFromDB();
    void uploadRoomToDB(const QSharedPointer<SrvRoom>& shp_new_room_) const;
    void getMessagesFromDB(const quint32& room_id_, const QDateTime& from_, const QDateTime& to_);
    QSet<QSharedPointer<User_Message>> getMessagesFromDB(const quint32& room_id_, const QDateTime& date_,
                                                         const bool& from_to_, quint32 pool_size_);
    void getMessagesFromLocalStorage(const quint32& room_id_, const QDateTime& from_, const QDateTime& to_);
    QSet<QSharedPointer<User_Message>> getMessagesFromLocalStorage(const quint32& room_id_, const QDateTime& time_, bool from_to_, const quint32& pool_size_);
    QMap<qint32, QSharedPointer<SrvRoom>> rooms_storage;
    bool started = false;
    
signals:

    void messageRetrieved(QList<QSharedPointer<SrvRoom>>);

};

#endif //MESSAGEHISTORY_SERVICE_H
