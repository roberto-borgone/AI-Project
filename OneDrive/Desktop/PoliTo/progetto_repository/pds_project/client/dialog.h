#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QSqlQuery>
#include "textedit.h"

class Window;

namespace Ui {
class Dialog;
}
//class Dialog : public QDialog
class Dialog : public QWidget
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent, Window *home, QString myName);
    ~Dialog();

private slots:
    void on_confirm_clicked();
    //void URIGenerator();
    //void showEvent( QShowEvent* aShowEvent );


private:
    Window *home;
    Ui::Dialog *ui;
    QLabel *name;
    QLineEdit *filename;
    QString myName;

    TextEdit *t;

};

#endif // DIALOG_H
