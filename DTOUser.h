#ifndef DTOUSER_H
#define DTOUSER_H

#include <QObject>
#include <QSharedPointer>
#include <QTcpSocket>
#include "DBUser.h"
#include "SrvUser.h"
namespace DTOModel {

	class DTOUser : public QObject {

		Q_OBJECT

	public:

		DTOUser(QObject* parent = nullptr);
		DTOUser(const QString& login_, const QString& password_, const QByteArray& userpic_, const quint32& rating_);
		~DTOUser();

		QString getUsername() const;
		void setUsername(const QString& username_);
		QString getPassword() const;
		void setPassword(const QString& password_);
		QByteArray getUserpic() const;
		void setUserpic(const QByteArray& userpic_);
		quint32 getRating() const;
		void setRating(const quint32& rating_);

		//static QSharedPointer<DTOModel::DTOUser> createDTOUser(const DBEntity::DBUser& db_user_);
		//static QSharedPointer<DBEntity::DBUser> createDBUser(const DTOModel::DTOUser& dto_user_);

		static QSharedPointer<SrvUser> createSrvUserFromDB(const DBEntity::DBUser& db_user_);
		static QSharedPointer<DBEntity::DBUser> createDBUser(const SrvUser& user_connection_);

		static QSharedPointer<DTOModel::DTOUser> createDTOUser(const SrvUser& user_connection_);
		static QSharedPointer<SrvUser> createSrvUserFromDTO(const DTOModel::DTOUser& dto_user_);

	private:

		QTcpSocket* user_socket;
		QString a_username;
		QString a_password;
		QByteArray a_userpic;
		quint32 a_rating;
		qint32 a_room_id;
		quint16 nextBlockSize = 0;

	};
}

Q_DECLARE_METATYPE(DTOModel::DTOUser)

#endif