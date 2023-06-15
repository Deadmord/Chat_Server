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
		static QSqlDatabase a_database;
		static QString a_connection_string;

	public:
		DBConnection(const QString& connection_string_);
		//~DBConnection();

		static QSqlDatabase getDatabase();
		static QString getConnectionString() ;
		static void setConnectionString(const QString& connection_string_);
		static void databaseConnectionOpen();
		static void databaseConnectionClose();

	};
}

#endif
