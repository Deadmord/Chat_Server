//#include "DTOUser.h"
//
//namespace DTOHelper {
//
//	DTOUser::DTOUser() {};
//
//	DBEntity::DBUser DTOUser::convertToDBModel(const User& user_) {
//		return DBEntity::DBUser(user_.user_name, user_.password, user_.userpic, user_.rating, false); //TODO: reshit' kak luchwe
//	}
//
//	User DTOUser::convertToEntity(const DBEntity::DBUser& db_user_) {
//		User user;
//		user.user_name = db_user_.getLogin();
//		user.password = db_user_.getPassword();
//		user.userpic = db_user_.getUserpic();
//		user.rating = db_user_.getRating();
//		user.lastMessageTime = QDateTime();
//
//		return user;
//	}
//}
