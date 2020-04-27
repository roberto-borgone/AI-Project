#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDesktopWidget>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QRegExp>
#include <QDebug>

#include "homepage.h"
#include "mytcpsocket.h"

QTcpSocket* MyTcpSocket::socket;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton_signup->setStyleSheet("QPushButton{border: none;}");
    QPixmap pix(":/login.png");
    QIcon ButtonIcon(pix);
    ui->pushButton_Login->setIcon(ButtonIcon);
    ui->pushButton_Login->setIconSize(QSize(150,150));
    QPixmap pix1(":/signup.png");
    QIcon ButtonIcon1(pix1);
    ui->pushButton_signup->setIcon(ButtonIcon1);
    ui->pushButton_signup->setIconSize(QSize(150,150));

    ui->lineEdit_username->setMaxLength(32);
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_password->setMaxLength(32);
    ui->lineEdit_userNamaSignUp->setMaxLength(32);
    ui->lineEdit_passwordSignUp->setEchoMode(QLineEdit::Password);
    ui->lineEdit_passwordSignUp->setMaxLength(32);
    ui->lineEdit_confPasswordSignUp->setEchoMode(QLineEdit::Password);
    ui->lineEdit_confPasswordSignUp->setMaxLength(32);
    const QRect availableGeometry=QApplication::desktop()->availableGeometry(this);
    this->resize(availableGeometry.width()/3,(availableGeometry.height()*2)/3);
    ui->widget->setFixedSize(availableGeometry.width()/2,availableGeometry.height()/3);
    ui->groupBox->setFixedSize(availableGeometry.width()/6,availableGeometry.height()/4);
    ui->groupBox_2->setFixedSize(availableGeometry.width()/6,availableGeometry.height()/4);
   // w = new Window();
    //w ->show();


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_Login_clicked()
{
    username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    bool error=false;
    QByteArray array;
    QTcpSocket *s = MyTcpSocket::socket;
    QString regSend = "+LOG    ";
    if(s->write(regSend.toUtf8()) != regSend.size()) error = true;
    else{
        array.insert(0,username+" "+password);
        qDebug() <<array;
        qint64 l=s->write(array);
        if(l != array.size()) error = true;
        else{
            ui->lineEdit_username->clear();
            ui->lineEdit_password->clear();
        }
    }
    if(error){
        QMessageBox::warning(this,"Login","Error during login.");
        ui->lineEdit_username->clear();
        ui->lineEdit_password->clear();
    }
}

void MainWindow::on_pushButton_signup_clicked()
{

    QString username = ui->lineEdit_userNamaSignUp->text();
    QString password = ui->lineEdit_passwordSignUp->text();
    QString passwordConfirmation = ui->lineEdit_confPasswordSignUp->text();
    bool error=false;
    QByteArray array;

    QRegExp reUser("((^[a-zA-Z0-9._$@*!-]{5,32}$))"); //Esclude tutti i caratteri e consente solo quelli contenuti
    QRegExp re("((?=.*[0-9])(?=.*[a-zA-Z]).{8,32})");

    if(reUser.exactMatch(username) == false){
        QMessageBox::information(this,"Registration","Username must contain at least 5 characters\nIt allows this special characters: . $ _ @ * ! -\nTry again!");
        ui->lineEdit_userNamaSignUp ->clear();
    }
    else if(re.exactMatch(password) == false){
        QMessageBox::information(this,"Registration","Password must be at least 8 characters with one letter and a number.");
        ui->lineEdit_passwordSignUp->clear();
    }
    else{
         if(password.compare(passwordConfirmation) != 0){
                 QMessageBox::information(this,"Registration","Passwords must be equal.");
                    ui->lineEdit_passwordSignUp->clear();
                    ui->lineEdit_confPasswordSignUp->clear();
                }else{
                    QTcpSocket *s = MyTcpSocket::socket;
                    QString md5pasw =QString(QCryptographicHash::hash((ui->lineEdit_passwordSignUp->text().toUtf8()),QCryptographicHash::Md5).toHex());
                    int len=username.size()+md5pasw.size(); //size ritorna un int su 2 byte
                    QString regSend = "+REG    ";
                    if(s->write(regSend.toUtf8()) != regSend.size())
                        error = true;
                    else{
                        array.setNum(len);
                        qint64 l=s->write(array); //di solito sono 2 byte perchè il numero entra in 1 byte + 1 byte per \0
                        if(l != array.size()) error = true;
                        else{
                            array.clear();
                            array.insert(0,username+" "+md5pasw);
                            qDebug() <<array;
                            l=s->write(array);
                            if(l != array.size()) error = true;
                            else{
                                ui->lineEdit_userNamaSignUp->clear();
                                ui->lineEdit_passwordSignUp->clear();
                                ui->lineEdit_confPasswordSignUp->clear();
                            }
                        }
                    }
                    if(error){
                        QMessageBox::information(this,"Registration","Error during registration");
                        ui->lineEdit_userNamaSignUp->clear();
                        ui->lineEdit_passwordSignUp->clear();
                        ui->lineEdit_confPasswordSignUp->clear();
                    }
         }
    }
}

void MainWindow::on_lineEdit_password_returnPressed()
{
    ui->pushButton_Login->click();
}

void MainWindow::on_lineEdit_username_returnPressed()
{
    ui->pushButton_Login->click();
}

void MainWindow::on_lineEdit_confPasswordSignUp_returnPressed()
{
    ui->pushButton_signup->click();
}

void MainWindow::on_lineEdit_userNamaSignUp_returnPressed()
{
    ui->pushButton_signup->click();
}

void MainWindow::on_lineEdit_passwordSignUp_returnPressed()
{
    ui->pushButton_signup->click();
}
