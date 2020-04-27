#ifndef SYMBOL_H
#define SYMBOL_H

#include <vector>
#include <QChar>
#include <QString>
#include <QFont>
#include <QDebug>

class Symbol
{
    int id;
    //std::string value;
    QString c;
    int counter;
    std::vector<int> pos;
    QFont font;


public:
    Symbol() = default;
    //Symbol(int id_client, int counter, std::string value, std::vector<int> pos);
    Symbol(int id_client, int counter, QString c, std::vector<int> pos, QFont font);
    //Symbol(int id_client, int counter, QString c, std::vector<int> pos);

    int getId() const;
    QString getValue() const;
    //std::string getValue();
    int getCounter() const;
    void setCounter(int c);
    std::vector<int> getPos() const;
    QFont getFont() const;
    void setFont(QFont f);
    //void setFont(QString family, int pointSize, int weight, bool underline, bool italic);
    QString toStr();
    std::vector<int> getResidualPos();

    bool operator== (Symbol& sym);


};

#endif // SYMBOL_H
