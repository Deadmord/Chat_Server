#pragma once
#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

class UserConnection : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(UserConnection)
public:
    explicit UserConnection(QObject* parent = nullptr);
    virtual bool setSocketDescriptor(qintptr socketDescriptor);
    QString getUserName() const;
    void setUserName(const QString& user_name);
    void sendJson(const QJsonObject& jsonData);
signals:
    void jsonReceived(const QJsonObject& jsonDoc);
    void disconnectedFromClient();
    void error();
    void logMessage(const QString& msg);
public slots:
    void disconnectFromClient();
private slots:
    void receiveJson();
private:
    QTcpSocket* user_socket;
    QString id;
    QString user_name;
    QString password;
    QString userpic_id;
    quint32 rating_likes;
    quint32 rating_dislikes;
    QDateTime lastMessageTime;
};

