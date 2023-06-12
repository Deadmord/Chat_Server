#pragma once
#include <QString>
#include <QObject>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>
#include <QTimer>
#include <QThread>
#include "LocalStorage_Service.h"

class MessageSaver_Service : public QObject
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

