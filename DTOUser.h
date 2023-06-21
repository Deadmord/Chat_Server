#ifndef DTOUSER_H
#define DTOUSER_H

#include <QObject>
#include <QSharedPointer>
#include <QTcpSocket>
#include "SrvUser.h"
#include "DBUser.h"

namespace DTOModel {

	class DTOUser : public QObject {

		Q_OBJECT

	public:

		DTOUser(QObject* parent = nullptr);
		DTOUser(const QString& login_, const QString& password_, const QByteArray& userpic_, const qint32& rating_, const quint32& room_id_);
		~DTOUser();

		QString getUsername() const;
		void setUsername(const QString& username_);
		QString getPassword() const;
		void setPassword(const QString& password_);
		QByteArray getUserpic() const;
		void setUserpic(const QByteArray& userpic_);
		qint32 getRating() const;
		void setRating(const qint32& rating_);
		quint32 getRoomId() const;
		void setRoomId(const quint32& room_id_);

		static QSharedPointer<SrvUser> createSrvUserFromDB(const DBEntity::DBUser& db_user_);
		static QSharedPointer<DBEntity::DBUser> createDBUserFromSrv(const SrvUser& srv_user_);

		static QSharedPointer<DTOModel::DTOUser> createDTOUserFromSrv(const SrvUser& srv_user_);
		//static QSharedPointer<SrvUser> createSrvUserFromDTO(const DTOModel::DTOUser& dto_user_);

	private:
		QString username;
		QString password;
		QByteArray userpic;
		qint32 rating;
		qint32 room_id;

	};
}

Q_DECLARE_METATYPE(DTOModel::DTOUser)

#endif