#include "DTORoom.h"

namespace DTOModel {

	DTORoom::DTORoom(QObject* parent) : QObject(parent) {};
	DTORoom::DTORoom(const qint32& id_, const QString& name_, const QString& description_, const qint32& topic_id_, const bool& is_private_, const QString& password_)
		: a_id(id_), a_name(name_), a_description(description_), a_topic_id(topic_id_), a_is_private(is_private_), a_password(password_), a_is_deleted(false) {};

	DTORoom::DTORoom(const DBEntity::DBRoom& db_room_)
		: a_id(db_room_.getId()), a_name(db_room_.getName()), a_description(db_room_.getDescription()), a_topic_id(db_room_.getTopicId()), a_is_private(db_room_.getTopicId()), a_password(db_room_.getPassword()), a_is_deleted(false) {};

	DTORoom::~DTORoom() {};

	qint32 DTORoom::getId() const { return this->a_id; }
	void DTORoom::setId(const qint32& id_) { this->a_id = id_; }
	QString DTORoom::getName() const { return this->a_name; }
	void DTORoom::setName(const QString& name_) { this->a_name = name_; }
	QString DTORoom::getDescription() const { return this->a_description; }
	void DTORoom::setDescription(const QString& description_) { this->a_description = description_; }
	qint32 DTORoom::getTopicId() const { return this->a_topic_id; }
	void DTORoom::setTopicId(const qint32& topic_id_) { this->a_topic_id = topic_id_; }
	bool DTORoom::isPrivate() const { return this->a_is_private; }
	void DTORoom::setIsPrivate(const bool& is_private_) { this->a_is_private = is_private_; }
	QString DTORoom::getPassword() const { return this->a_password; }
	void DTORoom::setPassword(const QString& password_) { this->a_password = password_; }
	bool DTORoom::isDeleted() const { return this->a_is_deleted; }
	void DTORoom::setIsDeleted(const bool& deleted_) { this->a_is_deleted = deleted_; }
}