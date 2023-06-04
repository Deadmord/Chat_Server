#include <QtCore/QCoreApplication>
#include "server.h"
#include "DBRoom.h"
#include "DBService.h"
#include <iostream>
#include "DBService.h"

int main(int argc, char* argv[])
{
    //QCoreApplication a(argc, argv);
    //Server server;
    //server.startServer();
    //return a.exec();
    
    DBService::DBConnection db("Driver={ODBC Driver 18 for SQL Server};Server=tcp:comp-zionet-server.database.windows.net,1433;Database=CPP_Chat_DB;Uid=Logika4417;Pwd=Fyyf1998;Encrypt=yes;TrustServerCertificate=no;Connection Timeout=30;");
    auto res = db.getRooms("SELECT * from room;");
    //DBEntity::DBRoom room(9, "UPDATED NAME", "UPDATED DESCRIPTION", 4, false, "", false);
    //db.addRoom("INSERT INTO room (name, description, topic_id, is_private, password, is_deleted) VALUES (:name, :description, :topic_id, :is_private, :password, :is_deleted)", room);
    //QFuture<void> result = db.updateRoom("UPDATE room SET name=:name, description=:description, topic_id=:topic_id, is_private=:is_private, password=:password, is_deleted=:is_deleted WHERE id=:id", room);
    QList<DBEntity::DBRoom> roomList = res.result();
    for (const DBEntity::DBRoom& room : roomList) {
        qDebug() << "ID: " << room.getId() << " Name: " << room.getName();
    }
}
