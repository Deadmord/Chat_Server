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

	
public slots:
	
	void userEntry(QSharedPointer<SrvRoom> shp_room_, SrvUser* user);
	void jsonReceived(QSharedPointer<SrvRoom> shp_room_, SrvUser* sender, const QJsonObject& doc);

private:

	void broadcastSend(QSharedPointer<SrvRoom> shp_room_, const QJsonObject& message, SrvUser* exclude);
	void sendJson(SrvUser* destination, const QJsonObject& message);
	


};

Q_DECLARE_METATYPE(RoomController)