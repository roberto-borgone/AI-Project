#include <QtWidgets>

#include "edituserprofile.h"
#include "ui_edituserprofile.h"
#include <QSqlQuery>
#include "mainwindow.h"
#include "mytcpsocket.h"

edituserprofile::edituserprofile(QWidget *parent, Window *h) :
    QMainWindow(),
    ui(new Ui::edituserprofile),
    h(h)
{
    ui->setupUi(this);

    setWindowTitle(tr("Edit Profile"));

    const QRect availableGeometry=QApplication::desktop()->availableGeometry(this);
    this->resize((availableGeometry.width()*2)/3,(availableGeometry.height()*2)/3);

    mainLayout = new QGridLayout(this);
    mainLayout2 = new QGridLayout(this);
    mainLayout3 = new QGridLayout(this);
    mainLayout4 = new QGridLayout(this);

    QPixmap pix4(":/default_profile.png");
    profile=new QLabel(this);
    profile->setPixmap(pix4);
    profile->setScaledContents(true);
    profile->setFixedSize(ui->confirm->width()*3,ui->confirm->height()*7);

    change = new QLabel("Do you want to change your data?",this);
    newFirstNameLabel = new QLabel("First name: ",this);
    newFirstName = new QLineEdit(this);
    newFirstName->setMaxLength(32);
    newLastNameLabel = new QLabel("Last name: ",this);
    newLastName = new QLineEdit(this);
    newLastName->setMaxLength(32);
    oldPasswordLabel = new QLabel("Old password: ",this);
    oldPassword = new QLineEdit(this);
    oldPassword->setMaxLength(32);
    oldPassword->setEchoMode(QLineEdit::Password);
    newPasswordLabel = new QLabel("New password: ",this);
    newPassword = new QLineEdit(this);
    newPassword->setMaxLength(32);
    newPassword->setEchoMode(QLineEdit::Password);
    confirmNewPasswordLabel = new QLabel("Confirm password: ",this);
    confirmNewPassword = new QLineEdit(this);
    confirmNewPassword->setMaxLength(32);
    confirmNewPassword->setEchoMode(QLineEdit::Password);
    newNicknameLabel = new QLabel("Nickname: ",this);
    newNickname = new QLineEdit(this);
    newNickname->setMaxLength(32);
    newGenderLabel = new QLabel("Gender: ",this);
    newGender = new QComboBox(this);
    newGender->addItem("");
    newGender->addItem("M");
    newGender->addItem("F");
    newDateOfBirthLabel = new QLabel("Date of birth: ", this);
    newDateOfBirth = new QCalendarWidget(this);
    newFirstName->setFixedSize(profile->width(),newFirstName->height());
    newLastName->setFixedSize(profile->width(),newLastName->height());
    oldPassword->setFixedSize(profile->width(),oldPassword->height());
    newPassword->setFixedSize(profile->width(),newPassword->height());
    confirmNewPassword->setFixedSize(profile->width(),confirmNewPassword->height());
    newNickname->setFixedSize(profile->width(),newNickname->height());
    newGender->setFixedSize(profile->width(),newGender->height());

    QLabel *spazio=new QLabel(this);
    QLabel *spazio1=new QLabel(this);

    first_name = new QLabel(this);
    last_name = new QLabel(this);
    nickname = new QLabel(this);
    gender = new QLabel(this);
    date_of_birth = new QLabel(this);

    ui->centralwidget->setLayout(mainLayout);

    mainLayout->addLayout(mainLayout2,0,0);
    mainLayout2->addWidget(profile,0,0,1,3);

    mainLayout->addLayout(mainLayout3,0,1);
    mainLayout3->addWidget(first_name,1,0);
    mainLayout3->addWidget(last_name,2,0);
    mainLayout3->addWidget(nickname,3,0);
    mainLayout3->addWidget(gender,4,0);
    mainLayout3->addWidget(date_of_birth,5,0);
    mainLayout3->addWidget(spazio1,6,0);
    //mainLayout3->addWidget(spazio2,7,0);

    mainLayout->addWidget(spazio,1,0);

    mainLayout->addWidget(change,2,0);
    mainLayout->addWidget(newFirstNameLabel,3,0);
    mainLayout->addWidget(newFirstName,3,1);
    mainLayout->addWidget(newLastNameLabel,4,0);
    mainLayout->addWidget(newLastName,4,1);
    mainLayout->addWidget(oldPasswordLabel,5,0);
    mainLayout->addWidget(oldPassword,5,1);
    mainLayout->addWidget(newPasswordLabel,5,2);
    mainLayout->addWidget(newPassword,5,3);
    mainLayout->addWidget(confirmNewPasswordLabel,5,4);
    mainLayout->addWidget(confirmNewPassword,5,5);
    mainLayout->addWidget(newNicknameLabel,6,0);
    mainLayout->addWidget(newNickname,6,1);
    mainLayout->addWidget(newGenderLabel,7,0);
    mainLayout->addWidget(newGender,7,1);
    mainLayout->addWidget(newDateOfBirthLabel,8,0);
    mainLayout->addWidget(newDateOfBirth,8,1);
    mainLayout->addLayout(mainLayout4,9,5);
    mainLayout4->addWidget(ui->confirm,0,0);
    mainLayout4->addWidget(ui->cancel,0,1);
    imageObject = nullptr;

    this->setWindowTitle("Poshed");
    this->setWindowIcon(QIcon("Graphics/LogoTemp/profile.png"));
}

edituserprofile::~edituserprofile()
{
    delete ui;
}



void edituserprofile::on_cancel_clicked()
{
    newFirstName->clear();
    newLastName->clear();
    oldPassword->clear();
    newPassword->clear();
    confirmNewPassword->clear();
    newNickname->clear();
    newGender->clear();
    newGender->addItem("");
    newGender->addItem("M");
    newGender->addItem("F");
    newDateOfBirth->showToday();
    h->show();
    this->close();
}

void edituserprofile::on_confirm_clicked()
{
        QByteArray array;
        QString username = MainWindow::username;
        QTcpSocket *s = MyTcpSocket::socket;
        QString regSend = "EDIT    ";
        bool error=false;

        QString editSend = username + ";";

        if(s->write(regSend.toUtf8()) != regSend.toUtf8().size()) error = true;
        else{
            if(!newFirstName->text().isEmpty())
                editSend += newFirstName->text();
            else
                editSend += "null";
            if(!newLastName->text().isEmpty())
                editSend += + ";"+newLastName->text();
            else editSend += ";null";
            if(!newNickname->text().isEmpty())
                editSend += ";"+newNickname->text();
            else editSend += ";null";

            if(!oldPassword->text().isEmpty() && !newPassword->text().isEmpty() && !confirmNewPassword->text().isEmpty()) {
                QRegExp re("((?=.*[0-9])(?=.*[a-zA-Z]).{8,32})");
                if(re.exactMatch(oldPassword->text()) == false || re.exactMatch(newPassword->text()) == false){
                                QMessageBox::information(this,"Change password","Password must contain one letter and a number.");
                                oldPassword->clear();
                                newPassword->clear();
                                confirmNewPassword->clear();
                                editSend += ";null;null;null";
                 }
                 else{
                    if(newPassword->text().compare(confirmNewPassword->text()) != 0){
                        QMessageBox::information(this,"Change password","Passwords must be equal.");
                        oldPassword->clear();
                        newPassword->clear();
                        confirmNewPassword->clear();
                        editSend += ";null;null;null";
                    }
                    else {

                        QString oldPsw = QString(QCryptographicHash::hash((oldPassword->text().toUtf8()),QCryptographicHash::Sha256).toHex());
                        QString newPsw = QString(QCryptographicHash::hash((newPassword->text().toUtf8()),QCryptographicHash::Sha256).toHex());
                        QString confirmNewPsw = QString(QCryptographicHash::hash((confirmNewPassword->text().toUtf8()),QCryptographicHash::Sha256).toHex());
                        editSend += ";"+oldPsw+";"+newPsw+";"+confirmNewPsw;
                    }
                  }
              }
            else if(oldPassword->text().isEmpty() && newPassword->text().isEmpty() && confirmNewPassword->text().isEmpty())
                editSend += ";null;null;null";
            else {
                editSend += ";error;error;error";
            }


              if(newGender->currentText()!=""){
                 editSend += ";"+QString::number(newGender->currentIndex()-1);
              }
              else editSend += ";null";

              if(newDateOfBirth->selectedDate()!=QDate::currentDate()) {
                  editSend += ";"+QString::number(newDateOfBirth->selectedDate().year())+"/"
                          +QString::number(newDateOfBirth->selectedDate().month())+"/"
                          +QString::number(newDateOfBirth->selectedDate().day());
               }
               else {
                  editSend += ";null";
              }

              if(s->write(editSend.toUtf8()) != editSend.toUtf8().size()) error = true;
              if(error) QMessageBox::warning(this,"Server Problem","Error during contacting the server");
              newFirstName->clear();
              newLastName->clear();
              newNickname->clear();
              oldPassword->clear();
              newPassword->clear();
              confirmNewPassword->clear();

        }
}

void edituserprofile::showImage(QByteArray mByteArray){
    QPixmap mPixmap;
    mPixmap.loadFromData(mByteArray,"JPEG(*.jpg *.jpeg);;PNG(*.png)");
    profile->setPixmap(mPixmap);
    profile->setScaledContents(true);
}

void edituserprofile::showInfo(QString info)
{
    QStringList info_ = info.split(";");

    if(info_[1] != "null")
        first_name->setText("First name: "+info_[1]);
    else
        first_name->setText("First name: ");

    if(info_[2] != "null")
        last_name->setText("Last name: "+info_[2]);
    else
        last_name->setText("Last name: ");

    if(info_[3] != "null")
        nickname->setText("Nickname: "+info_[3]);
    else
        nickname->setText("Nickname: ");

    if(info_[4] != "null") {
        if(info_[4] == '0')
            gender->setText("Gender: Male");
        else
            gender->setText("Gender: Female");
    }
    else
       gender->setText("Gender: ");

    if(info_[5] != "null")
        date_of_birth->setText("Date of birth: "+info_[5]);
    else
        date_of_birth->setText("Date of birth: ");
}
