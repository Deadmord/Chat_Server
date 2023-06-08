#pragma once
#include <QVector>
#include <QTime>
#include <QSet>
//#include "SrvModels/Message.h"

struct Message
{
    QString     id;
    quint32     room_id;
    QDateTime   date_time;
    QString     nickname;
    QString     text;
    QString     media_id;
    QString     parent_id;
    bool        deleted;
    QHash<QUuid, bool> likes;

    //
    //    void generateId() {
    //        id = QUuid::createUuid().toString();
    //    }
};

struct client
{
    QString userName = nullptr;
    QDateTime lastMessageTime;
    qint32 reiting;
};

struct room
{
    QString name;
    //QVector<User_Message> messages;
    QSet<client> clients;
    // Другие необходимые поля
};

