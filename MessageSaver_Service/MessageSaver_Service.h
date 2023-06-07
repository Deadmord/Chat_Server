#pragma once
#include <QString>
#include <QObject>
#include "messagesaver_service_global.h"
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>
#include <QTimer>
#include <QThread>

class MESSAGESAVER_SERVICE_EXPORT MessageSaver_Service : public QObject
{
    Q_OBJECT
private:
    MessageSaver_Service() {};
    MessageSaver_Service(MessageSaver_Service&);
    MessageSaver_Service(MessageSaver_Service&&);
    static MessageSaver_Service* service;
    ~MessageSaver_Service() {};

public:

    static bool start();
    static bool is_started;

public slots:

    void test();


};

extern void MESSAGESAVER_SERVICE_EXPORT initialize(plog::Severity severity, plog::IAppender * appender);