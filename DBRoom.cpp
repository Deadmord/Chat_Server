#include "DBRoom.h"

namespace DBEntity {

	DBRoom::DBRoom(const qint32& id_, const QString& name_, const QString& description_, const qint32& topic_id_, const QString& topic_name_, const bool& is_private_, const QString& password_, const bool& is_deleted_)
		: a_id(id_), a_name(name_), a_description(description_), a_topic(topic_id_, topic_name_), a_is_private(is_private_), a_password(password_), a_is_deleted(is_deleted_) {}
	
	DBRoom::DBRoom(QSharedPointer<SrvRoom> cshp_srv_room_) :
		a_id(cshp_srv_room_->getId()),
		a_name(cshp_srv_room_->getName()),
		a_description(cshp_srv_room_->getDescription()),
		a_topic(cshp_srv_room_->getTopicId(), cshp_srv_room_->getTopicName()),
		a_is_private(cshp_srv_room_->isPrivate()),
		a_password(cshp_srv_room_->getPassword()),
		a_is_deleted(cshp_srv_room_->isDeleted())
	{}
	
	qint32 DBRoom::getId() const { return this->a_id; }
	void DBRoom::setId(const qint32& id_) { this->a_id = id_; }
	QString DBRoom::getName() const { return this->a_name; }
	void DBRoom::setName(const QString& name_) { this->a_name = name_; }
	QString DBRoom::getDescription() const { return this->a_description; }
	void DBRoom::setDescription(const QString& description_) { this->a_description = description_; }
	qint32 DBRoom::getTopicId() const { return this->a_topic.getId(); }
	void DBRoom::setTopicId(const qint32& topic_id_) { this->a_topic.setId(topic_id_); }
	QString DBRoom::getTopicName() const { return this->a_topic.getName(); }
	void DBRoom::setTopicName(const QString& topic_name) { this->a_topic.setName(topic_name); }
	bool DBRoom::isPrivate() const { return this->a_is_private; }
	void DBRoom::setIsPrivate(const bool& is_private_) { this->a_is_private = is_private_; }
	QString DBRoom::getPassword() const { return this->a_password; }
	void DBRoom::setPassword(const QString& password_) { this->a_password = password_; }
	bool DBRoom::isDeleted() const { return this->a_is_deleted; }
	void DBRoom::setIsDeleted(const bool& deleted_) { this->a_is_deleted = deleted_; }

}
