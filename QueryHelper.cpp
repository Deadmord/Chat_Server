#include "QueryHelper.h"

namespace Helper {

	QString QueryHelper::getAllRooms() { return "SELECT r.*, t.name AS topic_name FROM room r JOIN topic t ON r.topic_id=t.id;"; }
	QString QueryHelper::getAllActiveRooms() { return "SELECT r.*, t.name AS topic_name FROM room r JOIN topic t ON r.topic_id=t.id WHERE r.is_deleted=0;"; }
	QString QueryHelper::getRoomById() { return "SELECT r.*, t.name AS topic_name FROM room r JOIN topic t ON r.topic_id = t.id WHERE r.id=:id;"; }
	QString QueryHelper::createRoom() { return "INSERT INTO room (name, description, topic_id, is_private, password, is_deleted) VALUES (:name, :description, :topic_id, :is_private, :password, :is_deleted)"; }
	QString QueryHelper::deleteRoom() { return "UPDATE room SET is_deleted=:is_deleted WHERE id=:id"; }
	QString QueryHelper::getUserByLogin() { return "SELECT * FROM [user] WHERE login=:login;"; }
	QString QueryHelper::createUser() { return "INSERT INTO [user] (login, password, userpic, rating, is_deleted) VALUES (:login, :password, :userpic, :rating, :is_deleted)"; }
	QString QueryHelper::updateUserRating() { return "UPDATE [user] SET rating=:rating WHERE login=:login"; }
	QString QueryHelper::getRatingByLogin() { return "SELECT rating from [user] WHERE login=:login"; }
}
