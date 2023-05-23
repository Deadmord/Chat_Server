#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QFile>
#include <QTime>
#include <QUuid>

#include "entities.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
    ~Server();
    void startServer();
    QTcpSocket *socket;              //почему указатель?

private:
    void loadMsgHistory(const QString path);
    void uploadMsgHistory(const QString path);
    void SendToClient(const Message &msg, QTcpSocket* socket);
    void SendToClient(const QVector<Message> &msgs, QTcpSocket* socket);

   // void SendToAllClients(const QString &str);
    void SendToAllClients(const Message &msg);

    Message createMessage(QString nickame, QString text);

private:
    QVector <QTcpSocket*> Sockets;
    QVector <Message> messanges;
    QByteArray Data;
    quint16 nextBlockSize = 0;

    QString msgHistoryPath;

public slots:                       //что за слоты??
    void incomingConnection(qintptr socketDescriptor);
    void slotDisconnect();
    void slotReadyRead();
    
};

#endif // SERVER_H
