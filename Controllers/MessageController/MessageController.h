#pragma once
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QFile>
class MessageController : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(MessageController)

public:
	static MessageController* instance(QObject* parent);

private:
	explicit MessageController(QObject* parent_ = nullptr);
	~MessageController() override;

	void jsonFromLoggedOut(UserConnection* sender_, const QJsonObject& doc_);
	void jsonFromLoggedInMsg(const UserConnection* sender_, const QJsonObject& doc_obj_);      //”брать в RoomController
	void jsonFromLoggedInCmd(UserConnection* sender_, const QJsonObject& doc_obj_);
	void sendJson(UserConnection* destination_, const QJsonObject& message_);
	User_Message createMessage(const QString& nickname_, const QString& text_);

private:
	static QSharedPointer<MessageController> shp_instance;

private slots:

	void broadcastSend(const QJsonObject& message_, const UserConnection* exclude_);
	void jsonReceived(UserConnection* sender_, const QJsonObject& doc_);



public slots:

//	void userEntry(QSharedPointer<SrvRoom> shp_room_, UserConnection* user);
//	void jsonReceived(QSharedPointer<SrvRoom> shp_room_, UserConnection* sender, const QJsonObject& doc);
//
//private:
//	void broadcastSend(QSharedPointer<SrvRoom> shp_room_, const QJsonObject& message, UserConnection* exclude);
//	void sendJson(UserConnection* destination, const QJsonObject& message);

};

