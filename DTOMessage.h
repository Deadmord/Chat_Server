#pragma once
#ifndef CHAT_SERVER_DTOMESSAGE_H
#define CHAT_SERVER_DTOMESSAGE_H
#include <QDateTime>
#include <QUuid>

#include "DBMessage.h"

namespace DTOModel {
	class DTOMessage
	{
		QUuid id;
		QDateTime date_time;
		QString login;
		QString parent_id;
		QString text;
		QString media_id;
		bool is_rtl;
		QMap<QString, bool> likes;


	public:
		DTOMessage(){}
		DTOMessage(const QUuid& id_, const QDateTime& date_time_, const QString& login_,
			const QString& text_, bool is_rtl_, const QString& media_id_, const QString& parent_id_)
			: id(id_),
			  date_time(date_time_),
			  login(login_),
			  parent_id(parent_id_),
			  text(text_),
			  media_id(media_id_),
			is_rtl(is_rtl_)
		{
		}

		[[nodiscard]] bool isRtl() const;
		[[nodiscard]] QUuid getId() const;
		[[nodiscard]] QDateTime getDateTime() const;
		[[nodiscard]] QString getLogin() const;
		[[nodiscard]] QString getParentId() const;
		[[nodiscard]] QString getText() const;
		void setLikes(const QMap<QString, bool>& likes_);
		[[nodiscard]] QString getMediaId() const;
		[[nodiscard]] QMap<QString, bool> getLikes() const;

		static QSharedPointer<DTOMessage> createDTOFromSrv(QSharedPointer< User_Message> srv_message_);
		static QSharedPointer<User_Message> createSrvFromDTO(QSharedPointer<DTOMessage> dto_model_);
		static QSharedPointer<DBEntity::DBMessage> createDBFromSrv(QSharedPointer< User_Message> srv_message_);
		static QSharedPointer<User_Message> createSrvFromDB(QSharedPointer<DBEntity::DBMessage> db_message_);

	};
}
#endif
