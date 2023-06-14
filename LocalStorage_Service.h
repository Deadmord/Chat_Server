#pragma once
#ifndef LOCALSTORAGE_SERVICE_H
#define LOCALSTORAGE_SERVICE_H

#include <QObject>
#include <QMutex>
#include <QMap>
#include <QDir>

#include <plog/Log.h>
#include <QTimer>

#include "DB/JSONS/messages.h"
#include "Entities/SrvModels/Message.h"

class LocalStorage_Service:
    public QObject
{
    Q_OBJECT
private:
    static LocalStorage_Service* instance;
    static QMutex mutex;
    explicit LocalStorage_Service(QObject* object_ = nullptr);
    QMap<QUuid, QList<QSharedPointer<DBEntity::DBMessage>>> message_storage;
    //TODO: do dtorooms
    QMap < DBEntity::DBRoom, QSet < QSharedPointer < DBEntity::DBMessage >> > current_messages;
    static QTimer* p_timer;
    QList<QString> searchForFiles(const QDateTime& from_, const QDateTime& to_,
                                  const DBEntity::DBRoom& room_);

public:
    static LocalStorage_Service* getInstance(int minutes_ = 0);
    LocalStorage_Service& operator=(const LocalStorage_Service&) = delete;
    LocalStorage_Service(const LocalStorage_Service&) = delete;

signals:
    void messageRetrieved(QList<QSharedPointer<DBEntity::DBMessage>> message_);
    void close();

public slots:
   
    void addMessages(DBEntity::DBMessage* message_, QUuid room_id_);
    void getMessages(const QDateTime& from_, const QDateTime& to_, const DBEntity::DBRoom& room_);
    void saveAllMessages();
    void deleteMessages();
    void safeExit();
};


#endif //LOCALSTORAGE_SERVICE_H
