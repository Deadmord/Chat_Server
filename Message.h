#pragma once
#include <QObject>
#include <QString>
#include <QDateTime>
#include <QHash>
#include <QUuid>
#include "entities.h"
#include "Enums.h"

class User_Message : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ getId)
    Q_PROPERTY(quint32 room_id READ getRoomId)
    Q_PROPERTY(QDateTime date_time READ getDateTime)
    Q_PROPERTY(QString nickname READ getNickname)
    Q_PROPERTY(QString text READ getText)
    Q_PROPERTY(QString media READ getMedia)
    Q_PROPERTY(QMap<QUuid, bool> likes READ getLikes WRITE setLikes)
    Q_PROPERTY(QString parent_id READ getParentId)
    Q_PROPERTY(bool deleted READ isDeleted WRITE setDeleted)


public:
    explicit User_Message(QObject* parent_ = nullptr);
    User_Message(const Message& msg_sruct_, QObject* parent_ = nullptr);
    User_Message(const QString& id_, const quint32& room_id_, const QDateTime& date_time_, const QString& nickname_, const QString& text_, const QString& media_id_ = nullptr, const QString& parent_id_ = nullptr, const bool& deleted_ = false, const QMap<QUuid, bool>& likes_ = {}, QObject* parent_ = nullptr);
    ~User_Message() override;
    User_Message(const User_Message& other);
    User_Message& operator = (const User_Message& other);

    const QString& getId() const;

    [[nodiscard]] const QDateTime& getDateTime() const;

    [[nodiscard]] quint32 getRoomId() const;

    [[nodiscard]] const QString& getNickname() const;

    [[nodiscard]] QString getParentId() const;

    [[nodiscard]] const QString& getText() const;

    [[nodiscard]] const QString& getMedia() const;

    [[nodiscard]] bool isDeleted() const;

    [[nodiscard]] const QMap<QUuid, bool>& getLikes() const;

    void setDeleted(bool flag);

    void setLikes(const QMap<QUuid, bool>& likes);

private:
    [[nodiscard]] QString generateId();

private:
    QString _id;
    //QUuid _id;
    quint32 _room_id {0};
    QDateTime _date_time;
    QString _nickname;
    QString _text;
    QString _media_id;
    QString _parent_id;
    //QUuid _parent_id;
    bool _deleted {false};
    QMap<QUuid, bool> _likes;

};
Q_DECLARE_METATYPE(User_Message)
