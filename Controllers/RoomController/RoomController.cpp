#include "RoomController.h"

RoomController::RoomController(QObject* parent) : QObject(parent), name("Default name")
{
	connect(this, qOverload<int>(&RoomController::signal_1), this, qOverload<int>(&RoomController::on_signal_1));
	connect(this, qOverload<QString>(&RoomController::signal_1), this, qOverload<QString>(&RoomController::on_signal_1));
}

RoomController::~RoomController() = default;

RoomController::RoomController(const RoomController& other)
{
	id = other.id;
	name = other.name;
	description = other.description;
	topic_id = other.topic_id;
	is_private = other.is_private;
	password = other.password;
	is_deleted = other.is_deleted;
	messages = other.messages;

	QVector<QMap<User, QTcpSocket*>*> connected_users;
}

RoomController& RoomController::operator=(const RoomController& other)
{
	if (this != &other)
	{
		name = other.name;
	}
	return *this;
}


void RoomController::initDir()
{

}

//-----------slots------------
void RoomController::on_signal_1(int val)
{
	qDebug() << "MyClass1::on_signal_1: " << val;
}

void RoomController::on_signal_1(QString str)
{
	qDebug() << "MyClass1::on_signal_1: " << str;
}