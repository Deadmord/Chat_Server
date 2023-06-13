#pragma once
#include <QCoreApplication>
#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QFile>

#include "../../DBRoom.h"
#include "../../Entities/Enums/Enums.h"
#include "../../Entities/SrvModels/Message.h"
#include "../../Entities/SrvModels/UserConnection.h"


class RoomController : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(RoomController)
	Q_PROPERTY(QString name MEMBER name READ getName WRITE setName NOTIFY nameChanged)

public:
	explicit RoomController(const quint32& id_, const QString& name_, const QString& description_, const quint32& topic_id_, const bool& is_private_, const QString& password_, bool is_deleted_, QObject* parent = nullptr);
	explicit RoomController(const quint32& id_, const QString& name_, const QString& description_, const quint32& topic_id_, const bool& is_private_, const QString& password_, const bool& is_deleted_);
	~RoomController() override;

	static void initializeList(const QVector<DBEntity::DBRoom>& rooms_list_);
	static void createNew(const QVector<DBEntity::DBRoom>& roomsList, DBEntity::DBRoom new_room_);
	static void deleteOne(const QVector<DBEntity::DBRoom>& roomsList, DBEntity::DBRoom deleted_room_);

	[[nodiscard]] quint32 getId() const;
	[[nodiscard]] QString getName() const;
	[[nodiscard]] QString getDescription() const;
	[[nodiscard]] quint32 getTopicId() const;
	[[nodiscard]] bool	  isPrivate() const;
	[[nodiscard]] QString getPassword() const;		//TODO!!! It seems like, should be changed on "bool checkPassword" !!!
	[[nodiscard]] bool	  isDeleted() const;

	void setName(const QString& val);
	void setDescription(const QString& val);
	void setPrivate(bool val);
	void setPassword(const QString& val);
	void Delete();

signals:

	void roomCreated(const RoomController* room);
	void nameChanged(const QString& val);
	void descriptionChanged(const QString& val);
	void privateChanged(bool val);
	void passwordChanged();
	void roomDeleted();

public slots:
	void initRoom();
	void userEntry(UserConnection* user);
	void jsonReceived(UserConnection* sender, const QJsonObject& doc);

private:
	void broadcastSend(const QJsonObject& message, UserConnection* exclude);
	void sendJson(UserConnection* destination, const QJsonObject& message);
	void loadMsgHistory(const QString path);
	void uploadMsgHistory(const QString path);

private:
	quint32 id;
	QString name;
	QString description;
	quint32	topic_id;
	bool	is_private;
	QString password;
	bool	is_deleted;

	QString msg_history_path = "./msg_history.json"; //get automaticly
	QVector<User_Message> messages;
	QVector<UserConnection*> connected_users;
};

Q_DECLARE_METATYPE(RoomController)