#include "symbol.h"

//Symbol::Symbol(int id_client, int counter, std::string value, std::vector<int> pos) : value(value), pos(pos)
Symbol::Symbol(int id_client, int counter, QString c, std::vector<int> pos, QFont font) :c(c), pos(pos), font(font)
//Symbol::Symbol(int id_client, int counter, QString c, std::vector<int> pos) :c(c), pos(pos)
{
    this->id = id_client+counter;
    this->counter = counter;
    //id_client è relativo al symbol
}

int Symbol::getId() const
{
    return this->id;
}

QString Symbol::getValue() const
{
    return this->c;
}
/*
std::string Symbol::getValue()
{
    return this->value;
}
*/
int Symbol::getCounter() const
{
    return this->counter;
}

void Symbol::setCounter(int c)
{
    this->counter = c;
}

std::vector<int> Symbol::getPos() const
{
    return this->pos;
}

QFont Symbol::getFont() const
{

    return this->font;
}

void Symbol::setFont(QFont f) {

    //this->font = f;

    this->font.setFamily(f.family());
    this->font.setPointSize(f.pointSize());
    this->font.setItalic(f.italic());
    this->font.setUnderline(f.underline());
    this->font.setWeight(f.weight());


}


/*
void Symbol::setFont(QString family, int pointSize, int weight, bool underline, bool italic)
{
    this->font.setFamily(family);
    this->font.setPointSize(pointSize);
    this->font.setItalic(italic);
    this->font.setUnderline(underline);
    this->font.setWeight(weight);

}
*/

QString Symbol::toStr()
{
    QString str = QString::number(this->getId())+";,";
    str = str + QString::number(this->getCounter())+";,";
    str = str + c + ";,";
    for(auto it =0; it< this->getPos().size();it++){
        if(it == this->getPos().size()-1){
        str = str + QString::number(this->getPos().at(it));
        }
        else{
            str = str + QString::number(this->getPos().at(it))+"|";
        }
    }
    str = str + ";," + this->font.toString();
    return str;
}

std::vector<int> Symbol::getResidualPos()
{
    std::vector<int> residual = this->getPos();
    //qDebug()<<"pos size"<<this->getPos().size();
    //qDebug()<<"res1"<<residual.size();
    residual.pop_back();
    //qDebug()<<"res2"<<residual.size();
    return residual;
}

bool Symbol::operator==(Symbol& sym)
{
    if(this->c == sym.getId() && this->counter == sym.getCounter()) return true;
    else return false;
}

