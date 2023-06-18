#include "SrvUser.h"

SrvUser::SrvUser(QObject* parent) : QObject(parent) , user_socket(new QTcpSocket(this))
{
    // connect readyRead() to the slot that will take care of reading the data in
    connect(user_socket, &QTcpSocket::readyRead, this, &SrvUser::receiveJson);
    // forward the disconnected and error signals coming from the socket
    connect(user_socket, &QTcpSocket::disconnected, this, &SrvUser::disconnectedFromClient);
    connect(user_socket, &QAbstractSocket::errorOccurred, this, &SrvUser::errorSignal);
}

SrvUser::SrvUser(const QString& username_, const QString& password_, const QByteArray& userpic_, const qint32& rating_, const qint32& room_id_)
    : username(username_), password(password_), userpic(userpic_), rating(rating_), room_id(room_id_) {}

bool SrvUser::setSocketDescriptor(qintptr socketDescriptor)
{
    return user_socket->setSocketDescriptor(socketDescriptor);
}

void SrvUser::disconnectFromClient()
{
    user_socket->disconnectFromHost();
    //-----------------сделать рефакторинг тут и в сервере-------------------------------
}

QString SrvUser::getUserName() const
{
    return username;
}

void SrvUser::setUserName(const QString& userName)
{
    username = userName;
}

qint32 SrvUser::getRoomId() const
{
    return room_id;
}

void SrvUser::setRoomId(const qint32& _room_id)
{
    room_id = _room_id;
}

QString SrvUser::getPassword() const { return password; }
void SrvUser::setPassword(const QString& password_) { password = password_; }
QByteArray SrvUser::getUserpic() const { return userpic; }
void SrvUser::setUserpic(const QByteArray& userpic_) { userpic = userpic_; }
qint32 SrvUser::getRating() const { return rating; }
void SrvUser::setRating(const qint32& rating_) { rating = rating_; }

bool SrvUser::isFloodLimit() const { return flood_limit; }

void SrvUser::setFloodLimit()
{
    QTimer::singleShot(5000, [&]() { flood_limit = false; });
    flood_limit = true;
}

void SrvUser::sendJson(const QJsonObject& json)
{
    // we crate a temporary QJsonDocument forom the object and then convert it
    // to its UTF-8 encoded version. We use QJsonDocument::Compact to save bandwidth
    const QByteArray jsonData = QJsonDocument(json).toJson(QJsonDocument::Compact);
    // we notify the central server we are about to send the message
    PLOGD << QLatin1String("Sending to ") + getUserName() + QLatin1String(" - ") + QString::fromUtf8(jsonData);
    QByteArray buffer;
    buffer.clear();
    // create a QDataStream for buffer operating 
    QDataStream socketStream(&buffer, QIODevice::WriteOnly);
    socketStream.setVersion(QDataStream::Qt_6_5);
    socketStream << quint16(0) << jsonData;
    socketStream.device()->seek(0); //go to beginning data storage
    socketStream << quint16(buffer.size() - sizeof(quint16));
    user_socket->write(buffer);
}

void SrvUser::receiveJson()
{
    // prepare a container to hold the UTF-8 encoded JSON we receive from the socket
    QByteArray jsonData;
    QJsonDocument json;
    QByteArray data;
    QByteArray type;
    // create a QDataStream operating on the socket
    QDataStream socketStream(user_socket);
    qint16 typeSize;
    qint16 jsonDataSize;
    qint16 dataSize;
    socketStream.setVersion(QDataStream::Qt_6_5);
   

   

    // set the version so that programs compiled with different versions of Qt can agree on how to serialise
    if (socketStream.status() == QDataStream::Ok)
    {
        
        PLOGD << "read...";
        // start an infinite loop
        for (;;) {
            if (nextBlockSize == 0)
            {
                if (user_socket->bytesAvailable() < 2)
                {
                    PLOGD << "Data < 2, break";
                    break;
                }
                socketStream >> nextBlockSize;
                PLOGD << "nextBlockSize = " + QString::number(nextBlockSize) + " byte. Available " + QString::number(user_socket->bytesAvailable());
            }
            if (user_socket->bytesAvailable() < nextBlockSize)
            {
                PLOGD << "Data not full, waiting..." + QString::number(user_socket->bytesAvailable()) + " byte available";
                break;
            }
            // we start a transaction so we can revert to the previous state in case we try to read more data than is available on the socket
            socketStream.startTransaction();
            // we try to read the JSON data
            socketStream >> data;

            socketStream >> type;
            PLOGE << type;
            //socketStream >> jsonData;
            //socketStream >> typeSize;
            //QByteArray typeBytes(typeSize, Qt::Uninitialized);
            //if (socketStream.readRawData(typeBytes.data(), typeSize) != typeSize) {
            //    // Handle error condition
            //    return;
            //}
            //type = QString::fromUtf8(typeBytes);
            //socketStream >> jsonDataSize;
            //jsonData.resize(jsonDataSize);
            //if (socketStream.readRawData(jsonData.data(), jsonDataSize) != jsonDataSize) {
            //    // Handle error condition
            //    return;
            //}
            //socketStream >> dataSize;
            //data.resize(dataSize);
            //if (socketStream.readRawData(data.data(), dataSize) != dataSize) {
            //    // Handle error condition
            //    return;
            //}
                if (socketStream.commitTransaction()) {
                    // we successfully read some data
                    // we now need to make sure it's in fact a valid JSON
                    QJsonParseError parseError;
                    PLOGI << type;
                    PLOGF << jsonData;
                    // we try to create a json document with the data we received
                	QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
                    if (parseError.error == QJsonParseError::NoError) {
                        // if the data was indeed valid JSON
                        if (jsonDoc.isObject())// and is a JSON object
                        {
                            if (type == "wMedia")
                            {
                                emit jsonWMedia(QSharedPointer<SrvUser>::create(this), jsonDoc.object(), data);
                            }
                            else if(type == "Json")
                            {
                                PLOGI << "WORKING!!!";
								emit jsonReceived(QSharedPointer<SrvUser>::create(this), jsonDoc.object()); // send the message to the central server
                            }
                            else if(type == "mediaRequest")
                            {
                                emit mediaRequest(QSharedPointer<SrvUser>::create(this), jsonDoc.object());
                            }
                        }
                        else
                            PLOGE << QLatin1String("Invalid message: ") + QString::fromUtf8(jsonData); //notify the server of invalid data
                    }
                    else {
                        PLOGE << QLatin1String("Invalid message: ") + QString::fromUtf8(jsonData); //notify the server of invalid data
                    }
                    // loop and try to read more JSONs if they are available
                }

                else {
                    // the read failed, the socket goes automatically back to the state it was in before the transaction started
                    // we just exit the loop and wait for more data to become available
                    break;
                }
            
            nextBlockSize = 0;
            break;
        }
    }
    else
    {
        PLOGE << "DataStream error";
    }
    
}

