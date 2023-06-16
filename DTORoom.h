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
		DTORoom(const qint32& id_, const QString& name_, const QString& description_, const qint32& topic_id_, const QString& topic_name_, const bool& is_private_, const QString& password_, const bool& is_deleted_ = false);
		~DTORoom();

		qint32 getId() const;
		void setId(const qint32& id_);
		QString getName() const;
		void setName(const QString& name_);
		QString getDescription() const;
		void setDescription(const QString& description_);
		qint32 getTopicId() const;
		void setTopicId(const qint32& topic_id_);
		QString getTopicName() const;
		void setTopicName(const QString& topic_name_);
		bool isPrivate() const;
		void setIsPrivate(const bool& is_private_);
		QString getPassword() const;
		void setPassword(const QString& password_);
		bool isDeleted() const;
		void setIsDeleted(const bool& is_deleted);
		QSet<QSharedPointer<User_Message>> getMessages() const;
		void setMessages(QSharedPointer<User_Message> shp_user_message_);
		QList<SrvUser*> getConnectedUsers() const;
		void setConnectedUsers(SrvUser* connected_user_);

		static QSharedPointer<DTOModel::DTORoom> createDTORoomFromSrv(const SrvRoom& srv_room);
		static QSharedPointer<SrvRoom> createSrvRoomFromDTO(const DTOModel::DTORoom& dto_model);
		static QSharedPointer<DBEntity::DBRoom> createDBRoomFromSrv(const SrvRoom& srv_room);
		static QSharedPointer<SrvRoom> createSrvRoomFromDB(const DBEntity::DBRoom& db_room);

	private:

		qint32 id;
		QString name;
		QString description;
		qint32 topic_id;
		QString topic_name;
		bool is_private;
		QString password;
		bool is_deleted;

		QSet<QSharedPointer<User_Message>> messages;
		QList<SrvUser*> connected_users;
	};
}

Q_DECLARE_METATYPE(DTOModel::DTORoom)

#endif 