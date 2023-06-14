#pragma once
#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QTimer>
#include "../Enums/Enums.h"
#include "../../DBUser.h"

class UserConnection : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(UserConnection)
public:
    explicit UserConnection(QObject* parent = nullptr);

    virtual bool setSocketDescriptor(qintptr _socket_descriptor);

    QString getId() const;
    void setId(const QString& _id);
    QString getUserName() const;
    void setUserName(const QString& _user_name);
    QString getPassword() const;
    void setPassword(const QString& _password);
    QString getUserPicId() const;
    void setUserPicId(const QString& _userpic_id);
    quint32 getRatingLikes() const;
    void setRatingLikes(const quint32& _rating_likes);
    quint32 getRatingDislikes() const;
    void setRatingDislikes(const quint32& _rating_dislikes);
    quint32 getRoomId() const;
    void setRoomId(const quint32& _room_id);

signals:
    void jsonReceived(const QJsonObject& _json_doc);
    void disconnectedFromClient();
    void errorSignal();
    void logMessage(enum Severity log_lvl, const QString& msg);

public slots:
    void disconnectFromClient();
    void sendJson(const QJsonObject& _json_data);

private slots:
    void receiveJson();
    bool isFloodLimit() const;

private:
    QTcpSocket* user_socket;
    QSharedPointer<DBEntity::DBUser> p_user;
    QString id;
    QString user_name;
    QString password;
    QString userpic_id;
    quint32 rating_likes;
    quint32 rating_dislikes;
    quint32 room_id;
    bool flood_limit = false;
    quint16 nextBlockSize = 0;
};

