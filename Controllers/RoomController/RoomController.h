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
#include "../../SrvRoom.h" 


class RoomController : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(RoomController)

public:
	explicit RoomController(QObject* parent = nullptr);
	~RoomController() override;

	

signals:

	void roomCreated(const RoomController* room);
	void nameChanged(const QString& val);
	void descriptionChanged(const QString& val);
	void privateChanged(bool val);
	void passwordChanged();
	void roomDeleted();

public slots:
	
	void userEntry(QSharedPointer<SrvRoom> shp_room_, UserConnection* user);
	void jsonReceived(QSharedPointer<SrvRoom> shp_room_, UserConnection* sender, const QJsonObject& doc);

private:
	void broadcastSend(QSharedPointer<SrvRoom> shp_room_, const QJsonObject& message, UserConnection* exclude);
	void sendJson(UserConnection* destination, const QJsonObject& message);
	


};

Q_DECLARE_METATYPE(RoomController)