#include "DTORoom.h"

namespace DTOModel {

	DTORoom::DTORoom(QObject* parent) : QObject(parent) {};
	DTORoom::DTORoom(const qint32& id_, const QString& name_, const QString& description_, const qint32& topic_id_, const bool& is_private_, const QString& password_, bool is_deleted_)
		: a_id(id_), a_name(name_), a_description(description_), a_topic_id(topic_id_), a_is_private(is_private_), a_password(password_), a_is_deleted(is_deleted_) {};

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

	QSharedPointer<DTOModel::DTORoom> DTORoom::createDTORoom(const DBEntity::DBRoom& db_room_) {
		QSharedPointer<DTOModel::DTORoom> dto_room = QSharedPointer<DTOModel::DTORoom>::create(db_room_.getId(), db_room_.getName(), db_room_.getDescription(), db_room_.getTopicId(), db_room_.isPrivate(), db_room_.getPassword());
		return dto_room;
	}

	QSharedPointer<DBEntity::DBRoom> DTORoom::createDBRoom(const DTOModel::DTORoom& dto_room_) {
		QSharedPointer<DBEntity::DBRoom> dto_room = QSharedPointer<DBEntity::DBRoom>::create(dto_room_.getId(), dto_room_.getName(), dto_room_.getDescription(), dto_room_.getTopicId(), dto_room_.isPrivate(), dto_room_.getPassword());
		return dto_room;
	}
}