#include <QtCore/QCoreApplication>
#include "server.h"
#include "DBRoom.h"
#include <iostream>

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    Server server;
    server.startServer();
    return a.exec();
}
