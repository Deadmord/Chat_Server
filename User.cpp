#include "User.h"

namespace Entity {

	User::User(const QString& login_, const QString& password_, const qint32& rating_, const QByteArray& userpic_)
		: a_login(login_), a_password(password_), a_rating(rating_), a_userpic(userpic_) {};

	QString User::getLogin() const { return this->a_login; }
	void User::setLogin(const QString& login_) { this->a_login = login_; }

	QString User::getPassword() const { return this->a_password; }
	void User::setPassword(const QString& password_) { this->a_password = password_; }

	qint32 User::getRating() const { return this->a_rating; }
	void User::setRating(const qint32& rating_) { this->a_rating = rating_; }

	QByteArray User::getUserpic() const { return this->a_userpic; }
	void User::setUserpic(const QByteArray& userpick_) { this->a_userpic = userpick_; }
}
