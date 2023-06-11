#ifndef USER_H
#define USER_H

#include <QString>
#include <QObject>

namespace Entity {

	class User : public QObject {

		Q_OBJECT

	private:
		QString a_login;
		QString a_password;
		qint32 a_rating;
		QByteArray a_userpic;


	public:
		explicit User(const QString& login_,const QString& password_, const qint32& rating_, const QByteArray& userpic_);

		QString getLogin() const;
		void setLogin(const QString& login_);

		QString getPassword() const;
		void setPassword(const QString& password_);

		qint32 getRating() const;
		void setRating(const qint32& rating_);

		QByteArray getUserpic() const;
		void setUserpic(const QByteArray& userpic_path_);
	};
}

#endif 
