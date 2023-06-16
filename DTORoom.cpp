#include "DTORoom.h"

namespace DTOModel {

	DTORoom::DTORoom(QObject* parent) : QObject(parent) {};
	DTORoom::DTORoom(const qint32& id_, const QString& name_, const QString& description_, const qint32& topic_id_, const QString& topic_name_, const bool& is_private_, const QString& password_, const bool& is_deleted_)
		: id(id_), name(name_), description(description_), topic_id(topic_id_),topic_name(topic_name_), is_private(is_private_), password(password_), is_deleted(is_deleted_), messages(), connected_users() 
	{
		messages = QSet<QSharedPointer<User_Message>>();
		connected_users = QList<SrvUser*>();
	};

	DTORoom::~DTORoom() {};

	qint32 DTORoom::getId() const { return this->id; }
	void DTORoom::setId(const qint32& id_) { this->id = id_; }
	QString DTORoom::getName() const { return this->name; }
	void DTORoom::setName(const QString& name_) { this->name = name_; }
	QString DTORoom::getDescription() const { return this->description; }
	void DTORoom::setDescription(const QString& description_) { this->description = description_; }
	qint32 DTORoom::getTopicId() const { return this->topic_id; }
	void DTORoom::setTopicId(const qint32& topic_id_) { this->topic_id = topic_id_; }
	QString DTORoom::getTopicName() const { return this->topic_name; }
	void DTORoom::setTopicName(const QString& topic_name_) { this->topic_name = topic_name_; }
	bool DTORoom::isPrivate() const { return this->is_private; }
	void DTORoom::setIsPrivate(const bool& is_private_) { this->is_private = is_private_; }
	QString DTORoom::getPassword() const { return this->password; }
	void DTORoom::setPassword(const QString& password_) { this->password = password_; }
	bool DTORoom::isDeleted() const { return this->is_deleted; }
	void DTORoom::setIsDeleted(const bool& deleted_) { this->is_deleted = deleted_; }
	QSet<QSharedPointer<User_Message>> DTORoom::getMessages() const { return messages; }
	void DTORoom::setMessages(QSharedPointer<User_Message> shp_user_message_) { messages.insert(shp_user_message_); }
	QList<SrvUser*> DTORoom::getConnectedUsers() const { return connected_users; }
	void DTORoom::setConnectedUsers(SrvUser* connected_user_) { connected_users.append(connected_user_); }

	QSharedPointer<DTOModel::DTORoom> DTORoom::createDTORoomFromSrv(const SrvRoom& srv_room) {
		QSharedPointer<DTOModel::DTORoom> shp_dto_room = QSharedPointer<DTOModel::DTORoom>::create(
			srv_room.getId(), srv_room.getName(), srv_room.getDescription(), srv_room.getTopicId(), srv_room.getTopicName(),
			srv_room.isPrivate(), srv_room.getPassword(), srv_room.isDeleted());
		return shp_dto_room;
	}

	QSharedPointer<SrvRoom> DTORoom::createSrvRoomFromDTO(const DTOModel::DTORoom& dto_model) {
		QSharedPointer<SrvRoom> shp_srv_room = QSharedPointer<SrvRoom>::create(
			dto_model.getId(), dto_model.getName(), dto_model.getDescription(), dto_model.getTopicId(), dto_model.getTopicName(),
			dto_model.isPrivate(), dto_model.getPassword(), dto_model.isDeleted());
		return shp_srv_room;
	}

	QSharedPointer<DBEntity::DBRoom> DTORoom::createDBRoomFromSrv(const SrvRoom& srv_room) {
		QSharedPointer<DBEntity::DBRoom> shp_db_room = QSharedPointer<DBEntity::DBRoom>::create(
			srv_room.getId(), srv_room.getName(), srv_room.getDescription(), srv_room.getTopicId(), srv_room.getTopicName(),
			srv_room.isPrivate(), srv_room.getPassword());
		return shp_db_room;
	}

	QSharedPointer<SrvRoom> DTORoom::createSrvRoomFromDB(const DBEntity::DBRoom& db_room) {
		QSharedPointer<SrvRoom> shp_srv_room = QSharedPointer<SrvRoom>::create(
			db_room.getId(), db_room.getName(), db_room.getDescription(), db_room.getTopicId(), db_room.getTopicName(),
			db_room.isPrivate(), db_room.getPassword(), db_room.isDeleted());
		return shp_srv_room;
	}

}