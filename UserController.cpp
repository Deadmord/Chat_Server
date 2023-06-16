#include "UserController.h"

UserController::UserController(QObject* object_) : QObject(object_) {}

UserController::~UserController() {}

QSharedPointer<UserController> UserController::instance()
{
    if (!shp_instance)
    {
        shp_instance = QSharedPointer<UserController>(new UserController());
    }

    connect(shp_instance.get(), &UserController::broadcastSend, MessageController::instance().get(), &MessageController::broadcastSend); //это нужно перенести в MessageController

    return shp_instance;
}

void UserController::addConnection(qintptr socket_descriptor_)
{
    QSharedPointer<SrvUser> user_connection = QSharedPointer<SrvUser>(new SrvUser());
    if (!user_connection->setSocketDescriptor(socket_descriptor_)) {
        user_connection->deleteLater();         //if the socket descriptor could not be set, delete the socket
        PLOGE << "Socket descriptor could not be set";
        return;
    }
    connect(user_connection.get(), &SrvUser::disconnectedFromClient, this, std::bind(&UserController::userDisconnected, this, user_connection));
    connect(user_connection.get(), &SrvUser::errorSignal, this, std::bind(&UserController::userError, this, user_connection));
    connect(user_connection.get(), &SrvUser::jsonReceived, MessageController::instance().get(), std::bind(&MessageController::jsonReceived, MessageController::instance().get(), user_connection, std::placeholders::_1));  //connect with MessageController (это нужно перенести в MessageController)

    connected_users.insert(user_connection);
    PLOGI << "New client Connected! Now users: " + QString::number(connected_users.size());
}

void UserController::disableUsers()
{
    for (QSharedPointer<SrvUser> user : connected_users) {
        user->disconnectFromClient();
    }
}

void UserController::userDisconnected(QSharedPointer<SrvUser> sender_)
{
    connected_users.remove(sender_);
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

void UserController::userError(const QSharedPointer<SrvUser> sender_)
{
    Q_UNUSED(sender_)
        PLOGE << QLatin1String("Error from ") + sender_->getUserName();
}



QSet<QSharedPointer<SrvUser>> UserController::getUsersList() const
{
    return connected_users;
}
