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

#include "DBRoom.h"
#include "Enums.h"
#include "Message.h"
#include "SrvUser.h"
#include "SrvRoom.h" 


class RoomController : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(RoomController)

public:

	explicit RoomController(QObject* parent = nullptr);
	~RoomController() override;
	static QSharedPointer<RoomController> instance();

	
public slots:
	
	void userEntry(const quint32& room_id, QSharedPointer<SrvUser> user_);
	void userLeave(QSharedPointer<SrvUser> user_);
	void jsonMsgReceived(const quint32& room_id_, QSharedPointer<SrvUser> sender, const QJsonObject& message);
	void roomListRequest(QSharedPointer<SrvUser> sender);
	void messageHystoryRequest(quint32 room_id_, QSharedPointer<SrvUser> sender_, QDateTime message_time_, quint32 pool_size_);
	void createRoom(QSharedPointer<SrvUser> sender_, const QJsonObject& room_);


private:
	void broadcastSend(const QJsonObject& message_, const quint32& room_id_, const QSharedPointer<SrvUser>& exclude_ = nullptr);
	void sendJson(const QSharedPointer<SrvUser>& destination, const QJsonObject& message);
	

	static inline QSharedPointer<RoomController> shp_instance = nullptr;

};

Q_DECLARE_METATYPE(RoomController)