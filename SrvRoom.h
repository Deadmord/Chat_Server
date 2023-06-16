#pragma once
#ifndef SRVROOM_H
#define SRVROOM_H

#include <qtypes.h>
#include <qmetatype.h>
#include <QSharedPointer>


#include "Message.h"
#include "UserConnection.h"

namespace DBEntity {

	class DBRoom;
}


class SrvRoom : public QObject
{
	Q_OBJECT

public:
	//explicit SrvRoom(const QSharedPointer<DTOHelper::DTORoom> cshp_dto_room_, QObject* parent = nullptr);
	explicit SrvRoom(QSharedPointer<DBEntity::DBRoom> cshp_db_room_, QObject* p_parent_ = nullptr);
	~SrvRoom() = default;

	[[nodiscard]] quint32 getId() const;
	[[nodiscard]] QString getName() const;
	[[nodiscard]] QString getDescription() const;
	[[nodiscard]] quint32 getTopicId() const;
	[[nodiscard]] QString getPassword() const;		//TODO!!! It seems like, should be changed on "bool checkPassword" !!!
	[[nodiscard]] bool isDeleted() const;
	[[nodiscard]] bool isPrivate() const;
	[[nodiscard]] QList<UserConnection*> getConnectedUsers() const;
	[[nodiscard]] QList<User_Message*> getMessages(const QDateTime& from_, const QDateTime& to_) const;


	//static void initializeList(const QVector<DBEntity::DBRoom>& rooms_list_);
	//static void createNew(const QVector<DBEntity::DBRoom>& roomsList, DBEntity::DBRoom new_room_);
	//static void deleteOne(const QVector<DBEntity::DBRoom>& roomsList, DBEntity::DBRoom deleted_room_);


	void addMessages(QSet<QSharedPointer<User_Message>> messages_);
	void addMessage(QSharedPointer<User_Message> p_message_);
	QSet<QSharedPointer<User_Message>> getMessages();
	void setName(const QString& val);
	void setDescription(const QString& val);
	void setPrivate(bool val);
	void setPassword(const QString& val);
	void Delete();

signals:

	void roomCreated(const QSharedPointer<SrvRoom> shp_room_);
	void nameChanged(const QString& val);
	void descriptionChanged(const QString& val);
	void privateChanged(bool val);
	void passwordChanged();
	void roomDeleted();

public slots:

	void initRoom();
	void connectUser(UserConnection* user);

private:
	quint32 id;
	QString name;
	QString description;
	quint32	topic_id;
	bool	is_private;
	QString password;
	bool	is_deleted;

	QSet<QSharedPointer<User_Message>> messages;
	QList<UserConnection*> connected_users;
};

#endif //SRVROOM_H