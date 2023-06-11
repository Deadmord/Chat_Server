#include "DBUser.h"

namespace DBEntity {

    DBUser::DBUser(const QString& login_, const QString& password_, const QString& userpic_path_, const qint32& rating_, const bool& is_deleted_)
        : a_login(login_), a_password(password_), a_userpic_path(userpic_path_), a_rating(rating_), a_is_deleted(is_deleted_) {}

    QString DBUser::getLogin() const { return this->a_login; }
    void DBUser::setLogin(const QString& login_) { this->a_login = login_; }

    QString DBUser::getPassword() const { return this->a_password; }
    void DBUser::setPassword(const QString& password_) { this->a_password = password_; }

    QString DBUser::getUserpicPath() const { return this->a_userpic_path; }
    void DBUser::setUserpicPath(const QString& userpic_path_) { this->a_userpic_path = userpic_path_; }

    qint32 DBUser::getRating() const { return this->a_rating; }
    void DBUser::setRating(const qint32& rating_) { this->a_rating = rating_; }

    bool DBUser::isDeleted() const { return a_is_deleted; }
    void DBUser::setIsDeleted(const bool& deleted) { this->a_is_deleted = deleted; }
}
