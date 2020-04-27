#include <QCoreApplication>
#include "server.h"
#include <exception>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // create MyTcpServer
    // MyTcpServer constructor will create QTcpServer
   try {
        Server server;
        return a.exec();
    } catch (...) {
        qDebug()<<"Server could not start";
    }

}
