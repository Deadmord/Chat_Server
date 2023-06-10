#include "server.h"

Server::Server(QObject* parent) : QTcpServer(parent) {}
Server::~Server(){}

void Server::startServer()
{
    loadConfig(CONFIG_FILE_PATH);
    openConnection();
    loadRooms();
    //определить функцию загрузки данных из истории
    loadMsgHistory(msg_history_path);
    emit logMessage(info, "Server initialized");
}

void Server::stopServer()
{
    if (isListening()) {
        disableUsers();
        close();
        emit logMessage(info, "Server stop - OK");
    }
    else
    {
        qDebug() << "Server alrady spopped";
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    UserConnection* user_connection = new UserConnection(this);
    if (!user_connection->setSocketDescriptor(socketDescriptor)) {
        user_connection->deleteLater();         //if the socket descriptor could not be set, delete the socket
        emit logMessage(error, "Socket descriptor could not be set");
        return;
    }
    connect(user_connection, &UserConnection::disconnectedFromClient, this, std::bind(&Server::userDisconnected, this, user_connection));
    connect(user_connection, &UserConnection::errorSignal, this, std::bind(&Server::userError, this, user_connection));
    connect(user_connection, &UserConnection::jsonReceived, this, std::bind(&Server::jsonReceived, this, user_connection, std::placeholders::_1));
    connect(user_connection, &UserConnection::logMessage, this, &Server::logMessage);
    connected_users.append(user_connection);
    emit logMessage(info ,"New client Connected");

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

//---------------old------------------
void Server::slotDisconnect()
{

    socket = (QTcpSocket*)sender();
    //socket->deleteLater();
    sockets.removeOne(socket);
    //Sockets.removeAll(socket);
    qDebug() << "Disconnected. " << socket->socketDescriptor() << " Now users : " << sockets.size();
    //лучше создать сервис который будет чистить вектор подключений периодически по таймеру
}

//---------------old------------------
void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_5);
    if (in.status() == QDataStream::Ok)
    {
        qDebug() << "read...";
        //        QString str;
        //        in >> str;
        //        qDebug() << str;
        //        SendToClient(str);
        for (;;)
        {
            if (nextBlockSize == 0)
            {
                qDebug() << "nextBlockSize = 0";
                if (socket->bytesAvailable() < 2)
                {
                    qDebug() << "Data < 2, break";
                    break;
                }
                in >> nextBlockSize;
                qDebug() << "nextBlockSize = " << nextBlockSize;
            }
            if (socket->bytesAvailable() < nextBlockSize)
            {
                qDebug() << "Data not full, waiting...";
                break;
            }
            //очевидно что условие выше выводит из цикла for, но как мы попадаем сюда снова? получается in.status обновляет значение после выполнения строки ниже?
            Message msg;
            in >> msg.id >> msg.date_time >> msg.nickname >> msg.deleted >> msg.text;
            nextBlockSize = 0;

            //parce to User_Message----------------------------------

            messages.push_back(msg);    //implecetly

            qDebug() << "The message: " << msg.text;
            //SendToAllClients(msg);      //implecetly

            //To archive messanges
            uploadMsgHistory(msg_history_path);

            break;
        }

    }
    else
    {
        qDebug() << "DataStream error";
    }
}

void Server::loadConfig(QString _path)
{
    QFile config_file;
    QJsonDocument config_file_doc;
    QJsonObject config_json;
    QJsonParseError jsonError;

    config_file.setFileName(_path);

    if (config_file.open(QIODevice::ReadOnly | QFile::Text))
    {
        config_file_doc = QJsonDocument::fromJson(QByteArray(config_file.readAll()), &jsonError);
        config_file.close();

        if (jsonError.error == QJsonParseError::NoError)
        {
            config_json = config_file_doc.object();

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

            if (const QJsonValue v = config_file_doc["MessagesHistorySettings"]["Path"]; v.isString())
                msg_history_path = v.toString();
            else
                qWarning() << "Error BlackListPath reading";

        }
        else
        {
            qWarning() << "Error config file read: " << jsonError.error;
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
            emit logMessage(info, "Server start - OK");
        }
        else
        {
            emit logMessage(error, "Sever start - Error");
        }
    }
    else
    {
        emit logMessage(debug, "Server alrady listen");
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
    //Load rooms from repository to vector<Room>
    //run loop to init all rooms
}

void Server::loadMsgHistory(const QString path)
{
    QJsonDocument msgHistory;
    QJsonArray msgArray;
    QJsonParseError jsonError;
    QFile msgFile;

    msgFile.setFileName(path);
    if (msgFile.open(QIODevice::ReadOnly|QFile::Text))
    {
        //тот нужно блокировать обращение к ресурсу msgFile
        msgHistory = QJsonDocument::fromJson(QByteArray(msgFile.readAll()),&jsonError);
        msgFile.close();

        if(jsonError.errorString().toInt() == QJsonParseError::NoError)
        {
            msgArray = QJsonValue(msgHistory.object().value("messanges")).toArray();
            for (const auto &msgJson : msgArray)
            {
                Message msg { msgJson.toObject().value("id").toString(),
                            msgJson.toObject().value("roomId").toInt(),
                            QDateTime::fromString(msgJson.toObject().value("time").toString()),
                            msgJson.toObject().value("nickname").toString(),
                            msgJson.toObject().value("text").toString(),
                            msgJson.toObject().value("mediaId").toString(),
                            msgJson.toObject().value("parentId").toString(),
                            msgJson.toObject().value("deleted").toBool() };
                messages.push_back(msg);
            }
        }
        else
        {
            qDebug() << "Error message history read: " << jsonError.error;
        }
    }
    else
    {
        qDebug() << "File message history can't be open.";
    }
}

void Server::uploadMsgHistory(const QString path)
{
    QJsonDocument msgHistory;
    QJsonArray msgArray;
    QFile msgFile;

    msgFile.setFileName(path);
    if (msgFile.open(QIODevice::WriteOnly|QFile::Text))
    {
        msgArray = msgHistory.object().value("messanges").toArray();

        for (const User_Message &msg : messages)
        {
            QVariantMap map;
            map.insert("id", msg.getId());
            map.insert("roomId", msg.getRoomId());
            map.insert("time", msg.getDateTime());
            map.insert("nickname",msg.getNickname());
            map.insert("text",msg.getText());
            map.insert("mediaId", msg.getMedia());
            map.insert("parentId", msg.getParentId());
            map.insert("deleted",msg.isDeleted());

            QJsonObject msgJson = QJsonObject::fromVariantMap(map);
            msgArray.append(msgJson);           //Тут нужно проверять есть ли такой элемент уже в массиве и вставлять если нет
        }
        msgHistory.setArray(msgArray);
        msgFile.write("{\"messanges\":"+msgHistory.toJson()+"}");
    }
    else
    {
        qDebug() << "File message history can't be open.";
    }
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

User_Message Server::createMessage(QString _nickname, QString _text)
{
    Message msg;
    msg.nickname = _nickname;
    msg.text = _text;
    msg.room_id = 0;
    msg.deleted = false;
    return User_Message(msg);
    //return User_Message(QUuid::createUuid().toString(), 0, QDateTime::currentDateTime(), _nickname, _text);
}

void Server::broadcastSend(const QJsonObject& message, UserConnection* exclude)
{
    for (UserConnection* user : connected_users) {
        Q_ASSERT(user);
        if (user == exclude)
            continue;
        sendJson(user, message);
    }
}

void Server::sendJson(UserConnection* destination, const QJsonObject& message)
{
    Q_ASSERT(destination);
    destination->sendJson(message);
}

void Server::jsonReceived(UserConnection* sender, const QJsonObject& doc)
{
    Q_ASSERT(sender);
    emit logMessage(info, QLatin1String("JSON received ") + QString::fromUtf8(QJsonDocument(doc).toJson()));
    if (sender->getUserName().isEmpty())
        return jsonFromLoggedOut(sender, doc);
    jsonFromLoggedIn(sender, doc);
}

void Server::userDisconnected(UserConnection* sender)
{
    connected_users.removeAll(sender);
    const QString userName = sender->getUserName();
    if (!userName.isEmpty()) {
        QJsonObject disconnectedMessage;
        disconnectedMessage[QStringLiteral("type")] = QStringLiteral("userdisconnected");
        disconnectedMessage[QStringLiteral("username")] = userName;
        broadcastSend(disconnectedMessage, nullptr);
        emit logMessage(info, userName + QLatin1String(" disconnected"));
    }
    sender->deleteLater();
}

void Server::userError(UserConnection* sender)
{
    Q_UNUSED(sender)
        emit logMessage(error, QLatin1String("Error from ") + sender->getUserName());
}

void Server::jsonFromLoggedOut(UserConnection* sender, const QJsonObject& docObj)
{
    Q_ASSERT(sender);
    const QJsonValue typeVal = docObj.value(QLatin1String("type"));
    if (typeVal.isNull() || !typeVal.isString())
        return;
    if (typeVal.toString().compare(QLatin1String("login"), Qt::CaseInsensitive) != 0)
        return;
    const QJsonValue usernameVal = docObj.value(QLatin1String("username"));
    if (usernameVal.isNull() || !usernameVal.isString())
        return;
    const QString newUserName = usernameVal.toString().simplified();
    if (newUserName.isEmpty())
        return;
    for (UserConnection* user : qAsConst(connected_users)) {    //Find duplicat username
        if (user == sender)
            continue;
        if (user->getUserName().compare(newUserName, Qt::CaseInsensitive) == 0) {
            QJsonObject message;
            message[QStringLiteral("type")] = QStringLiteral("login");
            message[QStringLiteral("success")] = false;
            message[QStringLiteral("reason")] = QStringLiteral("duplicate username");
            sendJson(sender, message);
            return;
        }
    }
    sender->setUserName(newUserName);
    QJsonObject successMessage;
    successMessage[QStringLiteral("type")] = QStringLiteral("login");
    successMessage[QStringLiteral("success")] = true;
    sendJson(sender, successMessage);
    QJsonObject connectedMessage;
    connectedMessage[QStringLiteral("type")] = QStringLiteral("newuser");
    connectedMessage[QStringLiteral("username")] = newUserName;
    broadcastSend(connectedMessage, sender);
}

void Server::jsonFromLoggedIn(UserConnection* sender, const QJsonObject& docObj)
{
    Q_ASSERT(sender);
    const QJsonValue typeVal = docObj.value(QLatin1String("type"));
    if (typeVal.isNull() || !typeVal.isString())
        return;
    if (typeVal.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) != 0)
        return;
    const QJsonValue textVal = docObj.value(QLatin1String("text"));
    if (textVal.isNull() || !textVal.isString())
        return;
    const QString text = textVal.toString().trimmed();
    if (text.isEmpty())
        return;
    QJsonObject message;
    message[QStringLiteral("type")] = QStringLiteral("message");
    message[QStringLiteral("text")] = text;
    message[QStringLiteral("sender")] = sender->getUserName();
    broadcastSend(message, sender);
}


