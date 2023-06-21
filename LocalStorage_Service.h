#pragma once
#ifndef LOCALSTORAGE_SERVICE_H
#define LOCALSTORAGE_SERVICE_H

#include <QObject>
#include <QMutex>
#include <QMap>
#include <QDir>
#include <QtCore>
#include <qthread.h>
#include <QtConcurrent>
#include <algorithm>

#include <plog/Log.h>
#include <QTimer>

#include "DBMessage.h"
#include "Message.h"
#include "DTOMessage.h"

#include "DTOMessage.h"

class LocalStorage_Service:
    public QObject
{
    Q_OBJECT

    static QSharedPointer<LocalStorage_Service> shp_instance;
    static QMutex mutex;
    explicit LocalStorage_Service(QObject* object_ = nullptr);
    QMap<quint32, QSet<QSharedPointer<DBEntity::DBMessage>>> message_storage;

public:
    [[nodiscard]] QString searchForFiles(const QDateTime& date_, const quint32& room_) const;
    QString searchForFiles(const QDateTime& date_, const quint32& room_, const bool& from_to_) const;
    bool addLikeToMessage(const quint32& room_id_, const QUuid& message_id_, const QDateTime& message_datetime_, const QString& user_login_, const bool like_dislike_);
    static QSet<QSharedPointer<DBEntity::DBMessage>> readMessagesFromDB(const QString& file_name_) ;
    QList<QString> searchForFiles(const QDateTime& from_, const QDateTime& to_, const quint32& room_) const;
    static QSharedPointer<LocalStorage_Service> getInstance(int minutes_ = 0);
    LocalStorage_Service& operator=(const LocalStorage_Service&) = delete;
    LocalStorage_Service(const LocalStorage_Service&) = delete;

signals:
    void messageRetrieved(QList<QSharedPointer<DBEntity::DBMessage>> messages_);
    void close();

public slots:
   
    void addMessages(const QSet<QSharedPointer<User_Message>>& messages_, const quint32 room_id_);
    void addMessage(const QSharedPointer<User_Message>& shp_message_, quint32 room_id_);
    QSet<QSharedPointer<User_Message>> getMessages(const QDateTime& from_, const QDateTime& to_, const quint32& room_id_);
    QSet<QSharedPointer<User_Message>> getMessages(const quint32& room_id_, const QDateTime& time_, bool from_to_, const quint32& pool_size_);
    void saveAllMessages();

    void safeExit() ;
};


#endif //LOCALSTORAGE_SERVICE_H
