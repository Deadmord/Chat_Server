#include "RoomStorage_Service.h"
#include "RoomRepository.h"
#include "LocalStorage_Service.h"

QSharedPointer<RoomStorage_Service> RoomStorage_Service::shp_instance = nullptr;

bool RoomStorage_Service::is_started()
{
    return started;
}

void RoomStorage_Service::make_started()
{
    started = true;
    PLOGI << "RoomStorage_Service started.";
}

RoomStorage_Service* RoomStorage_Service::getInstance()
{

    if (shp_instance == nullptr) {
        shp_instance = QSharedPointer<RoomStorage_Service>(new RoomStorage_Service, &QObject::deleteLater);
        PLOGD << "RoomStorage_Service instance created";
    }
    return shp_instance.get();
}

void RoomStorage_Service::init()
{
    if (getInstance()->is_started()) {

        getInstance()->make_started();


        shp_instance->downloadRoomsFromDB();
    }
}

QList<QSharedPointer<SrvRoom>> RoomStorage_Service::getRooms()
{
    return rooms_set.values();
}

QSharedPointer<SrvRoom> RoomStorage_Service::getRoom(qint32 room_id_)
{
    if (rooms_set.contains(room_id_)) {
        return rooms_set.value(room_id_);
    }
    else return nullptr;
}

void RoomStorage_Service::createRoom(QSharedPointer<SrvRoom> shp_new_room_)
{
    if (!rooms_set.contains(shp_new_room_->getId())) {
        rooms_set.insert(shp_new_room_->getId(), shp_new_room_);
        uploadRoomToDB(shp_new_room_);
        PLOGI << "New room created. " + shp_new_room_->getName();
    }
    else PLOGE << "Room already exist! " + shp_new_room_->getName();
}

void RoomStorage_Service::addMessageToRoom(qint32 room_id_, User_Message* message_)
{
    Q_ASSERT(rooms_set.contains(room_id_));
    if (rooms_set.contains(room_id_)) {
        rooms_set.value(room_id_)->addMessage(message_);
        LocalStorage_Service::getInstance()->addMessages(message_, room_id_);
    }
    else PLOGE << "Room doesn't exist id: " + room_id_;
}

void RoomStorage_Service::addConnecntedUserToRoom(qint32 room_id_, UserConnection* user_)
{
}



RoomStorage_Service::RoomStorage_Service(QObject* parent_) : QObject(parent_) {}

void RoomStorage_Service::downloadRoomsFromDB()
{
    auto future = DBService::RoomRepository::getAllActiveRooms();

    future.waitForFinished();

    QList<QSharedPointer<DBEntity::DBRoom>> dbrooms = future.result();

    auto result =  QtConcurrent::map(dbrooms, [dbrooms, this] (QSharedPointer<DBEntity::DBRoom> shp_room_) {
            rooms_set.insert(shp_room_->getId(), QSharedPointer<SrvRoom>(new SrvRoom(shp_room_), &QObject::deleteLater));
    });

}

void RoomStorage_Service::uploadRoomToDB(QSharedPointer<SrvRoom> new_room_)
{
    if (new_room_ != nullptr) {
        QtConcurrent::run(DBService::RoomRepository::createRoom, DBEntity::DBRoom(new_room_));
    }
}
