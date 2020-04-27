#include "cn.h"
#include <QDir>
#include <QDebug>
#include <QSqlQuery>
#include <QTest>

QSqlDatabase cn::_db = QSqlDatabase::addDatabase("QMYSQL");

cn *cn::_obj = nullptr;

cn::cn(QObject *parent) : QObject(parent)
{
        int retry=3;
        while(retry){
            _db.setHostName("localhost");
            _db.setUserName("root");
            _db.setPort(3307);
            _db.setDatabaseName("database_pds");

            if(_db.open()) {
                qDebug() << "Connessione stabilita ";
                break;
            }
            else {
                qDebug() << "Errore di connessione al database: 30 secondi al nuovo tentativo... ";
                retry--;
                QTest::qSleep(30000);
            }
        }
        if(retry==0) throw std::invalid_argument("server could not start");


}

cn *cn::obj()
{
    if(!_obj) {
        _obj = new cn();
    }

    return _obj;
}

QSqlDatabase cn::db()
{
    return _db;
}
