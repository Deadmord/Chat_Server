#include "DBService.h"
#include "DBRoom.h"
#include <QtSql>
#include <QSqlQuery>
#include <QString>
#include <QtConcurrent>
#include <plog/Log.h> 

namespace DBService {

	DBConnection::DBConnection(const QString& connection_string_) : a_connection_string(connection_string_) {};

	QSqlDatabase DBConnection::getDatabase() const { return this->a_database; }
	QString DBConnection::getConnectionString() const { return this->a_connection_string; }
	void DBConnection::setConnectionString(const QString& connection_string_) { this->a_connection_string = connection_string_; }

	void DBConnection::databaseConnectionOpen() {
		try
		{
			this->a_database = QSqlDatabase::addDatabase("QODBC");
			a_database.setDatabaseName(DBConnection::getConnectionString());
			if (a_database.open()) {
				PLOG_INFO << "connected to db.";
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

	void DBConnection::databaseConnectionOpenAsync() {
		QtConcurrent::run([this]() {
			try 
			{
				a_database = QSqlDatabase::addDatabase("QODBC");
				a_database.setDatabaseName(DBConnection::getConnectionString());

				if (a_database.isOpen()) {
					PLOG_INFO << "Async connection to db.";
				}
				else {
					PLOG_ERROR << "Falied to connect to db.";
				}
			}
			catch (const std::exception& exception)
			{
				PLOG_ERROR << "Exception while async connecting to db: " << exception.what();
			}
			});
	}

	void DBConnection::databaseConnectionCloseAsync() {
		QtConcurrent::run([this]() {
			try
			{
				if (a_database.isOpen()) {
					a_database.close();
					PLOG_INFO << "async connection to db closed.";
				}
			}
			catch (const std::exception& exception)
			{
				PLOG_ERROR << "Exception while async closing connection to db: " << exception.what();
			}
			});
	}

	//QSqlQueryModel* DBConnection::databaseQuery(const QString& query_string_) {
	//	QSqlQueryModel* model = new QSqlQueryModel;
	//	model->setQuery(query_string_);
	//	return model;
	//}

	//void DBConnection::databaseQuery(const QString& query_string, const QVariantList& parameters_) {
	//	QSqlQuery query;
	//	query.prepare(query_string);

	//	for (const QVariant& parameter : parameters_) {
	//		query.addBindValue(parameter);
	//	}

	//	if (query.exec()) {
	//		QSqlRecord record = query.record();
	//		qint32 field_count = record.count();

	//		while (query.next()) {
	//			for (int i = 0; i < field_count; i++) {
	//				qDebug() << record.fieldName(i) << ": " << query.value(i);
	//			}
	//		}
	//	}
	//	else {
	//		qDebug() << "Query error: " << query.lastError().text();
	//	}
	//}

	void DBConnection::databaseQueryFull(const QString& query_string_) {

		QSqlQuery query;
		query.exec(query_string_);

		QSqlRecord record = query.record();
		qint32 field_count = record.count();

		while (query.next()) {
			for (int i = 0; i < field_count; i++) {
				qDebug() << record.fieldName(i) << " : " << query.value(i).toString();
			}
			qDebug() << "----------------------";
		}

	}
}
