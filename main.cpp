#include <QtCore/QCoreApplication>
#include "server.h"
#include "DBRoom.h"
#include "DBService.h"
#include <iostream>
#include "RoomRepository.h"
#include <plog/Log.h> 
#include <plog/Initializers/RollingFileInitializer.h>

int main(int argc, char* argv[])
{
    plog::init(plog::debug, "log.txt", 1000000, 5);
    //QCoreApplication a(argc, argv);
    //Server server;
    //server.startServer();
    //return a.exec();

    DBService::DBConnection db("Driver={ODBC Driver 18 for SQL Server};Server=tcp:comp-zionet-server.database.windows.net,1433;Database=CPP_Chat_DB;Uid=Logika4417;Pwd=Fyyf1998;Encrypt=yes;TrustServerCertificate=no;Connection Timeout=30;");
    DBService::DBConnection* dbPTR = &db;
    DBService::RoomRepository userRepo(&db);
    //auto res = userRepo.getAllRooms();
    //QList<DBEntity::DBRoom> roomList = res.result();
    //for (const DBEntity::DBRoom& room : roomList) {
    //    qDebug() << "ID: " << room.getId() << " Name: " << room.getName();
    //}
    //DBEntity::DBRoom room(9, "test", "test", 2, false, "", false);
    //auto res = userRepo.createRoom(room);
    //qDebug() << res.result();
    //QFuture<void> result = db.updateRoom("UPDATE room SET name=:name, description=:description, topic_id=:topic_id, is_private=:is_private, password=:password, is_deleted=:is_deleted WHERE id=:id", room);
    //auto res = userRepo.deleteRoom(7);
    //qDebug() << res.result();

    auto res = userRepo.getAllActiveRooms();
    QList<DBEntity::DBRoom> roomList = res.result();
    for (const DBEntity::DBRoom& room : roomList) {
        qDebug() << "ID: " << room.getId() << " Name: " << room.getName();
    }
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




//Long simple macros

//PLOG_VERBOSE << "verbose";
//PLOG_DEBUG << "debug";
//PLOG_INFO << "info";
//PLOG_WARNING << "warning";
//PLOG_ERROR << "error";
//PLOG_FATAL << "fatal";
//PLOG_NONE << "none";




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
