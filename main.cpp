#include <QtCore/QCoreApplication>
#include <iostream>

#include "server.h"
#include "RoomController.h"
#include "DBRoom.h"
#include "DBService.h"

#include <plog/Log.h> 
#include <plog/Initializers/RollingFileInitializer.h>

Server server;

static void shutdown_routine()
{
    qDebug() << "The server shatdown.";
}

static void startup_routine()
{
    // set up some application settings
    qApp->setOrganizationName("C++ Final Project"); // through macro
    qApp->setApplicationName("InetChat");

    // add QT post exit rotine
    qAddPostRoutine(shutdown_routine);

    // add a routine to be executed just before application exits through signal
    QObject::connect(qApp, &QCoreApplication::aboutToQuit, []()
        {
            qDebug() << "AboutToQuit : InetChat finishing...";;
        }
    );

    // register some meta types
    qRegisterMetaType<RoomController>();

    plog::init(plog::debug, "log.txt", 1000000, 5);
    PLOGD << "Server Application Starting. Logging is enabled.";

    QTimer::singleShot(0, [&]()
        {
            server.startServer();

        QTimer::singleShot(100, []() { qDebug() << "Server initialized"; });
        });
}
Q_COREAPP_STARTUP_FUNCTION(startup_routine)

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    return a.exec();
}


//Logger severity
//enum Severity
//{
//    none = 0,
//    fatal = 1,
//    error = 2,
//    warning = 3,
//    info = 4,
//    debug = 5,
//    verbose = 6
//};

//Short simple macros
//PLOGV << "verbose";
//PLOGD << "debug";
//PLOGI << "info";
//PLOGW << "warning";
//PLOGE << "error";
//PLOGF << "fatal";
//PLOGN << "none";

//Conditional macros
//PLOGV_IF(cond) << "verbose";
//PLOGD_IF(cond) << "debug";
//PLOGI_IF(cond) << "info";
//PLOGW_IF(cond) << "warning";
//PLOGE_IF(cond) << "error";
//PLOGF_IF(cond) << "fatal";
//PLOGN_IF(cond) << "none";