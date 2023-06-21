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
#include "DBRoom.h"
#include "FileRepository.h"
#include <plog/Log.h>
#include <QObject>


namespace DBEntity{

    class DBMessage final {
        
    	QUuid id;
        QDateTime date_time;
      
    public:
        //explicit DBMessage(User_Message* message, QObject* parent_ = nullptr);
    	void setDeleted(bool flag_);
        void setLikes(const QMap<QString, bool> &likes_);
        void addLike(const QString& login_, bool like_or_dislike_);
        [[nodiscard]] const QUuid& getId() const;
        [[nodiscard]] const QDateTime &getDateTime() const;
     

        [[nodiscard]] const QString &getLogin() const;

        [[nodiscard]] QString getParentId() const;

        [[nodiscard]] const QString &getText() const;

        [[nodiscard]] const QString &getMedia() const;

        [[nodiscard]] bool isDeleted() const;
        [[nodiscard]] bool isRtl() const;

        [[nodiscard]] const QMap<QString, bool> &getLikes() const;
        [[nodiscard]] QJsonObject toJson() const;
        void fromJson(const QJsonObject& obj_);

    private:
        QString login;
		QString parent_id;
        QString text;
        QString media;
        bool is_rtl{};
        bool deleted{false};
        QMap<QString, bool> likes;

        
    public:
        DBMessage();
        DBMessage(const QUuid& id_, const QDateTime& date_time_, const QString& login_,
            const QString& text_, const QString& media_, const QString& parent_id_, bool is_rtl_);

       /* static void writeMessages(const QString& file_name_, const QList<DBEntity::DBMessage>& messages_) ;
        static void writeMessage(const QString& file_name_,const DBEntity::DBMessage& messages_) ;
        static QSet<QSharedPointer<DBEntity::DBMessage>> readMessages(const QString& file_name_) ;*/
    };

}
#endif
//CHAT_SERVER_MESSAGES_H
