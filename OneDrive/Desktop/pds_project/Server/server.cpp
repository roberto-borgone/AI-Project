#include "server.h"
#include <iostream>
#include <QSqlQuery>
#include <QtCore>
#include <QSqlRecord>
#include <QImage>
#include <QFont>
#include "cn.h"
#include <exception>
using namespace std;

static bool processing = true;
static std::queue<QString> clientRequestMessage;


Server::Server(QObject *parent) :
    QObject(parent)
{
    try {
        cn::_obj = new cn();
    } catch (...) {
        throw;
    }

    server = new QTcpServer(this);
    this->_siteID = 0;
    //this->path = "C:/Users/Riccardo/Documents/pds_project/Server/FileSystem/";
    this->path = "C:/Users/rebec/OneDrive/Desktop/pds_project/Server/FileSystem/";


    timer = new QTimer(this);
    QTimer *timerFormat = new QTimer(this);
    QTimer::connect(timer, SIGNAL(timeout()), this, SLOT(dispatchMessage()));
    QTimer::connect(timer, SIGNAL(timeout()), this, SLOT(dispatchMessage()));

    QTimer::connect(timerFormat, SIGNAL(timeout()), this, SLOT(dispatchFormat()));
    QTimer::connect(timerFormat, SIGNAL(timeout()), this, SLOT(dispatchFormat()));

    timer->start(10); //ogni decimo di secondo
    timerFormat->start(10);

    // whenever a user connects, it will emit signal
    connect(server, SIGNAL(newConnection()),
            this, SLOT(newConnection()));
    //connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    //connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));


    if(!server->listen(QHostAddress::Any, 9950))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
    }

}

Server::~Server()
{
    qDebug()<<"CI SON CASCATO DI NUOVO";
    server->close();
}

void Server::newConnection()
{
    //need to grab the socket
    this->socket.push_back(server->nextPendingConnection());

    //connect(socket.back(),SIGNAL(disconnected()),this,SLOT(closeEditor()));

    connect(socket.back(), SIGNAL(readyRead()),this, SLOT(readyRead()));
    connect(socket.back(),SIGNAL(disconnected()),this,SLOT(disconnected()));

    /*socket->write("Hello client\r\n");
    socket->flush();*/

    //socket->waitForReadyRead(3000);
    //socket->close();
}

void Server::readyRead()
{
    client = qobject_cast<QTcpSocket*>(sender());
    while(client->bytesAvailable()){
        qDebug() << "reading...";
        //socket->waitForReadyRead(8000);
        // read the data from the socket
        //char proto[9] = { '\0','\0','\0','\0','\0','\0','\0','\0','\0'};
        //int increasedSize = client->socketOption(QAbstractSocket::ReceiveBufferSizeSocketOption).toInt();
        //qDebug()<<"BUFFER SIZE"<<increasedSize;
        if(socket.contains(client)){
            QByteArray proto;
            QByteArray array;
            static QString username;
            QString psw;
            // socket->waitForReadyRead();
            //qint64 l=socket->read(proto,8);

            proto=client->readLine(9);

            //QByteArray proto1=client->readLine(9);
            //this->clientRequestProto.push(proto1);

            qDebug()<<proto.size()<<" "<<proto;
            if((proto.size()==8) && QString(proto).toUtf8().compare("+REG    ")==0){
                  qDebug() << proto;
                  array=client->readLine(3); //al massimo il numero dei caratteri username+password =64 (2 char) +1 per \0                 
                  if(!array.isEmpty()){
                      int len=array.toInt();
                      if(len <= 289){
                          //qDebug() <<len;
                          array.clear();
                          array=client->readLine(290);
                          if(len == array.size()-1){
                              QString DataAsString = QString(array);
                              QStringList list = DataAsString.split(' ');
                              //for (int i = 0; i < list.size(); ++i)
                                       //std::cout << list.at(i).toLocal8Bit().constData() << std::endl;
                              username = list[0];
                              psw = list[1];
                              //qDebug() <<username;
                              //qDebug() <<psw;

                              cn::obj();
                              QSqlDatabase::database().transaction();
                              QSqlQuery query1;
                              query1.prepare("SELECT username FROM users WHERE username = ?");
                              query1.bindValue(0,username);

                              if(query1.exec() == false) {
                                  array.clear();
                                  array.insert(0,"-ERR    ;");
                                  client->write(array);
                              }else{
                                  if (query1.next()) {
                                      array.clear();
                                      array.insert(0,"-ERR -u ;");
                                      client->write(array);
                                  }
                                  else {

                                      QSqlQuery query;
                                      query.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
                                      query.bindValue(0, username);
                                      query.bindValue(1, psw);
                                      if(query.exec() == false) {
                                          array.clear();
                                          array.insert(0,"-ERR    ;");
                                          client->write(array);
                                      }
                                      else{
                                          array.clear();
                                          array.insert(0,"+OK +r  ;");
                                          client->write(array);
                                          array.clear();
                                          if(QDir(path+"Documenti/"+username).exists() == false){
                                              //qDebug() <<"sono dentro";
                                              QDir().mkdir(path+"Documenti/"+username);
                                          }

                                            //socket->write("+OK +r");
                                      }
                                  }
                              }
                              QSqlDatabase::database().commit();
                          }
                      }
                  }
            }
            if((proto.size()==8) && QString(proto).toUtf8().compare("+LOG    ")==0){
                  qDebug() << proto;
                  array=client->readLine(290);
                  QString DataAsString = QString(array);
                  QStringList list = DataAsString.split(' ');
                  username = list[0];
                  psw = list[1];
                  //qDebug() <<username;
                  //qDebug() <<psw;
                  cn::obj();
                  QSqlQuery query1;
                  if(socketMap.contains(username)){
                      array.clear();
                      array.insert(0,"-ERR -a ;");
                      client->write(array);
                  }
                  else{
                      query1.prepare("SELECT username,password FROM users WHERE username = ? AND password = ?");
                      query1.bindValue(0, username);
                      query1.bindValue(1,QString(QCryptographicHash::hash((psw.toUtf8()),QCryptographicHash::Sha256).toHex()));
                      if(query1.exec() == false) {
                          array.clear();
                          array.insert(0,"-ERR    ;");
                          client->write(array);
                      }else{
                          if (!query1.next()) {
                              array.clear();
                              array.insert(0,"-ERR -up;");
                              client->write(array);
                          }
                          else {
                              array.clear();
                              array.insert(0,"+OK +l  ;");
                              client->write(array);
                              array.clear();
                              QString imagePath;
                              if(QFile(path+"images/"+username+"/profile_picture.jpg").exists() == false){
                                  imagePath=(path+"images/default/default_profile.png");
                              }else{
                                 imagePath=path+"images/"+username+"/profile_picture.jpg";
                              }
                              QFile file(imagePath); //file path
                              file.open(QIODevice::ReadOnly);
                              QByteArray q = file.readAll();
                              //qDebug()<<"size"<<q.size();
                              //socket->write(q);
                              qDebug()<<client->write(q);  //size dell'immagine
                              socketMap.insert(username, client); //
                          }
                      }

                }
            }
            if((proto.size()==8) && QString(proto).toUtf8().compare("FIND    ")==0){
                qDebug() << proto;
                array=client->readLine(66);
                QString DataAsString = QString(array);
                QStringList list = DataAsString.split(';');
                username = list[0];
                QString fileName = list[1];
                //qDebug() <<username;
                //qDebug() <<fileName;
                find(username,fileName);
            }
            if((proto.size()==8) && QString(proto).toUtf8().compare("OPENF   ")==0){
                qDebug()<<proto;
                array=client->readLine(66);
                QString DataAsString = QString(array);
                QStringList list = DataAsString.split(';');
                QString fileName = list[0];
                QString owner = list[1];
                QString username = list[2];
                //qDebug() <<fileName;
                //qDebug() <<owner;
                //find(owner,fileName);
                fileOpen(fileName,owner,username);
            }

            if((proto.size()==8) && QString(proto).toUtf8().compare("EDIT    ")==0){
                qDebug()<<proto;
                this->edit();
            }

            if((proto.size()==8) && QString(proto).toUtf8().compare("NEWFILE ")==0){
                qDebug()<<proto;
                this->createNewFile();
            }
            if((proto.size()==8) && QString(proto).toUtf8().compare("DELETEFI")==0){
                qDebug()<<proto;
                this->deleteFile();
            }

            if((proto.size()==8) && QString(proto).toUtf8().compare("GETURI  ")==0){
                qDebug()<<proto;
                this->sendURI();
            }

            if((proto.size()==8) && QString(proto).toUtf8().compare("SHARE   ")==0){
                qDebug()<<proto;
                this->share();
            }
            if((proto.size()==8) && QString(proto).toUtf8().compare("LI      ")==0){
                qDebug()<<proto;
                this->insert();
            }
            if((proto.size()==8) && QString(proto).toUtf8().compare("LE      ")==0){
                qDebug()<<proto;
                this->erase();
            }
            /*
            if((proto.size()==8) && QString(proto).toUtf8().compare("TCF     ")==0){
                qDebug()<<proto;
                this->changeFormat();
            }*/
            if((proto.size()==8) && QString(proto).toUtf8().compare("TCFM    ")==0){
                qDebug()<<proto;
                this->changeExistingFormat();
            }
            if((proto.size()==8) && QString(proto).toUtf8().compare("LOGOUT  ")==0){
                qDebug()<<proto;
                this->logout();
            }
            if((proto.size()==8) && QString(proto).toUtf8().compare("MULTI   ")==0){
                qDebug()<<proto;
                this->readMulti();
            }
            /*
            if((proto.size()==8) && QString(proto).toUtf8().compare("CHANGE  ")==0){
                qDebug()<<proto;
                this->changePosition();
            }
            */
            if((proto.size()==8) && QString(proto).toUtf8().compare("EPHOTO  ")==0){
                qDebug()<<proto;
                array=client->readLine(33);
                QStringList username = QString(array).split(";");

                //qDebug()<<username[1];

                QString imagePath;
                if(QFile(path+"images/"+username[1]+"/profile_picture.jpg").exists() == false){
                    imagePath=(path+"images/default/default_profile.png");
                }else{
                   imagePath=path+"images/"+username[1]+"/profile_picture.jpg";
                }

                client->write("EPHOTO  ;");
                QFile file(imagePath); //file path
                file.open(QIODevice::ReadOnly);
                QByteArray q = file.readAll();
                //qDebug()<<"size"<<q.size();
                qDebug()<<client->write(q); //size immagine

                QString msg = "INFOUTENTE";

                QSqlQuery query;
                query.prepare("SELECT first_name, last_name, nickname, gender, date_of_birth FROM users WHERE username = ?");
                query.bindValue(0, username[1]);

                if(query.exec() == false) {
                    msg += ";-1";
                }
                else {
                    if(query.next()) {

                        if(!query.value(0).toString().isEmpty())
                            msg += ";"+query.value(0).toString();
                        else
                            msg += ";null";

                        if(!query.value(1).toString().isEmpty())
                            msg += ";"+query.value(1).toString();
                        else
                            msg += ";null";

                        if(!query.value(2).toString().isEmpty())
                            msg += ";"+query.value(2).toString();
                        else
                            msg += ";null";

                        if(!query.value(3).toString().isEmpty()) {
                            QString res = query.value(3).toString();
                            if(res == '0')
                                msg += ";0";
                            else
                                msg += ";1";
                        }
                        else
                           msg += ";null";

                        if(!query.value(4).toString().isEmpty())
                            msg += ";"+query.value(4).toString();
                        else
                            msg += ";null";
                    }
                }

                qDebug()<<"Da server a client INFO: " + msg;
                client->write(msg.toUtf8());
            }

            if((proto.size()==8) && QString(proto).toUtf8().compare("REMOVE  ")==0){
                qDebug()<<proto;
                QString message="REMOVE  ;";
                array=client->readLine(33);
                QStringList username = QString(array).split(";");
                //qDebug()<<username[1];
                client->write(message.toUtf8());

                QByteArray msg;

                QString imagePath;

                if(QFile(path+"images/"+username[1]+"/profile_picture.jpg").exists() == true){
                    imagePath=(path+"images/default/default_profile.png");
                    QDir dir(path+"images/"+username[1]);
                    dir.removeRecursively();
                    //cosÃƒÂ¬ ho rimosso la directory
                    imagePath=(path+"images/default/default_profile.png");
                    QFile file(imagePath); //file path
                    file.open(QIODevice::ReadOnly);
                    msg = file.readAll();
                    //qDebug()<<"size"<<msg.size();

                }
                else{
                    //mandare errore
                    msg = "-1";
                    //qDebug()<<"No image"<<msg;
                }

                qDebug()<<client->write(msg);
            }

            if((proto.size()==8) && QString(proto).toUtf8().compare("DOWNLOAD")==0){
                qDebug()<<proto;
                array=client->readLine(33);
                QStringList username = QString(array).split(";");
                QString imagePath;

                client->write("DOWNLOAD;");

                if(QFile(path+"images/"+username[1]+"/profile_picture.jpg").exists() == true){
                    qDebug()<<client->write("+1");
                    imagePath=(path+"images/"+username[1]+"/profile_picture.jpg");
                    QFile file(imagePath); //file path
                    file.open(QIODevice::ReadOnly);
                    QByteArray image = file.readAll();
                    //qDebug()<<"size"<<image.size();
                    qDebug()<<client->write(image);

                }else{
                   client->write("-1");
                }
            }

            if((proto.size()==8) && QString(proto).toUtf8().compare("MODP    ")==0){
                qDebug()<<proto<<" "<<username;
                QByteArray photo;
                photo.clear();
                if(QDir(path+"images/"+username).exists() == false){
                    QDir().mkdir(path+"images/"+username);
                }
                QFile target;
                target.setFileName(path+"images/"+username+"/profile_picture.jpg");

                if(!target.open(QIODevice::WriteOnly)){
                    //qDebug() << "Can't open file for written";
                    return;
                }

                while(client->waitForReadyRead(1000)){
                    photo = client->readAll();
                    target.write(photo);
                    array.clear();
                }

                target.close();
                //qDebug() << "file size: " << target.size();
                //qDebug() << "Finished!" ;
            }
             if((proto.size()==8) && QString(proto).toUtf8().compare("CLOSE   ")==0){
                 this->discoNoSlot();
             }
            //processing = true;
        //}

        }
    }
}

void Server::find(QString username, QString fileName)
{
    cn::obj();
    QByteArray array;
    QSqlQuery query;
    int recNum=0;
    QStringList files;
    QStringList sizes;
    QStringList status;


    if(fileName == "*")
        query.prepare("SELECT * FROM documenti WHERE username = ?");
    else
        query.prepare("SELECT file,owner FROM documenti WHERE username = ? AND file LIKE '"+fileName+"%'");

    query.bindValue(0, username);

    if(query.exec() == false){
        array.clear();
        array.insert(0,"-ERR -db;");
        client->write(array);
     }
    else{
           QSqlRecord rec = query.record();
           int nameCol = rec.indexOf("file");
           int nameCol1 = rec.indexOf("username");
           int nameCol2 = rec.indexOf("owner");
           if (query.first() || query.value(nameCol1).toString()!=""){
               array.clear();
               if(deletef == false) array.insert(0,"+OK ff  ;");
               else array.insert(0,"+OK ffd ;");
               client->write(array);
               array.clear();
               do{
                    recNum++;
                   //if(fileName!="*")
                    fileName = query.value(nameCol).toString();

                   //QString path = QDir::cleanPath("/Users/vincenzotorcasio/Documents/pds_project/Server/Documenti/"+username); //TOGLIERE IL PATH ASSOLUTO E METTERE TUTTO NEL QRC
                   //QDir  currentDir = QDir(path);
               //! [3]

                   //updateComboBox(fileComboBox);
                  /* updateComboBox(textComboBox);*/
                   //updateComboBox(directoryComboBox);

               //! [4]

                   if (!fileName.isEmpty()) files.insert(recNum-1,fileName);
                   //qDebug() << files;
                   //if(QString::compare(query.value(nameCol2).toString(),username) == 0) status.insert(recNum-1,"Owner");
                   //else status.insert(recNum-1,"Shared");
                   status.insert(recNum-1,query.value(nameCol2).toString());
                   //qDebug() << status;
                   sizes.insert(recNum-1,QString::number(QFileInfo(path+"Documenti/"+query.value(nameCol2).toString()+"/"+fileName+".txt").size()));
                   //QDirIterator it(path, filter, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
                   //qDebug()<<sizes;
                   //while (it.hasNext())
                     //  files << it.next();
                   /*if (!text.isEmpty())
                       files = findFiles(files, text);*/
                   }while(query.next());
                   // files.sort();
                    QString s = QString::number(recNum);
                    array.insert(0,s.toUtf8());
                    client->write(array+";"); //di solito sono 2 byte perchÃƒÆ’Ã‚Â¨ il numero entra in 1 byte + 1 byte per \0
                    qDebug() <<"Dim: "<<array;
                    array.clear();
               for(int i = 0;i<recNum;i++){
                   //qDebug() << files.at(i);
                   /*s=QString::number(files.at(i).size());
                   array.insert(0,s.toUtf8());
                   client->write(array+";"); //di solito sono 2 byte perchÃƒÆ’Ã‚Â¨ il numero entra in 1 byte + 1 byte per \0 */
                   array.clear();
                   array.insert(0,files.at(i)+";"+status.at(i)+";"+sizes.at(i));
                   qDebug() << array;
                   client->write(array+";");
                   array.clear();
               //showFiles(files);
               }

           }
           else{
               array.clear();
               if (deletef == false) array.insert(0,"-FNF    ;");
               else array.insert(0,"-FNFd   ;");
               client->write(array);
               array.clear();
           }
    }
    deletef = false;
//}
    /* QString text = textComboBox->currentText();*/
}

void Server::edit()
{
    QByteArray array=client->readAll();
    QString DataAsString = QString(array);
    QStringList list = DataAsString.split(';');
    QString username = list[0];
    QString firstName = list[1];
    QString lastName = list[2];
    QString nickName = list[3];
    QString oldPassword = list[4];
    QString newPassword = list[5];
    QString confirmPassword = list[6];
    QString gender = list[7];
    QString dateOfBirth = list[8];

    QString message="EDIT    ;";

    if(firstName!="null") {

       QSqlQuery query;
       query.prepare("UPDATE users SET first_name = ? WHERE username = ?");
       query.bindValue(0, firstName);
       query.bindValue(1, username);

       if(!query.exec()) {
           message+= "-1";
       }
       else {
           message+= "+1";
       }
   }
    else {
        message+= "+0";
    }

   if(lastName!="null") {
       QSqlQuery query;
       query.prepare("UPDATE users SET last_name = ? WHERE username = ?");
       query.bindValue(0, lastName);
       query.bindValue(1, username);

       if(!query.exec()) {
           message+= ";-1";
       }
       else {
           message+= ";+1";
       }
   }
    else {
        message+= ";+0";
    }

   if(nickName!="null") {

      QSqlQuery query;
      query.prepare("UPDATE users SET nickname = ? WHERE username = ?");
      query.bindValue(0, nickName);
      query.bindValue(1, username);

      if(!query.exec()) {
          message+= ";-1";
      }
      else {
          message+= ";+1";
      }
  }
   else {
       message+= ";+0";
   }

   if(oldPassword=="error" && newPassword=="error" && confirmPassword=="error")
        message+= ";-5";

   else if(oldPassword=="null" && newPassword=="null" && confirmPassword=="null")
        message+= ";+0";

   else {

       QSqlQuery query1;
       query1.prepare("SELECT password FROM users WHERE username = ? AND password = ?");
       query1.bindValue(0, username);
       query1.bindValue(1, oldPassword);

       if(!query1.exec()) {
           message+= ";-1";
       }
       else{
           if (!query1.next()) {
               // sbagliata vecchia password
               message+= ";-2";
           }
          else{

               QRegExp re("((?=.*[0-9])(?=.*[a-zA-Z]).{8,32})");

               if(re.exactMatch(newPassword) == false){
                   // sbagliata nuova password
                   message+= ";-3";
               }
               else{
                   if(newPassword.compare(confirmPassword) != 0){
                       // nuova pass diversa da confirm
                       message+= ";-4";
                   }
                   else{
                       QSqlQuery query2;
                       query2.prepare("UPDATE users SET password = ? WHERE username = ?");
                       query2.bindValue(0, newPassword);
                       query2.bindValue(1, username);

                       if(!query2.exec()) {
                           message+= ";-1";
                       }
                       else
                        message+= ";+1";
                   }
               }
           }
       }
   }

   if(gender!="null"){
       QSqlQuery query;
       query.prepare("UPDATE users SET gender = ? WHERE username = ?");

       query.bindValue(0, gender);
       query.bindValue(1, username);

       if(!query.exec()) {
           message+=";-1";
       }
       else message+=";+1";
   }
   else message+=";+0";

   if(dateOfBirth != "null") {

       QString currentDate = QString::number(QDate::currentDate().year())+"/"+
                             QString::number(QDate::currentDate().month())+"/"+
                             QString::number(QDate::currentDate().day());

       if(dateOfBirth.split("/")[0].toInt() <= QDate::currentDate().year()-14) {
           QSqlQuery query;
           query.prepare("UPDATE users SET date_of_birth = ? WHERE username = ?");
           query.bindValue(0, dateOfBirth);
           query.bindValue(1, username);

           if(!query.exec()) {
              message += ";-1";
           }
           else
               message += ";+1";
       }
      else if(dateOfBirth >= currentDate)
          message += ";-3"; //date not valid
      else
           message += ";-2";
   }
   else message += ";+0";
   qDebug()<<message;
   qDebug()<<"Update profile done";

   client->write(message.toUtf8());

}

void Server::createNewFile()
{
    QByteArray array=client->readAll();
    QString DataAsString = QString(array);
    QStringList list = DataAsString.split(';');
    QString username = list[0];
    QString title = list[1];

    QString message="NEWFILE ";

    if(title.contains(";"))
        message += ";-3";
    else if(title=="") {
        QSqlQuery query1;
        query1.prepare("SELECT file FROM documenti WHERE file LIKE 'untitled%' AND username = ?");
        query1.bindValue(0, username);

        if(query1.exec() == false) {
            message += ";-1";
        }
        else {
            int num = query1.size()+1;
            QString title = "untitled"+QString::number(num);
            //qDebug() << title;

            //QString URI=QString(QCryptographicHash::hash((title.toUtf8()),QCryptographicHash::Md5).toHex());
            QString URI=QString(QCryptographicHash::hash((title.toUtf8()+username.toUtf8()),QCryptographicHash::Md5).toHex());

            QSqlQuery query2;
            query2.prepare("INSERT INTO documenti(owner, file, username, URI) VALUES(?, ?, ?, ?)");
            query2.bindValue(0, username);
            query2.bindValue(1, title);
            query2.bindValue(2, username);
            query2.bindValue(3,URI);

            if(query2.exec() == false) {
                message += ";-1";
            }
            else {
                QFile file;
                QFile fileFont;
                file.setFileName(path+"Documenti/"+username+"/"+title+".txt"); //cambia il path (vedi anche riga 94)
                fileFont.setFileName(path+"Documenti/"+username+"/"+title+"_font"+".txt");
                file.open(QIODevice::ReadWrite|QIODevice::Text);
                fileFont.open(QIODevice::ReadWrite|QIODevice::Text);
                std::vector<QString> clientConnessi;
                clientConnessi.push_back(username);
                clientMap.insert(pair<QString, std::vector<QString>>(username+title, clientConnessi));
                localMap.insert(username+title,std::vector<Symbol>());
                std::vector<std::pair<QString,int>> vec;
                vec.push_back(std::pair<QString,int>(username,1)); //il creatore ha sempre siteid 1 per tutti i file
                idMap.insert(std::pair<QString, std::vector<std::pair<QString,int>>>(username+title, vec));
                message += ";+1;"+title+";"+QString::number(1);
            }
        }
    }
    else {
        QSqlQuery query1;
        query1.prepare("SELECT * FROM documenti WHERE owner = ? AND file = ?");
        query1.bindValue(0, username);
        query1.bindValue(1, title);

        if(query1.exec() == false) {
            //qDebug() << "sono qui";
            message += ";-1";
        }
        else {
            if(query1.next()) {
                //titolo giÃƒÆ’  esistente
                message += ";-2";
            }
            else {
                //QString URI=QString(QCryptographicHash::hash((title.toUtf8()),QCryptographicHash::Md5).toHex());
                QString URI=QString(QCryptographicHash::hash((title.toUtf8()+username.toUtf8()),QCryptographicHash::Md5).toHex());
                QSqlQuery query2;
                query2.prepare("INSERT INTO documenti(owner, file, username,URI) VALUES(?, ?, ?, ?)");
                query2.bindValue(0, username);
                query2.bindValue(1, title);
                query2.bindValue(2, username);
                query2.bindValue(3,URI);

                if(query2.exec() == false) {
                    message += ";-1";
                }
                else {
                    QFile file;
                    QFile fileFont;
                    file.setFileName(path+"Documenti/"+username+"/"+title+".txt"); //cambia il path (vedi anche riga 56)
                    fileFont.setFileName(path+"Documenti/"+username+"/"+title+"_font"+".txt");
                    file.open(QIODevice::ReadWrite|QIODevice::Text);
                    fileFont.open(QIODevice::ReadWrite|QIODevice::Text);
                    std::vector<QString> clientConnessi;
                    clientConnessi.push_back(username);
                    clientMap.insert(pair<QString, std::vector<QString>>(username+title, clientConnessi));
                    localMap.insert(username+title,std::vector<Symbol>());
                    std::vector<std::pair<QString,int>> vec;
                    vec.push_back(std::pair<QString,int>(username,1)); //il creatore ha sempre siteid 1 per tutti i file
                    idMap.insert(std::pair<QString, std::vector<std::pair<QString,int>>>(username+title, vec));
                    message += ";+1;"+title;
                    message+=";"+QString::number(1);
                }
            }
        }
    }

    qDebug()<<message;
    client->write(message.toUtf8());

}

void Server::deleteFile()
{
    QByteArray array=client->readAll();
        QString DataAsString = QString(array);
        QStringList list = DataAsString.split(';');
        QString username = list[0];
        QString title = list[1];

        //qDebug()<<"usernameeeee:"<<username;

        QString message="DELETEFI";

        if(title.contains(";"))
            message += ";-3";
        else {
            QSqlQuery query1;
            query1.prepare("SELECT COUNT(*) FROM documenti WHERE file = ? AND username = ?");
            query1.bindValue(0, title);
            query1.bindValue(1, username);
            if(query1.exec() == false) {
                //qDebug() << "sono qui";
                message += ";-1";
            }
            else {
                query1.next();
                int num = query1.value(0).toInt();
                //qDebug()<<"numero:::: "<<num;
                if(num==0) message+=";-2"; //non esiste nessun file con quel nome
                else {
                    QSqlQuery query3;
                    query3.prepare("SELECT URI,owner FROM documenti WHERE file = ? AND owner = ? AND username = ?");
                    query3.bindValue(0, title);
                    query3.bindValue(1, username);
                    query3.bindValue(2, username);
                    if(query3.exec() == false) {
                        message += ";-1";
                    }
                    else{
                        if(query3.size() == 0) message+=";-4"; //non sei l'owner
                        else {
                            query3.next();
                            //qDebug()<<"owner:"<<query3.value(1).toString();
                            //qDebug()<<"uriiiiiiii:"<<query3.value(0).toString();
                            QSqlQuery query2;
                            query2.prepare("DELETE FROM documenti WHERE URI = ? ");
                            query2.bindValue(0, query3.value(0).toString());

                            if(query2.exec() == false) {
                                message += ";-1";
                            }
                            else{
                                query2.next();
                                QFile file;
                                file.setFileName(path+"Documenti/"+username+"/"+title+".txt"); //cambia il path (vedi anche riga 56)
                                file.remove();
                                QFile filefont;
                                filefont.setFileName(path+"Documenti/"+username+"/"+title+"_font"+".txt");
                                filefont.remove();
                                idMap.erase(idMap.find(username+title));
                                localMap.erase(localMap.find(username+title));
                                deletef = true;
                                find(username,"*");
                                message.clear();
                                message = title+";";
                            }
                        }
                    }
                }
            }
        }

        qDebug()<<message;
        client->write(message.toUtf8());

}

void Server::sendURI()
{
    QByteArray array=client->readAll();
    //qDebug()<<array;
    QString DataAsString = QString(array);
    QStringList list = DataAsString.split(';');
    QString username = list[0];
    QString filename = list[1];

    QString message="SENDURI ";

    QSqlQuery query1;
    query1.prepare("SELECT URI FROM documenti WHERE file = ? AND owner = ? GROUP BY URI");
    query1.bindValue(0, filename);
    query1.bindValue(1, username);

    if(query1.exec() == false) {
        message += ";-1";
    }
    else {
        if(query1.next())
            message += ";"+query1.value(0).toString();
    }

    qDebug()<<message;
    client->write(message.toUtf8());
}

void Server::share()
{
    QByteArray array=client->readAll();
    //qDebug()<<"Sono nella share"<<array;
    QString DataAsString = QString(array);
    QStringList list = DataAsString.split(';');
    QString username = list[0];
    QString URI = list[1];

    QString message="SHARE   ";

    QSqlDatabase::database().transaction();
    QSqlQuery query1;
    query1.prepare("SELECT owner, file, username FROM documenti WHERE URI = ?");
    query1.bindValue(0, URI);

    if(query1.exec() == false) {
        message += ";-1";
        //qDebug()<<"primo";
        client->write(message.toUtf8());
    }
    else{
        bool found = false;
        int count = 0;
        QString owner;
        QString filename;
        QString username_;

        while(query1.next()) {
            count++;
            owner = query1.value(0).toString();
            filename = query1.value(1).toString();
            username_ = query1.value(2).toString();

            if(username == owner || username == username_) {
                found = true;
                break;
            }
        }

        if(count==0) {
            message += ";-2"; //URI is not valid
            client->write(message.toUtf8());
        }
        else {
            if(!found) {

                QSqlQuery query2;

                query2.prepare("INSERT INTO documenti(owner, file, username, URI) VALUES(?, ?, ?, ?)");
                query2.bindValue(0, owner);
                query2.bindValue(1, filename);
                query2.bindValue(2, username);
                query2.bindValue(3, URI);

                if(query2.exec() == false) {
                    message += ";-1";
                    //qDebug()<<"secondo";
                    client->write(message.toUtf8());
                }
                else{
                     //_siteID++;
                     //gestire il caso mappa vuota
                    int siteID;
                    if(idMap.count(owner+filename)!=0) {
                        //qDebug()<<"idMap count != 0";
                        //siteID = idMap.at(owner+filename).at(idMap.at(owner+filename).size()-1).second+1;
                        siteID = idMap.at(owner+filename).back().second+1;
                        idMap.at(owner+filename).push_back(std::pair<QString,int>(username,siteID));
                     }
                     else {
                        //qDebug()<<"idMap count == 0";
                        siteID = 1;
                        std::vector<std::pair<QString, int>> siteIDvector;
                        siteIDvector.push_back(std::pair<QString, int>(username, siteID));
                        idMap.insert(std::pair<QString, std::vector<std::pair<QString, int>>>(owner+filename, siteIDvector));
                    }
                    /*
                     if(clientMap.count(owner+filename)!=0) {
                        qDebug()<<"clientMap count != 0";
                        clientMap.at(owner+filename).push_back(username);
                     }
                     else {
                         qDebug()<<"clientMap count == 0";
                         std::vector<QString> clientConnessi;
                         clientConnessi.push_back(username);
                         clientMap.insert(std::pair<QString, std::vector<QString>>(owner+filename, clientConnessi));
                     }
                     */

                     //message += ";+1"+filename+";"+owner+";"+QString::number(siteID);
                     message += ";+1;"+QString::number(siteID);
                     qDebug()<<"message "<<message;
                     qDebug()<<"socket map size "<<socketMap.size();
                     //qDebug()<<"socket map user "<<socketMap[username];
                     std::vector<QString> clientList  = clientMap.at(owner+filename); //
                     socketMap[username]->write(message.toUtf8());
                     socketMap[username]->flush();
                     //client->write(message.toUtf8());
                     //qDebug()<<"write "<<message;
                     this->fileOpen(filename, owner, username);
                }
            }
            else {
                message += ";-3"; //file is already in user directory
                client->write(message.toUtf8());
            }
        }
    }
    QSqlDatabase::database().commit();
}

void Server::insert()
{
    QString stringSize = client->read(3); //leggo le prime 3 cifre che contengono l'informazione sulla dimensione della stringa da leggere
    //qDebug()<<"stringSize"<<stringSize;
    QString data;

    clientRequestMessage.push(client->read(stringSize.toInt()));
    //qDebug()<<"prima size"<<clientRequestMessage.size()<<endl;

    while(!clientRequestMessage.empty() && processing){
        processing = false;
        //proto = clientRequestProto.front();
        //clientRequestProto.pop();
        //qDebug()<<"dopo size"<<clientRequestMessage.size()<<endl;
        data = clientRequestMessage.front();
        clientRequestMessage.pop();
        //qDebug()<<"socket buffer size "<<client->size();
        // qDebug()<<"data "<<data;


    /*qDebug()<<"prima socket size"<<client->size();
    do{
        data += client->read(stringSize.toInt()); //
        qDebug()<<"size "<<stringSize;
        qDebug()<<"data "<<data;
        qDebug()<<"socket size"<<client->size();
    } while(data.size() < stringSize);
    */

        QStringList data2 = data.split(";,");

        QString op = "LI";
        QString username = data2[0];
        QString title = data2[1];
        int id = data2[2].toInt();
        int counter_message = data2[3].toInt();
        QString charValue = data2[4];
        QStringList posList = data2[5].split("|");
        std::vector<int> pos_message;
        for(auto i = 0; i<posList.size(); i++){
            pos_message.push_back(posList[i].toInt());
        }
        QStringList fontList = data2[6].split(",");
        QFont font = QFont();
        font.setFamily(fontList[0]);
        font.setPointSizeF(fontList[1].toDouble());
        font.setWeight(fontList[4].toInt());
        font.setItalic(fontList[5].toInt());
        font.setUnderline(fontList[6].toInt());
        QString owner = data2[7];
        int x = data2[8].toInt();
        int y = data2[9].toInt();
        /*
        qDebug()<<op;
        qDebug()<<username;
        qDebug()<<ownerTitle;
        qDebug()<<id;
        qDebug()<<counter_message;
        qDebug()<<charValue;
        qDebug()<<posList;
        qDebug()<<"font"<<font;
        */

        Symbol s = Symbol(id-counter_message,counter_message,charValue,pos_message,font);

        /*std::vector<QString> clientList  = clientMap.at(owner+title); //
        for(size_t i=0; i<clientList.size(); i++){
            qDebug()<<"clientsize "<< clientList.size();
            if(clientList.at(i) != username){
                qDebug()<<"clientList "<<clientList.at(i);
                process(op,owner+title,s,clientList.at(i));
            }
        }*/
        //messageQueue.push(s.toStr());
        qDebug()<<"data "<<data;
        process(op,owner+title,s,username, x, y);
        //messageQueue.pop();
    /*
        for(auto it : localMap.find(owner+title).value()){
            qDebug()<<it.getValue()<<" "<<it.getPos();
        }
    */
        processing = true;
    }

}

void Server::erase()
{
    QString stringSize = client->read(3); //leggo le prime 3 cifre che contengono l'informazione sulla dimensione della stringa da leggere
    //qDebug()<<"stringSize"<<stringSize;
    ///QString data = client->read(stringSize.toInt());
    qDebug()<<"size"<<stringSize;
    //data.insert(0,"LE      ;"+stringSize+";,");
    ///qDebug()<<data;

    QString data;
    //qDebug()<<"prima socket size"<<client->size();
    do{
        data += client->read(stringSize.toInt()); //
        //qDebug()<<"size "<<stringSize;
        //qDebug()<<"data "<<data;
        //qDebug()<<"socket size"<<client->size();
    } while(data.size() < stringSize);


    //messageQueue.push(data);

    QStringList data2 = data.split(";,");
    QString op = "LE";
    QString username = data2[0];
    QString title = data2[1];
    int id = data2[2].toInt();
    int counter_message = data2[3].toInt();
    QString charValue = data2[4];
    QStringList posList = data2[5].split("|");
    std::vector<int> pos_message;
    for(auto i = 0; i<posList.size(); i++){
        pos_message.push_back(posList[i].toInt());
    }
    QStringList fontList = data2[6].split(",");
    QFont font = QFont();
    font.setFamily(fontList[0]);
    font.setPointSizeF(fontList[1].toDouble());
    font.setWeight(fontList[4].toInt());
    font.setItalic(fontList[5].toInt());
    font.setUnderline(fontList[6].toInt());
    QString owner = data2[7];
    int x = data2[8].toInt();
    int y = data2[9].toInt();
/*
    qDebug()<<op;
    qDebug()<<username;
    qDebug()<<ownerTitle;
    qDebug()<<id;
    qDebug()<<counter_message;
    qDebug()<<charValue;
    qDebug()<<posList;
*/
    Symbol s = Symbol(id-counter_message,counter_message,charValue,pos_message,font);

    //messageQueue.push(s.toStr());
    process(op,owner+title,s,username, x, y);
    //messageQueue.pop();

}

void Server::process(QString op, const QString& key, Symbol& symbol,QString username, int x, int y)
{
    QString supportSize;
    QString messageIS;
    auto size_m = symbol.getPos().size(); //numero di cifre nel vettore frazionario del simbolo da inserire
    auto size = localMap.find(key)->size();

    unsigned int index = 0; //indice del vettore _symbols in cui devo inserire il nuovo simbolo contenuto nel msg
    //auto idClientMessage = symbol.getPos()[size_m-1]; //ultimo elemento del vettore di pos (ID)
    /*
    std::vector<int> residualPos;
    for(size_t it = 0; it<size_m-1; it++){
        residualPos.push_back(symbol.getPos().at(it));
    }
    */

    std::vector<int> residualPos = symbol.getResidualPos();

    bool flag=false;
    //int counter=0; //???????????
    if(op == "LI") {
        // INSERT
        //qDebug()<<"entro il LI?"<<size;
        if(size==0){
            //qDebug()<<"sono nel primo if op=LI";
            localMap.find(key)->push_back(symbol); //devo inserire all'inizio se non ho inserito neanche un simbolo
        }
        else
        if(size==1){ //se ho un solo simbolo
            //qDebug()<<"sono nel caso s==1"<<size_m<<" "<<localMap.find(key)->at(0).getPos().size()<<" resid"<<localMap.find(key)->at(0).getResidualPos();
            if(size_m >= localMap.find(key)->at(0).getPos().size()){
                //qDebug()<<"residual? Residual!"<<residualPos<<" "<<localMap.find(key)->at(0).getResidualPos();
                if(size_m==localMap.find(key)->at(0).getPos().size() && residualPos==localMap.find(key)->at(0).getResidualPos()){
                    if(localMap.find(key)->at(0).getPos().at(localMap.find(key)->at(0).getPos().size()-1) < symbol.getPos().at(symbol.getPos().size()-1)) index=1;
                }
                else{
                    for(size_t i=0;i<localMap.find(key)->at(0).getPos().size();i++){ //scorro l'indice frazionario dell'unico simbolo presente
                        if(symbol.getPos()[i]>localMap.find(key)->at(0).getPos()[i]){
                            index=1;
                            break;
                        }
                    }
                }
            }
            else{
                for(size_t i=0;i<size_m;i++){
                    if(symbol.getPos()[i]>localMap.find(key)->at(0).getPos()[i]){
                        index=1;
                        break;
                    }
                }
            }
            if(index==1) localMap.find(key)->push_back(symbol); //inserisco dopo il primo carattere
            else localMap.find(key)->insert(localMap.find(key)->cbegin(),symbol); //altrimento lo inserisco all'inizio e sposto l'altro di una posizione
        }
        else if(size > 1){
            if(localMap.find(key)->at(size-1).getPos()[0]<symbol.getPos()[0]){
                index=localMap.find(key)->size();
                localMap.find(key)->push_back(symbol); //se il primo digit del mio simbolo ÃƒÆ’Ã‚Â¨ maggiore del primo dell'ultimo simbolo inserito(che ÃƒÆ’Ã‚Â¨ il maggiore) aggiungo in coda
            }
            else if(localMap.find(key)->at(0).getPos()[0]>symbol.getPos()[0]){
                index=0;
                localMap.find(key)->insert(localMap.find(key)->cbegin(),symbol); //se l'ultimo simbolo(il piu piccolo) ha il primo digit maggiore del primo del simbolo da inserire, lo devo mettere in testa
                //else if(this->_symbols[0].pos[0]==pos_message[0] && size_m<this->_symbols[0].pos.size()) this->_symbols.insert(this->_symbols.cbegin(),m.getSymbol());
            }
            /*
            else if(size_m==this->symbols[0].getPos().size() && residualPos==this->symbols[0].getResidualPos() &&
                    this->symbols[0].getPos().at(symbols[0].getPos().size()-1)>symbol.getPos().at(symbol.getPos().size()-1))
                this->symbols.insert(this->symbols.cbegin(),symbol);*/
                // if(this->_symbols[0].getId()>m.getSymbol().getId()) this->_symbols.insert(this->_symbols.cbegin(),m.getSymbol());
                // else this->_symbols.insert(this->_symbols.cbegin(),m.getSymbol());
            else{
                for(size_t i=0;i<size;i++){
                    if(localMap.find(key)->at(i).getPos().size() > size_m){
                        for(size_t j=0;j<size_m && flag!=true ;j++){
                            if(symbol.getPos()[j]==localMap.find(key)->at(i).getPos()[j]) continue;
                            else if(symbol.getPos()[j]<localMap.find(key)->at(i).getPos()[j]){
                                index=uint (i);
                                flag=true;
                            }
                            else if(symbol.getPos()[j]>localMap.find(key)->at(i).getPos()[j]) break;
                        }
                    }
                    if(flag==true) break;
                    if(localMap.find(key)->at(i).getPos().size() <= size_m){
                        if(size_m==localMap.find(key)->at(i).getPos().size() && residualPos==localMap.find(key)->at(0).getResidualPos()){
                            if(localMap.find(key)->at(0).getPos().at(localMap.find(key)->at(0).getPos().size()-1) < symbol.getPos().at(symbol.getPos().size()-1)){
                                index=uint(i);
                                flag=true;
                            }
                            //else if(i+1<size){
                               //     if(this->_symbols[i+1].getId()==m.getSymbol().getId() && m.getSymbol().getCounter()>this->_symbols[i+1].getCounter() ){
                               //       index=i+1;
                               //         flag=true;
                               //     }
                               //     else if(this->_symbols[i+1].getId()<=m.getSymbol().getId()){
                               //         index=i+1;
                               //         flag=true;
                               //     }
                            //}
                            //else this->_symbols.insert(this->_symbols.cbegin(),m.getSymbol());
                        }
                        for(size_t j=0;j<localMap.find(key)->at(i).getPos().size() && flag!=true ;j++){
                            if(symbol.getPos()[j]==localMap.find(key)->at(i).getPos()[j]) continue;
                            else if(symbol.getPos()[j]<localMap.find(key)->at(i).getPos()[j]){
                                index=uint(i);
                                flag=true;
                            }
                            else if(symbol.getPos()[j]>localMap.find(key)->at(i).getPos()[j]) break;
                        }
                    }
                    if(flag==true) break;
                }
                if(flag==true) localMap.find(key)->insert(localMap.find(key)->cbegin()+index,symbol);
                else {
                    index = localMap.find(key)->size();
                    localMap.find(key)->push_back(symbol);
                }
            }
      }

        messageIS = "LI;,"+symbol.toStr()+";,"+QString::number(x)+";,"+QString::number(y)+";,"+username+";,"+key; //QString::number(index)+
        int size = messageIS.toUtf8().size();
        if(size<10){
            supportSize = QString::number(size).insert(0,"0000");
        }
        else if(size<100){
            supportSize = QString::number(size).insert(0,"000");
        }
        else if(size<1000){
            supportSize = QString::number(size).insert(0,"00");
        }
        else if(size<10000){
            supportSize = QString::number(size).insert(0,"0");
        }
        else {
            supportSize = QString::number(size);
        }

       /*
       int nChar=1;
       QString msg;
       QString messageLI;
       if(multi==true){
           nChar = numChar.toInt();
           //qDebug()<<"numero di caratteri"<<nChar<<"flag "<<multi;
           if(nCharTemp==1)
                multi=false;
           if(nChar<500)
               messageLI = "LI;,"+QString::number(nChar)+";,"+QString::number(index)+";,"+symbol.toStr()+";,"+QString::number(x)+";,"+QString::number(y)+";,";
           else if(nCharTemp<500)
               messageLI = "LI;,"+QString::number(nCharTemp)+";,"+QString::number(index)+";,"+symbol.toStr()+";,"+QString::number(x)+";,"+QString::number(y)+";,";
           else
               messageLI = "LI;,"+QString::number(500)+";,"+QString::number(index)+";,"+symbol.toStr()+";,"+QString::number(x)+";,"+QString::number(y)+";,";

           numCharacters++;
           nCharTemp--;
           buffer.append(messageLI);

           if(numCharacters==500 || nCharTemp==0) {
               QString supportSize="";
               int size = buffer.size();
               qDebug()<<"BUFFER SIZE: "<<size;
               if(size<10){
                   supportSize = QString::number(size).insert(0,"0000");
               }
               else if(size<100){
                   supportSize = QString::number(size).insert(0,"000");
               }
               else if(size<1000){
                   supportSize = QString::number(size).insert(0,"00");
               }
               else if(size<10000){
                   supportSize = QString::number(size).insert(0,"0");
               }
               else {
                   supportSize = QString::number(size);
               }
               msg = "MULTI   ;"+supportSize+buffer;

               qDebug()<<"nChar:  "<<nChar;
               qDebug()<<"nCharTemp:  "<<nCharTemp;
               qDebug()<<"numCharacters:   "<<numCharacters;

               std::vector<QString> clientList  = clientMap.at(key);
               for(size_t i=0; i<clientList.size(); i++){
                   if(clientList.at(i) != username){
                       if(socketMap[clientList.at(i)]->write(msg.toUtf8()) != msg.size()){ //ci vuole il for perchÃ¨ devo mandarlo a tutti
                           socketMap[clientList.at(i)]->write("Error sending symbols");
                           socketMap[clientList.at(i)]->flush();
                        }
                       socketMap[clientList.at(i)]->flush();
                   }
               }

               numCharacters=0;
               buffer.clear();
           }
       }
       else{
           QString messageLI = "LI;,"+QString::number(index)+";,"+symbol.toStr()+";,"+QString::number(x)+";,"+QString::number(y)+";,";
           auto size = messageLI.toUtf8().size();
           QString supportSize;
           if(size<10){
               supportSize = QString::number(size).insert(0,"00");
           }
           else if(size<100){
               supportSize = QString::number(size).insert(0,"0");
           }
           else {
               supportSize = QString::number(size);
           }
           msg = "RECEIVE ;"+supportSize+messageLI;
           qDebug()<<msg;

           std::vector<QString> clientList  = clientMap.at(key);
           for(size_t i=0; i<clientList.size(); i++){
               if(clientList.at(i) != username){
                   if(socketMap[clientList.at(i)]->write(msg.toUtf8()) != msg.size()){ //ci vuole il for perchÃ¨ devo mandarlo a tutti
                       socketMap[clientList.at(i)]->write("Error sending symbols");
                       socketMap[clientList.at(i)]->flush();
                    }
                   socketMap[clientList.at(i)]->flush();
               }
           }
       }
       */
    }
    else{
        //DELETE
        //qDebug()<<"sono nella delete della process";
        for(size_t i=0;i<size;i++){

            //qDebug()<<"simbolo nel for"<<localMap.find(key)->at(i).toStr();
            //if(pos_message==this->symbols[i].getPos() && symbol.getId()==this->symbols[i].getId() && symbol.getCounter()==this->symbols[i].getCounter() ){
            if(symbol.getPos()==localMap.find(key)->at(i).getPos()){
                index=i;
                //qDebug()<<"indice "<<index<<"simbols "<<localMap.find(key)->at(i).getValue();
                localMap.find(key)->erase(localMap.find(key)->cbegin()+i);
                break;
            }
        }

        messageIS = "LE;,"+symbol.toStr()+";,"+QString::number(x)+";,"+QString::number(y)+";,"+username+";,"+key; //+QString::number(index)+";,"
        int size = messageIS.toUtf8().size();
        if(size<10){
            supportSize = QString::number(size).insert(0,"0000");
        }
        else if(size<100){
            supportSize = QString::number(size).insert(0,"000");
        }
        else if(size<1000){
            supportSize = QString::number(size).insert(0,"00");
        }
        else if(size<10000){
            supportSize = QString::number(size).insert(0,"0");
        }
        else {
            supportSize = QString::number(size);
        }

        /*
        int nChar=1;

        QString msg;
        QString messageLI;
        if(multi==true){
            nChar = numChar.toInt();
            //qDebug()<<"numero di caratteri"<<nChar<<"flag "<<multi;
            if(nCharTemp==1)
                 multi=false;
            if(nChar<500)
                messageLI = "LE;,"+QString::number(nChar)+";,"+QString::number(index)+";,"+symbol.toStr()+";,"+QString::number(x)+";,"+QString::number(y)+";,";
            else if(nCharTemp<500)
                messageLI = "LE;,"+QString::number(nCharTemp)+";,"+QString::number(index)+";,"+symbol.toStr()+";,"+QString::number(x)+";,"+QString::number(y)+";,";
            else
                messageLI = "LE;,"+QString::number(500)+";,"+QString::number(index)+";,"+symbol.toStr()+";,"+QString::number(x)+";,"+QString::number(y)+";,";

            numCharacters++;

            nCharTemp--;

            buffer.append(messageLI);

            if(numCharacters==500 || nCharTemp==0) {
                QString supportSize="";
                int size = buffer.size();
                qDebug()<<"BUFFER SIZE: "<<size;
                if(size<10){
                    supportSize = QString::number(size).insert(0,"0000");
                }
                else if(size<100){
                    supportSize = QString::number(size).insert(0,"000");
                }
                else if(size<1000){
                    supportSize = QString::number(size).insert(0,"00");
                }
                else if(size<10000){
                    supportSize = QString::number(size).insert(0,"0");
                }
                else {
                    supportSize = QString::number(size);
                }
                msg = "MULTI   ;"+supportSize+buffer;

                qDebug()<<"nChar:  "<<nChar;
                qDebug()<<"nCharTemp:  "<<nCharTemp;
                qDebug()<<"numCharacters:   "<<numCharacters;

                std::vector<QString> clientList  = clientMap.at(key);
                for(size_t i=0; i<clientList.size(); i++){
                    if(clientList.at(i) != username){
                        if(socketMap[clientList.at(i)]->write(msg.toUtf8()) != msg.size()){ //ci vuole il for perchÃ¨ devo mandarlo a tutti
                            socketMap[clientList.at(i)]->write("Error sending symbols");
                            socketMap[clientList.at(i)]->flush();
                         }
                        socketMap[clientList.at(i)]->flush();
                    }
                }

                numCharacters=0;
                buffer.clear();
            }
        }
        else{
            QString messageLI = "LE;,"+QString::number(index)+";,"+symbol.toStr()+";,"+QString::number(x)+";,"+QString::number(y)+";,";
            auto size = messageLI.toUtf8().size();
            QString supportSize;
            if(size<10){
                supportSize = QString::number(size).insert(0,"00");
            }
            else if(size<100){
                supportSize = QString::number(size).insert(0,"0");
            }
            else {
                supportSize = QString::number(size);
            }
            msg = "RECEIVE ;"+supportSize+messageLI;
            qDebug()<<msg;

            std::vector<QString> clientList  = clientMap.at(key);
            for(size_t i=0; i<clientList.size(); i++){
                if(clientList.at(i) != username){
                    if(socketMap[clientList.at(i)]->write(msg.toUtf8()) != msg.size()){ //ci vuole il for perchÃ¨ devo mandarlo a tutti
                        socketMap[clientList.at(i)]->write("Error sending symbols");
                        socketMap[clientList.at(i)]->flush();
                     }
                }
            }
        }
        */
    }
    QString msg = "RECEIVE ;"+supportSize+messageIS;
    qDebug()<<"message R"<<msg;
    insertQueue.push(msg);

}

void Server::changeExistingFormat()
{
    //QString data = socket->readAll();

    QString stringSize = client->read(3); //leggo le prime 3 cifre che contengono l'informazione sulla dimensione della stringa da leggere
    //qDebug()<<"stringSize"<<stringSize;
    QString data = client->read(stringSize.toInt());
    qDebug()<<"Size: "<<QString::number(stringSize.toInt())<<"Data: "<<data;
    //data.insert(0,"TCFM    ;,");
    //messageQueue.push(data);

    QStringList msg = data.split(";,");
    QString owner = msg[0];
    QString title = msg[1];
    QString index = msg[2];
    QStringList fontList = msg[7].split(",");
    QStringList posList = msg[6].split("|");
    QString username = msg[8].toUtf8();
    std::vector<int> pos_message;
    for(auto i = 0; i<posList.size(); i++){
        pos_message.push_back(posList[i].toInt());
    }
    QFont font = QFont();
    font.setFamily(fontList[0]);
    font.setPointSizeF(fontList[1].toDouble());
    font.setWeight(fontList[4].toInt());
    font.setItalic(fontList[5].toInt());
    font.setUnderline(fontList[6].toInt());
    Symbol symbol = Symbol(msg[3].toInt()-msg[4].toInt(), msg[4].toInt(), msg[5], pos_message,font);
/*
    qDebug()<<"owner "<<owner;
    qDebug()<<"title "<<title;
    qDebug()<<"index "<<index;
    qDebug()<<"symbol"<<symbol.toStr();
*/
    QString conc = owner+title;

    int i = 0;
    for(auto it = localMap.find(conc)->begin(); it!=localMap.find(conc)->end();it++){
        if(i==index.toInt()){
            //qDebug()<<"trovato";
            it->setFont(font);
        }
        i++;
    }

    QString message = owner+";,"+title+";,"+index+";,"+font.toString()+";,"+username;
    qDebug()<<message;
    auto size = message.toUtf8().size();
    QString supportSize;
    if(size<10){
        supportSize = QString::number(size).insert(0,"00");
    }
    else if(size<100){
        supportSize = QString::number(size).insert(0,"0");
    }
    else {
        supportSize = QString::number(size);
    }
    QString fontInsert = "TCFM    ;"+supportSize+message;
    formatQueue.push(fontInsert);
    QThread::msleep(5);
/*
    std::vector<QString> clientList  = clientMap.at(owner+title); //
    for(size_t i=0; i<clientList.size(); i++){
        if(clientList.at(i) != username){
            //qDebug()<<"front dentro "<<fontInsert;
            socketMap[clientList.at(i)]->write(fontInsert.toUtf8());
            QThread::msleep(2);
            //socketMap[clientList.at(i)]->flush();
        }
    }

*/

}

void Server::logout()
{
    QString data = client->readAll();
    qDebug()<<"data "<< data;
    QString username = data;
    socketMap.remove(username);
}

void Server::readMulti()
{
    //numChar = client->read(4);   //leggo le prime 4 cifre che contengono l'informazione sul numero di caratteri da leggere
    numChar = client->read(4);
    qDebug()<<numChar;
    multi=true;
    nCharTemp = numChar.toInt();
    for(int i=0; i<numChar.toInt(); i++){
        readyRead();
    }

}

void Server::dispatchMessage()
{
    while(!insertQueue.empty()){
        QString message = insertQueue.front();
        QString username = message.split(";,")[8];
        QString key = message.split(";,")[9];
        std::vector<QString> clientList  = clientMap.at(key);
        for(size_t i=0; i<clientList.size(); i++){
            if(clientList.at(i) != username){
                socketMap[clientList.at(i)]->write(message.toUtf8());
                socketMap[clientList.at(i)]->flush();
            }
        }
        message.clear();
        insertQueue.pop();
    }
    //client->flush();
}

void Server::dispatchFormat()
{
    while(!formatQueue.empty()){
        QString message = formatQueue.front();
        //qDebug()<<"message "<<message;
        QString ownerInc = message.split(";,")[0];
        QString title =  message.split(";,")[1];
        QString owner = ownerInc.split(";")[1];
        owner.remove(0,3);
        QString username = message.split(";,")[4];
        QString key = owner+title;
        std::vector<QString> clientList = clientMap.at(key);
        for(size_t i=0; i<clientList.size(); i++){
            if(clientList.at(i)!= username){
                socketMap[clientList.at(i)]->write(message.toUtf8());
                //QThread::msleep(5);
                socketMap[clientList.at(i)]->flush();
            }
        }
        message.clear();
        formatQueue.pop();
    }

}





void Server::fileOpen(QString filename, QString owner, QString username)
{
    QByteArray array;
    bool condition = false;
    QString newUserMessage;
    if(QFile(path+"/Documenti/"+owner+"/"+filename+".txt").exists() == true){
        //array.insert(0,"+OK ow  ;");
        //client->write(array);
        //array.clear();
        int siteID;
        if(localMap.contains(owner+filename)){  //vado a prendere i caratteri dalla mappa
            qDebug() <<"prima condizione";
            //qDebug()<<"idMap size: "<<idMap.size();
            //qDebug()<<"clientMap size: "<<clientMap.size();
            //vado nella mappa e vado a riprendere il mio clientID
            //auto it = std::find(idMap.at(owner+filename).begin(),idMap.at(owner+filename).end(),std::pair<QString,int>(username,));
            bool trovato = false;
            condition = true;
            //qDebug()<<"Owner + filename "<<idMap.count(owner+filename);

            for(auto pair : idMap.at(owner+filename)){
                if(pair.first==username){
                    //qDebug()<<"TROVATO";
                    siteID=pair.second; //cerco nella mappa la entry che corrisponde al mio username e prendo l'id precedentemente assegnatomi
                    trovato = true;
                }
            }
            if(!trovato) {
                //qDebug()<<"NON TROVATO";
                siteID = idMap.at(owner+filename).back().second+1;
                idMap.at(owner+filename).push_back(std::pair<QString,int>(username,siteID));
            }
            if(clientMap.count(owner+filename)!=0)
                clientMap.at(owner+filename).push_back(username);
            else {
                //qDebug()<<"clientMap count == 0";
                std::vector<QString> clientConnessi;
                clientConnessi.push_back(username);
                clientMap.insert(std::pair<QString, std::vector<QString>>(owner+filename, clientConnessi));
            }
            QString finalStr = username+";,"+filename+";,"+QString::number(siteID)+";,"+";,"+";,"+";,"+";,"+owner;
            QString supportSize;
            int size = finalStr.size();
            if(size<100){
                if(size<10){
                    supportSize = QString::number(size).insert(0,"0");
                }
                supportSize = QString::number(size).insert(0,"0");
            }
            else{
                supportSize = QString::number(size);
            }
            //qDebug()<<"size"<<supportSize;
            QString msg = "NEWUSER ;"+supportSize+finalStr;
            newUserMessage = msg;
            //qDebug()<<msg;
            //QString messageDispatch = "NEWUSER ;;,"+username+";,"+filename+";,"+QString::number(siteID)+";,"+";,"+";,"+";,"+";,"+owner;
            messageQueue.push(msg);
            messageQueue.pop();
            std::vector<Symbol> symbols = localMap.value(owner+filename);
            socketMap.insert(username,client);
            int index=0;
            foreach (Symbol s, symbols) {

                QString finalStr = owner+";,"+filename+";,"+ s.toStr()+";,"+QString::number(index);
                auto size = finalStr.size();
                QString supportSize;
                if(size<100){
                    if(size<10){
                        supportSize = QString::number(size).insert(0,"0");
                    }
                    supportSize = QString::number(size).insert(0,"0");
                }
                else{
                    supportSize = QString::number(size);
                }
                //qDebug()<<"size"<<supportSize;
                QString msg = "+OK ow  ;"+supportSize+finalStr;
                qDebug()<<msg;

                if(client->write(msg.toUtf8()) != msg.size()){
                    client->write("Error sending symbols");
                }

                QTime dieTime= QTime::currentTime().addMSecs(5);
                    while (QTime::currentTime() < dieTime)
                        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                index++;
            }
           /* QString msg_done = "+Ok Done;"+d;
            //msg_done.append(_siteID);
            qDebug()<<"Msg Done: "<<msg_done;
            if(client->write(msg_done.toUtf8()) != msg_done.size()){
                client->write("Error sending symbols");

            }
           */
        }else{
            qDebug() <<"seconda condizione"; //leggo i caratteri dal file, mi costruisco il symbol e lo mando al client
            //uguale a new file, sono il primo ad aprire il file quindi ho siteID 1
            std::vector<std::pair<QString,int>> vec;
            bool flagId = false;
            if(idMap.count(owner+filename)!=0){
                //siteID = idMap.at(owner+filename).back().second+1;
                for(auto pair : idMap.at(owner+filename)){
                    if(pair.first==username){
                        //qDebug()<<"TROVATO";
                        siteID=pair.second; //cerco nella mappa la entry che corrisponde al mio username e prendo l'id precedentemente assegnatomi
                        flagId = true;
                    }
                }
                if(flagId == false){
                    //qDebug()<<"Non Trovato";
                    siteID = idMap.at(owner+filename).back().second+1;
                    idMap.at(owner+filename).push_back(std::pair<QString,int>(username,siteID));
                }
                flagId=false;

            }
            else{
                siteID = 1;
                vec.push_back(std::pair<QString,int>(username,siteID)); //il creatore ha sempre siteid 1 per tutti i file
                idMap.insert(std::pair<QString, std::vector<std::pair<QString,int>>>(owner+filename, vec));
            }

            //qDebug()<<"siteId "<< siteID;

            QFile file(path+"Documenti/"+owner+"/"+filename+".txt");
            QFile fileFont(path+"Documenti/"+owner+"/"+filename+"_font"+".txt");
            file.open(QIODevice::ReadOnly);
            fileFont.open(QIODevice::ReadOnly);
            //QByteArray q = file.readAll();
            QString q = QString::fromUtf8(file.readAll());
            QString f = QString::fromUtf8(fileFont.readAll());
            QStringList fontCharList= f.split(";");
            std::vector<QFont> fontVector;
            std::vector<int> clientIdVector;

            qDebug()<<"File size: "<<q.size();
            qDebug()<<"FontCharList size: "<<fontCharList.size();

            for(auto i=0; i<fontCharList.size()-1; i++) {
                qDebug()<<"i: "<<i;
                QString f = fontCharList[i].split("*_*")[0];
                QStringList fontList = f.split(",");
                QString clientID = fontCharList[i].split("*_*")[1];
                QFont font = QFont();
                font.setFamily(fontList[0]);
                font.setPointSizeF(fontList[1].toDouble());
                font.setWeight(fontList[4].toInt());
                font.setItalic(fontList[5].toInt());
                font.setUnderline(fontList[6].toInt());
                fontVector.push_back(font);
                //qDebug()<<fontVector;
                clientIdVector.push_back(clientID.toInt());
                //qDebug()<<clientIdVector;
            }
            qDebug()<<"fontVector size: "<<fontVector.size();
            qDebug()<<"clientIDVector size: "<<clientIdVector.size();
            qDebug()<<"q size: "<<q.size();

            std::vector<int> pos;
            std::vector<Symbol> symbol_vector;
            for(int i=0;i<q.size();i++){
                pos.push_back(i);
                pos.push_back(clientIdVector.at(i));
                qDebug()<<"clientIdVector[i] "<<clientIdVector.at(i);
                qDebug()<<"q[i] "<<q[i];
                qDebug()<<"fontVector[i] "<<fontVector[i];
                Symbol symbol = Symbol(clientIdVector.at(i), i+1, QString (q.at(i)), pos, fontVector.at(i));
                qDebug() << "Symbol : " << symbol.getId() << " " <<symbol.getCounter() <<" " << symbol.getValue() << " " << symbol.getPos() << " " << symbol.getFont();
                symbol_vector.push_back(symbol);

                QString finalStr = owner+";,"+filename+";,"+ symbol.toStr()+";,"+QString::number(i);

                auto size = finalStr.size();
                QString supportSize;
                if(size<100){
                    if(size<10){
                        supportSize = QString::number(size).insert(0,"0");
                    }
                    supportSize = QString::number(size).insert(0,"0");
                }
                else{
                    supportSize = QString::number(size);
                }
                //qDebug()<<"size"<<supportSize;

                QString msg = "+OK ow  ;"+supportSize+finalStr;
                qDebug()<<msg;

                if(client->write(msg.toUtf8()) != msg.size()){
                    client->write("Error sending symbols");

                }
                //qDebug() << q.size() << " " << i;

               QTime dieTime= QTime::currentTime().addMSecs(5); //5
                    while (QTime::currentTime() < dieTime)
                        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);


                pos.clear();
            }

            localMap.insert(owner+filename,symbol_vector);
            socketMap.insert(username, client);
            vector<QString> clientConnessi;
            clientConnessi.push_back(username);
            clientMap.insert(pair<QString, std::vector<QString>>(owner+filename, clientConnessi));
        }
        QString final = QString::number(siteID) +";"+owner+";"+filename;
        auto size = final.size();
        QString supportSize;
        if(size<100){
            if(size<10){
                supportSize = QString::number(size).insert(0,"0");
            }
            supportSize = QString::number(size).insert(0,"0");
        }
        else{
            supportSize = QString::number(size);
        }
        //qDebug()<<"size"<<supportSize;
        QString msg_done = "+Ok Done;" + supportSize + final;
        //msg_done +=(d);
        qDebug()<<"Msg done: "<<msg_done;
        if(client->write(msg_done.toUtf8()) != msg_done.size()){
            client->write("Error sending symbols");
        }

        QTime dieTime= QTime::currentTime().addMSecs(500); //5
             while (QTime::currentTime() < dieTime)
                 QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

        if(condition){
            std::vector<QString> clientList  = clientMap.at(owner+filename); //
            for(size_t i=0; i<clientList.size(); i++){
                if(clientList.at(i) != username){
                    //qDebug()<<newUserMessage;
                    socketMap[clientList.at(i)]->write(newUserMessage.toUtf8());
                    socketMap[clientList.at(i)]->flush();
                    int siteIDtmp;
                    for(auto pairtmp : idMap.at(owner+filename)){
                        //qDebug()<<"entro nel for?"<<clientList.size()<<" "<<pairtmp.first;
                        if(pairtmp.first==clientList.at(i)){
                            //qDebug()<<"TROVATO";
                            siteIDtmp=pairtmp.second; //cerco nella mappa la entry che corrisponde al mio username e prendo l'id precedentemente assegnatomi
                            //trovato = true;
                            //qDebug()<<"iDtmp "<<siteIDtmp;
                        }
                    }
                    QString finalStrTmp = clientList.at(i)+";,"+filename+";,"+QString::number(siteIDtmp)+";,"+";,"+";,"+";,"+";,"+owner;
                    QString supportSizetmp;
                    int sizetmp = finalStrTmp.size();
                    if(sizetmp<100){
                        if(sizetmp<10){
                            supportSizetmp = QString::number(sizetmp).insert(0,"0");
                        }
                        supportSizetmp = QString::number(sizetmp).insert(0,"0");
                    }
                    else{
                        supportSizetmp = QString::number(sizetmp);
                    }
                    //qDebug()<<"size"<<supportSizetmp;
                    QString msgtmp = "NEWUSER ;"+supportSizetmp+finalStrTmp;
                    qDebug()<<msgtmp;
                    //socketMap[username]->write(msgtmp.toUtf8());
                    //socketMap[username]->flush();
                    client->write(msgtmp.toUtf8());
                    client->flush();
                    QTime dieTime= QTime::currentTime().addMSecs(500); //5
                         while (QTime::currentTime() < dieTime)
                             QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
            }
        }
    }
    else{
        client->write("Error during opening file");
    }
}

void Server::discoNoSlot()
{
    QString message = client->readAll();
    QStringList messageList = message.split(";");
    qDebug()<<"Message"<<message;
    QString username = messageList[0];
    QString owner = messageList[1];
    QString title = messageList[2];
    int flag = messageList[3].toInt();
    QString keyOwnerTitle = owner+title;

    if(localMap.find(keyOwnerTitle).value().empty()){
        qDebug()<<"prima condizione non faccio nada xd";
        auto it = std::find(clientMap.at(keyOwnerTitle).begin(),clientMap.at(keyOwnerTitle).end(),username);
        clientMap.at(keyOwnerTitle).erase(it);
        if(clientMap.at(keyOwnerTitle).empty()) {
            QFile file(path+"Documenti/"+owner+"/"+title+".txt");
            QFile fileFont(path+"Documenti/"+owner+"/"+title+"_font"+".txt");
            int counter;
            int retry = 4;
            file.open(QIODevice::ReadWrite);
            fileFont.open(QIODevice::ReadWrite);
            fileFont.resize(0);
            file.resize(0);
            file.seek(0);
            fileFont.seek(0);
            do{
                //qDebug()<<"salvo sono in do";
                //QString charBuffer;
                //QString fontBuffer;
                counter=0;
                QTextStream outChar(&file);
                QTextStream outFont(&fileFont);
                outChar.setCodec("UTF-8");
                outFont.setCodec("UTF-8");
                //int siteid;
                //per ogni carattere che inserisco devo fare la insert nella local map
                for(auto it = localMap.find(keyOwnerTitle)->cbegin(); it!= localMap.find(keyOwnerTitle)->cend(); it++){
                    int clientID = it->getId() - it->getCounter(); //prendo l'ID del simbolo
                    //qDebug()<<"clientID"<<clientID;
                    /*
                    for(auto it : idMap.at(keyOwnerTitle)){
                        if(it.first==username){
                            clientID = it.second;
                        }
                    }
                    */
                    outChar << it->getValue().toUtf8();
                    outFont << it->getFont().toString().toUtf8()+"*_*"+QString::number(clientID).toUtf8()+";";
                    counter +=it->getFont().toString().size()+QString::number(clientID).size()+2;
                    //charBuffer.append(it->getValue());
                    //fontBuffer.append(it->getFont().toString()+"_"+QString::number(clientID)+";");
                    //qDebug()<<charBuffer;
                    //qDebug()<<fontBuffer;
                }
                retry--;
            }while(localMap.find(keyOwnerTitle)->size()!=file.size() && counter!=fileFont.size() && retry!=0);
            //file.write(charBuffer.toUtf8());
            //fileFont.write(fontBuffer.toUtf8());
            file.close();
            fileFont.close();
            clientMap.erase(keyOwnerTitle);
            localMap.remove(keyOwnerTitle);
        }
    }
    else{
        auto it = std::find(clientMap.at(keyOwnerTitle).begin(),clientMap.at(keyOwnerTitle).end(),username);
        qDebug()<<"seconda";
        clientMap.at(keyOwnerTitle).erase(it);
        if(clientMap.at(keyOwnerTitle).empty()){
            //qDebug()<<"salvo";
            QFile file(path+"Documenti/"+owner+"/"+title+".txt");
            QFile fileFont(path+"Documenti/"+owner+"/"+title+"_font"+".txt");
            int counter;
            int retry = 4;
            file.open(QIODevice::ReadWrite);
            fileFont.open(QIODevice::ReadWrite);
            fileFont.resize(0);
            file.resize(0);
            file.seek(0);
            fileFont.seek(0);
            do{
                //qDebug()<<"salvo sono in do";
                //QString charBuffer;
                //QString fontBuffer;
                counter=0;
                QTextStream outChar(&file);
                QTextStream outFont(&fileFont);
                outChar.setCodec("UTF-8");
                outFont.setCodec("UTF-8");
                //int siteid;
                //per ogni carattere che inserisco devo fare la insert nella local map
                for(auto it = localMap.find(keyOwnerTitle)->cbegin(); it!= localMap.find(keyOwnerTitle)->cend(); it++){
                    int clientID = it->getId() - it->getCounter(); //prendo l'ID del simbolo
                    //qDebug()<<"clientID"<<clientID;
                    /*
                    for(auto it : idMap.at(keyOwnerTitle)){
                        if(it.first==username){
                            clientID = it.second;
                        }
                    }
                    */
                    outChar << it->getValue().toUtf8();
                    outFont << it->getFont().toString().toUtf8()+"*_*"+QString::number(clientID).toUtf8()+";";
                    counter +=it->getFont().toString().size()+QString::number(clientID).size()+2;
                    //charBuffer.append(it->getValue());
                    //fontBuffer.append(it->getFont().toString()+"_"+QString::number(clientID)+";");
                    //qDebug()<<charBuffer;
                    //qDebug()<<fontBuffer;
                }
                retry--;
            }while(localMap.find(keyOwnerTitle)->size()!=file.size() && counter!=fileFont.size() && retry!=0);
            //file.write(charBuffer.toUtf8());
            //fileFont.write(fontBuffer.toUtf8());
            file.close();
            fileFont.close();
            clientMap.erase(keyOwnerTitle);
            localMap.remove(keyOwnerTitle);
        }
    }
    int id;
    for(auto it : idMap.at(keyOwnerTitle)){
        if(it.first==username){
            id= it.second;
        }
    }
    //message.append(QString::number(id));
    if(clientMap.count(keyOwnerTitle)!=0){
        QString messageremoveTmp = username+";"+owner+";"+title+";"+QString::number(id);

        QString supportSize;
        if(messageremoveTmp.size()<100){
            if(messageremoveTmp.size()<10){
                supportSize = QString::number(messageremoveTmp.size()).insert(0,"0");
            }
            supportSize = QString::number(messageremoveTmp.size()).insert(0,"0");
        }
        else{
            supportSize = QString::number(messageremoveTmp.size());
        }
        //message.insert(0,"CLNTDISC;"+supportSize);
        QString messageremove="CLNTDISC;"+supportSize+messageremoveTmp;
        qDebug()<<"message remove "<<messageremove;
        std::vector<QString> clientList  = clientMap.at(owner+title); //
        for(size_t i=0; i<clientList.size(); i++){
            if(clientList.at(i) != username){
                socketMap[clientList.at(i)]->write(messageremove.toUtf8());
                socketMap[clientList.at(i)]->flush();
            }
        }
        messageQueue.push(messageremove);
        messageQueue.pop();
    }

    if(flag==1)
        socketMap.remove(username);

    QString close = "CLOSE   ;";
    client->write(close.toUtf8());
    client->flush();

}

void Server::disconnected()
{
    socket.removeAll(qobject_cast<QTcpSocket*>(sender()));
    qDebug() << "client disconnected: ";
}

/*
void Server::closeEditor(){
    qDebug()<<"closeditor";
    readyRead();
}
*/

//aggiustare il tempo delle sleep server
//gestione eccezioni far si che il file venga sempre salvato
