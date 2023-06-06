#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include "DBService.h"

namespace DBService {

	class RoomRepository : public QObject {

		Q_OBJECT

	private:
		DBConnection* a_dbConnection;
	public:
		explicit RoomRepository(DBConnection* connection_);
		~RoomRepository();
		QFuture<QList<DBEntity::DBRoom>> getAllRooms();
		QFuture<QList<DBEntity::DBRoom>> getAllActiveRooms();
		QFuture<qint32> createRoom(const DBEntity::DBRoom& room);
		//bool updateRoom(const QString& query_string_, const DBEntity::DBRoom& room);
		QFuture<bool> deleteRoom(const qint32& id_);
	};
}

#endif