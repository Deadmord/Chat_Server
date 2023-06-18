#include "RoomStorage_Service.h"
#include "DTOMessage.h"
#include "DTORoom.h"

void RoomStorage_Service::init()
{
    if (!getInstance()->is_started()) {

        getInstance()->make_started();

        shp_instance->downloadRoomsFromDB();
    }
    else
    {
        PLOGW << "Service already started";
    }
}

bool RoomStorage_Service::is_started()
{
    return started;
}

void RoomStorage_Service::make_started()
{
    started = true;
    PLOGI << "RoomStorage_Service started.";
}

void RoomStorage_Service::addLikeToMessage(const quint32& room_id_, const QUuid& message_id_, const QDateTime& message_datetime_, const QString& user_login_, const bool like_dislike_)
{
    QtConcurrent::run([](const quint32& room_id_, const QUuid& message_id_, const QDateTime& message_datetime_, const QString& user_login_, const bool like_dislike_) {
        LocalStorage_Service::getInstance()->addLikeToMessage(room_id_, message_id_, message_datetime_, user_login_, like_dislike_);
        RatingCounter_Service::getInstance()->addRating(user_login_, like_dislike_);

        }, room_id_, message_id_, message_datetime_, user_login_, like_dislike_);
}

void RoomStorage_Service::downloadRoomsFromDB()
{

    auto future = QtConcurrent::run([this]() {

        auto dbrooms = DBService::RoomRepository::getAllActiveRooms();
        for (auto& room : dbrooms) {
            rooms_storage.insert(room->getId(), QSharedPointer<SrvRoom>(new SrvRoom(room), &QObject::deleteLater));
        }
        PLOGD << "Uploaded rooms: " + QString::number(rooms_storage.size());

    });
}

RoomStorage_Service* RoomStorage_Service::getInstance()
{
    try
    {
        if (shp_instance == nullptr) {
            shp_instance = QSharedPointer<RoomStorage_Service>(new RoomStorage_Service, &QObject::deleteLater);
            PLOGD << "RoomStorage_Service instance created";
        }
        return shp_instance.get();
    }
    catch (const QException& ex)
    {
        PLOGE << "Cannot get instance" << ex.what();
        return nullptr;
    }

}

QList<QSharedPointer<SrvRoom>> RoomStorage_Service::getRooms()
{
    return rooms_storage.values();
}

QSharedPointer<SrvRoom> RoomStorage_Service::getRoom(const qint32& room_id_)
{
    if (rooms_storage.contains(room_id_)) {
        return rooms_storage.value(room_id_);
    }
    return nullptr;
}

void RoomStorage_Service::uploadRoomToDB(const QSharedPointer<SrvRoom>& shp_new_room_) const 
{
    
    if (shp_new_room_ != nullptr) {
        QtConcurrent::run(DBService::RoomRepository::createRoom, DBEntity::DBRoom(shp_new_room_));
    }
}

void RoomStorage_Service::createRoom(const QSharedPointer<SrvRoom>& shp_new_room_)
{
    try
    {
        if (!rooms_storage.contains(shp_new_room_->getId())) {
            addRoom(shp_new_room_);
            uploadRoomToDB(shp_new_room_);
            PLOGI << "New room created. " + shp_new_room_->getName();
        }
        else PLOGE << "Room already exist! " + shp_new_room_->getName();
    }
    catch (const QException& ex)
    {
        PLOGE << "Cannot create room in roomStorage" << ex.what();
    }
}

void RoomStorage_Service::addMessageToRoom(const qint32& room_id_, const QSharedPointer<User_Message>& message_)
{
    try
    {
        Q_ASSERT(rooms_storage.contains(room_id_));
        if (rooms_storage.contains(room_id_)) {
            rooms_storage.value(room_id_)->addMessage(message_);
            LocalStorage_Service::getInstance()->addMessage(message_, room_id_);
        }
        else PLOGE << "Room doesn't exist. Id: " + room_id_;
    }
    catch (const QException& ex)
    {
        PLOGE << "Cannot add message to roomStorage" << ex.what();
        
    }
}

void RoomStorage_Service::addRoom(const QSharedPointer<SrvRoom>& shp_room_)
{
    rooms_storage.insert(shp_room_->getId(), shp_room_);
}

RoomStorage_Service::RoomStorage_Service(QObject* parent_) : QObject(parent_) {};

bool RoomStorage_Service::addConnecntedUserToRoom(const qint32& room_id_, const QSharedPointer<SrvUser>& shp_user_)
{
    if (rooms_storage.contains(room_id_)) {
        return rooms_storage.value(room_id_)->connectUser(shp_user_);
    }
    else {
        PLOGE << "Room doesn't exist. Id: " + room_id_;
        return false;
    }
}

bool RoomStorage_Service::deleteConnecntedUserFromRoom(const qint32& room_id_, const QSharedPointer<SrvUser>& shp_user_)
{
    if (rooms_storage.contains(room_id_)) {
        return rooms_storage.value(room_id_)->disconnectUser(shp_user_);
    }
    else {
        PLOGE << "Room doesn't exist. Id: " + room_id_;
        return false;
    }
}

void RoomStorage_Service::addMessagesToRoom(const qint32& room_id_, const QSet<QSharedPointer<User_Message>>& messages_)
{
    if (rooms_storage.contains(room_id_)) {
        rooms_storage.value(room_id_)->addMessages(messages_);
    }
    else PLOGE << "Room doesn't exist. Id: " + room_id_;

}

void RoomStorage_Service::getMessagesFromDB(const quint32& room_id_, const QDateTime& from_, const QDateTime& to_)
{
    try
    {
        QSet<QSharedPointer<DBEntity::DBMessage>> messages;

        auto files = LocalStorage_Service::getInstance()->searchForFiles(from_, to_, room_id_);
        foreach(const auto & file, files)
        {
            messages.unite(LocalStorage_Service::readMessagesFromDB(file));
        }

        foreach(const auto & message, messages) {

            rooms_storage[room_id_]->addMessage(DTOModel::DTOMessage::createSrvFromDB(message));
        }
    }
    catch (const QException& ex)
    {
        PLOGE << "Cannot get messages from DB" << ex.what();
    }
}

QSet<QSharedPointer<User_Message>> RoomStorage_Service::getMessagesFromDB(const quint32& room_id_, const QDateTime& date_, const bool& from_to_, const quint32 pool_size_)
{
    try
    {
        QList<QSharedPointer<User_Message>> temp;
        auto date = date_;
        while (true)
        {
            auto file = LocalStorage_Service::getInstance()->searchForFiles(date, room_id_, from_to_);
            if(file == "")
            {
                break;
            }
            QSet<QSharedPointer<DBEntity::DBMessage>> messages;


            messages.unite(LocalStorage_Service::readMessagesFromDB(file));

            auto it = messages.begin();
            quint32 k = 0;
            

            while (it != messages.end() && k < pool_size_) {
                rooms_storage[room_id_]->addMessage(DTOModel::DTOMessage::createSrvFromDB(*it));
                temp.append(DTOModel::DTOMessage::createSrvFromDB(*it));
                k++;
            }
            if(k >= pool_size_)
            {
                break;
            }
            if(from_to_)
            {
				date = temp.last()->getDateTime().addSecs(60);
            }
            date = temp.first()->getDateTime().addSecs(-60);
        }
        return QSet(temp.begin(), temp.end());
    }
    catch (const QException& ex)
    {
        PLOGE << "Cannot get messages from DB" << ex.what();
        throw;
    }
}


void RoomStorage_Service::getMessagesFromLocalStorage(const quint32& room_id_, const QDateTime& from_, const QDateTime& to_)
{
    QSet<QSharedPointer<User_Message>> messages;

    messages.unite(LocalStorage_Service::getInstance()->getMessages(from_, to_, room_id_));
    

    rooms_storage[room_id_]->addMessages(messages);

}

QSet<QSharedPointer<User_Message>> RoomStorage_Service::getMessagesFromLocalStorage(const quint32& room_id_, const QDateTime& time_, bool from_to_, const quint32& pool_size_)
{
    QSet<QSharedPointer<User_Message>> messages;

    messages.unite(LocalStorage_Service::getInstance()->getMessages(room_id_, time_, from_to_, pool_size_));

    rooms_storage[room_id_]->addMessages(messages);

    return messages;

}


QSet<QSharedPointer<User_Message>> RoomStorage_Service::getMessages(const quint32& room_id_, const QDateTime& from_, const QDateTime& to_)
{
    
    QSet<QSharedPointer<User_Message>> messages;

    if (!rooms_storage.contains(room_id_))
    {
        auto future = DBService::RoomRepository::getRoomById(room_id_);
        future.waitForFinished();
        auto room = future.result();
        if (room != nullptr) {
            addRoom(DTOModel::DTORoom::createSrvRoomFromDB(*room));
        }
        else {
            PLOGE << "Room id: " + QString::number(room_id_) + " doesn't exist";
            return messages;
        }
    }

    QList<std::function<void(const quint32&, const QDateTime&, const QDateTime&)>> delegates;

    delegates.append([this](const quint32& room_id_, const QDateTime& from_, const QDateTime& to_) {
        getMessagesFromLocalStorage(room_id_, from_, to_);
        });

    delegates.append([this](const quint32& room_id_, const QDateTime& from_, const QDateTime& to_) {
        getMessagesFromDB(room_id_, from_, to_);
        });

    QtConcurrent::blockingMap(delegates, [&](std::function<void(const quint32&, const QDateTime&, const QDateTime&)> delegate) {
        delegate(room_id_, from_, to_);
        PLOGF << "DRATUTY!!!";
        });

    messages.unite(rooms_storage[room_id_]->getMessages(from_, to_));
    
    return messages;
    //emit messageRetrieved(QList(messages.begin(), messages.end()));
}

QSet<QSharedPointer<User_Message>> RoomStorage_Service::getMessages(const quint32& room_id_, const QDateTime& time_, const bool from_to_, const quint32& pool_size_)
{
    auto result = getRoom(room_id_)->getMessages(time_, from_to_, pool_size_);

    if (result.size() == pool_size_) {
        return result;
    }

    QDateTime new_time;

    if (from_to_) new_time = result.values().first()->getDateTime();
    else new_time = result.values().last()->getDateTime();

    auto new_pool_size = pool_size_ - result.size();

    result.unite(getMessagesFromLocalStorage(room_id_, new_time, from_to_, new_pool_size));

    auto temp_list = result.values();

    std::sort(temp_list.begin(), temp_list.end(), [](const QSharedPointer<User_Message>& a, const QSharedPointer<User_Message>& b) {
        return a->getDateTime() > b->getDateTime();
        });

    result = QSet<QSharedPointer<User_Message>>(temp_list.begin(), temp_list.end());

    if (result.size() == pool_size_) {
        return result;
    }

    if (from_to_) new_time = temp_list.first()->getDateTime();
    else new_time = temp_list.last()->getDateTime();

    new_pool_size = pool_size_ - result.size();

    result.unite(getMessagesFromDB(room_id_, new_time, from_to_, new_pool_size));

    temp_list = result.values();

    std::sort(temp_list.begin(), temp_list.end(), [](const QSharedPointer<User_Message>& a, const QSharedPointer<User_Message>& b) {
        return a->getDateTime() > b->getDateTime();
        });

    QSet<QSharedPointer<User_Message>> result_2 = QSet<QSharedPointer<User_Message>>(temp_list.begin(), temp_list.end());

    for (auto& var : temp_list)
    {
        result_2.insert(var);
    }

}





