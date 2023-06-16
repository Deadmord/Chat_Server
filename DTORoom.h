#ifndef DTOROOM_H
#define DTOROOM_H

#include <QString>
#include <QObject>
#include <QSharedPointer>
#include "DBRoom.h"
#include "SrvRoom.h"

namespace DTOModel {

	class DTORoom : public QObject {

		Q_OBJECT

	public:

		DTORoom(QObject* parent = nullptr);
		DTORoom(const qint32& id_, const QString& name_, const QString& description_, const qint32& topic_id_, const bool& is_private_, const QString& password_);
		~DTORoom();

		qint32 getId() const;
		void setId(const qint32& id_);
		QString getName() const;
		void setName(const QString& name_);
		QString getDescription() const;
		void setDescription(const QString& description_);
		qint32 getTopicId() const;
		void setTopicId(const qint32& topic_id_);
		bool isPrivate() const;
		void setIsPrivate(const bool& is_private_);
		QString getPassword() const;
		void setPassword(const QString& password_);

		//static QSharedPointer<DTOModel::DTORoom> createDTORoom(const DBEntity::DBRoom& db_room_);
		//static QSharedPointer<DBEntity::DBRoom> createDBRoom(const DTOModel::DTORoom& dto_room_);

	private:

		qint32 a_id;
		QString a_name;
		QString a_description;
		qint32 a_topic_id;
		bool a_is_private;
		QString a_password;
	};
}

Q_DECLARE_METATYPE(DTOModel::DTORoom)

#endif 