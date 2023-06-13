#pragma once
#ifndef LOCALSTORAGE_SERVICE_H
#define LOCALSTORAGE_SERVICE_H

#include <QObject>
#include <QMutex>
#include <QMap>
#include <QDir>

#include <plog/Log.h>

#include "DB/JSONS/messages.h"

class LocalStorage_Service :
    public QObject
{
    Q_OBJECT
private:
    static LocalStorage_Service* instance;
    static QMutex mutex;
    explicit LocalStorage_Service(QObject* object_ = nullptr);
    QMap<QUuid, QList<QSharedPointer<DBEntity::DBMessage>>> message_storage;
    

public:
    static LocalStorage_Service* getInstance();
    LocalStorage_Service& operator=(const LocalStorage_Service&) = delete;
    LocalStorage_Service(const LocalStorage_Service&) = delete;

signals:
    void messageRetrieved(QList<DBEntity::DBMessage*> message_);
    void close();

public slots:
    void addMessages(DBEntity::DBMessage* message_, QUuid room_id_);
    void getMessages(const QDateTime& from_, const QDateTime& to_, const QUuid& room_id_);
    void saveAllMessages();
    void safeExit();
};
#endif //LOCALSTORAGE_SERVICE_H