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
#include "../../FileRepository.h"
#include <plog/Log.h>
#include <QObject>


namespace DBEntity{
//    struct Likes{
//        QUuid user_id;
//        bool grade;
//    };


    class DBMessage final : public QObject{
        Q_OBJECT

    	QUuid id;
        QDateTime date_time;
        explicit DBMessage(QObject *parent_ = nullptr);
    public:
    	void setDeleted(bool flag_);
        void setLikes(const QMap<QString, bool> &likes_);
        [[nodiscard]] const QUuid& getId() const;
        [[nodiscard]] const QDateTime &getDateTime() const;


        [[nodiscard]] qint32 getRoomId() const;

        [[nodiscard]] const QString &getLogin() const;

        [[nodiscard]] QString getParentId() const;

        [[nodiscard]] const QString &getText() const;

        [[nodiscard]] const QString &getMedia() const;

        [[nodiscard]] bool isDeleted() const;

        [[nodiscard]] const QMap<QString, bool> &getLikes() const;
        [[nodiscard]] QJsonObject toJson() const;
        //DBMessage(const DBMessage& _message);
        DBMessage(DBMessage&& _message) = default;

    private:
        qint32 room_id{};
        QString login;
        QPointer<DBMessage> parent_id;
        QString text;

        //TODO: how to store media
        QString media;
        bool deleted{false};
        QMap<QString, bool> likes;

        void fromJson(const QJsonObject &obj_);
    public:

        DBMessage(qint32 room_id_, const QString &login_, const QString &text_, const QString &media_,const QObject* parent = nullptr);

        static void writeMessages(const QString& file_name_, const QList<DBEntity::DBMessage>& messages_) ;
        static void writeMessage(const QString& file_name_,const DBEntity::DBMessage& messages_) ;
        static QSet<QSharedPointer<DBEntity::DBMessage>> readMessages(const QString& file_name_) ;
    };

}
#endif
//CHAT_SERVER_MESSAGES_H
