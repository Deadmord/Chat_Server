#ifndef DBUSER_H
#define DBUSER_H

#include <QString>
#include <QObject>
#include "DTOUser.h"

namespace DBEntity {

    class DBUser {

    private:
        QString a_login;
        QString a_password;
        QByteArray a_userpic;
        quint32 a_rating;
        bool a_is_deleted;

    public:
        DBUser(const QString& login_, const QString& password_, const QByteArray& userpic_, const quint32& rating_);
        DBUser(const DTOModel::DTOUser& dtoUser_);

        QString getLogin() const;
        void setLogin(const QString& login_);

        QString getPassword() const;
        void setPassword(const QString& password_);

        QByteArray getUserpic() const;
        void setUserpic(const QByteArray& userpic_);

        quint32 getRating() const;
        void setRating(const quint32& rating_);

        bool isDeleted() const;
        void setIsDeleted(const bool& deleted);
    };
}

#endif
