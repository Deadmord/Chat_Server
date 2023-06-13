#ifndef DTOUSER_H
#define DTOUSER_H

#include <QString>
#include <QObject>
#include "DBUser.h"
#include "User.h"
#include "Entities/entities.h"

namespace DTOHelper {

	class DTOUser : public QObject {

		Q_OBJECT

	public:
		DTOUser(QObject* parent = nullptr);
		static QSharedPointer<DBEntity::DBUser> convertToDBModel(const User& user_);
		static QSharedPointer<User> convertToEntity(const DBEntity::DBUser& db_user_);
	};

}

Q_DECLARE_METATYPE(DTOHelper::DTOUser)

#endif