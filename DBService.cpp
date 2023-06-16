#include "DBService.h"


namespace DBService {

	QSqlDatabase DBConnection::a_database;
	QString DBConnection::a_connection_string;

	DBConnection::DBConnection(const QString& connection_string_) { a_connection_string = connection_string_; };

	QSqlDatabase DBConnection::getDatabase() { return a_database; }
	QString DBConnection::getConnectionString() { return a_connection_string; }
	void DBConnection::setConnectionString(const QString& connection_string_) { a_connection_string = connection_string_; }

	void DBConnection::databaseConnectionOpen() {
		try
		{
			a_database = QSqlDatabase::addDatabase("QODBC");
			a_database.setDatabaseName(a_connection_string);
			if (a_database.open()) {
				PLOG_INFO << "connected to db.";
			}
			else {
				PLOG_ERROR << a_database.lastError().text();
			}
		}
		catch (const std::exception& exception)
		{
			PLOG_ERROR << "Exception while connecting to db: " << exception.what();
		}
	}

	void DBConnection::databaseConnectionClose() {
		try
		{
			if (a_database.isOpen()) {
				a_database.close();
				PLOG_INFO << "connection to db closed.";
			}
		}
		catch (const std::exception& exception)
		{
			PLOG_ERROR << "Exception while closing connection to db: " << exception.what();
		}
	}

}
