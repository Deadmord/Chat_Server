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

	class DBConnection_Service {

	private:
		static QString a_connection_string;
		static QList<QSharedPointer<DBConnection_Service>> connections_pool;
		static quint32 pool_size;
		qint32 connection_id;
		QSharedPointer<QSqlDatabase> shp_database;

	public:
		static QFuture<void> init(qint32 pool_size_ = 5);
		static QFuture<void> exit();

		void openConnection();
		void closeConnection();
		static QSharedPointer<DBConnection_Service> getConnection();
		QSharedPointer<QSqlDatabase> getDatabase();

		DBConnection_Service(const QString& connection_string_, const quint32& connection_id_);

		static QString getConnectionString() ;
		static void setConnectionString(const QString& connection_string_);

	};
}

#endif
