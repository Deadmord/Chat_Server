#include "UserController.h"

UserController::UserController(QObject* object_) : QObject(object_) {}

UserController::~UserController() {}

QSharedPointer<UserController> UserController::instance()
{
    if (!shp_instance)
    {
        shp_instance = QSharedPointer<UserController>(new UserController());
    }

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
    QtConcurrent::map(connected_users, [](QSharedPointer<SrvUser> user) {
        user->disconnectFromClient();
        }).waitForFinished();
}

void UserController::userDisconnected(QSharedPointer<SrvUser> sender_)
{
    QtConcurrent::run([&](QSharedPointer<SrvUser> sender_) {
        connected_users.remove(sender_);
        const quint32 userRoom = sender_->getRoomId();
        if (userRoom != 0) {
            RoomController::instance()->userLeave(sender_);
            //добавить connection() и заменить на emit user leave
        }
        PLOGI << sender_->getUserName() + QLatin1String(" disconnected, total users left: ") + QString::number(connected_users.size());
        sender_->deleteLater();
    }, sender_)
        .then([&sender_]() {
            PLOGI << "User disconected from server. Id: " + sender_->getUserName(); 
        });
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
