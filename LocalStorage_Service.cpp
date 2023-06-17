#include "LocalStorage_Service.h"

QSharedPointer<LocalStorage_Service> LocalStorage_Service::shp_instance = nullptr;
QMutex LocalStorage_Service::mutex;


bool LocalStorage_Service::addLikeToMessage(const quint32& room_id_, const QUuid& message_id_, const QDateTime& message_datetime_, const QString& user_login_, const bool like_dislike_)
{
    return false;
    
}

QSharedPointer<LocalStorage_Service> LocalStorage_Service::getInstance(int minutes_) {
    if (!shp_instance)
    {
        QMutexLocker locker(&mutex); 
        if (!shp_instance)
        {
            shp_instance = QSharedPointer<LocalStorage_Service>(new LocalStorage_Service(), &QObject::deleteLater);
            connect(shp_instance.get(), &close, shp_instance.get(), &safeExit);
        }

    }
    return shp_instance;
}

LocalStorage_Service::LocalStorage_Service(QObject* object_) : QObject(object_) {}


void LocalStorage_Service::addMessage(QSharedPointer<User_Message> shp_message_, quint32 room_id_)
{

}

QSet<QSharedPointer<User_Message>> LocalStorage_Service::getMessages(const QDateTime& from_, const QDateTime& to_, const quint32& room_id_)
{
    QSet<QSharedPointer<User_Message>> result;
    QtConcurrent::map(message_storage.value(room_id_), [&result, &from_, &to_](QSharedPointer<DBEntity::DBMessage> message) {
        if (auto date = message->getDateTime(); date >= from_ && date <= to_)
        {
            result.insert(DTOModel::DTOMessage::createSrvFromDB(message));
        }
    }).waitForFinished();

    return result;
}

void LocalStorage_Service::saveAllMessages() {

    if (!shp_instance->message_storage.empty())
    {
        QMutexLocker locker(&mutex);
        auto keys = shp_instance->message_storage.keys();
        QtConcurrent::blockingMap(keys, [this](const quint32 key_) {
                QString current_time = QDateTime::currentDateTimeUtc().toString("yyyyMMdd_hhmm");
                QString file_name = "rooms/" + QString::number(key_) + "/" + current_time + ".json";
                QDir().mkpath("rooms/" + QString::number(key_));
                QJsonArray array;
                for (const auto& message : message_storage.value(key_)) {

                    array.append(message->toJson());
                }
                if (!FileRepository::writeJsonArr(file_name, array)) {
                    PLOGE << "Error writing to file";
                }
                PLOGI << "Writing messages successfully";

                shp_instance->message_storage.remove(key_);
        });
    }
    else PLOGI << "message_storage is empty.";
}



void LocalStorage_Service::safeExit()
{
    saveAllMessages();
    shp_instance->deleteLater();
    PLOGI << "Local storage service safely closed";
}


void LocalStorage_Service::addMessages(QSet<QSharedPointer<User_Message>> messages_, quint32 room_id_) 
{

}

