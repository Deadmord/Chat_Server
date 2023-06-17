#include "RoomStorage_Service.h"

void RoomStorage_Service::init()
{
    if (!getInstance()->is_started()) {

        getInstance()->make_started();

        shp_instance->downloadRoomsFromDB();
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

void RoomStorage_Service::downloadRoomsFromDB()
{

    auto future = QtConcurrent::run([this]() {

        auto dbrooms = DBService::RoomRepository::getAllActiveRooms();
        for (auto& room : dbrooms) {
            rooms_storage.insert(room->getId(), QSharedPointer<SrvRoom>(new SrvRoom(room), &QObject::deleteLater));
        }
        PLOGD << "Uploaded rooms: " + QString::number(rooms_storage.size());

    });

    //auto dbrooms = QSharedPointer<QList<QSharedPointer<DBEntity::DBRoom>>>::create(qMove(future.result()));
    //auto futurew = QtConcurrent::run([](auto dbrooms) -> decltype(rooms_set) {
    //    decltype(rooms_set) cash;
    //    for (auto& shp : *dbrooms) {
    //        cash.insert(shp->getId(), QSharedPointer<SrvRoom>(new SrvRoom(shp)));
    //        }

    //    return cash;
    //    }, dbrooms);

    //auto future_watcher = new QFutureWatcher<decltype(rooms_set)>();


}

RoomStorage_Service* RoomStorage_Service::getInstance()
{

    if (shp_instance == nullptr) {
        shp_instance = QSharedPointer<RoomStorage_Service>(new RoomStorage_Service, &QObject::deleteLater);
        PLOGD << "RoomStorage_Service instance created";
    }
    return shp_instance.get();
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
    else return nullptr;
}

void RoomStorage_Service::uploadRoomToDB(const QSharedPointer<SrvRoom>& shp_new_room_) const 
{
    if (shp_new_room_ != nullptr) {
        QtConcurrent::run(DBService::RoomRepository::createRoom, DBEntity::DBRoom(shp_new_room_));
    }
}

void RoomStorage_Service::createRoom(const QSharedPointer<SrvRoom>& shp_new_room_)
{
    if (!rooms_storage.contains(shp_new_room_->getId())) {
        rooms_storage.insert(shp_new_room_->getId(), shp_new_room_);
        uploadRoomToDB(shp_new_room_);
        PLOGI << "New room created. " + shp_new_room_->getName();
    }
    else PLOGE << "Room already exist! " + shp_new_room_->getName();
}

void RoomStorage_Service::addMessageToRoom(const qint32& room_id_, const QSharedPointer<User_Message>& message_)
{
    Q_ASSERT(rooms_storage.contains(room_id_));
    if (rooms_storage.contains(room_id_)) {
        rooms_storage.value(room_id_)->addMessage(message_);
        LocalStorage_Service::getInstance()->addMessage(message_, room_id_);
    }
    else PLOGE << "Room doesn't exist. Id: " + room_id_;
}

RoomStorage_Service::RoomStorage_Service(QObject* parent_) : QObject(parent_) {};

void RoomStorage_Service::addConnecntedUserToRoom(const qint32& room_id_, const QSharedPointer<SrvUser>& shp_user_)
{
    if (rooms_storage.contains(room_id_)) {
        rooms_storage.value(room_id_)->connectUser(shp_user_);
    }
    else PLOGE << "Room doesn't exist. Id: " + room_id_;
}

void RoomStorage_Service::deleteConnecntedUserFromRoom(const qint32& room_id_, const QSharedPointer<SrvUser>& shp_user_)
{
    if (rooms_storage.contains(room_id_)) {
        rooms_storage.value(room_id_)->connectUser(shp_user_);
    }
    else PLOGE << "Room doesn't exist. Id: " + room_id_;
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
    QSet<QSharedPointer<DBEntity::DBMessage>> messages;

    auto files = searchForFiles(from_, to_, room_id_);
    foreach(const auto & file, files)
    {
        messages.unite(DBEntity::DBMessage::readMessages(file));
    }

    foreach(const auto & message, messages) {

        rooms_storage[room_id_]->addMessage(fromDBMessageToUserMessage(message));
    }
}

QList<QString> RoomStorage_Service::searchForFiles(const QDateTime& from_, const QDateTime& to_, const quint32& room_)
{
    QList<QString> file_names;
    QDir directory("rooms/" + QString::number(room_));
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

void RoomStorage_Service::getMessagesFromLocalStorage(const quint32& room_id_, const QDateTime& from_, const QDateTime& to_)
{
    QSet<QSharedPointer<User_Message>> messages;

    foreach(const auto  room, rooms_storage[room_id_]->getMessages(from_, to_))
    {
        messages.insert(QSharedPointer<User_Message>(room));
    }

    foreach(const auto & message, messages) {

        rooms_storage[room_id_]->addMessage(message);

    }
}

QSharedPointer<DBEntity::DBMessage> RoomStorage_Service::fromUserMessageToDBMessage(const QSharedPointer<User_Message>& shp_user_message_)
{
    return QSharedPointer<DBEntity::DBMessage>();
}

QSharedPointer<User_Message> RoomStorage_Service::fromDBMessageToUserMessage(const QSharedPointer<DBEntity::DBMessage>& shp_user_message_)
{
    return QSharedPointer<User_Message>();
}

QList<QSharedPointer<User_Message>> RoomStorage_Service::getMessages(const quint32& room_id_, const QDateTime& from_, const QDateTime& to_)
{
    
    QSet<QSharedPointer<User_Message>> messages;
    if (rooms_storage.contains(room_id_))
    {
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

        rooms_storage[room_id_]->getMessages(from_, to_);

    }
    else
    {
        //TODO: auto future = RoomRepository::getRoomById(room_id_);
        // future.waitForFinished();
        // auto room = future.result();
        // addRoom(room);
        PLOGE << "Room id: " + QString::number(room_id_) + " doesn't exist";
    }

    return QList(messages.begin(), messages.end());
    //emit messageRetrieved(QList(messages.begin(), messages.end()));
}



