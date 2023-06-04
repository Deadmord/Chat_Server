#ifndef SERVER_H
#define SERVER_H
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
#include "entities.h"

const QString CONFIG_FILE_PATH = "./config.json";

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
    ~Server();

    void startServer();
    

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotDisconnect();
    void slotReadyRead();

private:
    void loadConfig(QString _path);
    void openConnection();
    void loadRooms();
    void loadMsgHistory(const QString path);
    void uploadMsgHistory(const QString path);
    void SendToClient(const Message &msg, QTcpSocket* socket);
    void SendToClient(const QVector<Message> &msgs, QTcpSocket* socket);

   // void SendToAllClients(const QString &str);
    void SendToAllClients(const Message &msg);

    Message createMessage(QString nickame, QString text);

private:
    QString server_address;
    quint16 server_port;
    quint16 flood_limit;
    QString black_list_path;
    QString msg_history_path;   // = "./msg_history.json";

    QTcpSocket* socket;
    QVector <QTcpSocket*> sockets;
    QVector <Message> messages;

    QByteArray Data;
    quint16 nextBlockSize = 0;
};

#endif // SERVER_H
