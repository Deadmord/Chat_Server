#ifndef DBUSER_H
#define DBUSER_H

#include <QString>
#include <QObject>

namespace DBEntity {

    class DBUser : public QObject {

        Q_OBJECT
        Q_PROPERTY(QString login READ getLogin WRITE setLogin NOTIFY loginChanged)
        Q_PROPERTY(QString password READ getPassword WRITE setPassword NOTIFY passwordChanged)
        Q_PROPERTY(QString userpick_path READ getUserpicPath WRITE setUserpicPath NOTIFY userpicPathChanged)
        Q_PROPERTY(qint32 rating READ getRating WRITE setRating NOTIFY ratingChanged)
        Q_PROPERTY(bool is_deleted READ isDeleted WRITE setIsDeleted NOTIFY isDeletedChanged)

    private:
        QString a_login;
        QString a_password;
        QString a_userpic_path;
        qint32 a_rating;
        bool a_is_deleted;

    public:
        explicit DBUser(QObject* parent = nullptr);

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
