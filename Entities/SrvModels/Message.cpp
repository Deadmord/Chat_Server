#include "Message.h"

User_Message::User_Message(QObject* parent_): _deleted(false) {}

//User_Message::User_Message(const Message& msg_sruct_, QObject* parent_)
//	: _id(msg_sruct_.id), _room_id(msg_sruct_.room_id), _date_time(msg_sruct_.date_time), _nickname(msg_sruct_.nickname), _text(msg_sruct_.text), _media_id(msg_sruct_.media_id), _parent_id(msg_sruct_.parent_id), _deleted(msg_sruct_.deleted), _likes(msg_sruct_.likes)
//{}


User_Message::User_Message(const QString& id_, const quint32& room_id_, const QDateTime& date_time_, const QString& nickname_, const QString& text_, const QString& media_id_, const QString& parent_id_, const bool& deleted_, const QHash<QUuid, bool>& likes_, QObject* parent_)
	: _id(id_), _room_id(room_id_), _date_time(date_time_), _nickname(nickname_), _text(text_), _media_id(media_id_), _parent_id(parent_id_), _deleted(deleted_), _likes(likes_), QObject(parent_)
{}

User_Message::~User_Message() = default;

User_Message::User_Message(const User_Message& other)
{
    _id = other._id + "copy";			//Change to new Id
	_room_id = other._room_id;
	_date_time = other._date_time;
	_nickname = other._nickname;
	_text = other._text;
	_media_id = other._media_id;
	_likes = other._likes;
	_parent_id = other._parent_id;
	_deleted = other._deleted;
}

User_Message& User_Message::operator=(const User_Message& other)
{
	if (this != &other)
	{
		_id = other._id + "copy";			//Change to new Id
		_room_id = other._room_id;
		_date_time = other._date_time;
		_nickname = other._nickname;
		_text = other._text;
		_media_id = other._media_id;
		_likes = other._likes;
		_parent_id = other._parent_id;
		_deleted = other._deleted;
	}
	return *this;
}

const QString& User_Message::getId() const {
	return _id;
}

quint32 User_Message::getRoomId() const {
	return _room_id;
}

const QDateTime& User_Message::getDateTime() const {
	return _date_time;
}

const QString& User_Message::getNickname() const {
	return _nickname;
}

const QString& User_Message::getText() const {
	return _text;
}

const QString& User_Message::getMedia() const {
	return _media_id;
}

const QHash<QUuid, bool>& User_Message::getLikes() const {
	return _likes;
}

QString User_Message::getParentId() const {
	return _parent_id;
}

bool User_Message::isDeleted() const {
	return _deleted;
}

void User_Message::setDeleted(bool flag) {
	_deleted = flag;
}

void User_Message::setLikes(const QHash<QUuid, bool>& likes) {
	_likes = likes;
}


