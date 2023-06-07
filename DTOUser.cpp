#include "DTOUser.h"

namespace DTOHelper {

	DTOUser::DTOUser() {};

	DBEntity::DBUser DTOUser::convertToDBModel(const Entity::User& user_) {
		return DBEntity::DBUser(user_.getLogin(), user_.getPassword(), user_.getUserpic(), user_.getRating(), false);
	}

	Entity::User DTOUser::convertToEntity(const DBEntity::DBUser& db_user_) {
		return Entity::User(db_user_.getLogin(), db_user_.getPassword(), db_user_.getRating(), db_user_.getUserpic());
	}
}
