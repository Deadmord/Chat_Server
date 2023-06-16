#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
//#include <QJsonObject>
//#include <QJsonDocument>
//#include <QJsonArray>
//#include <QJsonParseError>
//#include <QFile>
#include <QTime>
//#include <QUuid>
#include <QTimer>

#include <plog/Log.h>

#include "entities.h"
#include "Enums.h"
#include "RoomController.h"
#include "MessageController.h"
#include "RoomStorage_Service.h"


const QString CONFIG_FILE_PATH = "./config.json";

class Server final : public QTcpServer
{
    Q_OBJECT
    Q_DISABLE_COPY(Server)
public:
    explicit Server(QObject* parent_ = nullptr);
    ~Server();

signals:
    void jsonReceived(UserConnection* sender_, const QJsonObject& doc_);
    void broadcastSend(const QJsonObject& message_, const QSharedPointer<SrvRoom> room_, const UserConnection* exclude_);

public slots:
    void startServer();
    void stopServer();
    void incomingConnection(qintptr socket_descriptor_) override;
    QList<UserConnection*> getUsersList() const;                          //TODO remove from rerver with connected_users prop.

private slots:
    void userDisconnected(UserConnection* sender_);
    void userError(const UserConnection* sender_);

private: 
    void disableUsers();
    void loadConfig(const QString& path_);
    void openConnection();

private:
    QString server_address;
    quint16 server_port;
    quint16 flood_limit;
    QString black_list_path;

    QList<UserConnection*> connected_users;
    //переделать в QVector <User_Message*> messages и перенести в Room
    QList <User_Message> messages;

    QByteArray data_;
};