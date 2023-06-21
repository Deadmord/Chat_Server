#include "DBConncetion_Service.h"

namespace DBService {

	QString DBConnection_Service::a_connection_string;
	QList<QSharedPointer<DBConnection_Service>> DBConnection_Service::connections_pool;
	quint32 DBConnection_Service::pool_size;

	void DBConnection_Service::openConnection()
	{
		try
		{
			shp_database = QSharedPointer<QSqlDatabase>::create(QSqlDatabase::addDatabase("QODBC", "connection" + QString::number(connection_id)));
			shp_database->setDatabaseName(a_connection_string);
			if (shp_database->open()) {

				PLOG_INFO << "Connected to DB. Connection id: " + QString::number(connection_id);
			}
			else PLOGE << "DB " + shp_database->lastError().databaseText() + shp_database->connectionName();
		}
		catch (const std::exception& exception)
		{
			PLOG_ERROR << "Exception while connecting to DB. Connection id: " + QString::number(connection_id) << exception.what();
		}
	}

	void DBConnection_Service::closeConnection()
	{
		try
		{
			if (shp_database->isOpen()) {

				shp_database->close();
				PLOG_INFO << "Connection to db closed. Connection id: " + QString::number(connection_id);
			}
		}
		catch (const std::exception& exception)
		{
			PLOG_ERROR << "Exception while closing connection to db. Connection id: " + QString::number(connection_id) << exception.what();
		}
	}

	QSharedPointer<DBConnection_Service> DBConnection_Service::getConnection()
	{
		auto rand = QRandomGenerator::global()->bounded(pool_size);
		return connections_pool[rand];
	}

	DBConnection_Service::DBConnection_Service(const QString& connection_string_, const quint32& connection_id_) : connection_id(connection_id_) { a_connection_string = connection_string_; };

	QFuture<void> DBConnection_Service::init(qint32 pool_size_)
	{
		pool_size = pool_size_;
		for (qint32 i = 0; i < pool_size; i++)
		{
			auto new_connection = QSharedPointer<DBConnection_Service>(new DBConnection_Service(a_connection_string, i));
			connections_pool.append(new_connection);
		}
		PLOGD << "Initializing DBConnection_Service...";
		return QtConcurrent::map(connections_pool, [](QSharedPointer<DBConnection_Service> connection) {
			connection->openConnection();
		});
	}

	QFuture<void> DBConnection_Service::exit()
	{
		PLOGD << "Exiting DBConnection_Service...";
		return QtConcurrent::map(connections_pool, [](QSharedPointer<DBConnection_Service> connection) {
			connection->closeConnection();
		});
	}

	QSharedPointer<QSqlDatabase> DBConnection_Service::getDatabase() { return shp_database; }
	QString DBConnection_Service::getConnectionString() { return a_connection_string; }
	void DBConnection_Service::setConnectionString(const QString& connection_string_) { a_connection_string = connection_string_; }


}
