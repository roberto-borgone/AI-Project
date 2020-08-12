#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "textedit.h"
#include "homepage.h"
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static QString username;
    QMessageBox msgBox;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Login_clicked();

    void on_pushButton_signup_clicked();

    void on_lineEdit_password_returnPressed();

    void on_lineEdit_confPasswordSignUp_returnPressed();

    void on_lineEdit_username_returnPressed();

    void on_lineEdit_userNamaSignUp_returnPressed();

    void on_lineEdit_passwordSignUp_returnPressed();

private:
    Ui::MainWindow *ui;
    //TextEdit *textedit;
    Window *w;
};
#endif // MAINWINDOW_H
