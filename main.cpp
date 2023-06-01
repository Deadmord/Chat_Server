#include <QtCore/QCoreApplication>
#include "server.h"
#include "DBRoom.h"
#include "DBService.h"
#include <iostream>
#include "DBService.h"

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    Server server;
    server.startServer();
    return a.exec();
    
}
