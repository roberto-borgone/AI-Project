#include <QtWidgets>
#include "mytcpsocket.h"
#include <QtCore>
#include <QMessageBox>

//QTcpSocket * MyTcpSocket::socket=nullptr;

MyTcpSocket::MyTcpSocket(QObject *parent) :
    QObject(parent)
{
    if(socket == nullptr){
        socket = new QTcpSocket(this);
        connect(socket, SIGNAL(connected()),this, SLOT(connected()));
        connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
        connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
        connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

        qDebug() << "connecting...";
        bool flag=true;
        do{
         // this is not blocking call
         socket->connectToHost("127.0.0.1", 9950); //192.168.1.72

         // we need to wait...
         if(!socket->waitForConnected(5000))
         {
             qDebug() << "Error: " << socket->errorString();
             int ret = QMessageBox::critical(nullptr,"no connection","wanna retry?",QMessageBox::Yes,QMessageBox::No);
             //int ret = msgBox.exec();

             switch (ret) {
               case QMessageBox::No:
                 throw std::invalid_argument("Server not available");
               case QMessageBox::Yes:
                 continue;
             }
         }
         else flag=false;
         }while(flag==true);
             m = new MainWindow(nullptr);
             m->show();
          }
     }


MyTcpSocket::~MyTcpSocket(){

    socket->close();
    qDebug() << "distruttore client disconnected...";
}

/*QTcpSocket* MyTcpSocket::getSocket(){
    if(socket != nullptr){
        return socket;
    }
}*/

void MyTcpSocket::connected()
{
    qDebug() << "connected...";

    // Hey server, tell me about you.
    //socket->write("HEAD / HTTP/1.0\r\n\r\n\r\n\r\n");
}

void MyTcpSocket::disconnected()
{
    //w->textedit->disconnect();
    qDebug() << "client disconnected...";
}

void MyTcpSocket::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}


void MyTcpSocket::readyRead()
{
    counter = 0;
    counter2 = 0;
    while(socket->bytesAvailable()){
        qDebug() << "reading...";
        QByteArray buffer;
        QStringList list;
        QStringList read;
        QStringList read1;
        QStringList read2;
        QString data;
        data = QString(socket->read(9));
        list = data.split(";");
        if(list[0] == "-ERR    " ) {
            msgBox.setText("Error during registration.");
            msgBox.exec();
        }
        if(list[0] == "-ERR -u ") {
            msgBox.setText("Username already exists.");
            msgBox.exec();
        }
        if(list[0] == "-ERR -up") {
            msgBox.setText("Username or password is not correct.");
            msgBox.exec();
        }
        if(list[0]=="-ERR -a "){
            msgBox.setText("User already connected.");
            msgBox.exec();
        }
        if(list[0] == "+OK +r  "){
            msgBox.setText("Registration successfull.");
            msgBox.exec();
        }
        if(list[0] == "+OK +l  ") {
            msgBox.setText("Login correct.");
            msgBox.exec();
        }
        if(list[0] == "-ERR -db" ) {
            msgBox.setText("Error contacting the server");
            msgBox.exec();
        }
        if(list[0] == "-FNF    " ) {
            w->setFilesFoundLabel("file(s) not found");
            if(w->deletef == false){
                msgBox.setText("File(s) not found!");
                msgBox.exec();
            }
            w->deletef = false;
        }
        if((list[0] == "+OK ff  ") && (w->isVisible())){
            data=socket->readAll();
            qDebug()<<"+Ok ff data: "<<data;
            list = data.split(";");
            int numero = list[0].toInt();
            //qDebug() << "Numero file: " << numero;
            buffer.clear();

            for(int counter=1;counter<(numero*3)+1;counter+=3){
                 QString nameFile = list[counter];
                 QString status = list[counter+1];
                 QString size = list[counter+2];
                 read.append(nameFile);
                 read1.append(status);
                 read2.append(size);
            }
            //qDebug() <<read;
            //qDebug() <<read1;
            //qDebug() <<read2;

            w->setFilesTableNull();
            w->showFiles(read,read1,read2);
            if(w->deletef == false){
                msgBox.setText("File(s) "+list[0].toUtf8()+" found!");
                msgBox.exec();
            }
            w->deletef = false;
        }

        if(list[0] == "+OK ow  "){
            //QString data = socket->readAll();
            QString stringSize = socket->read(3); //leggo le prime 3 cifre che contengono l'informazione sulla dimensione della stringa da leggere
            //qDebug()<<"stringSize"<<stringSize;
            QString data = socket->read(stringSize.toInt());
            socket->flush();
            qDebug()<<"+Ok ow data: "<<stringSize<<" "<<data;
            QStringList msg = data.split(";,");
            QString username = msg[0];
            //qDebug()<<"user"<<username;
            QString title = msg[1];
            //qDebug()<<"title"<<title;
            int id = msg[2].toInt();
            //qDebug()<<"id"<< id;
            int counter = msg[3].toInt();
            //qDebug()<<"counter"<<counter;
            QString charValue = msg[4];
            //qDebug()<<"char "<< charValue;
            QStringList posList = msg[5].split("|");
            //qDebug()<<"pos"<<posList;
            std::vector<int> pos;
            for (auto i = 0; i<posList.size(); i++){
                //qDebug()<<"pos for";
                pos.push_back(posList[i].toInt());
            }
            //qDebug()<<"pos"<<posList;
            QStringList fontList = msg[6].split(",");
            QFont font = QFont();
            font.setFamily(fontList[0]);
            font.setPointSizeF(fontList[1].toDouble());
            font.setWeight(fontList[4].toInt());
            font.setItalic(fontList[5].toInt());
            font.setUnderline(fontList[6].toInt());
            int index=msg[7].toInt();
            Symbol global_symbol = Symbol(id-counter, counter, charValue, pos, font);
            //qDebug()<<"Symbol "<<global_symbol.getId()<<" "<<global_symbol.getCounter()<<" "<<global_symbol.getValue()<<" "<<global_symbol.getPos()<<" "<<global_symbol.getFont();

            if(w->textedit == nullptr){
                 qDebug() << "sono nullo";
            }
            //qDebug() << "getsymbol "<< w->textedit->getSymbol().size();

            w->textedit->setSymbol(index,global_symbol);
            buffer_caratteri.append(global_symbol.getValue().toUtf8()+";,");
            int siteID = id-counter;
            buffer_font.append(global_symbol.getFont().toString().toUtf8()+","+QString::number(siteID)+";");
            //qDebug()<<global_symbol.getFont().toString()+","+QString::number(siteID);
            //qDebug() << buffer_caratteri.size();
            //buffer.clear();
            //buffer=socket->readAll();
            //qDebug()<<"la grandezza del file ÃƒÂ¨: "<<buffer.size();
            //int buffer_size = buffer.size();
            }
            if(list[0] == "+Ok Done"){
                data.clear();
                //data = QString(socket->readAll());
                QString stringSize = QString(socket->read(3));
                //qDebug()<<"stringSize"<<stringSize;
                QString data = socket->read(stringSize.toInt());
                qDebug()<<"Ok Done data: "<<data;

                QStringList msg = data.split(";");
                int siteID = msg[0].toInt();
                QString owner = msg[1];
                QString filename = msg[2];
                //qDebug()<<"Site ID "<<siteID;
                const QRect availableGeometry = QApplication::desktop()->availableGeometry(w->textedit);
                w->textedit->resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
                w->textedit->move((availableGeometry.width() - w->textedit->width()) / 2,
                (availableGeometry.height() - w->textedit->height()) / 2);
                w->hide();
                w->textedit->setWindowTitle(filename);
                w->textedit->setOwner(owner);
                w->textedit->setSiteID(siteID);
                w->textedit->addUserLabel(MainWindow::username);
                w->textedit->load(buffer_caratteri, buffer_font);
                //w->textedit->setLowerToolbar(buffer_caratteri);
                w->textedit->show();
                buffer_caratteri.clear();
                buffer_font.clear();
                //qDebug()<<"Sono dentro l'ok done";
            }

        if(list[0] == "EDIT    "){
           QString result;
           data = QString(socket->read(17));
           if(data.size()!=17) {
                msgBox.setText("Communication error");
           }
           else {
               QStringList msg = data.split(";");
               qDebug() <<"EDIT msg: "<< msg;
               if(msg[0]+msg[1]+msg[2]+msg[3]+msg[4]+msg[5] == "+0+0+0+0+0+0") result = "Nothing to change";

               if(msg[0] == "-1")
                   result += "Impossible to change first name\n";
               else if(msg[0] == "+1") {
                   result += "First name successfully changed\n";
               }

               if(msg[1] == "-1")
                   result += "Impossible to change last name\n";
               else if(msg[1] == "+1")
                   result += "Last name successfully changed\n";

               if(msg[2] == "-1")
                   result += "Impossible to change nickname\n";
               else if(msg[2] == "+1")
                   result += "Nickname successfully changed\n";

               if(msg[3] == "-1")
                   result += "Impossible to change password\n";
               else if(msg[3] == "-2")
                   result += "Old password is wrong\n";
               else if(msg[3] == "-3")
                   result += "New password is not valid\n";
               else if(msg[3] == "-4")
                   result += "New password and confirmed password must be equal\n";
               else if(msg[3] == "-5")
                   result += "Fill all fields to change password\n";
               else if(msg[3] == "+1")
                   result += "Password successfully changed\n";

               if(msg[4] == "-1")
                   result += "Impossible to change gender\n";
               else if(msg[4] == "+1")
                   result += "Gender successfully changed\n";

               if(msg[5] == "-1")
                   result += "Impossible to change date of birth\n";
               else if(msg[5] == "-2")
                   result += "You are too young. You must be at least 14 years old\n";
               else if(msg[5] == "-3")
                   result += "Date is not valid\n";
               else if(msg[5] == "+1")
                   result += "Date of birth successfully changed\n";
               msgBox.setText(result);
               msgBox.exec();
           }
        }
        if(list[0] == "NEWFILE "){
            data = QString(socket->read(2));
            QString title = "";

            if(data.size()!=2) {
                 msgBox.setText("Communication error");
                  msgBox.exec();
            }
            else {

                if(data == "-1"){
                    msgBox.setText("Impossible access to DB\n");
                     msgBox.exec();
                }
                else if(data == "-2") {
                    msgBox.setText("Title already exist\n");
                     msgBox.exec();
                }
                else if(data == "-3") {
                    msgBox.setText("Title can not contains ';' character\n");
                     msgBox.exec();
                }
                else if(data == "+1"){
                    data = QString(socket->read(33)); //33
                    qDebug()<<"NEW FILE data: "<<data;
                    QStringList msg = data.split(";");
                    QString title = msg[1];
                    int _siteID = msg[2].toInt();
                    w->openFile(title,MainWindow::username, _siteID);
                }
            }
        }
        if(list[0] == "-FNFd   " ) {
            w->setFilesTableNull();
            data = socket->readAll();
            qDebug()<<"-FNFd data: "<<data;
            list = data.split(";");
            QString title = list[list.size()-2];
            msgBox.setText("File "+title+" deleted\n");
            msgBox.exec();
        }
        if((list[0] == "+OK ffd ") && (w->isVisible())){
            data=socket->readAll();
            list = data.split(";");
            int numero = list[0].toInt();
            qDebug() << "+Ok ffd data: " << data;
            buffer.clear();

            for(int counter=1;counter<(numero*3)+1;counter+=3){
                 QString nameFile = list[counter];
                 QString status = list[counter+1];
                 QString size = list[counter+2];
                 read.append(nameFile);
                 read1.append(status);
                 read2.append(size);
            }
            //qDebug() <<read;
            //qDebug() <<read1;
            //qDebug() <<read2;

            w->setFilesTableNull();
            w->showFiles(read,read1,read2);
            QString title = list[list.size()-2];
            msgBox.setText("File "+title+" deleted\n");
            msgBox.exec();
        }
        if(list[0] == "DELETEFI"){
                data = QString(socket->read(2));
                QString title = "";

                if(data.size()!=2) {
                     msgBox.setText("Communication error");
                      msgBox.exec();
                }
                else {

                    if(data == "-1"){
                        msgBox.setText("Impossible access to DB\n");
                         msgBox.exec();
                    }
                    else if(data == "-2") {
                        msgBox.setText("File not found\n");
                         msgBox.exec();
                    }
                    else if(data == "-3") {
                        msgBox.setText("Title can not contains ';' character\n");
                         msgBox.exec();
                    }
                    else if(data == "-4"){
                        msgBox.setText("You are not authorize to delete this file!\n");
                        msgBox.exec();
                    }
                    /*else if(data == "+1"){
                        data = QString(socket->read(33));
                        QStringList msg = data.split(";");
                        QString title = msg[1];
                        msgBox.setText("file "+title+" deleted\n");
                        msgBox.exec();
                    }*/
                }
        }
        if(list[0] == "SENDURI "){
            data = QString(socket->readAll());
            qDebug()<<"SENDURI data: "<<data;

            if(data.size()<2) {
                msgBox.setText("Communication error\n");
                msgBox.exec();
            }
            else if(data == "-1") {
                msgBox.setText("Impossible access to DB\n");
                msgBox.exec();
            }
            else if(data.size() == 32) {
                // MOSTRARE LA URI NELLA LINEDIT DEL TEXTEDITOR
                w->textedit->showUri(data);
            }
        }
        if(list[0] == "SHARE   "){
            data = QString(socket->read(2)); //2
            qDebug()<<"SHARE data: "<<data;

            if(data.size()<2) {
                msgBox.setText("Communication error\n");
                msgBox.exec();
            }
            else if(data == "-1"){
                msgBox.setText("Impossible access to DB\n");
                msgBox.exec();
            }
            else if(data == "-2"){
                msgBox.setText("URI not valid\n");
                msgBox.exec();
            }
            else if(data == "-3"){
                msgBox.setText("File is already in your directory\n");
                msgBox.exec();
            }
            else if(data == "+1") {
                data = QString(socket->read(2));
                QString siteID = data.split(";")[1];
                //qDebug()<<"SONO NELLA SHARE IL SITE ID E':   "<<siteID;
                w->textedit = new TextEdit(w,w);
                w->textedit->setSiteID(siteID.toInt());
                this->readyRead();
              /*data = QString(socket->readAll());
                QStringList msg = data.split(";");
                //t->setSiteID(msg[1].toInt());
                //da testare
                QString title = msg[0];
                QString owner = msg[1];
                int _siteID = msg[2].toInt();
                w->openFile(title, owner, _siteID);
                //w->textedit->setOwner();
                //msgBox.setText("URI is valid, file is now in your directory\n");
                */
            }
        }
        if(list[0] == "REMOVE  "){
            buffer = socket->readAll();
            qDebug()<<"REMOVE data: "<<data;

            if(buffer == "-1"){
                msgBox.setText("No image to remove!\n");
                msgBox.exec();
            }
            else{
                //qDebug() <<buffer.size();
                w->showImage(buffer);
            }
        }
        if(list[0] == "EPHOTO  "){
            //w->hide();
            //w->ed->show();
            buffer = socket->readAll();
            qDebug()<<"EPHOTO";
            qDebug()<<"STAMPA UNO: " + QString::number(buffer.size());
            qDebug()<<buffer;

            QString word = "INFOUTENTE";

            int index = buffer.indexOf("INFOUTENTE");           //cerco nel buffer la pos iniziale della parola "infoutente"

            qDebug()<<"Dopo index";

            QByteArray image = buffer.left(index-1);            //l'immagine sarÃƒ  a sx di "infoutente"

            QString info = buffer.mid(index);                   //il resto ÃƒÂ¨ relativo alle info dell'utente
            w->ed->showImage(image);
            w->ed->showInfo(info);
        }
        if(list[0] == "DOWNLOAD"){
            QByteArray bufferphoto;
            buffer.clear();
            buffer = socket->read(2);
            qDebug()<<"DOWNLOAD";

            if(buffer == "-1") {
                msgBox.setText("No image to download!\n");
                msgBox.exec();
            }
            else if(buffer == "+1") {
                QString imagePath=QFileDialog::getSaveFileName(m,tr("Save File"),"",tr("JPEG (*.jpg *.jpeg);;PNG (*.png)"));
                if(imagePath!=""){
                    //qDebug() << imagePath;
                    QFile target;
                    target.setFileName(imagePath);

                    if(!target.open(QIODevice::WriteOnly)){
                        qDebug() << "Can't open file for written";
                        return;
                    }
                    buffer.clear();
                    //qDebug() << target;
                    //while(socket->waitForReadyRead()){
                        bufferphoto = socket->readAll();
                        target.write(bufferphoto);
                     //   bufferphoto.clear();
                    //}
                    //qDebug() << "file size: " << target.size();
                    //qDebug() << "Finished!" ;
                    target.close();
                }
            }
        }
        if(list[0] == "+OK +l  "){
            m->hide();
            w = new Window(nullptr, m);
            w ->show();
            buffer = socket->readAll();
            qDebug()<<"+Ok +l";
            //qDebug() <<buffer;
            w->showImage(buffer);
            qDebug() << "file size: " << buffer.size();
            qDebug() << "Finished!" ;

        }
        if(list[0]=="MULTI   "){
            QString size = socket->read(5);
            qDebug()<<"CARATTERI DA LEGGERE: "<<size.toInt();
            data = socket->read(size.toInt());
            qDebug()<<"CARATTERI LETTI: "<<data.size();

            QStringList msg = data.split(";,");
            int nChar = msg[1].toInt();
            int n = nChar;
            qDebug()<<"nChar"<<nChar;
            for(int i=1;i<(nChar*10)+1;i+=10){
                QString op=msg[i-1].toUtf8();
                int index, id, counter, x, y;
                if(op=="LI" || op=="MULTI   ;LI"){
                    //qDebug()<< "LI";
                    index=msg[i+1].toInt();
                    id =msg[i+2].toInt();
                    counter=msg[i+3].toInt();
                    QString value = msg[i+4].toUtf8();
                    QStringList posList = msg[i+5].split("|");
                    //qDebug()<<"pos"<<posList;
                    std::vector<int> pos;
                    for (auto j = 0; j<posList.size(); j++){
                        pos.push_back(posList[j].toInt());
                    }
                    //qDebug()<<"pos: "<<pos;
                    QStringList fontList = msg[i+6].split(",");
                    //QFont font = QFont(fontList[0],fontList[1].toInt(),fontList[4].toInt(),fontList[5].toInt());
                    QFont font = QFont();
                    font.setFamily(fontList[0]);
                    font.setPointSizeF(fontList[1].toDouble());
                    font.setWeight(fontList[4].toInt());
                    font.setItalic(fontList[5].toInt());
                    font.setUnderline(fontList[6].toInt());

                    x = msg[i+7].toInt();
                    y = msg[i+8].toInt();

                    //qDebug()<<x;
                    //qDebug()<<y;
                    //qDebug()<<"index "<<index<<"value "<<value;

                    Symbol global_symbol = Symbol(id-counter, counter, value, pos, font);
                    //qDebug()<<"Symbol "<<global_symbol.getId()<<" "<<global_symbol.getCounter()<<" "<<global_symbol.getValue()<<" "<<global_symbol.getPos()<<" "<<global_symbol.getFont();

                    if(w->textedit == nullptr){
                         qDebug() << "sono nullo";
                    }

                    w->textedit->insertCharacter(index,global_symbol, x, y, n);
                    //if(n>1) n--;
                }
                else{
                    //qDebug()<< "LE";
                    index = msg[i+1].toInt();
                    id =msg[i+2].toInt();
                    counter=msg[i+3].toInt();
                    x = msg[i+7].toInt();
                    y = msg[i+8].toInt();
                    //qDebug()<<"x e y:"<<x<<y;
                    w->textedit->removeSymbol(index,id-counter, x, y, n);
                    //if(n>1) n--;
                }
            }
            emit readyRead();
        }
        if(list[0]=="RECEIVE "){
            QString stringSize = socket->read(5);
            data = socket->read(stringSize.toUtf8().toInt());
            qDebug()<<"data: "<<data;
            /*int n = 1;
            QStringList msg = data.split(";,");
            if(msg[0]=="LI"){
                int id, counter, x, y; //index,
                //index = msg[1].toInt();
                id = msg[1].toInt();
                counter = msg[2].toInt();
                QString value = msg[3].toUtf8();
                QStringList posList = msg[4].split("|");
                //qDebug()<<"pos"<<posList;
                std::vector<int> pos;
                for (auto j = 0; j<posList.size(); j++)
                    pos.push_back(posList[j].toInt());
                QStringList fontList = msg[5].split(",");
                QFont font = QFont();
                font.setFamily(fontList[0]);
                font.setPointSizeF(fontList[1].toDouble());
                font.setWeight(fontList[4].toInt());
                font.setItalic(fontList[5].toInt());
                font.setUnderline(fontList[6].toInt());
                x = msg[6].toInt();
                y = msg[7].toInt();
                //qDebug()<<x;
                //qDebug()<<y;
                //qDebug()<<"index "<<index<<"value "<<value;
                Symbol global_symbol = Symbol(id-counter, counter, value, pos, font);
                //qDebug()<<"Symbol "<<global_symbol.getId()<<" "<<global_symbol.getCounter()<<" "<<global_symbol.getValue()<<" "<<global_symbol.getPos()<<" "<<global_symbol.getFont();

                if(w->textedit == nullptr){
                     qDebug() << "sono nullo";
                }
                //w->textedit->insertCharacter(index,global_symbol, x, y, n);
            }
            else{
                int id, counter, x, y; //index,
                //index = msg[1].toInt();
                id = msg[1].toInt();
                counter = msg[2].toInt();
                QString value = msg[3].toUtf8();
                QStringList posList = msg[4].split("|");
                //qDebug()<<"pos"<<posList;
                std::vector<int> pos;
                for (auto j = 0; j<posList.size(); j++)
                    pos.push_back(posList[j].toInt());
                QStringList fontList = msg[5].split(",");
                QFont font = QFont();
                font.setFamily(fontList[0]);
                font.setPointSizeF(fontList[1].toDouble());
                font.setWeight(fontList[4].toInt());
                font.setItalic(fontList[5].toInt());
                font.setUnderline(fontList[6].toInt());
                x = msg[6].toInt();
                y = msg[7].toInt();
                //w->textedit->removeSymbol(index,id-counter, x, y, n);
            }*/
            if(w->textedit == nullptr){
                 qDebug() << "sono nullo";
            }
            w->textedit->process(data);
            counter++;
            if(counter == 500){
                counter = 0;
                QTime dieTime= QTime::currentTime().addMSecs(5);
                    while (QTime::currentTime() < dieTime)
                        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            }
            //emit readyRead();
        }
        if(list[0]=="TCFM    "){
            //data = socket->readAll();
            QString stringSize = socket->read(3);
            data = socket->read(stringSize.toInt());
            qDebug()<<"TCFM data: "<<data;
            /*
            QStringList msg = data.split(";,");
            QString owner = msg[0];
            QString title = msg[1];
            QString index = msg[2];
            QStringList fontList = msg[3].split(",");
            QFont font = QFont();
            font.setFamily(fontList[0]);
            font.setPointSizeF(fontList[1].toDouble());
            font.setWeight(fontList[4].toInt());
            font.setItalic(fontList[5].toInt());
            font.setUnderline(fontList[6].toInt());
            //qDebug()<<"font"<<font;
            w->textedit->setSymbolFont(index.toInt(),font);
            */

            w->textedit->setSymbolFontData(data);
            counter2++;
            if(counter2 == 500){
                counter2 = 0;
                QTime dieTime= QTime::currentTime().addMSecs(5);
                    while (QTime::currentTime() < dieTime)
                        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            }

        }
        if(list[0]=="NEWUSER "){
            QString stringSize = socket->read(3);
            data = socket->read(stringSize.toInt());
            qDebug()<<"NEWUSER data: "<<data;
            QStringList msg = data.split(";,");
            QString username = msg[0];
            //QString filename = msg[1];
            int siteID = msg[2].toInt();
            w->textedit->addUser(username,siteID);
        }
         if(list[0]=="CHANGE  "){
             qDebug()<<"CHANGE";
             QString stringSize = socket->read(3);
             data = socket->read(stringSize.toInt());
             QStringList msg = data.split(";,");
             int siteID = msg[2].toInt();
             int width = msg[3].toInt();
             int height = msg[4].toInt();
             w->textedit->updateLabel(siteID,width,height);
         }
         if(list[0]=="CLNTDISC"){
             QString stringSize = socket->read(3);
             data = socket->read(stringSize.toInt());
             qDebug()<<"CLIENTDISC data: "<<data;
             QStringList dataList = data.split(";");
             QString username = dataList[0];
             int id = dataList[3].toInt();
             w->textedit->removeLabel(username,id);
         }
         if(list[0]=="CLOSE   ") {
             qDebug()<<"CLOSE";
             w->textedit->flag = false;
             w->textedit->close();
             w->textedit->flag = true;
         }
    }
}
