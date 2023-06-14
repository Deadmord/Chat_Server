#ifndef DTOROOM_H
#define DTOROOM_H

#include <QString>
#include <QObject>
#include "DBRoom.h"
#include "Room.h"
#include "Controllers/RoomController/RoomController.h"

namespace DTOModel {

	class DTORoom : public QObject {

		Q_OBJECT

	public:

		DTORoom(QObject* parent = nullptr);

		static QSharedPointer<DBEntity::DBRoom> convertToDBModel(const RoomController& rc_);
		static QSharedPointer<RoomController> convertToEntity(const DBEntity::DBRoom& room_);

	private:
		qint32 a_id;

	};
}

Q_DECLARE_METATYPE(DTOModel::DTORoom)

#endif 