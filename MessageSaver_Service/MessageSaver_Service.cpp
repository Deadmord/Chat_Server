#include "MessageSaver_Service.h"

bool MessageSaver_Service::is_started = false;
MessageSaver_Service* MessageSaver_Service::service;

bool MessageSaver_Service::start()
{
	if (!is_started) {
		is_started = true;

		if (service == nullptr) {
			service = new MessageSaver_Service();
			PLOGI << "Archivator instance is created.";
		}
		else PLOGW << "Archivator instance already exist!!!";

		QThread* thread = new QThread();
		QTimer* timer = new QTimer();

		service->moveToThread(thread);
		connect(timer, &QTimer::timeout, LocalStorage_Service::getInstance(), &LocalStorage_Service::saveAllMessages);
		timer->start(1000);
		thread->start();
		PLOGI << "Archivator started.";
		return true;
	}
	else
	{
		PLOGW << "Archivator was already started!!!";
		return false;
	}
}

void MessageSaver_Service::test() { PLOGN << "ASDASDASDASDASD"; };

void MESSAGESAVER_SERVICE_EXPORT initialize(plog::Severity severity, plog::IAppender* appender)
{
	plog::init(severity, appender); // Initialize the shared library logger.
}
