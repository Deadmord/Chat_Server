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
#include "UserConnection.h"
#include "SrvRoom.h" 


class RoomController : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(RoomController)

public:

	explicit RoomController(QObject* parent = nullptr);
	~RoomController() override;

	
public slots:
	
	void userEntry(QSharedPointer<SrvRoom> shp_room_, UserConnection* user);
	void jsonReceived(QSharedPointer<SrvRoom> shp_room_, UserConnection* sender, const QJsonObject& doc);

private:

	void broadcastSend(QSharedPointer<SrvRoom> shp_room_, const QJsonObject& message, UserConnection* exclude);
	void sendJson(UserConnection* destination, const QJsonObject& message);
	


};

Q_DECLARE_METATYPE(RoomController)