#include "DTOUser.h"

namespace DTOModel {

	DTOUser::DTOUser(QObject* parent) : QObject(parent) {};
	DTOUser::DTOUser(const QString& login_, const QString& password_, const QByteArray& userpic_, const quint32& rating_)
		: a_username(login_), a_password(password_), a_userpic(userpic_), a_rating(rating_), a_is_deleted(false) {};

	DTOUser::DTOUser(const DBEntity::DBUser& db_user_) : a_username(db_user_.getLogin()), a_password(db_user_.getPassword()), a_userpic(db_user_.getUserpic()), a_rating(db_user_.getRating()), a_is_deleted(false) {};

	DTOUser::~DTOUser() {}

	QString DTOUser::getUsername() const { return a_username; }
	void DTOUser::setUsername(const QString& username_) { a_username = username_; }
	QString DTOUser::getPassword() const { return a_password; }
	void DTOUser::setPassword(const QString& password_) { a_password = password_; }
	QByteArray DTOUser::getUserpic() const { return a_userpic; }
	void DTOUser::setUserpic(const QByteArray& userpic_) { a_userpic = userpic_; }
	quint32 DTOUser::getRating() const { return a_rating; }
	void DTOUser::setRating(const quint32& rating_) { a_rating = rating_; }
	bool DTOUser::isDeleted() const { return a_is_deleted; }
	
}
