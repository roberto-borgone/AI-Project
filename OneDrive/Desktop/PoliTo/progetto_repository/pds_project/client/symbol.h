#ifndef SYMBOL_H
#define SYMBOL_H

#include <vector>
#include <QChar>
#include <QString>
#include <QFont>

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

    int getId();
    QString getValue();
    //std::string getValue();
    int getCounter();
    void setCounter(int c);
    std::vector<int> getPos();
    QFont getFont();
    void setFont(QFont f);
    QString toStr();
    std::vector<int> getResidualPos();
    void setBold(bool bold);
    void setItalic(bool italic);
    void setUnderline(bool underline);
    void setFamily(QString family);
    void setPointSize(int point);


};

#endif // SYMBOL_H
