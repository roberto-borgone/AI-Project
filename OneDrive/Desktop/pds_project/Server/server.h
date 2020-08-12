#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <map>
#include <vector>
#include <queue>
#include "symbol.h"


class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();
private:
    void find(QString username, QString fileName);
    void edit();
    void createNewFile();
    void deleteFile();
    void sendURI();
    void share();
    void insert();
    void erase();
    //void process(QString op,const QString& key, Symbol& symbol);
    void process(QString op,const QString& key, Symbol& symbol, QString username, int x, int y);//, QString clientAt);
    //void process2(QString op,const QString& key, Symbol& symbol);
    void changeFormat();
    //void changePosition();
    void readMulti();

    void fileOpen(QString filename, QString owner, QString username);
    void discoNoSlot();
    int _siteID;
    void changeExistingFormat();
    void logout();


signals:

public slots:
    void newConnection();
    void readyRead();
    void disconnected();
    void dispatchMessage();
    void dispatchFormat();
    //void save(QString key);
    //void closeEditor();

private:
    QTcpServer *server;
    QList<QTcpSocket*> socket;
    QTcpSocket *client;
    QString path;
    QString numChar; //numero caratteri passati
    bool multi=false; //ho chiamato il multi?
    std::queue<QString> messageQueue;
    std::queue<QString> insertQueue;
    std::queue<QString> formatQueue;
    QMap<QString,std::vector<Symbol>> localMap;
    std::map<QString,std::vector<QString>> clientMap; //quando si apre il file si popola la mappa con la chiave uri e il valore username
    std::map<QString, std::vector<std::pair<QString,int>>> idMap;
    QMap<QString,QTcpSocket*> socketMap;
    bool deletef = false;
    std::vector<Symbol> currentSymbols;
    int numCharacters=0;  
    int nCharTemp;
    std::queue<QByteArray> clientRequestProto;

    QString buffer="";
    QTimer *timer;
    int c =0;
};

#endif // SERVER_H
