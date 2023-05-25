#include "DBService.h"
#include "DBRoom.h"
#include <QtSql>
#include <QSqlQuery>
#include <QString>

namespace DBService {

	DBConnection::DBConnection(const QString& connection_string_) : a_connection_string(connection_string_) {};

	QString DBConnection::getConnectionString() const { return this->a_connection_string; }
	void DBConnection::setConnectionString(const QString& connection_string_) { this->a_connection_string = connection_string_; }

	void DBConnection::databaseConnectionOpen() {

		this->a_database = QSqlDatabase::addDatabase("QODBC");
		a_database.setDatabaseName(DBConnection::getConnectionString());
		if (a_database.open()) {
			qDebug() << "connected to db";
		}
		else {
			qDebug() << "did not connected to db";
			qDebug() << a_database.lastError().text();
		}
	}

	void DBConnection::databaseConnectionClose() {
		if (a_database.isOpen()) {
			a_database.close();
			qDebug() << "connection to db closed";
		}
	}

	void DBConnection::databaseQuery(const QString& query_string_) {

		QSqlQuery query;
		query.exec(query_string_);

		QSqlRecord record = query.record();
		qint32 field_count = record.count();

		while (query.next()) {
			for (int i = 0; i < field_count; i++) {
				qDebug() << record.fieldName(i) << ": " << query.value(i);
			}
		}
	}

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

	void DBConnection::getRooms(const QString& query_string_) {
		QList<DBEntity::DBRoom> roomList;

		QSqlQuery query;
		query.exec(query_string_);

		while (query.next()) {
			qint32 id = query.value("id").toInt();
			QString name = query.value("name").toString();
			QString description = query.value("description").toString();
			qint32 topic_id = query.value("topic_id").toInt();
			bool is_private = query.value("is_private").toBool();
			QString password = query.value("password").toString();
			bool is_deleted = query.value("is_deleted").toBool();

			DBEntity::DBRoom room(id, name, description, topic_id, is_private, password, is_deleted);
			roomList.append(room);
		}
		for (const DBEntity::DBRoom& room : roomList) {
			qDebug() << "ID: " << room.getId() << " Name: " << room.getName();
		}
	}
}
