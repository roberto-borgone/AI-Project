#include <QtWidgets>
#include <QSqlQuery>
#include <QSqlRecord>

#include "homepage.h"
#include "mainwindow.h"
#include "mytcpsocket.h"

//QString MainWindow::username;

//! [17]
enum { absoluteFileNameRole = Qt::UserRole + 1 };
//! [17]

//! [18]
static inline QString fileNameOfItem(const QTableWidgetItem *item)
{
    return item->data(absoluteFileNameRole).toString();
}
//! [18]

//! [14]
static inline void openFile(const QString &fileName)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));

}
//! [14]

//! [0]
Window::Window(QWidget *parent, MainWindow *mw)
    : QWidget(parent), mw(mw)
{
    setWindowTitle(tr("Home Page"));

    this->setAttribute(Qt::WA_DeleteOnClose);

    logoutButton = new QPushButton(tr("&Logout"), this);
    connect(logoutButton, &QAbstractButton::clicked, this, &Window::logout);

    const QRect availableGeometry=QApplication::desktop()->availableGeometry(this);
    this->resize((availableGeometry.width()*2)/3,(availableGeometry.height()*2)/3);
    findButton = new QPushButton(tr("&Find"), this);
    connect(findButton, &QAbstractButton::clicked, this, &Window::find);

    fileComboBox = createComboBox(tr("*"));
    connect(fileComboBox->lineEdit(), &QLineEdit::returnPressed,
            this, &Window::animateFindClick);
    //directoryComboBox = createComboBox(QDir::toNativeSeparators("C:/Users/Riccardo/Documents/Server/Documenti"));
    //connect(directoryComboBox->lineEdit(), &QLineEdit::returnPressed,
            //this, &Window::animateFindClick);

    filesFoundLabel = new QLabel(this);
    user = new QLabel(this);
    user->setText("<font color=\"green\">"+MainWindow::username+"</font>");


    createFilesTable();


    createButton = new QPushButton(tr("&New file"),this);
    connect(createButton, &QAbstractButton::clicked,this, &Window::on_createButton_clicked);

    //QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout = new QGridLayout(this);
    mainLayout2 = new QGridLayout(this);
    profileLabel = new QLabel(tr("&Insert here your image profile"),nullptr);
    downloadButton = new QPushButton(tr("&Download"), this);
    connect(downloadButton, &QAbstractButton::clicked, this, &Window::on_downloadButton_clicked);
    removeButton = new QPushButton(tr("&Remove"),this);
    connect(removeButton, &QAbstractButton::clicked, this, &Window::on_removeButton_clicked);
    modifyButton = new QPushButton(tr("&Change"),this);
    connect(modifyButton, &QAbstractButton::clicked, this, &Window::on_modifyButton_clicked);
    editProfile = new QPushButton(tr("&Edit Profile"),this);
    connect(editProfile, &QAbstractButton::clicked, this, &Window::on_editProfile_clicked);
    okButton = new QPushButton(tr("&Ok"), this);
    connect(okButton, &QAbstractButton::clicked, this, &Window::on_okButton_clicked);
    deleteFile = new QPushButton(tr("&Delete File"),this);
    connect(deleteFile, &QAbstractButton::clicked, this, &Window::on_deleteButton_clicked);

    URI = new QLineEdit(this);
    URI->setMaxLength(32);
    URI->setPlaceholderText(tr("Insert here the URI"));
    fileComboBox->setFixedHeight(fileComboBox->height());
    fileComboBox->lineEdit()->setPlaceholderText("⚲ Search files in your directory...");
    mainLayout->addWidget(user, 0, 0);
    mainLayout->addWidget(fileComboBox, 0, 1);
    mainLayout->addWidget(URI, 0, 2);
    mainLayout->addWidget(okButton, 0, 3);
    mainLayout->addWidget(filesTable, 1, 1, 1, 3);

    // DIALOG A SINISTRA
    QGridLayout *mainLayout3 = new QGridLayout(this);
    mainLayout->addLayout(mainLayout3,2,2);
    mainLayout->addWidget(filesFoundLabel, 2,1);
    mainLayout3->addWidget(createButton,0, 0,Qt::AlignRight);
    mainLayout3->addWidget(deleteFile, 0, 1);
    mainLayout->addWidget(findButton, 2, 3);

    findButton->setFixedSize(removeButton->size());
    createButton->setFixedSize(removeButton->size());
    deleteFile->setFixedSize(removeButton->size());

    mainLayout->addLayout(mainLayout2,1,0,Qt::AlignTop);
    mainLayout2->addWidget(modifyButton,1,0);
    mainLayout2->addWidget(downloadButton,2,0);
    mainLayout2->addWidget(removeButton,3,0);
    mainLayout2->addWidget(editProfile,4,0);
    mainLayout2->addWidget(logoutButton,5,0);

    //set fixed size
    modifyButton->setFixedSize(modifyButton->width(),modifyButton->height());
    downloadButton->setFixedSize(downloadButton->width(),downloadButton->height());
    removeButton->setFixedSize(removeButton->width(),removeButton->height());
    editProfile->setFixedSize(editProfile->width(),editProfile->height());
    logoutButton->setFixedSize(logoutButton->width(),logoutButton->height());


    QPixmap pix3(":/default_profile.png");
    profileLabel2 = new QLabel(tr("Insert here your image profile"),nullptr);
    profileLabel2->setFrameStyle(QFrame::Box);
    profileLabel2->setPixmap(pix3);
    mainLayout2->addWidget(profileLabel2,0,0);
    profileLabel2->setFixedSize(modifyButton->width(),profileLabel2->height()/5);
    profileLabel2->setScaledContents(true);
    imageObject = nullptr;

    this->d = nullptr;
    this->d2 = nullptr;
    this->setWindowTitle("Poshed - Homepage");
    this->setWindowIcon(QIcon("Graphics/LogoTemp/profile.png"));

//! [0]

//! [1]
    connect(new QShortcut(QKeySequence::Quit, this), &QShortcut::activated,
        qApp, &QApplication::quit);
//! [1]
}

Window::~Window() {
    qDebug()<<"distruttore hompage";
    /*
     *
     *
     */
}


static void updateComboBox(QComboBox *comboBox)
{
    if (comboBox->findText(comboBox->currentText()) == -1)
        comboBox->addItem(comboBox->currentText());
}

//! [3]
void Window::find()
{
    qDebug()<<"find ";
    if(fileComboBox->currentText() == ";"){
            QMessageBox::warning(this,"Find","Forbidden character!");
        }
        else{

            filesTable->setRowCount(0);
            QByteArray array;
            QSqlQuery query;
            QString fileName;
            QString username = MainWindow::username;
            QTcpSocket *s = MyTcpSocket::socket;
            QString regSend = "FIND    ";
            bool error=false;
            if(s->write(regSend.toUtf8()) != regSend.size()) error = true;
            else{
                array.insert(0,username+";"+fileComboBox->currentText());
                qDebug() <<array;
                qint64   l=s->write(array);
                if(l != array.size()) error=true;
            }
            if(error) QMessageBox::information(this,"Server Problem","Error during contacting the server");
        }
}

void Window::animateFindClick()
{
    findButton->animateClick();
}

void Window::on_createButton_clicked(){

    if(d2!=nullptr){
        d2->close();
        d2=nullptr;
    }

    if(d == nullptr){
       d = new Dialog(this, this, "Save file on server as: ");
       mainLayout2->addWidget(d,7,0);
       //mainLayout->addWidget(d,5,2);
       d->setFixedSize(this->modifyButton->width()*2,this->modifyButton->height()*4);
    }
}

void Window::showImage(QByteArray mByteArray){
    QPixmap mPixmap;
    mPixmap.loadFromData(mByteArray,"JPEG(*.jpg *.jpeg);;PNG(*.png)");
    profileLabel2->setPixmap(mPixmap);
    profileLabel2->setScaledContents(true);
}

QSize Window::getButtonSize()
{
    return modifyButton->size();
}

void Window::closeEvent(QCloseEvent *e)
{
    QTcpSocket *s = MyTcpSocket::socket;
    QString username = MainWindow::username;

    QString msg ="LOGOUT  "+username;
    if(s->write(msg.toUtf8()) != msg.size())
        QMessageBox::warning(this,"LOGOUT", "Impossible to communicate with server");
    e->accept();
    mw->show();
}

void Window::on_modifyButton_clicked(){

    QString imagePath = QFileDialog::getOpenFileName(this,tr("Open File"), "", tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" ));
    //QString imagePath = QFileDialog::getOpenFileName(this,tr("Open File"),tr("Images(*.jpg"));
    imageObject = new QImage();
    imageObject->load(imagePath);
    if(imagePath!=""){
        image = QPixmap::fromImage(*imageObject);
        //qDebug()<<"immagine: "<<image.size();
        scene= new QGraphicsScene(this);
        scene->addPixmap(image);
        scene->setSceneRect(image.rect());
        //profileLabel2->setPixmap(image);
        profileLabel2->setScaledContents(true);
        QTcpSocket *s = MyTcpSocket::socket;
        QByteArray array;
        QString regSend = "MODP    ";
        bool error=false;
        if(s->write(regSend.toUtf8()) != regSend.size()) error = true;
        else{
            QFile file(imagePath); //file path
            file.open(QIODevice::ReadOnly);
            QByteArray q = file.readAll();
            if(q.size() <= 307200){
                s->write(q);
                profileLabel2->setPixmap(image);
            }
            else QMessageBox::warning(this,"Image size","Select an image with max 300KB!");
        }
        if(error) QMessageBox::information(this,"Server Problem","Error during contacting the server");

    }
}

void Window::on_removeButton_clicked(){
    QString username = MainWindow::username;
    QTcpSocket *s = MyTcpSocket::socket;

    QString message="REMOVE  ;"+username;
    if(s->write(message.toUtf8()) != message.size()){
        QMessageBox::warning(this,"Remove", "Impossible to communicate with server");
    }
    qDebug() << message;
}

void Window::on_downloadButton_clicked(){

    QString username = MainWindow::username;
    QTcpSocket *s = MyTcpSocket::socket;
    QString message="DOWNLOAD;"+username;

    if(s->write(message.toUtf8()) != message.size()){
        QMessageBox::warning(this,"Download", "Impossible to communicate with server");
    }
}

void Window::on_editProfile_clicked(){
    ed = new edituserprofile(this, this);
    QString username = MainWindow::username;
    QTcpSocket *s = MyTcpSocket::socket;

    QString message="EPHOTO  ;"+username;

    //qDebug()<<"Da client a server: "+message;

    if(s->write(message.toUtf8()) != message.size()){
        QMessageBox::warning(this,"Image", "Impossible to communicate with server");
    }
     this->hide();
     ed->show();
}

void Window::on_okButton_clicked()
{
    if(URI->text().size()!=32)
        QMessageBox::warning(this,"URI","URI is not valid");
    else {
        QString username = MainWindow::username;
        QTcpSocket *s = MyTcpSocket::socket;
        QString msg = "SHARE   "+username+";"+URI->text();
        qDebug()<<msg;
        if(s->write(msg.toUtf8()) != msg.size())
            QMessageBox::warning(this,"Share", "Impossible to communicate with server");
        URI->clear();
    }
}

void Window::on_deleteButton_clicked()
{
    if(d!=nullptr){
        d->close();
        d=nullptr;
    }
    if(d2 == nullptr){
       d2 = new Dialog(this, this,"Delete File on Server : ");
       mainLayout2->addWidget(d2,8,0);
       //mainLayout->addWidget(d,5,2);
       d2->setFixedSize(this->modifyButton->width()*2,this->modifyButton->height()*4);
    }
    fileComboBox->lineEdit()->setText("*");
    deletef = true;
    this->find();
}

void Window::logout()
{
/*
    QTcpSocket *s = MyTcpSocket::socket;
    QString username = MainWindow::username;

    QString msg ="LOGOUT  "+username;
    if(s->write(msg.toUtf8()) != msg.size())
        QMessageBox::warning(this,"LOGOUT", "Impossible to communicate with server");

    mw->show();
    */
    this->close();
}

void Window::openFile(QString filename, QString owner, int siteid)
{
    qDebug()<<"openFile";
    textedit = new TextEdit(this,this);
    const QRect availableGeometry = QApplication::desktop()->availableGeometry(textedit);
    textedit->resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
    textedit->move((availableGeometry.width() - textedit->width()) / 2,
    (availableGeometry.height() - textedit->height()) / 2);
    this->hide();
    if(d!=nullptr){
        this->d->close();
        this->d = nullptr;
    }
    if(d2!=nullptr){
        this->d2->close();
        this->d2 = nullptr;
    }
    textedit->setSiteID(siteid);
    textedit->setWindowTitle(filename);
    textedit->setOwner(owner);
    textedit->addUserLabel(MainWindow::username);
    textedit->show();

}

void Window::setFilesTableNull()
{
    filesTable->setRowCount(0);
}

void Window::showFiles(const QStringList &paths,const QStringList &status,const QStringList &sizes)
{
    int count=0;
    for (const QString &filePath : paths) {
            const QString toolTip = QDir::toNativeSeparators(filePath);
            const QString relativePath = QDir::toNativeSeparators(currentDir.relativeFilePath(filePath));
            QTableWidgetItem *fileNameItem = new QTableWidgetItem(relativePath);
            fileNameItem->setData(absoluteFileNameRole, QVariant(filePath));
            fileNameItem->setToolTip(toolTip);
            fileNameItem->setFlags(fileNameItem->flags() ^ Qt::ItemIsEditable);
            int row = filesTable->rowCount();
            filesTable->insertRow(row);
            filesTable->setItem(row, 0, fileNameItem);
     }

    for (const QString &filePath : sizes) {
            const QString toolTip = QDir::toNativeSeparators(filePath);
            const QString relativePath = QDir::toNativeSeparators(currentDir.relativeFilePath(filePath));
            QTableWidgetItem *sizeItem = new QTableWidgetItem(filePath);
            sizeItem->setData(absoluteFileNameRole, QVariant(filePath));
            sizeItem->setToolTip(toolTip);
            sizeItem->setTextAlignment(Qt::AlignLeft | Qt::AlignCenter);
            sizeItem->setFlags(sizeItem->flags() ^ Qt::ItemIsEditable);
            filesTable->setItem(count, 1, sizeItem);
            count++;
      }

    count=0;
    for (const QString &filePath : status) {
            const QString toolTip = QDir::toNativeSeparators(filePath);
            const QString relativePath = QDir::toNativeSeparators(currentDir.relativeFilePath(filePath));
            QTableWidgetItem *fileOwnerItem = new QTableWidgetItem(filePath);
            fileOwnerItem ->setData(absoluteFileNameRole, QVariant(filePath));
            fileOwnerItem ->setToolTip(toolTip);
            fileOwnerItem ->setFlags(fileOwnerItem ->flags() ^ Qt::ItemIsEditable);
            filesTable->setItem(count,2, fileOwnerItem );
            count++;
        }
            filesFoundLabel->setText(tr("%n file(s) found (Double click on a file to open it)", nullptr, paths.size()));
            filesFoundLabel->setWordWrap(true);
}
//! [8]

//! [10]
QComboBox *Window::createComboBox(const QString &text)
{
    QComboBox *comboBox = new QComboBox(this);
    comboBox->setEditable(true);
    comboBox->addItem(text);
    comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return comboBox;
}
//! [10]

//! [11]
void Window::createFilesTable()
{
    filesTable = new QTableWidget(0, 3, this);
    filesTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QStringList labels;
    labels << tr("Filename") << tr("Size") << tr("Status");
    filesTable->setHorizontalHeaderLabels(labels);
    filesTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    filesTable->verticalHeader()->hide();
    filesTable->setShowGrid(false);
    filesTable->setStyleSheet("background-image: url(:/Graphics/LoginBg/17580.png);");
//! [15]
    filesTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(filesTable, &QTableWidget::customContextMenuRequested,
            this, &Window::contextMenu);
    connect(filesTable, &QTableWidget::cellActivated,
            this, &Window::openFileOfItem);
//! [15]
}
//! [11]


//! [12]

void Window::openFileOfItem(int row, int /* column */)
{
    QTcpSocket *s = MyTcpSocket::socket;
    const QTableWidgetItem *item = filesTable->item(row, 0);
    qDebug()<<"OpenFileOfItem";
    textedit = new TextEdit(this,this);
    const QRect availableGeometry = QApplication::desktop()->availableGeometry(textedit);
    textedit->resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
    textedit->move((availableGeometry.width() - textedit->width()) / 2,
    (availableGeometry.height() - textedit->height()) / 2);
    if(d!=nullptr){
        this->d->close();
        this->d = nullptr;
    }
    if(d2!=nullptr){
        this->d2->close();
        this->d2 = nullptr;
    }

    QString msg = "OPENF   "+fileNameOfItem(filesTable->item(row,0))+";"+fileNameOfItem(filesTable->item(row,2))+";"+MainWindow::username;
    qDebug()<<msg;
    if(s->write(msg.toUtf8()) != msg.size())
        QMessageBox::warning(this,"Openf", "Impossible to communicate with server");

}

//! [12]

//! [16]
void Window::contextMenu(const QPoint &pos)
{
    const QTableWidgetItem *item = filesTable->itemAt(pos);
    if (!item)
        return;
    QMenu menu;
#ifndef QT_NO_CLIPBOARD
    QAction *copyAction = menu.addAction("Copy Name");
#endif
    QAction *openAction = menu.addAction("Open");
    QAction *action = menu.exec(filesTable->mapToGlobal(pos));
    if (!action)
        return;
    const QString fileName = fileNameOfItem(item);
    if (action == openAction)
        //openFile(fileName, textedit->getSiteID());
        openFileOfItem(item->row(),textedit->getSiteID());
#ifndef QT_NO_CLIPBOARD
    else if (action == copyAction)
        QGuiApplication::clipboard()->setText(QDir::toNativeSeparators(fileName));
#endif
}
//! [16]
void Window::setFilesFoundLabel(QString test){
filesFoundLabel->setText(tr(test.toUtf8(), nullptr));
}
