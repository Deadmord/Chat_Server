#include "RatingCounter_Service.h"
#include "UserRepository.h"

QSharedPointer<RatingCounter_Service> RatingCounter_Service::shp_instance = nullptr;
QMutex RatingCounter_Service::mutex;

RatingCounter_Service::RatingCounter_Service(QObject* parent_) : QObject(parent_) {}

QSharedPointer<RatingCounter_Service> RatingCounter_Service::getInstance()
{
    if (!shp_instance)
    {
        QMutexLocker locker(&mutex);
        if (!shp_instance)
        {
            shp_instance = QSharedPointer<RatingCounter_Service>(new RatingCounter_Service(), &QObject::deleteLater);
            //connect(p_instance, &close, p_instance, &safeExit);
        }

    }
    return shp_instance;
}

void RatingCounter_Service::updateRating()
{
    if (!rating_storage.empty()) {
        QMutexLocker locker(&mutex);
        auto keys = rating_storage.keys();
        for (const auto& key : keys)
        {
            auto future = DBService::UserRepository::getRatingByLogin(key);
            future.waitForFinished();
            qint32 temp_rating = future.result();
            
            temp_rating += rating_storage.value(key);
            DBService::UserRepository::updateUserRating(key, temp_rating);
            PLOGI << "Update rating succesfully";

            shp_instance->rating_storage.remove(key);
        }
    }
    else PLOGI << "Rating storage is empty.";
}


void RatingCounter_Service::addRating(const QString user_login_, bool like_or_dis)
{

    if (!rating_storage.contains(user_login_)) {

        rating_storage.insert(user_login_, 0);
    }
    qint32 temp = rating_storage.value(user_login_);
    temp += (like_or_dis == false ? -1 : 1);
    rating_storage.insert(user_login_, temp);
}