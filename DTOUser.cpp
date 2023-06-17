#include "DTOUser.h"

namespace DTOModel {

	DTOUser::DTOUser(QObject* parent) : QObject(parent) {};
	DTOUser::DTOUser(const QString& login_, const QString& password_, const QByteArray& userpic_, const qint32& rating_, const quint32& room_id_)
		: username(login_), password(password_), userpic(userpic_), rating(rating_), room_id(room_id_) {};

	DTOUser::~DTOUser() {}

	QString DTOUser::getUsername() const { return username; }
	void DTOUser::setUsername(const QString& username_) { username = username_; }
	QString DTOUser::getPassword() const { return password; }
	void DTOUser::setPassword(const QString& password_) { password = password_; }
	QByteArray DTOUser::getUserpic() const { return userpic; }
	void DTOUser::setUserpic(const QByteArray& userpic_) { userpic = userpic_; }
	qint32 DTOUser::getRating() const { return rating; }
	void DTOUser::setRating(const qint32& rating_) { rating = rating_; }
	quint32 DTOUser::getRoomId() const { return room_id; }
	void DTOUser::setRoomId(const quint32& room_id_) { room_id = room_id_; }

	QSharedPointer<SrvUser> DTOUser::createSrvUserFromDB(const DBEntity::DBUser& db_user_) {
		QSharedPointer<SrvUser> shp_srv_user = QSharedPointer<SrvUser>::create(
			db_user_.getLogin(), db_user_.getPassword(), db_user_.getUserpic(), db_user_.getRating(), -1);
		return shp_srv_user;
	}

	QSharedPointer<DBEntity::DBUser> DTOUser::createDBUserFromSrv(const SrvUser& srv_user_) {
		QSharedPointer<DBEntity::DBUser> shp_db_user = QSharedPointer<DBEntity::DBUser>::create(
			srv_user_.getUserName(), srv_user_.getPassword(), srv_user_.getUserpic(), srv_user_.getRating());
		return shp_db_user;
	}

	QSharedPointer<DTOModel::DTOUser> DTOUser::createDTOUserFromSrv(const SrvUser& srv_user_) {
		QSharedPointer<DTOModel::DTOUser> shp_dto_user = QSharedPointer<DTOModel::DTOUser>::create(
			srv_user_.getUserName(), srv_user_.getPassword(), srv_user_.getUserpic(), srv_user_.getRating(), srv_user_.getRoomId());
		return shp_dto_user;
	}

}
