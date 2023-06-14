#include "DTORoom.h"

namespace DTOModel {

	DTORoom::DTORoom(QObject* parent) : QObject(parent) {};

	QSharedPointer<DBEntity::DBRoom> DTORoom::convertToDBModel(const RoomController& rc_) {
		QSharedPointer<DBEntity::DBRoom> dbRoom = QSharedPointer<DBEntity::DBRoom>::create(rc_.getId(), rc_.getName(), rc_.getDescription(), rc_.getTopicId(), rc_.isPrivate(), rc_.getPassword(), rc_.isDeleted());
		return dbRoom;
	}

	QSharedPointer<RoomController> DTORoom::convertToEntity(const DBEntity::DBRoom& room_) {
		QSharedPointer<RoomController> roomController = QSharedPointer<RoomController>::create(room_.getId(), room_.getName(), room_.getDescription(), room_.getTopicId(), room_.isPrivate(), room_.getPassword(), room_.isDeleted());
		return roomController;
	}
}