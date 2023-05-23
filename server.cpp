#include "server.h"

Server::Server(){}
Server::~Server(){}

void Server::startServer()
{
    if(this->listen(QHostAddress::Any, 5555))
    {
        qDebug() << "Server start - OK";
    }
    else
    {
        qDebug() << "Sever start - Error";
    }

    //определить функцию загрузки данных из истории
    msgHistoryPath = "./msg_history.json";
    loadMsgHistory(msgHistoryPath);
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
                Message msg {msgJson.toObject().value("nickname").toString(),
                            msgJson.toObject().value("text").toString(),
                            QTime::fromString(msgJson.toObject().value("time").toString()),
                            msgJson.toObject().value("id").toString(),
                            msgJson.toObject().value("deleted").toBool() };
                messanges.push_back(msg);
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

        for (const Message &msg : messanges)
        {
            QVariantMap map;
            map.insert("nickname",msg.nickname);
            map.insert("text",msg.text);
            map.insert("time",msg.time);
            map.insert("id",msg.id);
            map.insert("deleted",msg.deleted);

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



void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);      //сигнально слотовые соединения, как работают?
    connect(socket, &QTcpSocket::disconnected, this, &Server::slotDisconnect);
    //connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    //тут отправлять по новому соединению запрос данных клиента
    //после возврата и получения данных
    //записывать их как пару в vector сокетов

    Sockets.push_back(socket);

    qDebug() << "new client connected" << socketDescriptor << " Now users : " << Sockets.size();

    //Send msg history to the client
    SendToClient(messanges, socket);

    SendToAllClients(createMessage("Server msg", "New connection! Hello!"));
}

void Server::slotDisconnect()
{
    
    socket = (QTcpSocket*)sender();
    //socket->deleteLater();
    Sockets.removeOne(socket);
    //Sockets.removeAll(socket);
    qDebug() << "Disconnected. " << socket->socketDescriptor() << " Now users : " << Sockets.size();
    //лучше создать сервис который будет чистить вектор подключений периодически по таймеру
}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_5);
    if(in.status() == QDataStream::Ok)
    {
        qDebug() << "read...";
//        QString str;
//        in >> str;
//        qDebug() << str;
//        SendToClient(str);
        for(;;)
        {
            if(nextBlockSize == 0)
            {
                qDebug() << "nextBlockSize = 0";
                if(socket->bytesAvailable() < 2)
                {
                    qDebug() << "Data < 2, break";
                    break;
                }
                in >> nextBlockSize;
                qDebug() << "nextBlockSize = " << nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize)
            {
                qDebug() << "Data not full, waiting...";
                break;
            }
            //очевидно что условие выше выводит из цикла for, но как мы попадаем сюда снова? получается in.status обновляет значение после выполнения строки ниже?
            Message msg;
            in >> msg.id >> msg.time >> msg.nickname >> msg.deleted >> msg.text;
            nextBlockSize = 0;

            messanges.push_back(msg);

            qDebug() << "The message: " << msg.text;
            SendToAllClients(msg);

            //To archive messanges
            uploadMsgHistory(msgHistoryPath);

            break;
        }

    }
    else
    {
        qDebug() << "DataStream error";
    }
}

//----------------SendToClient-----------------

void Server::SendToClient(const Message &msg, QTcpSocket *socket)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);

    out << quint16(0) << msg.id << msg.time << msg.nickname << msg.deleted << msg.text; // преобразовали в stream
    out.device()->seek(0);          //переходим в начало "данных"
    out << quint16(Data.size() - sizeof(quint16));
    socket->write(Data);
}

void Server::SendToClient(const QVector<Message> &msgs, QTcpSocket *socket)
{
    for (const Message &msg : msgs)
    {
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out << quint16(0) << msg.id << msg.time << msg.nickname << msg.deleted << msg.text; // преобразовали в stream
    out.device()->seek(0);          //переходим в начало "данных"
    out << quint16(Data.size() - sizeof(quint16));
    socket->write(Data);
    }
}

//void Server::SendToAllClients(const QString &str)
//{
//    Data.clear();
//    QDataStream out(&Data, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_6_5);
//    Message msg { "Server", str, QTime::currentTime(), QUuid::createUuid().toString(), false};
//    out << quint16(0) << msg.id << msg.time << msg.nickname << msg.deleted << msg.text; // преобразовали в stream
//    out.device()->seek(0);          //переходим в начало "данных"
//    out << quint16(Data.size() - sizeof(quint16));
//    //socket->write(Data);
//    for(int i(0); i < Sockets.size(); i++)
//    {
//        Sockets[i]->write(Data);
//    }
//}

void Server::SendToAllClients(const Message &msg)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);

    out << quint16(0) << msg.id << msg.time << msg.nickname << msg.deleted << msg.text; // преобразовали в stream
    out.device()->seek(0);          //переходим в начало "данных"
    out << quint16(Data.size() - sizeof(quint16));
    //socket->write(Data);
    for(int i(0); i < Sockets.size(); i++)
    {
        Sockets[i]->write(Data);
    }
}

Message Server::createMessage(QString nickame, QString text)
{
    return Message{ nickame, text, QTime::currentTime(), QUuid::createUuid().toString(), false };
}
