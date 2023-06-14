#pragma once

#ifndef MESSAGESAVER_SERVICE_H
#define MESSAGESAVER_SERVICE_H

#include <QString>
#include <QObject>
#include <QTimer>
#include <QThread>

#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>

#include "LocalStorage_Service.h"
#include "RatingCounter_Service.h"

class MessageSaver_Service : public QObject
{
    Q_OBJECT

private:

    explicit MessageSaver_Service(QObject* parent_ = nullptr);

    static QSharedPointer<MessageSaver_Service> shp_instance;
    static bool is_started;
    static QSharedPointer<QTimer> shp_timer;
    static QSharedPointer<QThread> shp_thread;


public:

    MessageSaver_Service(const MessageSaver_Service&) = delete;
    MessageSaver_Service& operator=(const MessageSaver_Service&) = delete;
    static bool start(const float& minutes_);

public slots:

    void test();
    void safeExit();


};

#endif //MESSAGESAVER_SERVICE_H

