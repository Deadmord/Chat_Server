#pragma once
#ifndef SRVROOM_H
#define SRVROOM_H

#include <qtypes.h>
#include <qmetatype.h>


#include "Entities/SrvModels/Message.h"
#include "Entities/SrvModels/UserConnection.h"

class SrvRoom : public QObject
{
	Q_OBJECT

public:
	explicit SrvRoom(const quint32& id_, const QString& name_, const QString& description_, const quint32& topic_id_, const bool& is_private_, const QString& password_, bool is_deleted_, QObject* parent = nullptr);
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
	void addMessage(const User_Message& message_);
	void connectUser(UserConnection* user);

private:
	quint32 id;
	QString name;
	QString description;
	quint32	topic_id;
	bool	is_private;
	QString password;
	bool	is_deleted;

	QString msg_history_path = "./msg_history.json"; //get automaticly
	QList<User_Message*> messages;
	QList<UserConnection*> connected_users;
};

#endif //SRVROOM_H