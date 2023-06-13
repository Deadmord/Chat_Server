#include "RoomController.h"

RoomController::RoomController(const quint32& id_, const QString& name_, const QString& description_, const quint32& topic_id_, const bool& is_private_, const QString& password_, bool is_deleted_, QObject* parent)
    : id(id_), name(name_), description(description_), topic_id(topic_id_), is_private(is_private_), password(password_), is_deleted(is_deleted_), QObject(parent)
{
    emit roomCreated(this);
    QMetaObject::invokeMethod(this, "initRoom", Qt::QueuedConnection);      //Ran initRoom() at separate thread. Make sure it is..
    connect(this, &RoomController::roomCreated, this, &RoomController::initRoom);
}

RoomController::RoomController(const quint32& id_, const QString& name_, const QString& description_, const quint32& topic_id_, const bool& is_private_, const QString& password_, const bool& is_deleted_)
    : id(id_), name(name_), description(description_), topic_id(topic_id_), is_private(is_private_), password(password_), is_deleted(is_deleted_), messages(QVector<User_Message>{}), connected_users(QVector<UserConnection*>{}) {};

RoomController::~RoomController() = default;

void RoomController::initRoom()
{
    loadMsgHistory(msg_history_path);
}

quint32 RoomController::getId() const { return id; }
QString RoomController::getName() const { return name; }
QString RoomController::getDescription() const { return description; }
quint32 RoomController::getTopicId() const { return topic_id; }
bool RoomController::isPrivate() const { return is_private; }
QString RoomController::getPassword() const { return password; }
bool RoomController::isDeleted() const { return is_deleted; }

void RoomController::setName(const QString& val) { name = val; emit nameChanged(val); }
void RoomController::setDescription(const QString& val) { description = val; emit descriptionChanged(val); }
void RoomController::setPrivate(bool val) { is_private = val; emit privateChanged(val); }
void RoomController::setPassword(const QString& val) { password = val; emit passwordChanged(); }
void RoomController::Delete() { is_deleted = true; emit roomDeleted(); }

void RoomController::userEntry(UserConnection* user)
{
    connected_users.append(user);
}

void RoomController::jsonReceived(UserConnection* sender, const QJsonObject& docObj)
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

//---!!!---это от сюда убрать будет не нужно
void RoomController::broadcastSend(const QJsonObject& message, UserConnection* exclude)
{
    for (UserConnection* user : connected_users) {
        Q_ASSERT(user);
        if (user == exclude)
            continue;
        sendJson(user, message);
    }
}

//---!!!---это от сюда убрать будет не нужно
void RoomController::sendJson(UserConnection* destination, const QJsonObject& message)
{
    Q_ASSERT(destination);
    destination->sendJson(message);
}

//-----------------------Load/Upload History--------------------------
void RoomController::loadMsgHistory(const QString path)
{
    QJsonDocument msgHistory;
    QJsonArray msgArray;
    QJsonParseError jsonError;
    QFile msgFile;

    msgFile.setFileName(path);
    if (msgFile.open(QIODevice::ReadOnly | QFile::Text))
    {
        //тот нужно блокировать обращение к ресурсу msgFile
        msgHistory = QJsonDocument::fromJson(QByteArray(msgFile.readAll()), &jsonError);
        msgFile.close();

        if (jsonError.errorString().toInt() == QJsonParseError::NoError)
        {
            msgArray = QJsonValue(msgHistory.object().value("messanges")).toArray();
            for (const auto& msgJson : msgArray)
            {
                Message msg{ msgJson.toObject().value("id").toString(),
                           static_cast<quint32>(msgJson.toObject().value("roomId").toInt()),
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

void RoomController::uploadMsgHistory(const QString path)
{
    QJsonDocument msgHistory;
    QJsonArray msgArray;
    QFile msgFile;

    msgFile.setFileName(path);
    if (msgFile.open(QIODevice::WriteOnly | QFile::Text))
    {
        msgArray = msgHistory.object().value("messanges").toArray();

        for (const User_Message& msg : messages)
        {
            QVariantMap map;
            map.insert("id", msg.getId());
            map.insert("roomId", msg.getRoomId());
            map.insert("time", msg.getDateTime());
            map.insert("nickname", msg.getNickname());
            map.insert("text", msg.getText());
            map.insert("mediaId", msg.getMedia());
            map.insert("parentId", msg.getParentId());
            map.insert("deleted", msg.isDeleted());

            QJsonObject msgJson = QJsonObject::fromVariantMap(map);
            msgArray.append(msgJson);           //Тут нужно проверять есть ли такой элемент уже в массиве и вставлять если нет
        }
        msgHistory.setArray(msgArray);
        msgFile.write("{\"messanges\":" + msgHistory.toJson() + "}");
    }
    else
    {
        qDebug() << "File message history can't be open.";
    }
}