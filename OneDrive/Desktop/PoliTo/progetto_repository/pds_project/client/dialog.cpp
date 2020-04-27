#include <QtWidgets>
#include "dialog.h"
#include "ui_dialog.h"

#include "mainwindow.h"
#include "mytcpsocket.h"

QString MainWindow::username;

Dialog::Dialog(QWidget *parent, Window *home, QString myName) :
    //QDialog(parent),home(home),
    QWidget(parent), home(home), myName(myName),
    ui(new Ui::Dialog)

{
    //cambiare ui_dialog.h
    ui->setupUi(this);
    name = new QLabel(myName, this);
    filename = new QLineEdit(this);
    filename->setFocus();
    //this->setModal(false);

    //name->setFocusPolicy(Qt::NoFocus);
    //ui->confirm->setFocusPolicy(Qt::NoFocus);
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(name, 0, 0);
    //mainLayout->addWidget(filename, 0, 1);
    mainLayout->addWidget(filename, 1, 0);
    //mainLayout->addWidget(ui->confirm, 1, 0);
    mainLayout->addWidget(ui->confirm, 2, 0);

    //set fixed lenght
    /*
    QSize size = home->getButtonSize();
    int h = size.height();
    int w = size.width();
    name->setFixedSize(w,h);
    filename->setFixedSize(w,h);
    ui->confirm->setFixedSize(w,h);
    */
    this->setAttribute(Qt::WA_DeleteOnClose);
    connect(filename,&QLineEdit::returnPressed,this,&Dialog::on_confirm_clicked);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_confirm_clicked()
{
    QString username = MainWindow::username;
    QTcpSocket *s = MyTcpSocket::socket;
    QString msg;
    if(myName == "Save file on server as: "){
        home->setFilesTableNull();
        msg = "NEWFILE "+username+";"+filename->text();
    }
    else msg = "DELETEFI"+username+";"+filename->text();
    qDebug()<<msg;

    if(s->write(msg.toUtf8()) != msg.size())
       //aggiungere QMEssageBox per DELETE FILE
       QMessageBox::warning(this,"New file","Impossible to communicate with server");
}

/*
void Dialog::showEvent( QShowEvent* aShowEvent )
{
    //QDialog::showEvent( aShowEvent );
    QWidget::showEvent( aShowEvent );
    activateWindow();
}
*/

