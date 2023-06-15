#include "SrvRoom.h"

#include "DBRoom.h"

SrvRoom::SrvRoom(QSharedPointer<DBEntity::DBRoom> cshp_db_room_, QObject* p_parent_) :
    id(cshp_db_room_->getId()),
    name(cshp_db_room_->getName()),
    description(cshp_db_room_->getDescription()),
    topic_id(cshp_db_room_->getTopicId()),
    is_private(cshp_db_room_->isPrivate()),
    password(cshp_db_room_->getPassword()),
    is_deleted(cshp_db_room_->isDeleted()),
    QObject(p_parent_)
{}

quint32 SrvRoom::getId() const { return id; }
QString SrvRoom::getName() const { return name; }
QString SrvRoom::getDescription() const { return description; }
quint32 SrvRoom::getTopicId() const { return topic_id; }
bool SrvRoom::isPrivate() const { return is_private; }
QString SrvRoom::getPassword() const { return password; }
bool SrvRoom::isDeleted() const { return is_deleted; }

void SrvRoom::addMessage(User_Message* p_message_)
{
    messages.append(p_message_);
}

void SrvRoom::setName(const QString& val) { name = val; emit nameChanged(val); }
void SrvRoom::setDescription(const QString& val) { description = val; emit descriptionChanged(val); }
void SrvRoom::setPrivate(bool val) { is_private = val; emit privateChanged(val); }
void SrvRoom::setPassword(const QString& val) { password = val; emit passwordChanged(); }
void SrvRoom::Delete() { is_deleted = true; emit roomDeleted(); }

QList<UserConnection*> SrvRoom::getConnectedUsers() const
{
    return QList<UserConnection*>();
}

QList<User_Message*> SrvRoom::getMessages(const QDateTime& from_, const QDateTime& to_) const
{
    return QList<User_Message*>();
}



void SrvRoom::connectUser(UserConnection* user)
{
}

void SrvRoom::initRoom() {

}

//void SrvRoom::loadMsgHistory(QDateTime from_, QDateTime to_)
//{
    //QJsonDocument msgHistory;
    //QJsonArray msgArray;
    //QJsonParseError jsonError;
    //QFile msgFile;

    //msgFile.setFileName(path);
    //if (msgFile.open(QIODevice::ReadOnly | QFile::Text))
    //{
    //    //тот нужно блокировать обращение к ресурсу msgFile
    //    msgHistory = QJsonDocument::fromJson(QByteArray(msgFile.readAll()), &jsonError);
    //    msgFile.close();

    //    if (jsonError.errorString().toInt() == QJsonParseError::NoError)
    //    {
    //        msgArray = QJsonValue(msgHistory.object().value("messanges")).toArray();
    //        for (const auto& msgJson : msgArray)
    //        {
    //            Message msg{ msgJson.toObject().value("id").toString(),
    //                       static_cast<quint32>(msgJson.toObject().value("roomId").toInt()),
    //                        QDateTime::fromString(msgJson.toObject().value("time").toString()),
    //                        msgJson.toObject().value("nickname").toString(),
    //                        msgJson.toObject().value("text").toString(),
    //                        msgJson.toObject().value("mediaId").toString(),
    //                        msgJson.toObject().value("parentId").toString(),
    //                        msgJson.toObject().value("deleted").toBool() };
    //            messages.push_back(msg);
    //        }
    //    }
    //    else
    //    {
    //        qDebug() << "Error message history read: " << jsonError.error;
    //    }
    //}
    //else
    //{
    //    qDebug() << "File message history can't be open.";
    //}
//}