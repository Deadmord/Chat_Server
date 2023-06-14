#include "DBUser.h"

namespace DBEntity {

    DBUser::DBUser(const QString& login_, const QString& password_, const QByteArray& userpic_, const quint32& rating_)
        : a_login(login_), a_password(password_), a_userpic(userpic_), a_rating(rating_), a_is_deleted(false) {}

    DBUser::DBUser(const DTOModel::DTOUser& dto_user_) : a_login(dto_user_.getUsername()), a_password(dto_user_.getPassword()), a_userpic(dto_user_.getUserpic()), a_rating(dto_user_.getRating()), a_is_deleted(false) {};

    QString DBUser::getLogin() const { return this->a_login; }
    void DBUser::setLogin(const QString& login_) { this->a_login = login_; }

    QString DBUser::getPassword() const { return this->a_password; }
    void DBUser::setPassword(const QString& password_) { this->a_password = password_; }

    QByteArray DBUser::getUserpic() const { return this->a_userpic; }
    void DBUser::setUserpic(const QByteArray& userpic_) { this->a_userpic = userpic_; }

    quint32 DBUser::getRating() const { return this->a_rating; }
    void DBUser::setRating(const quint32& rating_) { this->a_rating = rating_; }

    bool DBUser::isDeleted() const { return a_is_deleted; }
    void DBUser::setIsDeleted(const bool& deleted) { this->a_is_deleted = deleted; }
}
