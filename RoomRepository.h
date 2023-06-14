#ifndef ROOMREPOSITORY_H
#define ROOMREPOSITORY_H

#include "DBService.h"
#include <QtConcurrent>
#include <plog/Log.h> 

namespace DBService {

	class RoomRepository : public QObject {

		Q_OBJECT

	private:
		static DBConnection a_dbConnection;

	public:
		RoomRepository(const QString& connection_string_);

		~RoomRepository();
		static QFuture<QList<QSharedPointer<DBEntity::DBRoom>>> getAllRooms();
		static QFuture<QList<QSharedPointer<DBEntity::DBRoom>>> getAllActiveRooms();
		static QFuture<qint32> createRoom(const DBEntity::DBRoom& room_);
		static QFuture<bool> deleteRoom(const qint32& id_);
	};
}

Q_DECLARE_METATYPE(DBService::RoomRepository)

#endif