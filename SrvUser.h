#pragma once
#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QTimer>

#include <plog/Log.h>


class SrvUser : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(SrvUser)

public:
    explicit SrvUser(QObject* parent = nullptr);
    explicit SrvUser(const QString& username_, const QString& password_, const QByteArray& userpic_, const qint32& rating_, const qint32& room_id_);
    //SrvUser(const QTcpSocket)
    virtual bool setSocketDescriptor(qintptr _socket_descriptor);

    QString getUserName() const;
    void setUserName(const QString& username_);
    QString getPassword() const;
    void setPassword(const QString& password_);
    QByteArray getUserpic() const;
    void setUserpic(const QByteArray& userpic_);
    qint32 getRating() const;
    void setRating(const qint32& rating_);
    qint32 getRoomId() const;
    void setRoomId(const qint32& room_id_);
    bool isFloodLimit() const;
    void setFloodLimit();

signals:
    void jsonReceived(const QJsonObject& _json_doc);
    void disconnectedFromClient();
    void errorSignal();

public slots:
    void disconnectFromClient();
    void sendJson(const QJsonObject& _json_data);

private slots:
    void receiveJson();

private:
    QTcpSocket* user_socket;
    QString username;
    QString password;
    QByteArray userpic;
    qint32 rating = 0;
    qint32 room_id = 0;
    bool flood_limit = false;
    quint16 nextBlockSize = 0;
};

