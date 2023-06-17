#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include "DBConncetion_Service.h"
#include "DBUser.h"
#include "QueryHelper.h"
#include <QtConcurrent>
#include <plog/Log.h> 
#include <QCryptographicHash>

namespace DBService {

	class UserRepository : public QObject {

		Q_OBJECT

	private:
		static DBConnection_Service a_dbConnection;

	public:
		UserRepository(const QString& connection_string_);
		~UserRepository();
		static QFuture<QSharedPointer<DBEntity::DBUser>> getUserByLogin(const QString& login_);
		static QFuture<bool> createUser(const DBEntity::DBUser& user_);
		static QFuture<bool> updateUserPasswordUserpic(const QString& login_, const QString& new_password_, const QByteArray& new_userpic_);
		static QFuture<QPair<bool, qint32>> updateUserRating(const QString& login_, const qint32& rating_);
		static QFuture<qint32> getRatingByLogin(const QString& login_);
	};
}

Q_DECLARE_METATYPE(DBService::UserRepository)

#endif 
