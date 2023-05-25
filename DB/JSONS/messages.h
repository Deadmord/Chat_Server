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

namespace DBEntity{
//    struct Likes{
//        QUuid user_id;
//        bool grade;
//    };


    class DBMessage : public QObject{
        //Q_OBJECT

        //Q_PROPERTY(QUuid id READ getId)
        //Q_PROPERTY(QDateTime date_time READ getDateTime)
        //Q_PROPERTY(QUuid room_id READ getRoomId)
        //Q_PROPERTY(QString login READ getLogin)
        //Q_PROPERTY(QUuid parent_id READ getParentId)
        //Q_PROPERTY(QString text READ getText)
        //Q_PROPERTY(QString media READ getMedia)
        //Q_PROPERTY(bool deleted READ isDeleted WRITE setDeleted)
        //Q_PROPERTY(QHash<QUuid, bool> likes READ getLikes WRITE setLikes)
    private:
        QUuid id;
    public:
        void setDeleted(bool flag);

        void setLikes(const QHash<QUuid, bool> &likes);

    private:
        QDateTime date_time;
    public:
        [[nodiscard]] const QDateTime &getDateTime() const;

        [[nodiscard]] const QUuid &getRoomId() const;

        [[nodiscard]] const QString &getLogin() const;

        [[nodiscard]] const QUuid &getParentId() const;

        [[nodiscard]] const QString &getText() const;

        [[nodiscard]] const QString &getMedia() const;

        [[nodiscard]] bool isDeleted() const;

        [[nodiscard]] const QHash<QUuid, bool> &getLikes() const;

    private:
        QUuid room_id;
        QString login;
        QUuid parent_id;
        QString text;

        //TODO: how to store media
        QString media;
        bool deleted{false};
        QHash<QUuid, bool> likes;

        [[nodiscard]] QJsonObject toJson() const;
    public:
        DBMessage(QUuid id_room_, const QString& login_, const QString& text_, const QString& media_){
            id = QUuid::createUuid();
            date_time = QDateTime::currentDateTime();
            room_id = id_room_;
            login = login_;
            text = text_;
            media = media_;

        }
        ~DBMessage()= default;


        const QUuid &getId() const;


        static void writeMessage(const QString& file_name_, const DBEntity::DBMessage& message_) ;

        static QList<DBMessage> readMessage(const QString& file_name_) ;
    };

}
#endif
//CHAT_SERVER_MESSAGES_H
