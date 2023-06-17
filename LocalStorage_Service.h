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

class LocalStorage_Service:
    public QObject
{
    Q_OBJECT
private:
    static QSharedPointer<LocalStorage_Service> shp_instance;
    static QMutex mutex;
    explicit LocalStorage_Service(QObject* object_ = nullptr);
    QMap<quint32, QList<QSharedPointer<DBEntity::DBMessage>>> message_storage;
    

public:
    bool addLikeToMessage(const quint32& room_id_, const QUuid& message_id_, const QDateTime& message_datetime_, const QString& user_login_, const bool like_dislike_);
    static QSharedPointer<LocalStorage_Service> getInstance(int minutes_ = 0);
    LocalStorage_Service& operator=(const LocalStorage_Service&) = delete;
    LocalStorage_Service(const LocalStorage_Service&) = delete;

signals:
    void messageRetrieved(QList<QSharedPointer<DBEntity::DBMessage>> messages_);
    void close();

public slots:
   
    void addMessages(QSet<QSharedPointer<User_Message>> messages_, quint32 room_id_);
    void addMessage(QSharedPointer<User_Message> shp_message_, quint32 room_id_);
    QSet<QSharedPointer<User_Message>> getMessages(const QDateTime& from_, const QDateTime& to_, const quint32& room_id_);
    void saveAllMessages();

    void safeExit();
};


#endif //LOCALSTORAGE_SERVICE_H
