#ifndef DBSERVICE_H
#define DBSERVICE_H

#include <QtSql>

namespace DBService {

	class DBConnection {

	private:
		QSqlDatabase a_database;
		QString a_connection_string;

	public:
		explicit DBConnection(const QString& connection_string_);

		QString getConnectionString() const;
		void setConnectionString(const QString& connection_string_);

		void databaseConnectionOpen();
		void databaseConnectionClose();
		void databaseQuery(const QString& query_string_);
		void getRooms(const QString& query_string_);
		void databaseQueryFull(const QString& query_string_);
	};
}

#endif
