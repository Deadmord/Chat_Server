#include "LocalStorage_Service.h"
#include <qthread.h>

LocalStorage_Service* LocalStorage_Service::instance = nullptr;
QMutex LocalStorage_Service::mutex;

QList<QString> LocalStorage_Service::searchForFiles(
	const QDateTime& from_, const QDateTime& to_,
	const DBEntity::DBRoom& room_)
{
    QList<QString> file_names;
    QDir directory("rooms/" + room_.getId());
    QStringList all_files = directory.entryList(QDir::Files);
    bool is_empty = true;
    const QRegularExpression regex(R"((\d{8}_\d{4})\.json)"); // Regular expression to match file names like "20230102_1000.json"

    for (const QString& file_name : all_files)
    {
        if (QRegularExpressionMatch match = regex.match(file_name); match.hasMatch())
        {
            QString date_string = match.captured(1);

            if (QDateTime file_date_time = QDateTime::fromString(date_string, "yyyyMMdd_hhmm"); file_date_time >= from_ && file_date_time <= to_)
            {
                file_names.append(file_name);
                is_empty = false;
            }
        }
    }
    if (is_empty)
    {
        PLOGW << "No messages for this date was retrieved from database";
    }
    return file_names;

}

LocalStorage_Service* LocalStorage_Service::getInstance(int minutes_) {
    if (!instance)
    {
        QMutexLocker locker(&mutex); 
        if (!instance)
        {
            instance = new LocalStorage_Service();
            p_timer = new QTimer();
            int timeout = minutes_ == 0? 5*60000 : minutes_ * 60000;
            connect(instance, &close, instance, &safeExit);
            connect(p_timer, &QTimer::timeout, instance, &LocalStorage_Service::deleteMessages);
            p_timer->start(timeout);
        }

    }
    return instance;
}

LocalStorage_Service::LocalStorage_Service(QObject* object_) : QObject(object_) {}


void LocalStorage_Service::saveAllMessages() {

    if (!instance->message_storage.empty())
    {
        QMutexLocker locker(&mutex);
        auto keys = instance->message_storage.keys();
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

            instance->message_storage.remove(key);
        }
    }
    else PLOGI << "message_storage is empty.";
}

void LocalStorage_Service::deleteMessages()
{
    current_messages.clear();
}


void LocalStorage_Service::safeExit()
{
    saveAllMessages();
    p_timer->stop();
    p_timer->deleteLater();
    instance->deleteLater();
    PLOGI << "Local storage service safely closed";
}


void LocalStorage_Service::addMessages(DBEntity::DBMessage* message_, QUuid room_id_) {
    if (!message_storage.contains(room_id_)) {
        QList<QSharedPointer<DBEntity::DBMessage>> new_room_history;
        message_storage.insert(room_id_, new_room_history);
    }
    if(!current_messages.contains(room_id_))
    {
	    
    }
    message_storage.value(room_id_).append(QSharedPointer<DBEntity::DBMessage>(message_, &QObject::deleteLater));
}

void LocalStorage_Service::getMessages(const QDateTime& from_, const QDateTime& to_, const DBEntity::DBRoom& room_) {
    QSet<QSharedPointer<DBEntity::DBMessage>> messages;
    QMutexLocker locker(&mutex);
	auto files = searchForFiles(from_, to_, room_);
	foreach(const auto& file,files)
	{
		messages.unite(DBEntity::DBMessage::readMessages(file));
	}
	
    if (current_messages.contains(room_))
    {
    	foreach(const auto& message, current_messages.value(room_))
	    {
		    if(auto date = message->getDateTime(); date>= from_ && date <= to_)
		    {
                messages.insert(message);
		    }
	    }
        current_messages[room_].unite(messages);
    }
    else
    {
        current_messages.insert(room_, messages);
    }
    
    emit messageRetrieved(QList(messages.begin(), messages.end()));
    

}