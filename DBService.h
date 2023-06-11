#ifndef DBSERVICE_H
#define DBSERVICE_H

#include <QtSql>
#include "DBRoom.h"
#include "DBRoom.h"
#include <QSqlQuery>
#include <QString>
#include <QtConcurrent>
#include <plog/Log.h> 

namespace DBService {

	class DBConnection {

	private:
		QSqlDatabase a_database;
		QString a_connection_string;

	public:
		explicit DBConnection(const QString& connection_string_);
		//~DBConnection();

		QSqlDatabase getDatabase() const;
		QString getConnectionString() const;
		void setConnectionString(const QString& connection_string_);
		void databaseConnectionOpen();
		void databaseConnectionOpenAsync();
		void databaseConnectionClose();
		void databaseConnectionCloseAsync();
		//QSqlQueryModel* databaseQuery(const QString& query_string_);
		void databaseQueryFull(const QString& query_string_);
	};
}

#endif
