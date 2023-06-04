#include "DBService.h"
#include "DBRoom.h"
#include <QtSql>
#include <QSqlQuery>
#include <QString>
#include <QtConcurrent>

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

	QFuture<QList<DBEntity::DBRoom>> DBConnection::getRooms(const QString& query_string_) {
		return QtConcurrent::run([this, query_string_]() {
			QList<DBEntity::DBRoom> roomList;
			databaseConnectionOpen();

			if (this->a_database.isOpen()) {
				QSqlQueryModel queryModel;
				queryModel.setQuery(query_string_);

				if (queryModel.lastError().isValid()) {
					qDebug() << "Query error: " << queryModel.lastError().text();
					return roomList;
				}

				const int rowCount = queryModel.rowCount();
				for (int i = 0; i < rowCount; ++i) {
					qint32 id = queryModel.record(i).value("id").toInt();
					QString name = queryModel.record(i).value("name").toString();
					QString description = queryModel.record(i).value("description").toString();
					qint32 topic_id = queryModel.record(i).value("topic_id").toInt();
					bool is_private = queryModel.record(i).value("is_private").toBool();
					QString password = queryModel.record(i).value("password").toString();
					bool is_deleted = queryModel.record(i).value("is_deleted").toBool();

					DBEntity::DBRoom room(id, name, description, topic_id, is_private, password, is_deleted);
					roomList.append(room);
				}

				databaseConnectionClose();

				if (!roomList.isEmpty()) {
					return roomList;
				}
				else {
					qDebug() << "No data found";
				}
			}

			return roomList;
		});
	}

	bool DBConnection::addRoom(const QString& query_string_, const DBEntity::DBRoom& room_) {
		databaseConnectionOpen();

		if (this->a_database.isOpen()) {

			QSqlQuery query;
			query.prepare(query_string_);

			query.bindValue(":name", room_.getName());
			query.bindValue(":description", room_.getDescription());
			query.bindValue(":topic_id", room_.getTopicId());
			query.bindValue(":is_private", room_.isPrivate());
			query.bindValue(":password", room_.getPassword());
			query.bindValue(":is_deleted", room_.isDeleted());

			if (query.exec()) {
				qint32 id = query.lastInsertId().toInt();
				qDebug() << "id of a new entity: " << id;
				databaseConnectionClose();
				return true;
			}
			else {
				qDebug() << "error adding an entity";
				databaseConnectionClose();
				return false;
			}
		}
		else {
			return false;
		}
	}

	bool DBConnection::updateRoom(const QString& query_string_, const DBEntity::DBRoom& room_) {
		databaseConnectionOpen();

		if (this->a_database.isOpen()) {

			QSqlQuery query;
			query.prepare(query_string_);

			query.bindValue(":id", room_.getId());
			query.bindValue(":name", room_.getName());
			query.bindValue(":description", room_.getDescription());
			query.bindValue(":topic_id", room_.getTopicId());
			query.bindValue(":is_private", room_.isPrivate());
			query.bindValue(":password", room_.getPassword());
			query.bindValue(":is_deleted", room_.isDeleted());

			if (query.exec()) {
				qDebug() << "room with ID " << room_.getId() << "was updated successfully";
				databaseConnectionClose();
				return true;
			}
			else {
				qDebug() << "room with ID " << room_.getId() << "was not updated";
				databaseConnectionClose();
				return false;
			}
		}
		else {
			return false;
		}
	}
}
