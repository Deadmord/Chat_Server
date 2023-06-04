//
// Created by Admin on 5/24/2023.
//

#ifndef CHAT_SERVER_MESSAGES_H
#define CHAT_SERVER_MESSAGES_H
//#include "server.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QFile>
#include <QTime>
#include <QUuid>
#include <QPointer>
#include "../../DBRoom.h"
#include "../../file_repository.h"


namespace DBEntity{
//    struct Likes{
//        QUuid user_id;
//        bool grade;
//    };


    class DBMessage : public QObject{
        Q_OBJECT



        Q_PROPERTY(QUuid id READ getId)
        Q_PROPERTY(QDateTime date_time READ getDateTime)
        Q_PROPERTY(quint32 room_id READ getRoomId)
        Q_PROPERTY(QString login READ getLogin)
        Q_PROPERTY(QString parent_id READ getParentId)
        Q_PROPERTY(QString text READ getText)
        Q_PROPERTY(QString media READ getMedia)
        Q_PROPERTY(bool deleted READ isDeleted WRITE setDeleted)
        Q_PROPERTY(QHash<QUuid, bool> likes READ getLikes WRITE setLikes)
    private:
        QUuid id;
    public:
        void setDeleted(bool flag_);

        void setLikes(const QHash<QUuid, bool> &likes_);

    private:
        QDateTime date_time;
    public:
        [[nodiscard]] const QDateTime &getDateTime() const;

       [[nodiscard]] qint32 getRoomId() const;

        [[nodiscard]] const QString &getLogin() const;

        [[nodiscard]] QString getParentId() const;

        [[nodiscard]] const QString &getText() const;

        [[nodiscard]] const QString &getMedia() const;

        [[nodiscard]] bool isDeleted() const;

        [[nodiscard]] const QHash<QUuid, bool> &getLikes() const;

    private:
        qint32 room_id;
        QString login;
        QPointer<DBMessage> parent_id;
        QString text;

        //TODO: how to store media
        QString media;
        bool deleted{false};
        QHash<QUuid, bool> likes;

        [[nodiscard]] QJsonObject toJson() const;
    public:

        DBMessage(qint32 room_id_, const QString &login_, const QString &text_, const QString &media_);
        [[nodiscard]] const QUuid &getId() const;
       
        static void writeMessage(const QString& file_name_, const QList<DBEntity::DBMessage>& message_) ;
        static void writeMessage(const QString& file_name_,const DBEntity::DBMessage& messages_) ;
        static QList<DBMessage> readMessage(const QString& file_name_) ;
    };

}
#endif
//CHAT_SERVER_MESSAGES_H
