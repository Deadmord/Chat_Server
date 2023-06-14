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

private:
	static QSharedPointer<MessageController> shp_instance;





public slots:

//	void userEntry(QSharedPointer<SrvRoom> shp_room_, UserConnection* user);
//	void jsonReceived(QSharedPointer<SrvRoom> shp_room_, UserConnection* sender, const QJsonObject& doc);
//
//private:
//	void broadcastSend(QSharedPointer<SrvRoom> shp_room_, const QJsonObject& message, UserConnection* exclude);
//	void sendJson(UserConnection* destination, const QJsonObject& message);

};

