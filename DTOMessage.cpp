#include "DTOMessage.h"

bool DTOModel::DTOMessage::isRtl() const
{
	return is_rtl;
}

QUuid DTOModel::DTOMessage::getId() const
{
	return id;
}

QDateTime DTOModel::DTOMessage::getDateTime() const
{
	return date_time;
}

QString DTOModel::DTOMessage::getLogin() const
{
	return login;
}

QString DTOModel::DTOMessage::getParentId() const
{
	return parent_id;
}

QString DTOModel::DTOMessage::getText() const
{
	return text;
}

void DTOModel::DTOMessage::setLikes(const QMap<QString, bool>& likes_)
{
	likes = likes_;
}

QString DTOModel::DTOMessage::getMediaId() const
{
	return media_id;
}

QMap<QString, bool> DTOModel::DTOMessage::getLikes() const
{
	return likes;
}


QSharedPointer<DTOModel::DTOMessage> DTOModel::DTOMessage::createDTOFromSrv(QSharedPointer<User_Message> srv_message_)
{
	QSharedPointer<DTOModel::DTOMessage> shp_message = QSharedPointer<DTOModel::DTOMessage>(new DTOMessage(
		srv_message_->getId(), srv_message_->getDateTime(), srv_message_->getNickname(), srv_message_->getText(), srv_message_->isRtl(), srv_message_->getMedia(),
		srv_message_->getParentId()));
	shp_message->setLikes(srv_message_->getLikes());
	return shp_message;
}

//TODO:generate id before using this function
QSharedPointer<User_Message> DTOModel::DTOMessage::createSrvFromDTO(QSharedPointer<DTOMessage> dto_model_)
{
	QSharedPointer<User_Message> shp_message = QSharedPointer<User_Message>(new User_Message(
	dto_model_->getId(), dto_model_->getDateTime(), dto_model_->getLogin(), dto_model_->getText(), dto_model_->isRtl(), dto_model_->getMediaId(),
		dto_model_->getParentId()));
	shp_message->setLikes(dto_model_->getLikes());
	return shp_message;
}


QSharedPointer<DBEntity::DBMessage> DTOModel::DTOMessage::createDBFromSrv(QSharedPointer<User_Message> srv_message_)
{
	QSharedPointer<DBEntity::DBMessage> shp_message = QSharedPointer<DBEntity::DBMessage>(new DBEntity::DBMessage(
		srv_message_->getId(), srv_message_->getDateTime(), srv_message_->getNickname(), srv_message_->getText(), srv_message_->getMedia(),
		srv_message_->getParentId(), srv_message_->isRtl()));
	shp_message->setDeleted(srv_message_->isDeleted());
	shp_message->setLikes(srv_message_->getLikes());
	return shp_message;
	 
}

QSharedPointer<User_Message> DTOModel::DTOMessage::createSrvFromDB(QSharedPointer<DBEntity::DBMessage> db_message_)
{
	QSharedPointer<User_Message> shp_message = QSharedPointer<User_Message>(new User_Message(
		db_message_->getId(), db_message_->getDateTime(), db_message_->getLogin(), db_message_->getText(), db_message_->isRtl(), db_message_->getMedia(),
		db_message_->getParentId()));
	shp_message->setDeleted(db_message_->isDeleted());
	shp_message->setLikes(db_message_->getLikes());
	return shp_message;
}


