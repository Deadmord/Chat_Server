#include "UserConnection.h"
UserConnection::UserConnection(QObject* parent) : QObject(parent)
    , user_socket(new QTcpSocket(this))
{
    // connect readyRead() to the slot that will take care of reading the data in
    connect(user_socket, &QTcpSocket::readyRead, this, &UserConnection::receiveJson);
    // forward the disconnected and error signals coming from the socket
    connect(user_socket, &QTcpSocket::disconnected, this, &UserConnection::disconnectedFromClient);
    connect(user_socket, &QAbstractSocket::errorOccurred, this, &UserConnection::error);
}


bool UserConnection::setSocketDescriptor(qintptr socketDescriptor)
{
    return user_socket->setSocketDescriptor(socketDescriptor);
}

void UserConnection::sendJson(const QJsonObject& json)
{
    // we crate a temporary QJsonDocument forom the object and then convert it
    // to its UTF-8 encoded version. We use QJsonDocument::Compact to save bandwidth
    const QByteArray jsonData = QJsonDocument(json).toJson(QJsonDocument::Compact);
    // we notify the central server we are about to send the message
    emit logMessage(QLatin1String("Sending to ") + getUserName() + QLatin1String(" - ") + QString::fromUtf8(jsonData));
    // we send the message to the socket in the exact same way we did in the client
    QDataStream socketStream(user_socket);
    socketStream.setVersion(QDataStream::Qt_5_7);
    socketStream << jsonData;
}

void UserConnection::disconnectFromClient()
{
    user_socket->disconnectFromHost();
}

QString UserConnection::getUserName() const
{
    return user_name;
}

void UserConnection::setUserName(const QString& userName)
{
    user_name = userName;
}

void UserConnection::receiveJson()
{
    // prepare a container to hold the UTF-8 encoded JSON we receive from the socket
    QByteArray jsonData;
    // create a QDataStream operating on the socket
    QDataStream socketStream(user_socket);
    // set the version so that programs compiled with different versions of Qt can agree on how to serialise
    socketStream.setVersion(QDataStream::Qt_5_7);
    // start an infinite loop
    for (;;) {
        // we start a transaction so we can revert to the previous state in case we try to read more data than is available on the socket
        socketStream.startTransaction();
        // we try to read the JSON data
        socketStream >> jsonData;
        if (socketStream.commitTransaction()) {
            // we successfully read some data
            // we now need to make sure it's in fact a valid JSON
            QJsonParseError parseError;
            // we try to create a json document with the data we received
            const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
            if (parseError.error == QJsonParseError::NoError) {
                // if the data was indeed valid JSON
                if (jsonDoc.isObject()) // and is a JSON object
                    emit jsonReceived(jsonDoc.object()); // send the message to the central server
                else
                    emit logMessage(QLatin1String("Invalid message: ") + QString::fromUtf8(jsonData)); //notify the server of invalid data
            }
            else {
                emit logMessage(QLatin1String("Invalid message: ") + QString::fromUtf8(jsonData)); //notify the server of invalid data
            }
            // loop and try to read more JSONs if they are available
        }
        else {
            // the read failed, the socket goes automatically back to the state it was in before the transaction started
            // we just exit the loop and wait for more data to become available
            break;
        }
    }
}
