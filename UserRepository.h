#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include "DBService.h"
#include "DBUser.h"

namespace DBService {

	class UserRepository : public QObject {

		Q_OBJECT

	private:
		//DBConnection* a_dbConnection;
		DBConnection a_dbConnection;

	public:
		//explicit UserRepository(DBConnection* connection_);
		explicit UserRepository(const QString& connection_string_);
		~UserRepository();
		QFuture<DBEntity::DBUser*> getUserByLogin(const QString& login_);
		QFuture<bool> createUser(const DBEntity::DBUser& user_);
		QFuture<bool> updateUser(const QString& login_, const QString& new_password_, const QByteArray& new_userpic_);
	};
}

#endif 
