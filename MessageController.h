#pragma once
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QFile>
#include "SrvRoom.h"
#include "DTOMessage.h"
#include "DTOUser.h"
#include "Message.h"
#include "SrvUser.h"
#include "UserController.h"
#include "RoomStorage_Service.h"
#include "SwearHelper.h"

class MessageController : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(MessageController)

public:
	static QSharedPointer<MessageController> instance();

signals:
	void userEntrySignal(const quint32& room_id, QSharedPointer<SrvUser> user_);
	void userLeaveSignal(QSharedPointer<SrvUser> user_);
	void createRoomSignal(QSharedPointer<SrvUser> user_, const QJsonObject room_ );
	void messageToRoom(const quint32& room_id_, QSharedPointer<SrvUser> sender, const QJsonObject& message);
	void roomListRequestSignal(QSharedPointer<SrvUser> sender);
	void messageHystoryRequestSignal(quint32 room_id_, QSharedPointer<SrvUser> sender_, QDateTime message_time_, quint32 pool_size_);

public slots:

	//void broadcastSend(QSharedPointer<User_Message> spr_srv_msg, const QSharedPointer<SrvRoom> room_, const QSharedPointer<SrvUser> exclude_);
	void jsonReceived(QSharedPointer<SrvUser> sender_, const QJsonObject& doc_);
	void jsonWMediaReceived(QSharedPointer<SrvUser> sender_, const QJsonObject& doc_, const QByteArray& data_);
	void jsonMediaRequest(QSharedPointer<SrvUser> sender_, const QJsonObject& doc_);

public:
	explicit MessageController(QObject* parent_ = nullptr);
	~MessageController() override;

	void jsonFromLoggedOut(QSharedPointer<SrvUser> sender_, const QJsonObject& doc_);
	void jsonFromLoggedIn(QSharedPointer<SrvUser> sender_, const QJsonObject& doc_obj_);     
	void jsonFromLoggedIn(QSharedPointer<SrvUser> sender_, const QJsonObject& doc_obj_, const QByteArray& data_);
	void jsonFromLoggedWoRoom(QSharedPointer<SrvUser> sender_, const QJsonObject& doc_obj_);
	void sendJson(QSharedPointer<SrvUser> destination_, const QJsonObject& message_);
	void sendMedia(QSharedPointer<SrvUser> destination_, const QByteArray& data_);

private:
	inline static QSharedPointer<MessageController> shp_instance{};
};

