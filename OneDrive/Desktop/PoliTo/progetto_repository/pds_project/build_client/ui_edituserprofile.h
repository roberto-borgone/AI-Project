/********************************************************************************
** Form generated from reading UI file 'edituserprofile.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITUSERPROFILE_H
#define UI_EDITUSERPROFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_edituserprofile
{
public:
    QWidget *centralwidget;
    QPushButton *cancel;
    QPushButton *confirm;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *edituserprofile)
    {
        if (edituserprofile->objectName().isEmpty())
            edituserprofile->setObjectName(QString::fromUtf8("edituserprofile"));
        edituserprofile->resize(800, 600);
        centralwidget = new QWidget(edituserprofile);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        cancel = new QPushButton(centralwidget);
        cancel->setObjectName(QString::fromUtf8("cancel"));
        cancel->setGeometry(QRect(650, 480, 93, 28));
        confirm = new QPushButton(centralwidget);
        confirm->setObjectName(QString::fromUtf8("confirm"));
        confirm->setGeometry(QRect(490, 320, 93, 28));
        edituserprofile->setCentralWidget(centralwidget);
        menubar = new QMenuBar(edituserprofile);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 25));
        edituserprofile->setMenuBar(menubar);
        statusbar = new QStatusBar(edituserprofile);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        edituserprofile->setStatusBar(statusbar);

        retranslateUi(edituserprofile);

        QMetaObject::connectSlotsByName(edituserprofile);
    } // setupUi

    void retranslateUi(QMainWindow *edituserprofile)
    {
        edituserprofile->setWindowTitle(QApplication::translate("edituserprofile", "MainWindow", nullptr));
        cancel->setText(QApplication::translate("edituserprofile", "go back", nullptr));
        confirm->setText(QApplication::translate("edituserprofile", "ok", nullptr));
    } // retranslateUi

};

namespace Ui {
    class edituserprofile: public Ui_edituserprofile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITUSERPROFILE_H
