#ifndef DTOROOM_H
#define DTOROOM_H

#include <QString>
#include <QObject>
#include "DBRoom.h"
#include "Room.h"
#include "Controllers/RoomController/RoomController.h"

namespace DTOHelper {

	class DTORoom : public QObject {

		Q_OBJECT

	public:

		DTORoom(QObject* parent = nullptr);
		static QSharedPointer<DBEntity::DBRoom> convertToDBModel(const RoomController& rc_);
		static QSharedPointer<RoomController> convertToEntity(const DBEntity::DBRoom& room_);
	};
}

Q_DECLARE_METATYPE(DTOHelper::DTORoom)

#endif 