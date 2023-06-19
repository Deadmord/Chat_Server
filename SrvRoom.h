#pragma once
#ifndef SRVROOM_H
#define SRVROOM_H

#include <qtypes.h>
#include <qmetatype.h>
#include <QSharedPointer>
#include <QtConcurrent>
#include <algorithm>


#include "Message.h"
#include "SrvUser.h"

namespace DBEntity {

	class DBRoom;
}


class SrvRoom : public QObject
{
	Q_OBJECT

public:
	//explicit SrvRoom(const QSharedPointer<DTOHelper::DTORoom> cshp_dto_room_, QObject* parent = nullptr);
	explicit SrvRoom(QSharedPointer<DBEntity::DBRoom> cshp_db_room_, QObject* p_parent_ = nullptr);
	explicit SrvRoom(const qint32& id_, const QString& name_, const QString& description_, const qint32& topic_id_, const QString& topic_name_, const bool& is_private_, const QString& password_, const bool& is_deleted_);
	explicit SrvRoom(const QString& name_, const QString& description_, const QString& topic_name_, const bool& is_private_, const QString& password_, const bool& is_deleted_ = false);

	~SrvRoom() = default;

	[[nodiscard]] quint32 getId() const;
	[[nodiscard]] QString getName() const;
	[[nodiscard]] QString getDescription() const;
	[[nodiscard]] quint32 getTopicId() const;
	[[nodiscard]] QString getTopicName() const;
	[[nodiscard]] QString getPassword() const;		//TODO!!! It seems like, should be changed on "bool checkPassword" !!!
	[[nodiscard]] bool isDeleted() const;
	[[nodiscard]] bool isPrivate() const;
	[[nodiscard]] QSet<QSharedPointer<SrvUser>> getConnectedUsers() const;
	[[nodiscard]] QSet<QSharedPointer<User_Message>> getMessages() const;
	[[nodiscard]] QSet<QSharedPointer<User_Message>> getMessages(const QDateTime& from_, const QDateTime& to_) const;
	// If from_to_ == false - search for older than time_
	[[nodiscard]] QSet<QSharedPointer<User_Message>> getMessages(const QDateTime& time_, const bool from_to_, const quint32& pool_size_) const; 
	


	//static void initializeList(const QVector<DBEntity::DBRoom>& rooms_list_);
	//static void createNew(const QVector<DBEntity::DBRoom>& roomsList, DBEntity::DBRoom new_room_);
	//static void deleteOne(const QVector<DBEntity::DBRoom>& roomsList, DBEntity::DBRoom deleted_room_);


	void addMessages(const QSet<QSharedPointer<User_Message>>& messages_);
	void addMessage(const QSharedPointer<User_Message>& p_message_);
	void setName(const QString& val);
	void setDescription(const QString& val);
	void setPrivate(bool val);
	void setTopicName(const QString& val);
	void setTopicId(const quint32& val);
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

	[[nodiscard]] bool connectUser(const QSharedPointer<SrvUser>& shp_user_);
	[[nodiscard]] bool disconnectUser(const QSharedPointer<SrvUser>& shp_user_);

private:
	quint32 id;
	QString name;
	QString description;
	qint32	topic_id;
	QString topic_name;
	bool	is_private;
	QString password;
	bool	is_deleted = false;

	QSet<QSharedPointer<User_Message>> messages;
	QSet<QSharedPointer<SrvUser>> connected_users;
};

#endif //SRVROOM_H