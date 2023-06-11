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
#include "DTOUser.h"
#include "DTORoom.h"
int main(int argc, char* argv[])
{
    plog::init(plog::debug, "log.txt", 1000000, 5);
    //QCoreApplication a(argc, argv);
    //Server server;
    //server.startServer();
    //return a.exec();

    QByteArray arrayy("55554544");
    //const char* data = "11111";
    //QByteArray arrayThis = QByteArray::fromRawData(data, strlen(data));
    DBService::UserRepository userRepo("Driver={ODBC Driver 18 for SQL Server};Server=tcp:comp-zionet-server.database.windows.net,1433;Database=CPP_Chat_DB;Uid=Logika4417;Pwd=Fyyf1998;Encrypt=yes;TrustServerCertificate=no;Connection Timeout=30;");

    //auto future = userRepo.updateUser("logginn", "", arrayy);
    //qDebug() << "result: " << future.result();

    //DBEntity::DBUser user("logginn", "mypassword", arrayThis, 0, false);

    //auto future = userRepo.createUser(user);
    //qDebug() << future.result();
 
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