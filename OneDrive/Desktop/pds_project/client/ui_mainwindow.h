/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_6;
    QWidget *widget;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_6;
    QLabel *label;
    QLineEdit *lineEdit_username;
    QLabel *label_2;
    QLineEdit *lineEdit_password;
    QPushButton *pushButton_Login;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_3;
    QLineEdit *lineEdit_userNamaSignUp;
    QLabel *label_4;
    QLineEdit *lineEdit_passwordSignUp;
    QLabel *label_5;
    QLineEdit *lineEdit_confPasswordSignUp;
    QPushButton *pushButton_signup;
    QStatusBar *statusbar;
    QMenuBar *menubar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1368, 1678);
        MainWindow->setStyleSheet(QString::fromUtf8("background-image: url(:/Graphics/LoginBg/17580.jpg);\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        centralwidget->setStyleSheet(QString::fromUtf8("background-color:rgb(214, 214, 214); "));
        verticalLayout_5 = new QVBoxLayout(centralwidget);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy1);
        groupBox_3->setLayoutDirection(Qt::LeftToRight);
        groupBox_3->setStyleSheet(QString::fromUtf8("border: none;\n"
"background: transparent;\n"
"\n"
""));
        groupBox_3->setAlignment(Qt::AlignCenter);
        groupBox_3->setFlat(false);
        groupBox_3->setCheckable(false);
        verticalLayout_6 = new QVBoxLayout(groupBox_3);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        widget = new QWidget(groupBox_3);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setEnabled(true);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(0, 0));
        widget->setMaximumSize(QSize(16777215, 16777215));
        widget->setBaseSize(QSize(0, 0));
        widget->setLayoutDirection(Qt::LeftToRight);
        widget->setStyleSheet(QString::fromUtf8(" background: transparent;"));
        verticalLayout_4 = new QVBoxLayout(widget);
        verticalLayout_4->setSpacing(50);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout_4->setContentsMargins(-1, 9, -1, -1);
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Ignored, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy2);
        groupBox->setMinimumSize(QSize(400, 0));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 0));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        groupBox->setPalette(palette);
        groupBox->setLayoutDirection(Qt::LeftToRight);
        groupBox->setStyleSheet(QString::fromUtf8(""));
        groupBox->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);
        groupBox->setFlat(false);
        groupBox->setCheckable(false);
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, -1, -1, 0);
        groupBox_4 = new QGroupBox(groupBox);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setLayoutDirection(Qt::LeftToRight);
        groupBox_4->setStyleSheet(QString::fromUtf8("border-radius: 25px; border-color: #a58ca5"));
        groupBox_4->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);
        verticalLayout_3 = new QVBoxLayout(groupBox_4);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_6 = new QLabel(groupBox_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy3);
        label_6->setMinimumSize(QSize(200, 100));
        label_6->setPixmap(QPixmap(QString::fromUtf8(":/Graphics/LogoTemp/Poshed.png")));
        label_6->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_6);

        label = new QLabel(groupBox_4);
        label->setObjectName(QString::fromUtf8("label"));
        QPalette palette1;
        QBrush brush1(QColor(75, 75, 75, 200));
        brush1.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush);
        QBrush brush2(QColor(120, 120, 120, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush);
        label->setPalette(palette1);
        QFont font;
        font.setFamily(QString::fromUtf8("Lucida Console"));
        font.setPointSize(12);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("margin: 5px;"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label);

        lineEdit_username = new QLineEdit(groupBox_4);
        lineEdit_username->setObjectName(QString::fromUtf8("lineEdit_username"));
        lineEdit_username->setEnabled(true);
        QSizePolicy sizePolicy4(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(lineEdit_username->sizePolicy().hasHeightForWidth());
        lineEdit_username->setSizePolicy(sizePolicy4);
        lineEdit_username->setMinimumSize(QSize(250, 50));
        lineEdit_username->setMaximumSize(QSize(16777215, 50));
        QPalette palette2;
        QBrush brush3(QColor(120, 120, 200, 25));
        brush3.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush3);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush3);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush3);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush3);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush3);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        lineEdit_username->setPalette(palette2);
        lineEdit_username->setLayoutDirection(Qt::LeftToRight);
        lineEdit_username->setAutoFillBackground(false);
        lineEdit_username->setStyleSheet(QString::fromUtf8("background-color: rgba(120,120,200,0.1); border: none;"));
        lineEdit_username->setCursorPosition(0);
        lineEdit_username->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(lineEdit_username);

        label_2 = new QLabel(groupBox_4);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush);
        label_2->setPalette(palette3);
        label_2->setFont(font);
        label_2->setStyleSheet(QString::fromUtf8("margin: 5px;"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_2);

        lineEdit_password = new QLineEdit(groupBox_4);
        lineEdit_password->setObjectName(QString::fromUtf8("lineEdit_password"));
        sizePolicy4.setHeightForWidth(lineEdit_password->sizePolicy().hasHeightForWidth());
        lineEdit_password->setSizePolicy(sizePolicy4);
        lineEdit_password->setMinimumSize(QSize(370, 50));
        lineEdit_password->setMaximumSize(QSize(16777215, 16777215));
        lineEdit_password->setStyleSheet(QString::fromUtf8("background-color: rgba(120,120,200,0.1); border: none;"));
        lineEdit_password->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(lineEdit_password);

        pushButton_Login = new QPushButton(groupBox_4);
        pushButton_Login->setObjectName(QString::fromUtf8("pushButton_Login"));
        sizePolicy3.setHeightForWidth(pushButton_Login->sizePolicy().hasHeightForWidth());
        pushButton_Login->setSizePolicy(sizePolicy3);
        pushButton_Login->setMinimumSize(QSize(200, 50));
        pushButton_Login->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_Login->setStyleSheet(QString::fromUtf8(""));
        pushButton_Login->setIconSize(QSize(10, 10));
        pushButton_Login->setAutoDefault(false);
        pushButton_Login->setFlat(false);

        verticalLayout_3->addWidget(pushButton_Login);


        verticalLayout->addWidget(groupBox_4);


        verticalLayout_4->addWidget(groupBox);


        verticalLayout_6->addWidget(widget);

        groupBox_2 = new QGroupBox(groupBox_3);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setEnabled(true);
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        groupBox_2->setMinimumSize(QSize(100, 0));
        groupBox_2->setMaximumSize(QSize(16777215, 16777215));
        groupBox_2->setStyleSheet(QString::fromUtf8("border-radius: 25px; border-color: #a58ca5"));
        groupBox_2->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setSpacing(5);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(50, 20, 50, 20);
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::Button, brush);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush);
        label_3->setPalette(palette4);
        QFont font1;
        font1.setFamily(QString::fromUtf8("Lucida Console"));
        font1.setPointSize(9);
        label_3->setFont(font1);
        label_3->setStyleSheet(QString::fromUtf8("margin: 5px"));
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_3);

        lineEdit_userNamaSignUp = new QLineEdit(groupBox_2);
        lineEdit_userNamaSignUp->setObjectName(QString::fromUtf8("lineEdit_userNamaSignUp"));
        sizePolicy4.setHeightForWidth(lineEdit_userNamaSignUp->sizePolicy().hasHeightForWidth());
        lineEdit_userNamaSignUp->setSizePolicy(sizePolicy4);
        lineEdit_userNamaSignUp->setMinimumSize(QSize(200, 50));
        lineEdit_userNamaSignUp->setMaximumSize(QSize(16777215, 50));
        lineEdit_userNamaSignUp->setAutoFillBackground(false);
        lineEdit_userNamaSignUp->setStyleSheet(QString::fromUtf8("background-color: rgba(120,120,200,0.1); border: none;"));
        lineEdit_userNamaSignUp->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(lineEdit_userNamaSignUp);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::Button, brush);
        palette5.setBrush(QPalette::Active, QPalette::Base, brush);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush);
        label_4->setPalette(palette5);
        label_4->setFont(font1);
        label_4->setStyleSheet(QString::fromUtf8("margin: 5px"));
        label_4->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_4);

        lineEdit_passwordSignUp = new QLineEdit(groupBox_2);
        lineEdit_passwordSignUp->setObjectName(QString::fromUtf8("lineEdit_passwordSignUp"));
        sizePolicy4.setHeightForWidth(lineEdit_passwordSignUp->sizePolicy().hasHeightForWidth());
        lineEdit_passwordSignUp->setSizePolicy(sizePolicy4);
        lineEdit_passwordSignUp->setMinimumSize(QSize(200, 50));
        lineEdit_passwordSignUp->setMaximumSize(QSize(16777215, 50));
        lineEdit_passwordSignUp->setStyleSheet(QString::fromUtf8("background-color: rgba(120,120,200,0.1); border: none;"));
        lineEdit_passwordSignUp->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(lineEdit_passwordSignUp);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::Button, brush);
        palette6.setBrush(QPalette::Active, QPalette::Base, brush);
        palette6.setBrush(QPalette::Active, QPalette::Window, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::Window, brush);
        label_5->setPalette(palette6);
        label_5->setFont(font1);
        label_5->setStyleSheet(QString::fromUtf8("margin: 5px;"));
        label_5->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_5);

        lineEdit_confPasswordSignUp = new QLineEdit(groupBox_2);
        lineEdit_confPasswordSignUp->setObjectName(QString::fromUtf8("lineEdit_confPasswordSignUp"));
        QSizePolicy sizePolicy5(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(lineEdit_confPasswordSignUp->sizePolicy().hasHeightForWidth());
        lineEdit_confPasswordSignUp->setSizePolicy(sizePolicy5);
        lineEdit_confPasswordSignUp->setMinimumSize(QSize(200, 50));
        lineEdit_confPasswordSignUp->setStyleSheet(QString::fromUtf8("background-color: rgba(120,120,200,0.1); border: none;"));
        lineEdit_confPasswordSignUp->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(lineEdit_confPasswordSignUp);

        pushButton_signup = new QPushButton(groupBox_2);
        pushButton_signup->setObjectName(QString::fromUtf8("pushButton_signup"));
        sizePolicy3.setHeightForWidth(pushButton_signup->sizePolicy().hasHeightForWidth());
        pushButton_signup->setSizePolicy(sizePolicy3);
        pushButton_signup->setMinimumSize(QSize(200, 50));
        pushButton_signup->setMaximumSize(QSize(16777215, 16777215));
        pushButton_signup->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_signup->setStyleSheet(QString::fromUtf8(""));
        pushButton_signup->setIconSize(QSize(10, 10));

        verticalLayout_2->addWidget(pushButton_signup);


        verticalLayout_6->addWidget(groupBox_2);


        verticalLayout_5->addWidget(groupBox_3);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1368, 21));
        MainWindow->setMenuBar(menubar);

        retranslateUi(MainWindow);

        pushButton_Login->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox_3->setTitle(QString());
        groupBox->setTitle(QString());
        groupBox_4->setTitle(QString());
        label_6->setText(QString());
        label->setText(QApplication::translate("MainWindow", "Username", nullptr));
        label_2->setText(QApplication::translate("MainWindow", " Password ", nullptr));
        pushButton_Login->setText(QString());
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Not registered? Create an account ", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "Username", nullptr));
        lineEdit_userNamaSignUp->setInputMask(QString());
        label_4->setText(QApplication::translate("MainWindow", "Password", nullptr));
        lineEdit_passwordSignUp->setPlaceholderText(QApplication::translate("MainWindow", "(8-32 alphanumeric values)", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "Confirm Password", nullptr));
        lineEdit_confPasswordSignUp->setPlaceholderText(QString());
        pushButton_signup->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
