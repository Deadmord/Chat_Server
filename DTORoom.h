#ifndef DTOROOM_H
#define DTOROOM_H

#include <QString>
#include <QObject>
#include "DBRoom.h"
#include "Room.h"

namespace DTOHelper {

	class DTORoom : public QObject {

		Q_OBJECT

	public:
		DTORoom();

		DBEntity::DBRoom convertToDBModel();
		Entity::Room convertToEntity(const DBEntity::DBRoom& room_);
	};
}

#endif 
