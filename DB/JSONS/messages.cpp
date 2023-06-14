//
// Created by Admin on 5/24/2023.
//

#include "messages.h"

QJsonObject DBEntity::DBMessage::toJson() const {
    QJsonObject obj;
    obj["id"] = id.toString();
    obj["date_time"] = date_time.toString();
    obj["room_id"] = room_id;
    obj["login"] = login;
    obj["parent_id"] = parent_id.isNull()?"":parent_id->getId().toString();
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

//DBEntity::DBMessage::DBMessage(const DBMessage& _message) 
//{
//    id = _message.id;
//    date_time = _message.date_time;
//    room_id = _message.room_id;
//    login = _message.login;
//    parent_id = _message.parent_id;
//    text = _message.text;
//    media = _message.media;
//    deleted = _message.deleted;
//    likes = _message.likes;
//}


void DBEntity::DBMessage::fromJson(const QJsonObject &obj_) {

    id = QUuid::fromString(obj_.value("id").toString());
    date_time = QDateTime::fromString(obj_.value("date_time").toString());
    room_id = obj_.value("room_id").toInt();
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

void DBEntity::DBMessage::setDeleted(const bool flag_) {
    DBMessage::deleted = flag_;
}

void DBEntity::DBMessage::setLikes(const QMap<QString, bool> &likes_) {
    DBMessage::likes = likes_;
}

void DBEntity::DBMessage::writeMessages(const QString& file_name_,const QList<DBEntity::DBMessage>& messages_) {

    QJsonArray array;
    for (const auto& message: messages_) {
        array.append(message.toJson());
    }
    if (!FileRepository::writeJsonArr(file_name_, array)){
        PLOGE << "Error writing to file";
    }

}

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
           QSharedPointer<DBMessage> message (new DBMessage, &QObject::deleteLater);
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

qint32 DBEntity::DBMessage::getRoomId() const {
    return room_id;
}

const QString &DBEntity::DBMessage::getLogin() const {
    return login;
}

QString DBEntity::DBMessage::getParentId() const {
    return parent_id.isNull()?"":parent_id->getId().toString();
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

DBEntity::DBMessage::DBMessage(const qint32 room_id_, const QString &login_, const QString &text_, const QString &media_, const QObject* parent_) {

    id = QUuid::createUuid();
    date_time = QDateTime::currentDateTime();
    room_id = room_id_;
    login = login_;
    text = text_;
    media = media_;
}
    DBEntity::DBMessage::DBMessage(QObject * parent_):QObject(parent_)
    {}


    //TODO: заглушка парсера
    //DBEntity::DBMessage::DBMessage(User_Message* message, QObject* parent_) : QObject(parent_) { ; }



