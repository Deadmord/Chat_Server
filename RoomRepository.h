#ifndef ROOMREPOSITORY_H
#define ROOMREPOSITORY_H

#include "DBConncetion_Service.h"
#include "QueryHelper.h"
#include <QtConcurrent>
#include <plog/Log.h> 

namespace DBService {

	class RoomRepository : public QObject {

		Q_OBJECT

	private:
		static DBConnection_Service a_dbConnection;

	public:

		RoomRepository(const QString& connection_string_);

		~RoomRepository();
		static QFuture<QList<QSharedPointer<DBEntity::DBRoom>>> getAllRooms();
		static QList<QSharedPointer<DBEntity::DBRoom>> getAllActiveRooms();
		static QFuture<QSharedPointer<DBEntity::DBRoom>> getRoomById(const qint32& room_id_);
		static qint32 createRoom(const DBEntity::DBRoom& room_);
		static QFuture<bool> deleteRoom(const qint32& id_);
		static qint32 getTopicIdByTopicName(const QString& topic_name_);

	};
}

Q_DECLARE_METATYPE(DBService::RoomRepository)

#endif