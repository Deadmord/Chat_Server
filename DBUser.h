#ifndef DBUSER_H
#define DBUSER_H

#include <QString>
#include <QObject>

namespace DBEntity {

    class DBUser {

    private:
        QString a_login;
        QString a_password;
        QString a_userpic_path;
        qint32 a_rating;
        bool a_is_deleted;

    public:
        explicit DBUser(QString login_, QString password_, QString userpic_path_, qint32 rating_, bool is_deleted_);

        QString getLogin() const;
        void setLogin(const QString& login_);

        QString getPassword() const;
        void setPassword(const QString& password_);

        QString getUserpicPath() const;
        void setUserpicPath(const QString& userpic_path_);

        qint32 getRating() const;
        void setRating(const qint32& rating_);

        bool isDeleted() const;
        void setIsDeleted(const bool& deleted);
    };
}

#endif
