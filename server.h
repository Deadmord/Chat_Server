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
#include "Controllers/MessageController/MessageController.h"
#include "MessageHistory_Service.h"


const QString CONFIG_FILE_PATH = "./config.json";

class Server final : public QTcpServer
{
    Q_OBJECT
    Q_DISABLE_COPY(Server)
public:
    explicit Server(QObject* parent_ = nullptr);
    ~Server();

signals:
    

public slots:
    void startServer();
    void stopServer();
    void incomingConnection(qintptr socket_descriptor_) override;

private slots:

    void broadcastSend(const QJsonObject& message_, const UserConnection* exclude_);
    void jsonReceived(UserConnection* sender_, const QJsonObject& doc_);
    void userDisconnected(UserConnection* sender_);
    void userError(const UserConnection* sender_);

private: 
    void disableUsers();
    void loadConfig(const QString& path_);
    void openConnection();


    //void SendToClient(const User_Message& msg, QTcpSocket* socket);
    //void SendToClient(const QVector<User_Message>& msgs, QTcpSocket* socket);
    //void SendToAllClients(const QString &str);
    //void SendToAllClients(const User_Message&msg);

    void jsonFromLoggedOut(UserConnection* sender_, const QJsonObject& doc_);
    void jsonFromLoggedInMsg(const UserConnection* sender_, const QJsonObject& doc_obj_);      //Убрать в RoomController
    void jsonFromLoggedInCmd(UserConnection* sender_, const QJsonObject& doc_obj_);
    void sendJson(UserConnection* destination_, const QJsonObject& message_);
    User_Message createMessage(const QString& nickname_, const QString& text_);

private:
    QString server_address;
    quint16 server_port;
    quint16 flood_limit;
    QString black_list_path;

    //QTcpSocket* socket;
    //QVector <QTcpSocket*> sockets;
    QVector<UserConnection*> connected_users;
    //переделать в QVector <User_Message*> messages и перенести в Room
    QVector <User_Message> messages;

    QByteArray data_;
};