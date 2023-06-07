#include "UserRepository.h"
#include <QtConcurrent>
#include <plog/Log.h> 
#include <QCryptographicHash>

namespace DBService {

	//UserRepository::UserRepository(DBConnection* connection_) : a_dbConnection(connection_) {}
	UserRepository::UserRepository(const QString& connection_string_) : a_dbConnection(connection_string_) {}
	UserRepository::~UserRepository(){}

	QFuture<DBEntity::DBUser*> UserRepository::getUserByLogin(const QString& login_) {
		return QtConcurrent::run([this, query_string_ = "SELECT * FROM [user] WHERE login=:login;", login_]() {
			try
			{
				a_dbConnection.databaseConnectionOpen();
				if (a_dbConnection.getDatabase().isOpen()) {
					QSqlQuery query;
					query.prepare(query_string_);
					query.bindValue(":login", login_);

					if (query.exec() && query.next()) {
						QString login = query.value("login").toString();
						QString password = query.value("password").toString();
						QByteArray userpic = query.value("userpic").toByteArray();
						qint32 rating = query.value("rating").toInt();
						bool is_deleted = query.value("is_deleted").toBool();

						a_dbConnection.databaseConnectionClose();
						DBEntity::DBUser* user = new DBEntity::DBUser(login, password, userpic, rating, is_deleted);
						return user;
					}
					else {
						PLOG_ERROR << "Cannot get user by login: " << login_;
						a_dbConnection.databaseConnectionClose();
						return static_cast<DBEntity::DBUser*>(nullptr);
					}
				}
				else {
					PLOG_ERROR << "Cannot connect to the data base.";
					a_dbConnection.databaseConnectionClose();
					return static_cast<DBEntity::DBUser*>(nullptr);
				}
			}
			catch (const std::exception& exception)
			{
				PLOG_ERROR << "Exception in getUserByLogin method: " << exception.what();
				return static_cast<DBEntity::DBUser*>(nullptr);
			}
			});
	}

	QFuture<bool> UserRepository::createUser(const DBEntity::DBUser& user_) {
		return QtConcurrent::run([this, query_string_ = "INSERT INTO [user] (login, password, userpic, rating, is_deleted) VALUES (:login, :password, :userpic, :rating, :is_deleted)", user_]() {
			try
			{
				a_dbConnection.databaseConnectionOpen();
				if (a_dbConnection.getDatabase().isOpen()) {

					QSqlQuery query;
					query.prepare(query_string_);

					QString userPassword = user_.getPassword();
					QCryptographicHash hash(QCryptographicHash::Sha256);
					hash.addData(userPassword.toUtf8());

					query.bindValue(":login", user_.getLogin());
					query.bindValue(":password", QString(hash.result().toHex()));
					query.bindValue(":userpic", user_.getUserpic());
					//query.bindValue(":rating", user_.getRating());
					query.bindValue(":rating", 0);
					query.bindValue(":is_deleted", false);

					if (query.exec()) {
						PLOG_INFO << "Created new User db entity with login: " << user_.getLogin();
						a_dbConnection.databaseConnectionClose();
						return true;
					}
					else {
						PLOG_ERROR << "Error adding a new User db entity.";
						a_dbConnection.databaseConnectionClose();
						return false;
					}
				}
				else {
					PLOG_ERROR << "Cannot connect to the data base.";
					a_dbConnection.databaseConnectionClose();
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

	QFuture<bool> UserRepository::updateUser(const QString& login_, const QString& new_password_, const QByteArray& new_userpic_) {
		return QtConcurrent::run([this, login_, new_password_, new_userpic_]() {
			try
			{
				a_dbConnection.databaseConnectionOpen();
				if (a_dbConnection.getDatabase().isOpen()) {
					QSqlQuery query;
					QString query_string = "UPDATE [user] SET ";;

					if (!new_password_.isEmpty()) {
						QCryptographicHash hash(QCryptographicHash::Sha256);
						hash.addData(new_password_.toUtf8());
						query_string += "password = '" + QString(hash.result().toHex()) + "'";
					}
					if (!new_userpic_.isEmpty()) {
						if (!new_password_.isEmpty()) {
							query_string += ", ";
						}
						query_string += "userpic=:userpic";// TODO: ne rabotaet ((((
						query.bindValue(":userpic", QVariant(new_userpic_));
					}

					query_string += " WHERE login=:login";
					query.prepare(query_string);
					query.bindValue(":login", login_);

					if (query.exec()) {
						PLOG_INFO << "User updated.";
						a_dbConnection.databaseConnectionClose();
						return true;
					}
					else {
						PLOG_ERROR << "Error updating a user.";
						a_dbConnection.databaseConnectionClose();
						return false;
					}
				}
				else {
					PLOG_ERROR << "Cannot connect to the data base.";
					a_dbConnection.databaseConnectionClose();
					return false;
				}
			}
			catch (const std::exception& exception)
			{
				PLOG_ERROR << "Exception in updateUser method: " << exception.what();
				return false;
			}
			});
	}

}
