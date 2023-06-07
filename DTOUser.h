#ifndef DTOUSER_H
#define DTOUSER_H

#include <QString>
#include <QObject>
#include "DBUser.h"
#include "User.h"

namespace DTOHelper {

	class DTOUser : public QObject {

		Q_OBJECT

	public:
		DTOUser();

		DBEntity::DBUser convertToDBModel(const Entity::User& user_);
		Entity::User convertToEntity(const DBEntity::DBUser& db_user_);
	};
}

#endif
