#ifndef DTOUSER_H
#define DTOUSER_H

#include <QObject>
#include "DBUser.h"
#include <QSharedPointer>

namespace DTOModel {

	class DTOUser : public QObject {

		Q_OBJECT

	public:

		DTOUser(QObject* parent = nullptr);
		DTOUser(const QString& login_, const QString& password_, const QByteArray& userpic_, const quint32& rating_);
		DTOUser(const DBEntity::DBUser& db_user_);
		~DTOUser();

		QString getUsername() const;
		void setUsername(const QString& username_);
		QString getPassword() const;
		void setPassword(const QString& password_);
		QByteArray getUserpic() const;
		void setUserpic(const QByteArray& userpic_);
		quint32 getRating() const;
		void setRating(const quint32& rating_);
		bool isDeleted() const;

		static QSharedPointer<DBEntity::DBUser> convertToDBModel(const DTOUser& dtoUser_);
		static QSharedPointer<DTOModel::DTOUser> convertToEntity(const DBEntity::DBUser& db_user_);

	private:

		QString a_username;
		QString a_password;
		QByteArray a_userpic;
		quint32 a_rating;
		bool a_is_deleted;

	};
}

Q_DECLARE_METATYPE(DTOModel::DTOUser)

#endif