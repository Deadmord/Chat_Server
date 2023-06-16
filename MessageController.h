#pragma once
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QFile>
#include "SrvRoom.h"
#include "Message.h"
#include "SrvUser.h"
#include "RoomStorage_Service.h"

class MessageController : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(MessageController)

public:
	static QSharedPointer<MessageController> instance();

public slots:

	void broadcastSend(const QJsonObject& message_, const QSharedPointer<SrvRoom> room_, const SrvUser* exclude_);
	void jsonReceived(SrvUser* sender_, const QJsonObject& doc_);

public:
	explicit MessageController(QObject* parent_ = nullptr);
	~MessageController() override;

	void jsonFromLoggedOut(SrvUser* sender_, const QJsonObject& doc_);
	void jsonFromLoggedInMsg(const SrvUser* sender_, const QJsonObject& doc_obj_);      //Убрать в RoomController
	void jsonFromLoggedInCmd(SrvUser* sender_, const QJsonObject& doc_obj_);
	void sendJson(SrvUser* destination_, const QJsonObject& message_);
	User_Message createMessage(const QString& nickname_, const QString& text_);

private:
	inline static QSharedPointer<MessageController> shp_instance{};
};

