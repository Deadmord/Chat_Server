#include "LocalStorage_Service.h"
#include <qthread.h>
#include <QtConcurrent>

QSharedPointer<LocalStorage_Service> LocalStorage_Service::shp_instance = nullptr;
QMutex LocalStorage_Service::mutex;
QSharedPointer<QTimer> LocalStorage_Service::shp_timer = nullptr;


QSharedPointer<LocalStorage_Service> LocalStorage_Service::getInstance(int minutes_) {
    if (!shp_instance)
    {
        QMutexLocker locker(&mutex); 
        if (!shp_instance)
        {
            shp_instance = QSharedPointer<LocalStorage_Service>(new LocalStorage_Service(), &QObject::deleteLater);
            shp_timer = QSharedPointer<QTimer>(new QTimer(), &QObject::deleteLater);
            int timeout = minutes_ == 0? 5*60000 : minutes_ * 60000;
            connect(shp_instance.get(), &close, shp_instance.get(), &safeExit);
            shp_timer->start(timeout);
        }

    }
    return shp_instance;
}

LocalStorage_Service::LocalStorage_Service(QObject* object_) : QObject(object_) {}


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
    shp_timer->stop();
    shp_timer->deleteLater();
    shp_instance->deleteLater();
    PLOGI << "Local storage service safely closed";
}


void LocalStorage_Service::addMessages(User_Message* message_, quint32 room_id_) {
    //if (!message_storage.contains(room_id_)) {
    //    QList<QSharedPointer<DBEntity::DBMessage>> new_room_history;
    //    message_storage.insert(room_id_, new_room_history);
    //}
    //if(!current_messages.contains(room_id_))
    //{
	   // 
    //}
    ////new DBEntity::DBMessage(message_) - ��������
    //message_storage.value(room_id_).append(QSharedPointer<DBEntity::DBMessage>(new DBEntity::DBMessage(message_), &QObject::deleteLater));
}

