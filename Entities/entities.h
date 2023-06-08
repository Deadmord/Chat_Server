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

struct User
{
    QString id;
    QString user_name;
    QString password;
    QString userpic_id;
    quint32 rating_likes;
    quint32 rating_dislikes;
    QDateTime lastMessageTime;
};

struct Topic
{
    quint32 id;
    QString name;
};

