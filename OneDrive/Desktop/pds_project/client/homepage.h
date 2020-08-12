#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QDir>
#include <QGraphicsScene>
#include <QGridLayout>

//#include "textedit.h"
#include "edituserprofile.h"
#include "dialog.h"

QT_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class QPushButton;
class QTableWidget;
class QTableWidgetItem;
QT_END_NAMESPACE

//! [0]

class MainWindow;

class Window : public QWidget
{
    Q_OBJECT

public:

    explicit Window(QWidget *parent, MainWindow *mw);
    ~Window();

    edituserprofile *ed;
    TextEdit *textedit;
    //std::unique_ptr<TextEdit> textedit;
    Dialog *d;
    Dialog *d2;
    bool deletef = false;

    void openFile(QString filename, QString owner, int siteid);
    void showFiles(const QStringList &paths,const QStringList &status,const QStringList &sizes);
    void setFilesTableNull();
    void setFilesFoundLabel(QString test);
    void showImage(QByteArray mByteArray);
    QSize getButtonSize();

protected:
    void closeEvent(QCloseEvent *e) override;

private slots:
    /*void browse();*/
    void find();
    void on_modifyButton_clicked();
    void on_removeButton_clicked();
    void on_downloadButton_clicked();
    void animateFindClick();
    void openFileOfItem(int row, int column);
    void contextMenu(const QPoint &pos);
    void on_createButton_clicked();
    void on_editProfile_clicked();
    void on_okButton_clicked();
    void on_deleteButton_clicked();
    void logout();

private:
    //QStringList findFiles(const QStringList &files, const QString &text);
    QComboBox *createComboBox(const QString &text = QString());
    void createFilesTable();

    QComboBox *fileComboBox;
    //QComboBox *textComboBox;
    QComboBox *directoryComboBox;
    QLabel *filesFoundLabel;
    QPushButton *findButton;
    QPushButton *downloadButton;
    QPushButton *removeButton;
    QPushButton *modifyButton;
    QPushButton *createButton;
    QPushButton *editProfile;
    QPushButton *okButton;
    QPushButton *deleteFile;
    QLineEdit *URI;
    QTableWidget *filesTable;
    QLabel *profileLabel;
    QLabel *profileLabel2;
    QGridLayout *mainLayout;
    QGridLayout *mainLayout2;
    QGraphicsView *rep;

    QImage *imageObject;
    QGraphicsScene *scene;
    QPixmap image;

    QDir currentDir;

    QPushButton *logoutButton;
    MainWindow *mw;

    QLabel *user;
};
//! [0]

#endif
