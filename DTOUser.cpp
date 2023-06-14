#include "DTOUser.h"

namespace DTOHelper {

	DTOUser::DTOUser(QObject* parent) : QObject(parent) {};
	DTOUser::~DTOUser() {}

	QSharedPointer<DBEntity::DBUser> DTOUser::convertToDBModel(const User& user_) {
		QSharedPointer<DBEntity::DBUser> dbUser = QSharedPointer<DBEntity::DBUser>::create(user_.user_name, user_.password, user_.userpic, user_.rating, false);
		return dbUser;
	}

	QSharedPointer<User> DTOUser::convertToEntity(const DBEntity::DBUser& db_user_) {
		QSharedPointer<User> user = QSharedPointer<User>::create();
		user->user_name = db_user_.getLogin();
		user->password = db_user_.getPassword();
		user->userpic = db_user_.getUserpic();
		user->rating = db_user_.getRating();
		user->lastMessageTime = QDateTime();

		return user;
	}
}