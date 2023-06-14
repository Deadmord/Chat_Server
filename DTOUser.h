#ifndef DTOUSER_H
#define DTOUSER_H

#include <QObject>
#include "DBUser.h"
#include "Entities/entities.h"

namespace DTOModel {

	class DTOUser : public QObject {

		Q_OBJECT

	public:

		DTOUser(QObject* parent = nullptr);
		DTOUser(const QString& login_, const QString& password_, const QByteArray& userpic_, const quint32& rating_, const bool& is_deleted_);
		~DTOUser();

		QString getUsername() const;
		void setUsername(const QString& username_);
		QString getPassword() const;
		void setPassword

		static QSharedPointer<DBEntity::DBUser> convertToDBModel(const User& user_);
		static QSharedPointer<User> convertToEntity(const DBEntity::DBUser& db_user_);

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