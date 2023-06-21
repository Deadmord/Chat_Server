#include "UserRepository.h"

namespace DBService {

	DBConnection_Service UserRepository::a_dbConnection("Driver={ODBC Driver 18 for SQL Server};Server=tcp:comp-zionet-server.database.windows.net,1433;Database=CPP_Chat_DB;Uid=Logika4417;Pwd=Fyyf1998;Encrypt=yes;TrustServerCertificate=no;Connection Timeout=30;", 0);
	UserRepository::UserRepository(const QString& connection_string_) { a_dbConnection.setConnectionString(connection_string_); }
	UserRepository::~UserRepository(){}

	QFuture<QSharedPointer<DBEntity::DBUser>> UserRepository::getUserByLogin(const QString& login_) {
		return QtConcurrent::run([query_string_ = Helper::QueryHelper::getUserByLogin(), login_]() {
			try
			{
				auto connection = DBService::DBConnection_Service::getConnection();
				if (connection->getDatabase()->isOpen()) {
					QSqlQuery query(*connection->getDatabase());
					query.prepare(query_string_);
					query.bindValue(":login", login_);

					if (query.exec() && query.next()) {
						QString login = query.value("login").toString();
						QString password = query.value("password").toString();
						QByteArray userpicc = query.value("userpic").toByteArray();
						qint32 rating = query.value("rating").toInt();
						bool is_deleted = query.value("is_deleted").toBool();
						QSharedPointer<DBEntity::DBUser> shp_user = QSharedPointer<DBEntity::DBUser>::create(login, password, userpicc, rating, is_deleted);
						return shp_user;
					}
					else {
						PLOG_ERROR << "Cannot get user by login: " << login_;
						return static_cast<QSharedPointer<DBEntity::DBUser>>(nullptr);
					}
				}
				else {
					PLOG_ERROR << "Cannot connect to the data base.";
					return static_cast<QSharedPointer<DBEntity::DBUser>>(nullptr);
				}
			}
			catch (const std::exception& exception)
			{
				PLOG_ERROR << "Exception in getUserByLogin method: " << exception.what();
				return static_cast<QSharedPointer<DBEntity::DBUser>>(nullptr);
			}
			});
	}

	QFuture<bool> UserRepository::createUser(const DBEntity::DBUser& user_) {
		return QtConcurrent::run([query_string_ = Helper::QueryHelper::createUser(), user_]() {
			try
			{
				auto connection = DBService::DBConnection_Service::getConnection();
				if (connection->getDatabase()->isOpen()) {

					QSqlQuery query(*connection->getDatabase());
					query.prepare(query_string_);

					const QString user_password = user_.getPassword();
					QCryptographicHash hash(QCryptographicHash::Sha256);
					hash.addData(user_password.toUtf8());

					query.bindValue(":login", user_.getLogin());
					query.bindValue(":password", QString(hash.result().toHex()));
					query.bindValue(":userpic", QVariant(user_.getUserpic()));
					query.bindValue(":rating", 0);
					query.bindValue(":is_deleted", false);

					if (query.exec()) {
						PLOG_INFO << "Created new User db entity with login: " << user_.getLogin();
						return true;
					}
					else {
						PLOG_ERROR << "Error adding a new User db entity.";
						return false;
					}
				}
				else {
					PLOG_ERROR << "Cannot connect to the data base.";
					return false;
				}
			}
			catch (const std::exception& exception)
			{
				PLOG_ERROR << "Exception in createUser method: " << exception.what();
				return false;
			}
			});
	}

	QFuture<bool> UserRepository::updateUserPasswordUserpic(const QString& login_, const QString& new_password_, const QByteArray& new_userpic_) {
		return QtConcurrent::run([new_password_, new_userpic_](const QString login_) {
			try {
				auto connection = DBService::DBConnection_Service::getConnection();
				if (connection->getDatabase()->isOpen()) {
					QSqlQuery query(*connection->getDatabase());
					QString query_string = "UPDATE [user] SET ";

					if (!new_password_.isEmpty()) {
						QCryptographicHash hash(QCryptographicHash::Sha256);
						hash.addData(new_password_.toUtf8());
						query_string += "password='" + QString(hash.result().toHex()) + "'";
					}
					if (!new_userpic_.isEmpty()) {
						if (!new_password_.isEmpty()) {
							query_string += ", ";
						}
						query_string += "userpic=:userpic";
						query.prepare(query_string);
						query.bindValue(":userpic", QVariant(new_userpic_), QSql::In | QSql::Binary);
					}
					else {
						query_string += "userpic=userpic";
						query.prepare(query_string);
					}

					query_string += " WHERE login=:login";
					query.bindValue(":login", login_);
					query.exec();
					//if (query.exec()) {
					//	PLOG_INFO << "User updated.";
					//	return true;
					//}
					//else {
					//	PLOG_ERROR << "Error updating a user.";
					//	return false;
					//}
				}
				else {
					PLOG_ERROR << "Cannot connect to the database.";
					return false;
				}
			}
			catch (const std::exception& exception) {
				PLOG_ERROR << "Exception in updateUserPasswordUserpic method: " << exception.what();
				return false;
			}
			}, login_);
	}

	QFuture<QPair<bool, qint32>> UserRepository::updateUserRating(const QString& login_, const qint32& rating_) {
		return QtConcurrent::run([login_, rating_]() {
			try
			{
				auto connection = DBService::DBConnection_Service::getConnection();
				if (connection->getDatabase()->isOpen()) {
					QSqlQuery query(*connection->getDatabase());
					QString query_string = Helper::QueryHelper::updateUserRating();
					query.prepare(query_string);
					query.bindValue(":rating", rating_);
					query.bindValue(":login", login_);

					if (query.exec()) {
						QSqlQuery queryResult;
						QString query_string_result = Helper::QueryHelper::getRatingByLogin();
						queryResult.prepare(query_string_result);
						queryResult.bindValue(":login", login_);

						qint32 new_rating = rating_;
						if (queryResult.exec() && queryResult.next()) {
							new_rating = queryResult.value("rating").toInt();
							PLOG_INFO << "User rating updated. User '" << login_ << "'" << " new rating: " << new_rating;
						}
						else {
							PLOG_ERROR << "Cannot get new rating.";
						}

						return qMakePair(true, new_rating);
					}
					else {
						PLOG_ERROR << "Error updating rating.";
						return qMakePair(false, -1);
					}
				}
				else {
					PLOG_ERROR << "Cannot connect to the database.";
					return qMakePair(false, -1);
				}
			}
			catch (const std::exception& exception)
			{
				PLOG_ERROR << "Exception in updateUserRating methhod: " << exception.what();
				return qMakePair(false, -1);
			}
			});
	}


	QFuture<qint32> UserRepository::getRatingByLogin(const QString& login_) {
		return QtConcurrent::run([login_]() {
			try
			{
				auto connection = DBService::DBConnection_Service::getConnection();
				if (connection->getDatabase()->isOpen()) {
					QSqlQuery query(*connection->getDatabase());
					QString query_string = Helper::QueryHelper::getRatingByLogin();
					query.prepare(query_string);
					query.bindValue(":login", login_);

					if (query.exec() && query.next()) {
						qint32 rating = query.value("rating").toInt();
						PLOG_INFO << "User '" << login_ << "' rating : " << rating;
						return rating;
					}
					else {
						PLOG_ERROR << "Cannot get rating.";
						return -1;
					}
				}
				else {
					PLOG_ERROR << "Cannot connect to the data base.";
					return -1;
				}
			}
			catch (const std::exception& exception)
			{
				PLOG_ERROR << "Exception in getRatingByLogin method: " << exception.what();
				return -1;
			}
			});
	}

}
