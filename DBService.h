#ifndef DBSERVICE_H
#define DBSERVICE_H

#include <QtSql>
#include "DBRoom.h"

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
		QFuture<QList<DBEntity::DBRoom>> getRooms(const QString& query_string_);
		void databaseQueryFull(const QString& query_string_);
		bool addRoom(const QString& query_string_, const DBEntity::DBRoom& room);
		bool updateRoom(const QString& query_string_, const DBEntity::DBRoom& room);
	};
}

#endif
