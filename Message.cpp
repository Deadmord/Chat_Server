#include "Message.h"


User_Message::User_Message(const QUuid& id_, const QDateTime& date_time_, const QString& nickname_,
	const QString& text_, const bool is_rtl_, const QString& media_id_, const QString& parent_id_, const bool& deleted_,
	const QMap<QString, bool>& likes_, QObject* parent_) : id(id_), date_time(date_time_),
	nickname(nickname_), text(text_), media_id(media_id_), parent_id(parent_id_), deleted(deleted_),
	likes(likes_), is_rtl(is_rtl_)
{
}

User_Message::~User_Message(){}


const QUuid& User_Message::getId() const {
	return id;
}

//quint32 User_Message::getRoomId() const {
//	return _room_id;
//}

const QDateTime& User_Message::getDateTime() const {
	return date_time;
}

const QString& User_Message::getNickname() const {
	return nickname;
}

const QString& User_Message::getText() const {
	return text;
}

const QString& User_Message::getMedia() const {
	return media_id;
}

const QMap<QString, bool>& User_Message::getLikes() const {
	return likes;
}

QString User_Message::getParentId() const {
	return parent_id.toString();
}

bool User_Message::isDeleted() const {
	return deleted;
}

void User_Message::setDeleted(bool flag_) {
	deleted = flag_;
}

void User_Message::setLikes(const QMap<QString, bool>& likes_) {
	likes = likes_;
}

bool User_Message::isRtl() const
{
	return is_rtl;
}


