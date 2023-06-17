#pragma once
#include <QObject>
#include <QString>
#include <QDateTime>
#include <QHash>
#include <QUuid>
#include "Enums.h"

class User_Message : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(User_Message)
	

public:

    User_Message(const QUuid& id_, const QDateTime& date_time_, const QString& nickname_, const QString& text_, bool is_rtl_, const QString& media_id_ = nullptr, const QString& parent_id_ = nullptr, const bool& deleted_ = false, const QMap<QString, bool>& likes_ = {}, QObject* parent_ = nullptr);

	~User_Message() override;

    [[nodiscard]] const QUuid& getId() const;

    [[nodiscard]] const QDateTime& getDateTime() const;

    [[nodiscard]] quint32 getRoomId() const;

    [[nodiscard]] const QString& getNickname() const;

    [[nodiscard]] QString getParentId() const;

    [[nodiscard]] const QString& getText() const;

    [[nodiscard]] const QString& getMedia() const;

    [[nodiscard]] bool isDeleted() const;

    [[nodiscard]] const QMap<QString, bool>& getLikes() const;

	[[nodiscard]] bool isRtl() const;

    void setDeleted(bool flag_);

    void setLikes(const QMap<QString, bool>& likes_);


private:
	QUuid id;
    quint32 room_id {0};
    QDateTime date_time;
    QString nickname;
    QString text;
    QString media_id;
    QUuid parent_id;
    bool is_rtl;
    bool deleted {false};
    QMap<QString, bool> likes;

};
Q_DECLARE_METATYPE(User_Message)
