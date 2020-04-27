/********************************************************************************
** Form generated from reading UI file 'homepage.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOMEPAGE_H
#define UI_HOMEPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_homepage
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *homepage)
    {
        if (homepage->objectName().isEmpty())
            homepage->setObjectName(QString::fromUtf8("homepage"));
        homepage->resize(800, 600);
        centralwidget = new QWidget(homepage);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(0, 520, 93, 28));
        homepage->setCentralWidget(centralwidget);
        menubar = new QMenuBar(homepage);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        homepage->setMenuBar(menubar);
        statusbar = new QStatusBar(homepage);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        homepage->setStatusBar(statusbar);

        retranslateUi(homepage);

        QMetaObject::connectSlotsByName(homepage);
    } // setupUi

    void retranslateUi(QMainWindow *homepage)
    {
        homepage->setWindowTitle(QApplication::translate("homepage", "MainWindow", nullptr));
        pushButton->setText(QApplication::translate("homepage", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class homepage: public Ui_homepage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOMEPAGE_H
