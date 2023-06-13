#include "LocalStorage_Service.h"
#include <qthread.h>

LocalStorage_Service* LocalStorage_Service::p_instance = nullptr;
QMutex LocalStorage_Service::mutex;

LocalStorage_Service* LocalStorage_Service::getInstance() {
    if (!p_instance)
    {
        QMutexLocker locker(&mutex); 
        if (!p_instance)
        {
            p_instance = new LocalStorage_Service();
            connect(p_instance, &close, p_instance, &safeExit);
        }

    }
    return p_instance;
}

LocalStorage_Service::LocalStorage_Service(QObject* object_) : QObject(object_) {}


void LocalStorage_Service::saveAllMessages() {

    if (!p_instance->message_storage.empty())
    {
        QMutexLocker locker(&mutex);
        auto keys = message_storage.keys();
        for (const auto& key : keys)
        {
            QString current_time = QDateTime::currentDateTimeUtc().toString("yyyyMMdd_hhmm");
            QString file_name = "rooms/" + key.toString() + "/" + current_time + ".json";
            QDir().mkpath("rooms/" + key.toString());
            QJsonArray array;
            for (const auto& message : message_storage.value(key)) {

                array.append(message->toJson());
            }
            if (!FileRepository::writeJsonArr(file_name, array)) {
                PLOGE << "Error writing to file";
            }
            PLOGI << "Writing messages successfully";

            p_instance->message_storage.remove(key);
        }
    }
    else PLOGI << "message_storage is empty.";
}

void LocalStorage_Service::safeExit()
{
    saveAllMessages();
    p_instance->deleteLater();
    PLOGI << "Local storage service safely closed";
}


void LocalStorage_Service::addMessages(DBEntity::DBMessage* message_, QUuid room_id_) {
    if (!message_storage.contains(room_id_)) {
        QList<QSharedPointer<DBEntity::DBMessage>> new_room_history;
        message_storage.insert(room_id_, new_room_history);
    }
    message_storage.value(room_id_).append(QSharedPointer<DBEntity::DBMessage>(message_, &QObject::deleteLater));
}
