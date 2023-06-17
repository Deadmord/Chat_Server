#pragma once
#include <QObject>

#include "RoomController.h"
#include "MessageController.h"



class UserController : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(UserController)

public:
	explicit UserController(QObject* parent_ = nullptr);
	~UserController() override;
	static QSharedPointer<UserController> instance();

private:


signals:
	void jsonReceived(QSharedPointer<SrvUser> sender_, const QJsonObject& doc_);
	void broadcastSend(const QJsonObject& message_, const QSharedPointer<SrvRoom> room_, const QSharedPointer<SrvUser> exclude_);

public slots:
	void addConnection(qintptr socket_descriptor_);
	void disableUsers();
	QSet<QSharedPointer<SrvUser>> getUsersList() const;                          //TODO remove from rerver with connected_users prop.

private slots:
	void userDisconnected(QSharedPointer<SrvUser> sender_);
	void userError(const QSharedPointer<SrvUser> sender_);

private:
	inline static QSharedPointer<UserController> shp_instance{};
	QSet<QSharedPointer<SrvUser>> connected_users;
};

