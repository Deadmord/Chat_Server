#include "DTOUser.h"

namespace DTOModel {

	DTOUser::DTOUser(QObject* parent) : QObject(parent) {};
	DTOUser::DTOUser(const QString& login_, const QString& password_, const QByteArray& userpic_, const quint32& rating_, bool is_deleted_)
		: a_username(login_), a_password(password_), a_userpic(userpic_), a_rating(rating_), a_is_deleted(is_deleted_) {};

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
	void DTOUser::setIsDeleted(const bool& is_deleted_) { a_is_deleted - is_deleted_; }

	QSharedPointer<DTOModel::DTOUser> DTOUser::createDTOUser(const DBEntity::DBUser& db_user_) {
		QSharedPointer<DTOModel::DTOUser> dto_room = QSharedPointer<DTOModel::DTOUser>::create(db_user_.getLogin(), db_user_.getPassword(), db_user_.getUserpic(), db_user_.getRating());
		return dto_room;
	}

	QSharedPointer<DBEntity::DBUser> DTOUser::createDBUser(const DTOModel::DTOUser& dto_user_) {
		QSharedPointer<DBEntity::DBUser> dto_room = QSharedPointer<DBEntity::DBUser>::create(dto_user_.getUsername(), dto_user_.getPassword(), dto_user_.getUserpic(), dto_user_.getRating());
		return dto_room;
	}
}
