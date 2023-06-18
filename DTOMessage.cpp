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


bool DTOModel::DTOMessage::toDTOMessageFromJson(DTOModel::DTOMessage& user_masg_dto_, const QJsonObject& user_msg_)
{
    const QJsonValue id_val = user_msg_.value(QLatin1String("id"));
    if (id_val.isNull() || !id_val.isString())
        return false;
    const QString id = id_val.toString().trimmed();
    if (id.isEmpty())
        return false;

    const QJsonValue parentid_val = user_msg_.value(QLatin1String("parentid"));
    if (parentid_val.isNull() || !parentid_val.isString())
        return false;
    const QString parentid = parentid_val.toString().trimmed();

    const QJsonValue date_time_val = user_msg_.value(QLatin1String("datetime"));
    if (date_time_val.isNull() || !date_time_val.isString())
        return false;
    const QString date_time = date_time_val.toString().trimmed();
    if (date_time.isEmpty())
        return false;

    const QJsonValue nickname_val = user_msg_.value(QLatin1String("nickname"));
    if (nickname_val.isNull() || !nickname_val.isString())
        return false;
    const QString nickname = nickname_val.toString().trimmed();
    if (nickname.isEmpty())
        return false;

    const QJsonValue text_val = user_msg_.value(QLatin1String("text"));
    if (text_val.isNull() || !text_val.isString())
        return false;
    const QString text = text_val.toString().trimmed();
    if (text.isEmpty())
        return false;

    const QJsonValue mediaid_val = user_msg_.value(QLatin1String("mediaid"));
    if (mediaid_val.isNull() || !mediaid_val.isString())
        return false;
    const QString mediaid = mediaid_val.toString().trimmed();

    const QJsonValue rtl_val = user_msg_.value(QLatin1String("rtl"));
    if (rtl_val.isNull() || !mediaid_val.isBool())
        return false;
    const bool rtl = rtl_val.toBool();


    const auto likes_val = user_msg_.value(QLatin1String("likes")).toObject().toVariantMap();
    QMap<QString, bool> likes;
    for (auto it = likes_val.constBegin(); it != likes_val.constEnd(); ++it)
    {
        likes.insert(it.key(), it.value().toBool());
    }

    user_masg_dto_ = DTOModel::DTOMessage(QUuid(id), QDateTime::fromString(date_time), nickname,
        text, rtl, mediaid, parentid);
    user_masg_dto_.setLikes(likes);

    return true;
}

bool DTOModel::DTOMessage::toJsonFromDTOMessage(QJsonObject& user_msg_, const DTOModel::DTOMessage& user_masg_dto_)
{
    return false;
}
