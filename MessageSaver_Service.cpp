#include "MessageSaver_Service.h"

MessageSaver_Service* MessageSaver_Service::p_instance = nullptr;
bool MessageSaver_Service::is_started = false;
QTimer* MessageSaver_Service::p_timer = nullptr;
QThread* MessageSaver_Service::p_thread = nullptr;


MessageSaver_Service::MessageSaver_Service(QObject* parent_) : QObject(parent_) {}

bool MessageSaver_Service::start(const float& minutes_)
{
	if (!is_started) {
		is_started = true;

		if (p_instance == nullptr) {
			p_instance = new MessageSaver_Service();
			PLOGI << "Archivator instance is created.";
		}
		else PLOGW << "Archivator instance already exist!!!";
		int timeout = (int)(minutes_ * 60000);

		p_thread = new QThread();
		p_timer = new QTimer();

		p_instance->moveToThread(p_thread);
		connect(p_timer, &QTimer::timeout, LocalStorage_Service::getInstance(), &LocalStorage_Service::saveAllMessages);
		p_timer->start((int)timeout);
		p_thread->start();
		PLOGI << "Archivator service started.";
		return true;
	}
	else
	{
		PLOGW << "Archivator was already started!!!";
		return false;
	}
}

void MessageSaver_Service::safeExit()
{
	p_timer->stop();
	p_timer->deleteLater();
	p_thread->quit();
	p_thread->deleteLater();
	p_instance->deleteLater();
}

void MessageSaver_Service::test() { PLOGN << "ASDASDASDASDASD"; };