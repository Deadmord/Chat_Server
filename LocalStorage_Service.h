#pragma once

#include <QObject>
#include <QMutex>
#include <QMap>
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
public slots:


    //void getMessages(QDateTime from_, QDateTime to_, QUuid room_id_);
    void saveAllMessages();
};
