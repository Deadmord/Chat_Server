#pragma once
#include <QObject>
#include <QMutex>
#include <QString>
#include <QMap>

#include <plog/Log.h>

class RatingCounter_Service :
    public QObject
{
    Q_OBJECT

private:

    static RatingCounter_Service* p_instance;
    static QMutex mutex;
    explicit RatingCounter_Service(QObject* parent = nullptr);
    QMap<QString, qint32> rating_storage;

public:

    static RatingCounter_Service* getInstance();
    RatingCounter_Service(const RatingCounter_Service&) = delete;
    RatingCounter_Service& operator=(const RatingCounter_Service&) = delete;

public slots:

    void addRating(const QString user_login_, bool like_or_dis);
    void updateRating();
};

