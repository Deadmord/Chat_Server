#include "RoomRepository.h"

namespace DBService {

	DBConnection_Service RoomRepository::a_dbConnection("Driver={ODBC Driver 18 for SQL Server};Server=tcp:comp-zionet-server.database.windows.net,1433;Database=CPP_Chat_DB;Uid=Logika4417;Pwd=Fyyf1998;Encrypt=yes;TrustServerCertificate=no;Connection Timeout=30;", 0);
	RoomRepository::RoomRepository(const QString& connection_string_) { a_dbConnection.setConnectionString(connection_string_); }
	RoomRepository::~RoomRepository() {}

	QFuture<QList<QSharedPointer<DBEntity::DBRoom>>> RoomRepository::getAllRooms() {
		return QtConcurrent::run([query_string_ = "SELECT r.*, t.name AS topic_name FROM room r JOIN topic t ON r.topic_id=t.id;"]() {
			QList<QSharedPointer<DBEntity::DBRoom>> room_list;
			try
			{
				auto connection = DBService::DBConnection_Service::getConnection();

				if (connection->getDatabase()->isOpen()) {
					QSqlQueryModel query_model;
					query_model.setQuery(query_string_, *connection->getDatabase());

					if (query_model.lastError().isValid()) {
						PLOG_ERROR << "Query error: " << query_model.lastError().text();
						return room_list;
					}

					const int row_count = query_model.rowCount();
					for (int i = 0; i < row_count; ++i) {
						qint32 id = query_model.record(i).value("id").toInt();
						QString name = query_model.record(i).value("name").toString();
						QString description = query_model.record(i).value("description").toString();
						qint32 topic_id = query_model.record(i).value("topic_id").toInt();
						QString topic_name = query_model.record(i).value("topic_name").toString();
						bool is_private = query_model.record(i).value("is_private").toBool();
						QString password = query_model.record(i).value("password").toString();
						bool is_deleted = query_model.record(i).value("is_deleted").toBool();

						auto shp_room = QSharedPointer<DBEntity::DBRoom>(new DBEntity::DBRoom(id, name, description, topic_id, topic_name, is_private, password, is_deleted));
						room_list.append(shp_room);
					}

					if (!room_list.isEmpty()) {
						return room_list;
					}
					else {
						PLOG_INFO << "No data found.";
					}
				}
				else {
					PLOG_ERROR << "Cannot connect to the database.";
				}
			}
			catch (const std::exception& exception)
			{
				PLOG_ERROR << "Exception in getAllRooms method: " << exception.what();
			}

			return room_list;
			});
	}








	QList<QSharedPointer<DBEntity::DBRoom>> RoomRepository::getAllActiveRooms() {
			auto query_string_ = "SELECT r.*, t.name AS topic_name FROM room r JOIN topic t ON r.topic_id=t.id WHERE r.is_deleted=0;";
			QList<QSharedPointer<DBEntity::DBRoom>> room_list;
			try
			{
				auto connection = DBService::DBConnection_Service::getConnection();

				if (connection->getDatabase()->isOpen()) {
					QSqlQueryModel query_model;
					query_model.setQuery(query_string_, *connection->getDatabase());

					if (query_model.lastError().isValid()) {
						PLOG_ERROR << "Query error: " << query_model.lastError().text();
						return room_list;
					}

					const int rowCount = query_model.rowCount();
					for (int i = 0; i < rowCount; ++i) {
						qint32 id = query_model.record(i).value("id").toInt();
						QString name = query_model.record(i).value("name").toString();
						QString description = query_model.record(i).value("description").toString();
						qint32 topic_id = query_model.record(i).value("topic_id").toInt();
						QString topic_name = query_model.record(i).value("topic_name").toString();
						bool is_private = query_model.record(i).value("is_private").toBool();
						QString password = query_model.record(i).value("password").toString();
						bool is_deleted = query_model.record(i).value("is_deleted").toBool();

						auto shp_room = QSharedPointer<DBEntity::DBRoom>(new DBEntity::DBRoom (id, name, description, topic_id, topic_name, is_private, password, is_deleted));
						room_list.append(shp_room);
					}

					if (!room_list.isEmpty()) {
						return room_list;
					}
					else {
						PLOG_INFO << "No data found.";
					}
				}
				else {
					PLOG_ERROR << "Cannot connect to the data base.";
				}
			}
			catch (const std::exception& exception)
			{
				PLOG_ERROR << "Exception in getAllActiveRooms method: " << exception.what();
			}
			return room_list;
	}

	QFuture<QSharedPointer<DBEntity::DBRoom>> RoomRepository::getRoomById(const qint32& room_id_) {
		return QtConcurrent::run([query_string_ = "SELECT r.*, t.name AS topic_name FROM room r JOIN topic t ON r.topic_id = t.id WHERE r.id=:id;", &room_id_]() {
			try
			{
				auto connection = DBService::DBConnection_Service::getConnection();

				if (connection->getDatabase()->isOpen()) {
					QSqlQuery query(*connection->getDatabase());
					query.prepare(query_string_);
					query.bindValue(":id", room_id_);

					if (query.exec() && query.next()) {
						QString name = query.value("name").toString();
						QString description = query.value("description").toString();
						qint32 topic_id = query.value("topic_id").toInt();
						QString topic_name = query.value("topic_name").toString();
						bool is_private = query.value("is_private").toBool();
						QString password = query.value("password").toString();
						bool is_deleted = query.value("is_deleted").toBool();
						QSharedPointer<DBEntity::DBRoom> shp_room = QSharedPointer<DBEntity::DBRoom>::create(room_id_, name, description, topic_id, topic_name, is_private, password);
						return shp_room;;
					}
					else {
						PLOG_ERROR << "Cannot get rom by id: " << room_id_;
						return static_cast<QSharedPointer<DBEntity::DBRoom>>(nullptr);
					}
				}
				else {
					PLOG_ERROR << "Cannot connect to the data base.";
					return static_cast<QSharedPointer<DBEntity::DBRoom>>(nullptr);
				}
			}
			catch (const std::exception& exception)
			{
				PLOG_ERROR << "Exception in getRoomById method: " << exception.what();
				return static_cast<QSharedPointer<DBEntity::DBRoom>>(nullptr);
			}
			});
	}

	QFuture<qint32> RoomRepository::createRoom( const DBEntity::DBRoom& room_) {
		return QtConcurrent::run([query_string_ = "INSERT INTO room (name, description, topic_id, is_private, password, is_deleted) VALUES (:name, :description, :topic_id, :is_private, :password, :is_deleted)", &room_]() {
			try
			{
				auto connection = DBService::DBConnection_Service::getConnection();
				if (connection->getDatabase()->isOpen()) {

					QSqlQuery query(*connection->getDatabase());
					query.prepare(query_string_);

					query.bindValue(":name", room_.getName());
					query.bindValue(":description", room_.getDescription());
					query.bindValue(":topic_id", room_.getTopicId());
					query.bindValue(":is_private", room_.isPrivate());
					query.bindValue(":password", room_.getPassword());
					query.bindValue(":is_deleted", false);

					if (query.exec()) {
						qint32 id = query.lastInsertId().toInt();
						PLOG_INFO << "ID of a new Room db entity: " << id;
						return id;
					}
					else {
						PLOG_ERROR << "Error adding a new Room db entity.";
						return -1;
					}
				}
				else {
					PLOG_ERROR << "Cannot connect to the data base.";
					return -1;
				}
			}
			catch (const std::exception& exception)
			{
				PLOG_ERROR << "Exception in createRoom method: " << exception.what();
				return -1;
			}
			});
	}

	QFuture<bool> RoomRepository::deleteRoom(const qint32& id_) {
		return QtConcurrent::run([query_string_ = "UPDATE room SET is_deleted=:is_deleted WHERE id=:id", id_]() {
			try
			{
				auto connection = DBService::DBConnection_Service::getConnection();
				if (connection->getDatabase()->isOpen()) {

					QSqlQuery query(*connection->getDatabase());
					query.prepare(query_string_);

					query.bindValue(":id", id_);
					query.bindValue(":is_deleted", true);

					if (query.exec()) {
						PLOG_INFO << "Room with ID: " << id_ << " was deleted successfully.";
						return true;
					}
					else {
						PLOG_ERROR << "Room with ID: " << id_ << " was not deleted.";
						return false;
					}
				}
				else {
					PLOG_ERROR << "Cannot connect to the data base.";
					return false;
				}
			}
			catch (const std::exception& exception)
			{
				PLOG_ERROR << "Exception in deleteRoom method: " << exception.what();
				return false;
			}
			});
	}
}
