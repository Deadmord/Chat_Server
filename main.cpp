#include <QtCore/QCoreApplication>
#include "server.h"
#include "DBRoom.h"
#include "DBService.h"
#include <iostream>
#include "DBService.h"
#include <plog/Log.h> 
#include <plog/Initializers/RollingFileInitializer.h>
#include "RoomRepository.h"
#include "UserRepository.h"

int main(int argc, char* argv[])
{
    plog::init(plog::debug, "log.txt", 1000000, 5);
    //QCoreApplication a(argc, argv);
    //Server server;
    //server.startServer();
    //return a.exec();
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