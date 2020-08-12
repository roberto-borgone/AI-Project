#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QMainWindow>
#include <QMap>
#include <QPointer>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <random>
#include <QTextEdit>
#include <QListWidget>
#include <ctime>
#include "symbol.h"


QT_BEGIN_NAMESPACE
class QAction;
class QComboBox;
class QFontComboBox;
class QTextEdit;
class QTextCharFormat;
class QMenu;
class QPrinter;
QT_END_NAMESPACE

class Window;
class MainWindow;

class TextEdit : public QMainWindow
{
    Q_OBJECT

public:
    explicit TextEdit(QWidget *parent, Window *h);
    ~TextEdit() override;
    bool load(const QByteArray arrayByte, const QByteArray arrayFont);
    void showUri(QString str);
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void printPos();
    void addUserLabel(QString username);
    void setSiteID(int siteID);
    void setForm(QTextCharFormat f);
    //void setAl(QString alignment);
    void setSymbol(int index, Symbol s);
    void removeSymbol(int index, int id, int x, int y, int nChar);
    void setOwner(QString owner);
    QString getOwner();
    void setSymbolFont(int index, QFont f);
    void setSymbolFontData(const QString& data);
    void disconnect(int flag);
    int getSiteID();
    void insertCharacter(int index, Symbol global_symbol, int x, int y, int numChar);
    std::vector<int> localInsert(unsigned int index);
    std::vector<int> localRedo(unsigned int index, int id);
    std::vector<Symbol> getSymbol();
    static QVector<QColor> highlight(const QColor &bg, const QColor &fg, int noColors);
    QVector<QColor> colors;
    void addUser(QString& username, int id);
    void updateLabel(int id, int width, int height);
    void removeLabel(QString& username, int id);
    QString supportSize(int size);
    bool flag;

public slots:
    //void fileNew();
    //bool fileSaveAs();
    void on_homepageButton_clicked();
    void myPaste();
    void myCut();
    void myRedo();
    void myUndo();
    void blink();

    //void textChanged();
    //void process(QString message); //passaggio per riferimento? perchè modifichiamo il valore di message facendo lo split
    void process(const QString& message);

protected:
    void closeEvent(QCloseEvent *e) override;
private slots:
    //void fileOpen();
    //bool fileSave();
    //void filePrint();
    //void filePrintPreview();
    void filePrintPdf();

    void textBold();
    void textUnderline();
    void textItalic();
    void textFamily(const QString &f);
    void textSize(const QString &p);
    //void textStyle(int styleIndex);
    //void textColor();
    //void textAlign(QAction *a);

    void currentCharFormatChanged(const QTextCharFormat &format);
    void cursorPositionChanged();

    void clipboardDataChanged();
    void about();
    //void printPreview(QPrinter *);

    void on_share_clicked();
    void on_copyUri_clicked();


private:
    void setupFileActions();
    void setupEditActions();
    void setupTextActions();
    bool maybeSave();
    void setCurrentFileName(const QString &fileName);

    //std::vector<int> localInsert(unsigned int index);
    void localErase(int index);


    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void fontChanged(const QFont &f);
    //void colorChanged(const QColor &c);
    //void alignmentChanged(Qt::Alignment a);

    //QAction *actionSave;
    QAction *actionTextBold;
    QAction *actionTextUnderline;
    QAction *actionTextItalic;
    //QAction *actionTextColor;
    /*
    QAction *actionAlignLeft;
    QAction *actionAlignCenter;
    QAction *actionAlignRight;
    QAction *actionAlignJustify;
    */
    QAction *actionUndo;
    QAction *actionRedo;
#ifndef QT_NO_CLIPBOARD
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
#endif

    QComboBox *comboStyle;
    QFontComboBox *comboFont;
    QComboBox *comboSize;

    QToolBar *tb;
    QString fileName;

    QLabel *po;
    QLabel *uri;
    QPushButton *share;
    QPushButton *homepage;
    QPushButton *copyUri;

    // QLabel *userActive; // numero utenti
    //QLabel *userLabel; //"users connected"
    //int userNumber;
    std::vector<QLabel*> usersVector;

    //riga, colonna, numero caratteri, numero parole, zoom
    QLabel *row;
    QLabel *numRow;
    int r;
    QLabel *column;
    QLabel *numColumn;
    int c;
    QLabel *words;
    QLabel *numWords;
    int w;
    QLabel *characters;
    QLabel *numCharacters;
    int numC;

    Window *h;
    QTextEdit *textEdit;

    int counter;
    int _siteId;
    int combination;
    std::vector<Symbol> symbols;

    bool wait = false;

    QToolBar *ut;
    QString owner;

    QVector<QPair<QString,int>> userConnessi;
    QVector<QPair<QLabel*,int>> labelVector;

    QTimer* timer;
    QSize labelFixedSize;

    QVector<int> undoString;
    QVector<int> insertString;
    QListWidget  *userList;
    QDockWidget *dock;
    QStringList user;

};

#endif // TEXTEDIT_H
