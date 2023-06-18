#include "server.h"

Server::Server(QObject* parent_) : QTcpServer(parent_) 
{
    connect(this, &Server::incomingConnection, UserController::instance().get(), &UserController::addConnection);
}
Server::~Server(){}

void Server::startServer()
{
    loadConfig(CONFIG_FILE_PATH);
    openConnection();
    DBService::DBConnection_Service::init().then([]() {
        RoomStorage_Service::init();
        });
    PLOGI << "Server initialized";
}

void Server::stopServer()
{
    if (isListening()) {
        close();
        PLOGI << "Server stop - OK";
        emit serverStopped();
    }
    else
    {
        PLOGD << "Server already stopped";
    }
}

void Server::openConnection()
{
    if (!this->isListening()) {
        if (this->listen(QHostAddress::Any, server_port))      //QHostAddress::Any, 5555
        {
            PLOGD << "Server start - OK";
            emit serverStarted();
        }
        else
        {
            PLOGE << "Sever start - Error";
        }
    }
    else
    {
        PLOGD << "Server is already listening";
    }
}

void Server::loadConfig(const QString& path_)
{
    QFile config_file;
    QJsonParseError json_error;

    config_file.setFileName(path_);

    if (config_file.open(QIODevice::ReadOnly | QFile::Text))
    {
	    const QJsonDocument config_file_doc = QJsonDocument::fromJson(QByteArray(config_file.readAll()), &json_error);
        config_file.close();

        if (json_error.error == QJsonParseError::NoError)
        {
	        QJsonObject config_json = config_file_doc.object();

            if (const QJsonValue v = config_json["ServerAddress"]; v.isString())
                server_address = v.toString();
            else
                qWarning() << "Error ServerAddress reading";

            if (const QJsonValue v = config_json["ServerPort"]; v.isDouble())
                server_port = v.toInt();
            else
                qWarning() << "Error ServerPort reading";

            if (const QJsonValue v = config_json["FloodLimit"]; v.isDouble())
                flood_limit = v.toInt();
            else
                qWarning() << "Error FloodLimit reading";

            if (const QJsonValue v = config_json["BlackListPath"]; v.isString())
                black_list_path = v.toString();
            else
                qWarning() << "Error BlackListPath reading";

            //if (const QJsonValue v = config_file_doc["MessagesHistorySettings"]["Path"]; v.isString())
            //    msg_history_path = v.toString();
            //else
            //    qWarning() << "Error BlackListPath reading";

        }
        else
        {
            qWarning() << "Error config file read: " << json_error.error;
        }
    }
    else
    {
        qWarning("Couldn't open config file.");
    }
    
}

//void Server::loadRooms()
//{
//    //Load rooms from repository to vector<Room>        ------------------!!!!!-----------------------
//    //run loop to init all rooms
//
//    //QVector<DBEntity::DBRoom> dbRooms =
//    //{
//    //    DBEntity::DBRoom{1, "room1", "a_description", 11, false, "a_password", false },
//    //    DBEntity::DBRoom{2, "room2", "b_description", 12, true, "b_password", false },
//    //    DBEntity::DBRoom{3, "room3", "c_description", 13, false, "c_password", true }
//    //};
//
//    //for(const DBEntity::DBRoom& db_room: dbRooms)
//    //{
//	   // auto* room = new SrvRoom(db_room.getId(), db_room.getName(), db_room.getDescription(), db_room.getTopicId(), db_room.isPrivate(), db_room.getPassword(), db_room.isDeleted(), this);
//    //    //connect();
//    //    rooms.append(room);
//    //    PLOGI << "New room created! Now rooms: " + QString::number(rooms.size());
//    //}
//}


