#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include "DBService.h"
#include "DBUser.h"
#include <QtConcurrent>
#include <plog/Log.h> 
#include <QCryptographicHash>

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
		QFuture<bool> updateUserPasswordUserpic(const QString& login_, const QString& new_password_, const QByteArray& new_userpic_);
		QFuture<QPair<bool, qint32>> updateUserRating(const QString& login_, const qint32& rating_);
		QFuture<qint32> getRatingByLogin(const QString& login_);
	};
}

#endif 
