#ifndef EDITUSERPROFILE_H
#define EDITUSERPROFILE_H

#include <QMainWindow>

#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QGraphicsScene>
#include <QGridLayout>
#include <QCalendarWidget>
#include <QComboBox>

class Window;

namespace Ui {
class edituserprofile;
}

class edituserprofile : public QMainWindow
{
    Q_OBJECT

public:
    explicit edituserprofile(QWidget *parent, Window *h);
    ~edituserprofile();
    void showImage(QByteArray mByteArray);
    void showInfo(QString info);

private slots:
    void on_cancel_clicked();
    void on_confirm_clicked();

private:
    Ui::edituserprofile *ui;

private:

    Window *h;

    QLabel *profile;

    QLabel *first_name;
    QLabel *last_name;
    QLabel *nickname;
    QLabel *gender;
    QLabel *date_of_birth;

    QLabel *change;

    QLineEdit *newFirstName;
    QLineEdit *newLastName;
    QLineEdit *newNickname;
    QLineEdit *oldPassword;
    QLineEdit *newPassword;
    QLineEdit *confirmNewPassword;
    QComboBox *newGender;
    QCalendarWidget *newDateOfBirth;

    QLabel *newFirstNameLabel;
    QLabel *newLastNameLabel;
    QLabel *newNicknameLabel;
    QLabel *oldPasswordLabel;
    QLabel *newPasswordLabel;
    QLabel *confirmNewPasswordLabel;
    QLabel *newGenderLabel;
    QLabel *newDateOfBirthLabel;


    QImage *imageObject;
    QGraphicsScene *scene;
    QPixmap image;

    QGridLayout *mainLayout;
    QGridLayout *mainLayout2;
    QGridLayout *mainLayout3;
    QGridLayout *mainLayout4;

};

#endif // EDITUSERPROFILE_H
