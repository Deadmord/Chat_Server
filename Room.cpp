#include "Room.h"

namespace Entity {

	Room::Room(const qint32& id_, const QString& name_, const QString& description_, const QString& password_, const bool& is_deleted_) :
		a_id(id_), a_name(name_), a_description(description_), a_password(password_),a_is_deleted(is_deleted_) {};

	qint32 Room::getId() const { return this->a_id; }
	void Room::setId(const qint32& id_) { this->a_id = id_; }

	QString Room::getName() const { return this->a_name; }
	void Room::setName(const QString& name_) { this->a_name = name_; }

	QString Room::getDescription() const { return this->a_description; }
	void Room::setDescription(const QString& description_) { this->a_description = description_; }

	QString Room::getPassword() const { return this->a_password; }
	void Room::setPassword(const QString& password_) { this->a_password = password_; }

	bool Room::isDeleted() const { return this->a_is_deleted; }
	void Room::setIsDeleted(const bool& is_deleted_) { this->a_is_deleted = is_deleted_; }
}