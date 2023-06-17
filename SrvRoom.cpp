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

SrvRoom::SrvRoom(const qint32& id_, const QString& name_, const QString& description_, const qint32& topic_id_, const QString& topic_name_, const bool& is_private_, const QString& password_, const bool& is_deleted_)
    : id(id_), name(name_), description(description_), topic_id(topic_id_), topic_name(topic_name_), is_private(is_private_), password(password_), is_deleted(is_deleted_), messages(), connected_users() {}

quint32 SrvRoom::getId() const { return id; }
QString SrvRoom::getName() const { return name; }
QString SrvRoom::getDescription() const { return description; }
quint32 SrvRoom::getTopicId() const { return topic_id; }
QString SrvRoom::getTopicName() const { return topic_name; }
bool SrvRoom::isPrivate() const { return is_private; }
QString SrvRoom::getPassword() const { return password; }
bool SrvRoom::isDeleted() const { return is_deleted; }

void SrvRoom::addMessage(const QSharedPointer<User_Message>& p_message_)
{
    messages.insert(p_message_);
}

QSet<QSharedPointer<User_Message>> SrvRoom::getMessages() const
{
    return messages;
}

void SrvRoom::addMessages(const QSet<QSharedPointer<User_Message>>& messages_)
{
    messages.unite(messages_);
}

void SrvRoom::setName(const QString& val) { name = val; emit nameChanged(val); }
void SrvRoom::setDescription(const QString& val) { description = val; emit descriptionChanged(val); }
void SrvRoom::setPrivate(bool val) { is_private = val; emit privateChanged(val); }
void SrvRoom::setPassword(const QString& val) { password = val; emit passwordChanged(); }
void SrvRoom::Delete() { is_deleted = true; emit roomDeleted(); }

QSet<QSharedPointer<SrvUser>> SrvRoom::getConnectedUsers() const
{
    return connected_users;
}

QSet<QSharedPointer<User_Message>> SrvRoom::getMessages(const QDateTime& from_, const QDateTime& to_) const
{
    QSet<QSharedPointer<User_Message>> result;
    QtConcurrent::map(messages, [&result, &from_, &to_](QSharedPointer<User_Message> message) {
        if (auto date = message->getDateTime(); date >= from_ && date <= to_)
        {
            result.insert(message);
        }
    }).waitForFinished();

    return result;
}

bool SrvRoom::connectUser(const QSharedPointer<SrvUser>& shp_user_)
{
    if (!connected_users.contains(shp_user_)) {
        connected_users.insert(shp_user_);
        return true;
    }
    else {
        PLOGE << "User already connected! Login: " + shp_user_->getUserName();
        return false;
    }
}


bool SrvRoom::disconnectUser(const QSharedPointer<SrvUser>& shp_user_)
{
    if (connected_users.contains(shp_user_)) {
        connected_users.remove(shp_user_);
        return true;
    }
    else {
        PLOGE << "User isn't connected! Login: " + shp_user_->getUserName();
        return false;
    }
}