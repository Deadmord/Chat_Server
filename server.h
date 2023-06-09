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
#include "Entities/SrvModels/Message.h"
#include "Controllers/RoomController/RoomController.h"

const QString CONFIG_FILE_PATH = "./config.json";

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
    ~Server();

public slots:
    void startServer();
    void stopServer();
    void incomingConnection(qintptr socketDescriptor);
    void slotDisconnect();
    void slotReadyRead();

private:
    void loadConfig(QString _path);
    void openConnection();
    void disableUsers();
    void loadRooms();
    void loadMsgHistory(const QString path);
    void uploadMsgHistory(const QString path);
    void SendToClient(const User_Message &msg, QTcpSocket* socket);
    void SendToClient(const QVector<User_Message> &msgs, QTcpSocket* socket);

   // void SendToAllClients(const QString &str);
    void SendToAllClients(const User_Message&msg);

    User_Message createMessage(QString nickame, QString text);

private:
    QString server_address;
    quint16 server_port;
    quint16 flood_limit;
    QString black_list_path;
    QString msg_history_path;   // = "./msg_history.json";

    QTcpSocket* socket;
    QVector <QTcpSocket*> sockets;
    QVector <User_Message> messages;

    QByteArray Data;
    quint16 nextBlockSize = 0;
};