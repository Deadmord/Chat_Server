#include "LocalStorage_Service.h"


QSharedPointer<LocalStorage_Service> LocalStorage_Service::shp_instance = nullptr;
QMutex LocalStorage_Service::mutex;
QSharedPointer<QTimer> LocalStorage_Service::shp_timer = nullptr;


QSharedPointer<LocalStorage_Service> LocalStorage_Service::getInstance(const int minutes_) {
    try
    {


        if (!shp_instance)
        {
            QMutexLocker locker(&mutex);
            if (!shp_instance)
            {
                shp_instance = QSharedPointer<LocalStorage_Service>(new LocalStorage_Service(), &QObject::deleteLater);
                shp_timer = QSharedPointer<QTimer>(new QTimer(), &QObject::deleteLater);
                const int timeout = minutes_ == 0 ? 5 * 60000 : minutes_ * 60000;
                connect(shp_instance.get(), &close, shp_instance.get(), &safeExit);
                shp_timer->start(timeout);
            }

        }
        return shp_instance;
    }
    catch (const QException& ex)
    {
        PLOGE << "Error occurred while creating instance of localStorage: " << ex.what();
    }
    return nullptr;
}

LocalStorage_Service::LocalStorage_Service(QObject* object_) : QObject(object_) {}


void LocalStorage_Service::addMessage(const QSharedPointer<User_Message>& shp_message_, const quint32 room_id_)
{
    try
    {


        if (message_storage.contains(room_id_))
        {
            message_storage[room_id_].insert(DTOModel::DTOMessage::createDBFromSrv(shp_message_));
            PLOGI << "Message saved in local storage";
        }
        else
        {
            message_storage.insert(room_id_, QSet{ DTOModel::DTOMessage::createDBFromSrv(shp_message_) });
            PLOGI << "Message saved in local storage";
        }
    }
    catch (const QException& ex)
    {
        PLOGE << "Error occurred while adding message to local storage: " << ex.what();
    }
}

void LocalStorage_Service::saveAllMessages() 
{

	try {
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
    catch(const QException& ex)
    {
        PLOGE << "Error occurred while saving all messages to file system: " << ex.what();
    }
}



void LocalStorage_Service::safeExit()
{
    saveAllMessages();
    shp_timer->stop();
    shp_timer->deleteLater();
    shp_instance->deleteLater();
    PLOGI << "Local storage service safely closed";
}


void LocalStorage_Service::addMessages(QSet<QSharedPointer<User_Message>> messages_, quint32 room_id_) {
    try
    {


        if (!message_storage.contains(room_id_)) {
            QSet<QSharedPointer<DBEntity::DBMessage>> new_room_history;
            message_storage.insert(room_id_, new_room_history);
            PLOGI << "Added messages to local storage";
        }
        else
        {
            foreach(const auto & message, messages_)
            {
                message_storage.value(room_id_).insert(DTOModel::DTOMessage::createDBFromSrv(message));
            }
            PLOGI << "Added room with messages to local storage";
        }
    }
    catch (const QException& ex)
    {
        PLOGE << "Error occurred while saving all messages to storage: " << ex.what();
    }
}

