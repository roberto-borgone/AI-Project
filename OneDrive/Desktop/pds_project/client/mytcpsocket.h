#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include "mainwindow.h"
#include "homepage.h"
#include "textedit.h"
#include <QMessageBox>

class MyTcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = 0);
    ~MyTcpSocket();

    //QTcpSocket* doConnect();
    //QTcpSocket* getSocket();
    static QTcpSocket* socket;
private:
    MainWindow *m;
    Window *w;
    //TextEdit *t;
    QMessageBox msgBox;
    QByteArray buffer_caratteri;
    QByteArray buffer_font;
    int counter;
    int counter2;


signals:

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

};

#endif // MYTCPSOCKET_H

