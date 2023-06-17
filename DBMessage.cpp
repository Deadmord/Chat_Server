//
// Created by Admin on 5/24/2023.
//

#include "DBMessage.h"

QJsonObject DBEntity::DBMessage::toJson() const {
    QJsonObject obj;
    obj["id"] = id.toString();
    obj["date_time"] = date_time.toString();
    obj["login"] = login;
    obj["parent_id"] = parent_id;
    obj["text"] = text;
    obj["media"] = media.isNull()?"":media;
    obj["deleted"] = deleted;
    QJsonObject json;
    QMapIterator<QString, bool> i(likes);
    while (i.hasNext()) {
        i.next();
        json.insert(i.key(), i.value());
    }
    obj["likes"] = json;

    return obj;
}

void DBEntity::DBMessage::fromJson(const QJsonObject &obj_) {

    id = QUuid::fromString(obj_.value("id").toString());
    date_time = QDateTime::fromString(obj_.value("date_time").toString());
   
    login = obj_.value("login").toString();
//    todo: parent id
//    parent_id = obj.value("parent_id").toString();
    text = obj_.value("text").toString();
    media = obj_.value("media").toString();
    likes.clear();
    const auto temp = obj_.value("likes").toObject().toVariantMap();
    for (auto it =temp.constBegin(); it != temp.constEnd(); ++it)
    {
        
        likes.insert(it.key() , it.value().toBool());
    }
}

DBEntity::DBMessage::DBMessage()
{
}

bool DBEntity::DBMessage::isRtl() const
{
	return is_rtl;
}

void DBEntity::DBMessage::setDeleted(const bool flag_) {
    DBMessage::deleted = flag_;
}

void DBEntity::DBMessage::setLikes(const QMap<QString, bool> &likes_) {
    DBMessage::likes = likes_;
}

//void DBEntity::DBMessage::writeMessages(const QString& file_name_,const QList<DBEntity::DBMessage>& messages_) {
//
//    QJsonArray array;
//    for (const auto& message: messages_) {
//        array.append(message.toJson());
//    }
//    if (!FileRepository::writeJsonArr(file_name_, array)){
//        PLOGE << "Error writing to file";
//    }
//
//}

void DBEntity::DBMessage::writeMessage(const QString& file_name_,const DBEntity::DBMessage& messages_) {

    QJsonArray array;

    array.append(messages_.toJson());

    if (!FileRepository::writeJsonArr(file_name_, array)){
       PLOGE << "Error writing to file";
    }
    PLOGI << "Writing message successfully";
}

QSet<QSharedPointer<DBEntity::DBMessage>> DBEntity::DBMessage::readMessages(const QString& file_name_)
{
    QJsonArray array;
   FileRepository::readJsonArr(file_name_, array);
   QSet<QSharedPointer<DBMessage>> messages;

   for(const auto  obj: array) {
       if (obj.isObject())
       {
           QJsonObject json_object = obj.toObject();
           QSharedPointer<DBMessage> message (new DBMessage());
           message->fromJson(json_object);
           messages.insert(message);
       }
       else
       {
           PLOGE << "File is corrupted";
       }
   }
  
   	return messages;

}

const QDateTime &DBEntity::DBMessage::getDateTime() const {
    return date_time;
}


const QString &DBEntity::DBMessage::getLogin() const {
    return login;
}

QString DBEntity::DBMessage::getParentId() const {
    return parent_id;
}

const QString &DBEntity::DBMessage::getText() const {
    return text;
}

const QString &DBEntity::DBMessage::getMedia() const {
    return media;
}

bool DBEntity::DBMessage::isDeleted() const {
    return deleted;
}

const QMap<QString, bool> &DBEntity::DBMessage::getLikes() const {
    return likes;
}

const QUuid &DBEntity::DBMessage::getId() const {
    return id;
}

DBEntity::DBMessage::DBMessage(const QUuid& id_, const QDateTime& date_time_, const QString& login_,
                               const QString& text_, const QString& media_, const QString& parent_id_, const bool is_rtl_
                               )
{

    id = id_;
    date_time = date_time_;
    login = login_;
    text = text_;
    media = media_;
    parent_id = parent_id_;
    is_rtl = is_rtl_;
}


    //TODO: заглушка парсера
    //DBEntity::DBMessage::DBMessage(User_Message* message, QObject* parent_) : QObject(parent_) { ; }



