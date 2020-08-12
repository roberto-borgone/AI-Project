#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "edituserprofile.h"
#include "mytcpsocket.h"
#include <QTest>
#include <QStyleFactory>

int main(int argc, char *argv[]){
    //cn::obj(); //da non mettere globale ma locale a ogni funzione
    QApplication a(argc, argv);
   /* QMessageBox msg;
    msg.setStandardButtons(0);
    msg.setWindowModality(Qt::NonModal); */
    //MainWindow w;
    //edituserprofile g;
    //TextEdit t;
    //t.show();
    /*int retry=3;
    while(retry>0){*/

    /*
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette; darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    qApp->setPalette(darkPalette);
    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");

    */

    try {
        MyTcpSocket socket;
        return a.exec();
        //break;
    } catch (...) {
        qDebug()<<"eccezione catturata";
        exit(-1);
    }
    //}
    //if(retry==0) exit(-1);*/
   // w.show();
    //g.show();
}
