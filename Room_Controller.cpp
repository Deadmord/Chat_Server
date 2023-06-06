#include "Room_Controller.h"

Room_Controller::Room_Controller(QObject* parent) : QObject(parent), _text("Default Text")
{
	connect(this, qOverload<int>(&Room_Controller::signal_1), this, qOverload<int>(&Room_Controller::on_signal_1));
	connect(this, qOverload<QString>(&Room_Controller::signal_1), this, qOverload<QString>(&Room_Controller::on_signal_1));
}

Room_Controller::Room_Controller(const Room_Controller& other)
{
	_text = other._text;
}

Room_Controller::~Room_Controller()
= default;

Room_Controller& Room_Controller::operator=(const Room_Controller& other)
{
	if (this != &other)
	{
		_text = other._text;
	}
	return *this;
}

void Room_Controller::initDir()
{

}


//-----------slots------------
void Room_Controller::on_signal_1(int val)
{
	qDebug() << "MyClass1::on_signal_1: " << val;
}

void Room_Controller::on_signal_1(QString str)
{
	qDebug() << "MyClass1::on_signal_1: " << str;
}