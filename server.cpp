#include "server.h"

Server::Server(QObject* parent_) : QTcpServer(parent_) {}
Server::~Server(){}

void Server::startServer()
{
    loadConfig(CONFIG_FILE_PATH);
    openConnection();
    MessageHistory_Service::init();
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
    connect(user_connection, &UserConnection::disconnectedFromClient, this, [this, user_connection]
    {
	    userDisconnected(user_connection);
    });
    connect(user_connection, &UserConnection::errorSignal, this,
            [this, user_connection] { userError(user_connection); });
    connect(user_connection, &UserConnection::jsonReceived, this, [this, user_connection](auto&& ph1_)
    {
	    jsonReceived(user_connection, std::forward<decltype(ph1_)>(ph1_));
    });
    
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

void Server::loadRooms()
{
    //Load rooms from repository to vector<Room>        ------------------!!!!!-----------------------
    //run loop to init all rooms

    //QVector<DBEntity::DBRoom> dbRooms =
    //{
    //    DBEntity::DBRoom{1, "room1", "a_description", 11, false, "a_password", false },
    //    DBEntity::DBRoom{2, "room2", "b_description", 12, true, "b_password", false },
    //    DBEntity::DBRoom{3, "room3", "c_description", 13, false, "c_password", true }
    //};

    //for(const DBEntity::DBRoom& db_room: dbRooms)
    //{
	   // auto* room = new SrvRoom(db_room.getId(), db_room.getName(), db_room.getDescription(), db_room.getTopicId(), db_room.isPrivate(), db_room.getPassword(), db_room.isDeleted(), this);
    //    //connect();
    //    rooms.append(room);
    //    PLOGI << "New room created! Now rooms: " + QString::number(rooms.size());
    //}
}

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

//---!!!---это от сюда убрать, будет не нужно
User_Message Server::createMessage(const QString& nickname_, const QString& text_)
{
    Message msg;
    msg.nickname = nickname_;
    msg.text = text_;
    msg.room_id = 0;
    msg.deleted = false;
    return User_Message(msg);
    //return User_Message(QUuid::createUuid().toString(), 0, QDateTime::currentDateTime(), _nickname, _text);
}

//---!!!---это от сюда убрать, будет не нужно
void Server::broadcastSend(const QJsonObject& message_, const UserConnection* exclude_)
{
    for (UserConnection* user : connected_users) {
        Q_ASSERT(user);
        if (user == exclude_)
            continue;
        sendJson(user, message_);
    }
}

//---!!!---это от сюда убрать, будет не нужно
void Server::sendJson(UserConnection* destination_, const QJsonObject& message_)
{
    Q_ASSERT(destination_);
    destination_->sendJson(message_);
}

void Server::jsonReceived(UserConnection* sender_, const QJsonObject& doc_)
{
    Q_ASSERT(sender_);
    PLOGI << QLatin1String("JSON received: ") + QJsonDocument(doc_).toJson(QJsonDocument::Compact);
    if (sender_->getUserName().isEmpty())
        return jsonFromLoggedOut(sender_, doc_);
    //-------------- проверять принадлежность комнате ------------------
    // -----------------если принадлежит то отправлять сообщение в конкретную комнату---------------
    //вместо этого
    jsonFromLoggedInCmd(sender_, doc_);
}

void Server::userDisconnected(UserConnection* sender_)
{
    connected_users.removeAll(sender_);
    const QString userName = sender_->getUserName();
    if (!userName.isEmpty()) {
        QJsonObject disconnectedMessage;
        disconnectedMessage[QStringLiteral("type")] = QStringLiteral("userdisconnected");
        disconnectedMessage[QStringLiteral("username")] = userName;
        broadcastSend(disconnectedMessage, nullptr);
        PLOGI << userName + QLatin1String(" disconnected, users left: ") + QString::number(connected_users.size());
    }
    sender_->deleteLater();
}

void Server::userError(const UserConnection* sender_)
{
    Q_UNUSED(sender_)
        PLOGE << QLatin1String("Error from ") + sender_->getUserName();
}

void Server::jsonFromLoggedOut(UserConnection* sender_, const QJsonObject& doc_obj_)
{
    Q_ASSERT(sender_);
    const QJsonValue type_val = doc_obj_.value(QLatin1String("type"));
    if (type_val.isNull() || !type_val.isString())
        return;
    if (type_val.toString().compare(QLatin1String("login"), Qt::CaseInsensitive) != 0)
        return;
    const QJsonValue username_val = doc_obj_.value(QLatin1String("username"));
    if (username_val.isNull() || !username_val.isString())
        return;
    const QString new_user_name = username_val.toString().simplified();
    if (new_user_name.isEmpty())
        return;
    for (const UserConnection* user : qAsConst(connected_users)) {    //Find duplicat username
        if (user == sender_)
            continue;
        if (user->getUserName().compare(new_user_name, Qt::CaseInsensitive) == 0) {
            QJsonObject message;
            message[QStringLiteral("type")] = QStringLiteral("login");
            message[QStringLiteral("success")] = false;
            message[QStringLiteral("reason")] = QStringLiteral("duplicate username");
            sendJson(sender_, message);
            return;
        }
    }
    sender_->setUserName(new_user_name);
    QJsonObject success_message;
    success_message[QStringLiteral("type")] = QStringLiteral("login");
    success_message[QStringLiteral("success")] = true;
    sendJson(sender_, success_message);
    QJsonObject connected_message;
    connected_message[QStringLiteral("type")] = QStringLiteral("newuser");
    connected_message[QStringLiteral("username")] = new_user_name;
    broadcastSend(connected_message, sender_);
}

void Server::jsonFromLoggedInCmd(UserConnection* sender_, const QJsonObject& doc_obj_)
{
    Q_ASSERT(sender_);
    const QJsonValue type_val = doc_obj_.value(QLatin1String("type"));
    if (type_val.isNull() || !type_val.isString())
        return;
    if (type_val.toString().compare(QLatin1String("roomListRequest"), Qt::CaseInsensitive) == 0)
    {

    }
    if (type_val.toString().compare(QLatin1String("roomEntry"), Qt::CaseInsensitive) == 0)
    {
        const QJsonValue room_val = doc_obj_.value(QLatin1String("room"));
        if (room_val.isNull() || !room_val.isDouble())
            return;
        const quint32 room_id = room_val.toInt();
        if (!room_id)
            return;
        //проверить что комната с таким номером вообще существует
        sender_->setRoomId(room_id);
        //Отправить юзера в нужную комнату
        //уже в комнате по сигналу вхождения юзера сделать рассылку
    }
    if (type_val.toString().compare(QLatin1String("roomLeave"), Qt::CaseInsensitive) == 0)
    {

    }
    if (type_val.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) == 0)
    {
        const QJsonValue text_val = doc_obj_.value(QLatin1String("text"));
        if (text_val.isNull() || !text_val.isString())
            return;
        const QString text = text_val.toString().trimmed();
        if (text.isEmpty())
            return;
        QJsonObject message;
        message[QStringLiteral("type")] = QStringLiteral("message");
        message[QStringLiteral("text")] = text;
        message[QStringLiteral("sender")] = sender_->getUserName();
        broadcastSend(message, sender_);
        return;
    }


}

//---!!!---это от сюда убрать, будет не нужно
void Server::jsonFromLoggedInMsg(const UserConnection* sender_, const QJsonObject& doc_obj_)
{
    Q_ASSERT(sender_);
    const QJsonValue type_val = doc_obj_.value(QLatin1String("type"));
    if (type_val.isNull() || !type_val.isString())
        return;
    if (type_val.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) != 0)
        return;
    const QJsonValue text_val = doc_obj_.value(QLatin1String("text"));
    if (text_val.isNull() || !text_val.isString())
        return;
    const QString text = text_val.toString().trimmed();
    if (text.isEmpty())
        return;
    QJsonObject message;
    message[QStringLiteral("type")] = QStringLiteral("message");
    message[QStringLiteral("text")] = text;
    message[QStringLiteral("sender")] = sender_->getUserName();
    broadcastSend(message, sender_);
}


