#include "LocalStorage_Service.h"

#include <QDir>
#include <plog/Log.h>
#include "DB/JSONS/messages.h"

LocalStorage_Service* LocalStorage_Service::instance = nullptr;
QMutex LocalStorage_Service::mutex;

LocalStorage_Service* LocalStorage_Service::getInstance() {
    if (!instance)
    {
        QMutexLocker locker(&mutex); 
        if (!instance)
        {
            instance = new LocalStorage_Service();
        }
    }
    return instance;
}

LocalStorage_Service::LocalStorage_Service(QObject* object_) : QObject(object_) {

}

void LocalStorage_Service::saveAllMessages() {

    if (!message_storage.empty()) 
    {
        QMutexLocker locker(&mutex);
        auto keys = message_storage.keys();
        for(const auto &key: keys)
        {
            QString current_time = QDateTime::currentDateTimeUtc().toString("yyyyMMdd_hhmm");
            QString file_name = "rooms/" + key.toString() + "/" + current_time + ".json";
            QDir().mkpath("rooms/" + key.toString());
            QJsonArray array;
            for (const auto message : message_storage[key]) {
                
                array.append(message->toJson());
            }
            if (!FileRepository::writeJsonArr(file_name, array)) {
                PLOGE << "Error writing to file";
            }
            PLOGI << "Writing messages successfully";

            message_storage.remove(key);
        }
    }
}

//void LocalStorage_Service::saveMessages(const DBEntity::DBMessage& message_, const QUuid& room_id_) const {
//
//    QString current_time = QDateTime::currentDateTimeUtc().toString("yyyyMMdd_hhmm");
//    QString file_name = "rooms/" + room_id_.toString() + "/" + current_time + ".json";
//    QDir().mkpath("rooms/" + room_id_.toString());
//    QJsonArray array;
//    array.append(message_.toJson());
//
//    if (!FileRepository::writeJsonArr(file_name, array)) {
//        PLOGE << "Error writing to file";
//    }
//    PLOGI << "Writing message successfully";
//}
//
//void LocalStorage_Service::getMessages(QDateTime from_, QDateTime to_, QUuid room_id_) {
//    QList<DBEntity::DBMessage*> messages;
//    QDir directory("rooms/" + room_id_.toString());
//
//    QStringList all_files = directory.entryList(QDir::Files);
//
//    QRegularExpression regex(R"((\d{8}_\d{4})\.json)"); 
//
//    for (const QString& fileName : all_files)
//    {
//        QRegularExpressionMatch match = regex.match(fileName);
//        if (match.hasMatch())
//        {
//            QString dateString = match.captured(1);
//            QDateTime fileDateTime = QDateTime::fromString(dateString, "yyyyMMdd_hhmm");
//
//            if (fileDateTime >= from_ && fileDateTime <= to_)
//            {
//                messages.append(DBEntity::DBMessage::readMessages(fileName));
//            }
//        }
//    }
//    emit messageRetrieved(messages);
//}
