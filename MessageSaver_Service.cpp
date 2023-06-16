#include "MessageSaver_Service.h"

QSharedPointer<MessageSaver_Service> MessageSaver_Service::shp_instance = nullptr;
bool MessageSaver_Service::is_started = false;
QSharedPointer<QTimer> MessageSaver_Service::shp_timer = nullptr;
QSharedPointer<QThread> MessageSaver_Service::shp_thread = nullptr;


MessageSaver_Service::MessageSaver_Service(QObject* parent_) : QObject(parent_) {}

bool MessageSaver_Service::start(const float& minutes_)
{
	if (!is_started) {
		is_started = true;

		if (shp_instance == nullptr) {
			shp_instance = QSharedPointer<MessageSaver_Service>(new MessageSaver_Service(), &QObject::deleteLater);
			PLOGI << "Archivator instance is created.";
		}
		else PLOGW << "Archivator instance already exist!!!";
		int timeout = (int)(minutes_ * 60000);

		shp_thread = QSharedPointer<QThread>(new QThread(), &QObject::deleteLater);
		shp_timer = QSharedPointer<QTimer>(new QTimer(), &QObject::deleteLater);

		shp_instance->moveToThread(shp_thread.get());
		connect(shp_timer.get(), &QTimer::timeout, LocalStorage_Service::getInstance().get(), &LocalStorage_Service::saveAllMessages);
		connect(shp_timer.get(), &QTimer::timeout, RatingCounter_Service::getInstance().get(), &RatingCounter_Service::updateRating);
		shp_timer->start((int)timeout);
		shp_thread->start();
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
	shp_timer->stop();
	shp_timer->deleteLater();
	shp_thread->quit();
	shp_thread->deleteLater();
	shp_instance->deleteLater();
}

void MessageSaver_Service::test() { PLOGN << "ASDASDASDASDASD"; };