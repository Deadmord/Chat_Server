﻿#include "LocalStorage_Service.h"


QSharedPointer<LocalStorage_Service> LocalStorage_Service::shp_instance = nullptr;
QMutex LocalStorage_Service::mutex;


QSharedPointer<LocalStorage_Service> LocalStorage_Service::getInstance(const int minutes_)
{
    try
    {
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
    catch (const QException& ex)
    {
        PLOGE << "Error occurred while creating instance of localStorage: " << ex.what();
    }
    return nullptr;
}

LocalStorage_Service::LocalStorage_Service(QObject* object_) : QObject(object_) {}

bool LocalStorage_Service::addLikeToMessage(const quint32& room_id_, const QUuid& message_id_, const QDateTime& message_datetime_, const QString& user_login_, const bool like_dislike_)
{
    try
    {
        const auto file = searchForFiles(message_datetime_, room_id_);
        auto db_messages = readMessagesFromDB("rooms/" + QString::number(room_id_) + file);
        if (!db_messages.empty())
        {
            foreach(auto & message, db_messages)
            {
                if (message->getId() == message_id_)
                {
                    message->addLike(user_login_, like_dislike_);
                    break;
                }
            }
            QJsonArray array;
            for (const auto& message : db_messages) {

                array.append(message->toJson());
            }
            FileRepository::writeJsonArr("rooms/" + QString::number(room_id_) + file, array, true);
            return true;
        }
        return false;
    }
    catch (const QException& ex)
    {
        PLOGE << "Error occurred while adding message to local storage: " << ex.what();
        return false;
    }
}

QSet<QSharedPointer<DBEntity::DBMessage>> LocalStorage_Service::readMessagesFromDB(const QString& file_name_) 
{
    QJsonArray array;
    FileRepository::readJsonArr(file_name_, array);
    QSet<QSharedPointer<DBEntity::DBMessage>> messages;

    for (const auto obj : array) {
        if (obj.isObject())
        {
            QJsonObject json_object = obj.toObject();
            QSharedPointer<DBEntity::DBMessage> message(new DBEntity::DBMessage());
            message->fromJson(json_object);
            messages.insert(message);
        }
        else
        {
            PLOGE << "File is corrupted";
        }
    }

    return messages;

}

QList<QString> LocalStorage_Service::searchForFiles(const QDateTime& from_, const QDateTime& to_, const quint32& room_) const
{
    QList<QString> file_names;
    QDir directory("rooms/" + QString::number(room_));
    QStringList all_files = directory.entryList(QDir::Files);
    bool is_empty = true;
    const QRegularExpression regex(R"(((\d{8}_\d{4})&(\d{8}_\d{4}))\.json)"); 

    for (const QString& file_name : all_files)
    {
        if (QRegularExpressionMatch match = regex.match(file_name); match.hasMatch())
        {
            QString earliest_date = match.captured(2);
            QString latest_date = match.captured(3);
            QDateTime from = QDateTime::fromString(earliest_date, "yyyyMMdd_hhmm");
            QDateTime to = QDateTime::fromString(latest_date, "yyyyMMdd_hhmm");
            if ( from >= from_ || to <= to_)
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

QString LocalStorage_Service::searchForFiles(const QDateTime& date_, const quint32& room_) const
{
    QString file_names_res;
    QDir directory("rooms/" + QString::number(room_));
    QStringList all_files = directory.entryList(QDir::Files);
    bool is_empty = true;
    const QRegularExpression regex(R"(((\d{8}_\d{4})&(\d{8}_\d{4}))\.json)");
    for (const QString& file_name : all_files)
    {
        if (QRegularExpressionMatch match = regex.match(file_name); match.hasMatch())
        {
            QString earliest_date = match.captured(2);
            QString latest_date = match.captured(3);
            QDateTime from = QDateTime::fromString(earliest_date, "yyyyMMdd_hhmm");
            QDateTime to = QDateTime::fromString(latest_date, "yyyyMMdd_hhmm");
            if (from <= date_ && to >= date_)
            {
                return file_name;
            }
        }
    }
    if (is_empty)
    {
        PLOGW << "No messages for this date was retrieved from database";
    }
    return "";
}
QString LocalStorage_Service::searchForFiles(const QDateTime& date_, const quint32& room_, const bool& from_to_) const
{
    
    QDir directory("rooms/" + QString::number(room_));
    QStringList all_files = directory.entryList(QDir::Files);
    bool is_empty = true;
    const QRegularExpression regex(R"(((\d{8}_\d{4})&(\d{8}_\d{4}))\.json)");
    for (const QString& file_name : all_files)
    {
        if (QRegularExpressionMatch match = regex.match(file_name); match.hasMatch())
        {
            QString earliest_date = match.captured(2);
            QString latest_date = match.captured(3);
            QDateTime from = QDateTime::fromString(earliest_date, "yyyyMMdd_hhmm");
            QDateTime to = QDateTime::fromString(latest_date, "yyyyMMdd_hhmm");
            if(from_to_)
            {
        		if (to >= date_)
	            {
	                return file_name;
	            }
                
            }
            else
            {
	            if(from <= date_)
	            {
                    return file_name;
	            }
            }
        }
    }
    if (is_empty)
    {
        PLOGW << "No messages for this date was retrieved from database";
    }
    return "";
}


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


QSet<QSharedPointer<User_Message>> LocalStorage_Service::getMessages(const quint32& room_id_, const QDateTime& time_, bool from_to_, const quint32& pool_size_)
{
    QList<QSharedPointer<DBEntity::DBMessage>> temp_messages;
    QSet<QSharedPointer<User_Message>> result;
    QtConcurrent::map(message_storage.value(room_id_), [&temp_messages, &time_, &from_to_, &pool_size_](QSharedPointer<DBEntity::DBMessage> message) {

        if (from_to_) {
            if (auto date = message->getDateTime(); date > time_)
            {
                temp_messages.append(message);
            }
        }
        else {

            if (auto date = message->getDateTime(); date < time_)
            {
                temp_messages.append(message);
            }
        }

        }).waitForFinished();

        std::sort(temp_messages.begin(), temp_messages.end(), [](const QSharedPointer<DBEntity::DBMessage>& a, const QSharedPointer<DBEntity::DBMessage>& b) {
            return a->getDateTime() > b->getDateTime();
            });

        if (!temp_messages.empty()) {
            int j = 0;
            for (QList<QSharedPointer<DBEntity::DBMessage>>::iterator i = temp_messages.begin(); i != temp_messages.end() || j < pool_size_; i++)
            {
                j++;
                result.insert(DTOModel::DTOMessage::createSrvFromDB(*i));
            }
        }

        return result;
}


void LocalStorage_Service::saveAllMessages() 
{

	try {
        if (!shp_instance->message_storage.empty())
        {
            QMutexLocker locker(&mutex);
            auto keys = shp_instance->message_storage.keys();
            QtConcurrent::blockingMap(keys, [this](const quint32 key_) {
                auto list = QList(message_storage[key_].begin(), message_storage[key_].end());
                std::sort(list.begin(), list.end(), [](const QSharedPointer<DBEntity::DBMessage>& a, 
                 const QSharedPointer<DBEntity::DBMessage>& b)
                {
                        return a->getDateTime() > b ->getDateTime();
                });
                QString earliest_time = list.first()->getDateTime().toString("yyyyMMdd_hhmm");
                QString latest_time = list.last()->getDateTime().toString("yyyyMMdd_hhmm");
                QString file_name = "rooms/" + QString::number(key_) + "/" + earliest_time +"&" + latest_time + ".json";
                QDir().mkpath("rooms/" + QString::number(key_));
                QJsonArray array;
                for (const auto& message : list) {

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
    shp_instance->deleteLater();
    PLOGI << "Local storage service safely closed";
}


void LocalStorage_Service::addMessages(const QSet<QSharedPointer<User_Message>>& messages_, const quint32 room_id_) {
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

