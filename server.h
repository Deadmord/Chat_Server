#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QFile>
#include <QTime>
#include <QUuid>
#include <QTimer>
#include <plog/Log.h>
#include "Entities/entities.h"
#include "Entities/Enums/Enums.h"
#include "Entities/SrvModels/Message.h"
#include "Controllers/RoomController/RoomController.h"

const QString CONFIG_FILE_PATH = "./config.json";

class Server : public QTcpServer
{
    Q_OBJECT
    Q_DISABLE_COPY(Server)
public:
    explicit Server(QObject* parent = nullptr);
    ~Server();

signals:
    

public slots:
    void startServer();
    void stopServer();
    void incomingConnection(qintptr socketDescriptor) override;

private slots:

    void broadcastSend(const QJsonObject& message, UserConnection* exclude);
    void jsonReceived(UserConnection* sender, const QJsonObject& doc);
    void userDisconnected(UserConnection* sender);
    void userError(UserConnection* sender);

private: 
    void disableUsers();
    void loadConfig(QString _path);
    void openConnection();
    void loadRooms();


    //void SendToClient(const User_Message& msg, QTcpSocket* socket);
    //void SendToClient(const QVector<User_Message>& msgs, QTcpSocket* socket);
    ////void SendToAllClients(const QString &str);
    //void SendToAllClients(const User_Message&msg);

    void jsonFromLoggedOut(UserConnection* sender, const QJsonObject& doc);
    void jsonFromLoggedInMSG(UserConnection* sender, const QJsonObject& doc);      //Убрать в RoomController
    void jsonFromLoggedInCMD(UserConnection* sender, const QJsonObject& doc);
    void sendJson(UserConnection* destination, const QJsonObject& message);
    User_Message createMessage(QString nickame, QString text);

private:
    QString server_address;
    quint16 server_port;
    quint16 flood_limit;
    QString black_list_path;

    //QTcpSocket* socket;
    //QVector <QTcpSocket*> sockets;
    QVector<UserConnection*> connected_users;
    QVector<RoomController*> rooms;
    //переделать в QVector <User_Message*> messages и перенести в Room
    QVector <User_Message> messages;

    QByteArray Data;
};