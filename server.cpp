#include "server.h"

Server::Server(QObject* parent_) : QTcpServer(parent_) {}
Server::~Server(){}

void Server::startServer()
{
    loadConfig(CONFIG_FILE_PATH);
    openConnection();
    RoomStorage_Service::init();
    PLOGI << "Server initialized";
}

void Server::stopServer()
{
    if (isListening()) {
        disableUsers();
        close();
        PLOGI << "Server stop - OK";
    }
    else
    {
        PLOGD << "Server already stopped";
    }
}

void Server::incomingConnection(qintptr socket_descriptor_)
{
    UserConnection* user_connection = new UserConnection();
    if (!user_connection->setSocketDescriptor(socket_descriptor_)) {
        user_connection->deleteLater();         //if the socket descriptor could not be set, delete the socket
        PLOGE << "Socket descriptor could not be set";
        return;
    }
    connect(user_connection, &UserConnection::disconnectedFromClient, this, std::bind(&Server::userDisconnected, this, user_connection));
    connect(user_connection, &UserConnection::errorSignal, this, std::bind(&Server::userError, this, user_connection));
    connect(user_connection, &UserConnection::jsonReceived, this, std::bind(&Server::jsonReceived, this, user_connection, std::placeholders::_1));  //it become to signal now
    //connect(user_connection, &UserConnection::jsonReceived, MessageController::instance(this), std::bind(&MessageController::jsonReceived, this, user_connection, std::placeholders::_1));  //connect with MessageController (это нужно перенести в MessageController)
    //connect(this, &Server::broadcastSend, MessageController::instance(this), &MessageController::broadcastSend); //это нужно перенести в MessageController

    connected_users.append(user_connection);
    PLOGI << "New client Connected! Now users: " + QString::number(connected_users.size());


    ////---------------old------------------
    ////socket = nextPendingConnection();     //return nullptr
    //socket = new QTcpSocket(this);
    //socket->setSocketDescriptor(socketDescriptor);
    //connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);      //сигнально слотовые соединения, как работают?
    //connect(socket, &QTcpSocket::disconnected, this, &Server::slotDisconnect);
    ////connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    ////тут отправлять по новому соединению запрос данных клиента
    ////после возврата и получения данных
    ////записывать их как пару в vector сокетов

    //sockets.push_back(socket);

    //qDebug() << "new client connected" << socketDescriptor << " Now users : " << sockets.size();

    ////Send msg history to the client
    //SendToClient(messages, socket);

    //SendToAllClients(createMessage("Server", "New connection! Hello!"));
}

QList<UserConnection*> Server::getUsersList() const
{
    return connected_users;
}

////---------------old------------------
//void Server::slotDisconnect()
//{
//
//    socket = (QTcpSocket*)sender();
//    //socket->deleteLater();
//    sockets.removeOne(socket);
//    //Sockets.removeAll(socket);
//    qDebug() << "Disconnected. " << socket->socketDescriptor() << " Now users : " << sockets.size();
//    //лучше создать сервис который будет чистить вектор подключений периодически по таймеру
//}
//
////---------------old------------------
//void Server::slotReadyRead()
//{
//    socket = (QTcpSocket*)sender();
//    QDataStream in(socket);
//    in.setVersion(QDataStream::Qt_6_5);
//    if (in.status() == QDataStream::Ok)
//    {
//        qDebug() << "read...";
//        //        QString str;
//        //        in >> str;
//        //        qDebug() << str;
//        //        SendToClient(str);
//        for (;;)
//        {
//            //if (nextBlockSize == 0)
//            //{
//            //    qDebug() << "nextBlockSize = 0";
//            //    if (socket->bytesAvailable() < 2)
//            //    {
//            //        qDebug() << "Data < 2, break";
//            //        break;
//            //    }
//            //    in >> nextBlockSize;
//            //    qDebug() << "nextBlockSize = " << nextBlockSize;
//            //}
//            //if (socket->bytesAvailable() < nextBlockSize)
//            //{
//            //    qDebug() << "Data not full, waiting...";
//            //    break;
//            //}
//            //очевидно что условие выше выводит из цикла for, но как мы попадаем сюда снова? получается in.status обновляет значение после выполнения строки ниже?
//            Message msg;
//            in >> msg.id >> msg.date_time >> msg.nickname >> msg.deleted >> msg.text;
//            //nextBlockSize = 0;
//
//            //parce to User_Message----------------------------------
//
//            messages.push_back(msg);    //implecetly
//
//            qDebug() << "The message: " << msg.text;
//            //SendToAllClients(msg);      //implecetly
//
//            //To archive messanges
//            uploadMsgHistory(msg_history_path);
//
//            break;
//        }
//
//    }
//    else
//    {
//        qDebug() << "DataStream error";
//    }
//}

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

void Server::openConnection()
{
    if (!this->isListening()) {
        if (this->listen(QHostAddress::Any, server_port))      //QHostAddress::Any, 5555
        {
            PLOGD << "Server start - OK";
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

void Server::disableUsers()
{
    for (UserConnection* user : connected_users) {
        user->disconnectFromClient();
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

//----------------SendToClient-----------------

//void Server::SendToClient(const User_Message &msg, QTcpSocket *socket)
//{
//    Data.clear();
//    QDataStream out(&Data, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_6_5);
//
//    out << quint16(0) << msg.getId() << msg.getDateTime() << msg.getNickname() << msg.isDeleted() << msg.getText(); // преобразовали в stream
//    out.device()->seek(0);          //переходим в начало "данных"
//    out << quint16(Data.size() - sizeof(quint16));
//    socket->write(Data);
//}
//
//void Server::SendToClient(const QVector<User_Message> &msgs, QTcpSocket *socket)
//{
//    for (const User_Message& msg : msgs)
//    {
//    Data.clear();
//    QDataStream out(&Data, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_6_5);
//    out << quint16(0) << msg.getId() << msg.getDateTime() << msg.getNickname() << msg.isDeleted() << msg.getText(); // преобразовали в stream
//    out.device()->seek(0);          //переходим в начало "данных"
//    out << quint16(Data.size() - sizeof(quint16));
//    socket->write(Data);
//    }
//}

//void Server::SendToAllClients(const QString &str)
//{
//    Data.clear();
//    QDataStream out(&Data, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_6_5);
//    Message msg { "Server", str, QDateTime::currentTime(), QUuid::createUuid().toString(), false};
//    out << quint16(0) << msg.id << msg.time << msg.nickname << msg.deleted << msg.text; // преобразовали в stream
//    out.device()->seek(0);          //переходим в начало "данных"
//    out << quint16(Data.size() - sizeof(quint16));
//    //socket->write(Data);
//    for(int i(0); i < Sockets.size(); i++)
//    {
//        Sockets[i]->write(Data);
//    }
//}

//void Server::SendToAllClients(const User_Message &msg)
//{
//    Data.clear();
//    QDataStream out(&Data, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_6_5);
//
//    out << quint16(0) << msg.getId() << msg.getDateTime() << msg.getNickname() << msg.isDeleted() << msg.getText(); // преобразовали в stream
//    out.device()->seek(0);          //переходим в начало "данных"
//    out << quint16(Data.size() - sizeof(quint16));
//    //socket->write(Data);
//    for(int i(0); i < sockets.size(); i++)
//    {
//        sockets[i]->write(Data);
//    }
//}

void Server::userDisconnected(UserConnection* sender_)
{
    connected_users.removeAll(sender_);
    const QString userName = sender_->getUserName();
    if (!userName.isEmpty()) {
        QJsonObject disconnectedMessage;
        disconnectedMessage[QStringLiteral("type")] = QStringLiteral("userdisconnected");
        disconnectedMessage[QStringLiteral("username")] = userName;
        broadcastSend(disconnectedMessage, RoomStorage_Service::getInstance()->getRoom(sender_->getRoomId()), nullptr);
        PLOGI << userName + QLatin1String(" disconnected, users left: ") + QString::number(connected_users.size());
    }
    sender_->deleteLater();
}

void Server::userError(const UserConnection* sender_)
{
    Q_UNUSED(sender_)
        PLOGE << QLatin1String("Error from ") + sender_->getUserName();
}


