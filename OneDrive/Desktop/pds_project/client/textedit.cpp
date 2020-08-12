#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QMenu>
#include <QMenuBar>
#include <QTextCodec>
#include <QTextEdit>
#include <QStatusBar>
#include <QToolBar>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QtDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QMimeData>
#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QPainter>
#include <QTimer>
#include <QDockWidget>
#include <QThread>

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printer)
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif
#include <QPrinter>
#if QT_CONFIG(printpreviewdialog)
#include <QPrintPreviewDialog>
#endif
#endif
#endif

#include "textedit.h"
#include "mainwindow.h"
#include "mytcpsocket.h"

#ifdef Q_OS_MAC
const QString rsrcPath = ":/images/mac";
#else
const QString rsrcPath = ":/images/win";
#endif


TextEdit::TextEdit(QWidget *parent, Window *h)
    : QMainWindow(parent), h(h)
{

#ifdef Q_OS_OSX
    setUnifiedTitleAndToolBarOnMac(true);
#endif
    setWindowTitle(QCoreApplication::applicationName());
    this->counter = 0;
    combination = 0;
    this->flag = true;

    textEdit = new QTextEdit(this);
    this->grabKeyboard();

    this->setAttribute(Qt::WA_DeleteOnClose);
    //this è *TetxEdit puntatore alla classe TextEdit
    //textEdit invece è un puntatore a QTextEdit

    connect(textEdit, &QTextEdit::currentCharFormatChanged, this, &TextEdit::currentCharFormatChanged);
    connect(textEdit, &QTextEdit::cursorPositionChanged, this, &TextEdit::cursorPositionChanged);


    //connect(textEdit->document(),&QTextDocument::modificationChanged,this,&TextEdit::textChanged);

    ////  connect(textEdit->document(),&QTextDocument::contentsChange,this,&TextEdit::textChanged);

    setCentralWidget(textEdit);

    setToolButtonStyle(Qt::ToolButtonFollowStyle);

    setupFileActions();
    setupEditActions();
    setupTextActions();

    {
        QMenu *helpMenu = menuBar()->addMenu(tr("Help"));
        helpMenu->addAction(tr("About"), this, &TextEdit::about);
    }

    QFont textFont("Helvetica");
    textFont.setStyleHint(QFont::SansSerif);
    textEdit->setFont(textFont);
    fontChanged(textEdit->font());
    //colorChanged(textEdit->textColor());

    //alignmentChanged(textEdit->alignment());

    //connect(textEdit->document(), &QTextDocument::modificationChanged,
      //      actionSave, &QAction::setEnabled);
    connect(textEdit->document(), &QTextDocument::modificationChanged,
            this, &QWidget::setWindowModified);
    connect(textEdit->document(), &QTextDocument::undoAvailable,
            actionUndo, &QAction::setEnabled);
    connect(textEdit->document(), &QTextDocument::redoAvailable,
            actionRedo, &QAction::setEnabled);

    setWindowModified(textEdit->document()->isModified());

    //actionSave->setEnabled(textEdit->document()->isModified());
    actionUndo->setEnabled(textEdit->document()->isUndoAvailable());
    actionRedo->setEnabled(textEdit->document()->isRedoAvailable());

#ifndef QT_NO_CLIPBOARD
    actionCut->setEnabled(false);
    connect(textEdit, &QTextEdit::copyAvailable, actionCut, &QAction::setEnabled);

    actionCopy->setEnabled(false);

    connect(textEdit, &QTextEdit::copyAvailable, actionCopy, &QAction::setEnabled);

    connect(QApplication::clipboard(), &QClipboard::dataChanged, this, &TextEdit::clipboardDataChanged);


#endif

    textEdit->setFocus();
    textEdit->setFocusPolicy(Qt::TabFocus);
    this->setFocusPolicy(Qt::NoFocus);
    setCurrentFileName(QString());


#ifdef Q_OS_MACOS
    // Use dark text on light background on macOS, also in dark mode.
    QPalette pal = textEdit->palette();
    pal.setColor(QPalette::Base, QColor(Qt::white));
    pal.setColor(QPalette::Text, QColor(Qt::black));
    textEdit->setPalette(pal);
#endif

    colors = this->highlight(QColor(255, 255, 255), QColor(0, 0, 0), 10);

    int i = 1;
    int j = colors.size()-2;
    for(int it = 0; it<(colors.size()/2)-1;it++){
        colors.move(i,j);
        i++;
        j--;
    }

    timer = new QTimer(this);
    QTimer::connect(timer, SIGNAL(timeout()), this, SLOT(blink()));

    this->setWindowIcon(QIcon("Graphics/LogoTemp/profile.png"));

}

TextEdit::~TextEdit()
{
    qDebug()<<"destroying";
    delete textEdit;
}

QVector<QColor> TextEdit::highlight(const QColor &bg, const QColor &fg, int noColors)
{
    QVector<QColor> colors_;
    const int HUE_BASE = (bg.hue() == -1) ? 90 : bg.hue();
    int h, s, v;
    for (int i = 0; i < noColors; i++) {
        h = int(HUE_BASE + (360.0 / noColors * i)) % 360;
        s = 240;
        v = int(qMax(bg.value(), fg.value()) * 0.85);
        const int M = 35;
       if ((h < bg.hue() + M && h > bg.hue() - M)
           || (h < fg.hue() + M && h > fg.hue() - M))
       {
         h = ((bg.hue() + fg.hue()) / (i+1)) % 360;
         s = ((bg.saturation() + fg.saturation() + 2*i)/ 2) % 256;
         v = ((bg.value() + fg.value() + 2*i) / 2)
             % 256;
       }
       colors_.append(QColor::fromHsv(h, s, v));
    }
    return colors_;
}

void TextEdit::updateLabel(int id, int x, int y)
{
    for(auto pair : labelVector){
        if(pair.second==id){
            //qDebug()<<"x e y "<<pair.first->x()<<" "<<pair.first->y();
            pair.first->move(x, y);
            qDebug()<<"x e y "<<pair.first->x()<<" "<<pair.first->y();
            pair.first->setFixedSize(textEdit->cursorRect().width()*3, textEdit->cursorRect().height());
            break;
        }

    }

    timer->start(500);

}

void TextEdit::removeLabel(QString &username, int id) //, int x, int y
{
    qDebug()<<"RemoveLabel "<<usersVector.size();
    //tolgo le label da uservector e labelvector
    for(auto it=usersVector.begin(); it!= usersVector.end();it++){
        if((*it)->text()==username){
            delete (*it);
            (*it) = nullptr;
            usersVector.erase(it);
            break;
        }
    }
    int i = 0;
    qDebug()<<"labelVector size "<<labelVector.size();
    for(auto pair : labelVector){
        if(pair.second == id){
            labelVector.remove(i);
            delete pair.first;
            pair.first = nullptr;
        }
        i++;
    }
    qDebug()<<"userConnessi ";
    int j = 0;
    for(auto pair : userConnessi){
        if(pair.first == username){
            userConnessi.remove(j);
        }
        j++;
    }
    for(int i = 0; i<userList->count(); i++){
        if(userList->item(i)->text()==username){
            delete userList->takeItem(i);
        }
    }
    //this->userNumber--;
    //this->userActive->setNum(userNumber);

}

QString TextEdit::supportSize(int s)
{
    QString supportSize;
    if(s<10){
        supportSize = QString::number(s).insert(0,"000");
    }
    else if(s<100){
        supportSize = QString::number(s).insert(0,"00");
    }
    else if(s<1000){
        supportSize = QString::number(s).insert(0,"0");
    }
    else {
        supportSize = QString::number(s);
    }
    return supportSize;
}

void TextEdit::blink()
{
    for(auto pair : labelVector){
    if(pair.first->isHidden())
        pair.first->show();
    else
        pair.first->hide();
    }
}

void TextEdit::closeEvent(QCloseEvent *e)
{
    if(flag == true){
        qDebug()<<"closeEvent";
        if (maybeSave()){            
            this->disconnect(1);
            e->ignore();
        }
        else
            e->ignore();
    }
}

void TextEdit::setupFileActions()
{
    QToolBar *tb = addToolBar(tr("File Actions"));
    //QToolBar *tb = mainwindow->addToolBar(tr("File Actions"));

    QMenu *menu = menuBar()->addMenu(tr("&File"));
    //QMenu *menu = mainwindow->menuBar()->addMenu(tr("&File"));
    menu->setFocusPolicy(Qt::NoFocus);
/*
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(rsrcPath + "/filenew.png"));

    QAction *a = menu->addAction(newIcon,  tr("&New"), this, &TextEdit::fileNew);

    //tb->setFixedHeight(75);
    tb->setMovable(false);
    tb->addAction(a);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::New);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(rsrcPath + "/fileopen.png"));

    //a = menu->addAction(openIcon, tr("&Open..."), this, &TextEdit::fileOpen);


    a->setShortcut(QKeySequence::Open);
    tb->addAction(a);

    menu->addSeparator();

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(rsrcPath + "/filesave.png"));

    //actionSave = menu->addAction(saveIcon, tr("&Save"), this, &TextEdit::fileSave);


    //actionSave->setShortcut(QKeySequence::Save);
    //actionSave->setEnabled(false);
    //tb->addAction(actionSave);

    //a = menu->addAction(tr("Save &As..."), this, &TextEdit::fileSaveAs);

    a->setPriority(QAction::LowPriority);
    menu->addSeparator();

*/


#ifndef QT_NO_PRINTER
    /*
    const QIcon printIcon = QIcon::fromTheme("document-print", QIcon(rsrcPath + "/fileprint.png"));

    //a = menu->addAction(printIcon, tr("&Print..."), this, &TextEdit::filePrint);

    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Print);
    tb->addAction(a);

    const QIcon filePrintIcon = QIcon::fromTheme("fileprint", QIcon(rsrcPath + "/fileprint.png"));

    menu->addAction(filePrintIcon, tr("Print Preview..."), this, &TextEdit::filePrintPreview);
*/
    const QIcon exportPdfIcon = QIcon::fromTheme("exportpdf", QIcon(rsrcPath + "/exportpdf.png"));

    QAction *a = menu->addAction(exportPdfIcon, tr("&Export PDF..."), this, &TextEdit::filePrintPdf);

    a->setPriority(QAction::LowPriority);
    a->setShortcut(Qt::CTRL + Qt::Key_D);
    tb->addAction(a);
    tb->setMovable(false);

    menu->addSeparator();
#endif

    a = menu->addAction(tr("&Quit"), this, &QWidget::close);

    a->setShortcut(Qt::CTRL + Qt::Key_Q);
}

void TextEdit::setupEditActions()
{
    QToolBar *tb = addToolBar(tr("Edit Actions"));
    //tb->setFixedHeight(75);
    tb->setMovable(false);
    QMenu *menu = menuBar()->addMenu(tr("&Edit"));
    //QToolBar *tb = mainwindow->addToolBar(tr("Edit Actions"));
    //QMenu *menu = mainwindow->menuBar()->addMenu(tr("&Edit"));

    const QIcon undoIcon = QIcon::fromTheme("edit-undo", QIcon(rsrcPath + "/editundo.png"));
    //actionUndo = menu->addAction(undoIcon, tr("&Undo"), textEdit, &QTextEdit::undo);
    actionUndo = menu->addAction(undoIcon, tr("&Undo"), this, &TextEdit::myUndo);
    actionUndo->setShortcut(QKeySequence::Undo);
    tb->addAction(actionUndo);

    const QIcon redoIcon = QIcon::fromTheme("edit-redo", QIcon(rsrcPath + "/editredo.png"));
    // actionRedo = menu->addAction(redoIcon, tr("&Redo"), textEdit, &QTextEdit::redo);
    actionRedo = menu->addAction(redoIcon, tr("&Redo"), this, &TextEdit::myRedo);
    actionRedo->setPriority(QAction::LowPriority);
    actionRedo->setShortcut(QKeySequence::Redo);
    tb->addAction(actionRedo);
    menu->addSeparator();

#ifndef QT_NO_CLIPBOARD
    const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(rsrcPath + "/editcut.png"));
    //actionCut = menu->addAction(cutIcon, tr("Cu&t"), textEdit, &QTextEdit::cut);
    actionCut = menu->addAction(cutIcon, tr("Cu&t"), this, &TextEdit::myCut);
    actionCut->setPriority(QAction::LowPriority);
    actionCut->setShortcut(QKeySequence::Cut);
    tb->addAction(actionCut);

    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(rsrcPath + "/editcopy.png"));
    actionCopy = menu->addAction(copyIcon, tr("&Copy"), textEdit, &QTextEdit::copy);
    //actionCopy = menu->addAction(copyIcon, tr("&Copy"), this, &TextEdit::myCopy);
    actionCopy->setPriority(QAction::LowPriority);
    actionCopy->setShortcut(QKeySequence::Copy);
    tb->addAction(actionCopy);

    const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(rsrcPath + "/editpaste.png"));
    //actionPaste = menu->addAction(pasteIcon, tr("&Paste"), textEdit, &QTextEdit::paste);
    actionPaste = menu->addAction(pasteIcon, tr("&Paste"), this, &TextEdit::myPaste);
    actionPaste->setPriority(QAction::LowPriority);
    actionPaste->setShortcut(QKeySequence::Paste);
    tb->addAction(actionPaste);
    if (const QMimeData *md = QApplication::clipboard()->mimeData())
        actionPaste->setEnabled(md->hasText());

#endif
}

void TextEdit::setupTextActions()
{

    //URI

    tb = addToolBar(tr("Share Actions"));
    tb->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    addToolBarBreak(Qt::TopToolBarArea);
    //tb->setFixedHeight(75);
    tb->setMovable(false);
    share = new QPushButton("Share",this);
    share->setFocusPolicy(Qt::NoFocus);

    connect(share, &QAbstractButton::clicked, this, &TextEdit::on_share_clicked);

    homepage = new QPushButton("Go to Homepage",this);
    homepage->setFocusPolicy(Qt::NoFocus);

    connect(homepage, &QAbstractButton::clicked,this, &TextEdit::on_homepageButton_clicked);

    uri = new QLabel("Click on Share to get the URI", this);
    //uri->setReadOnly(true);
    uri->setFocusPolicy(Qt::NoFocus);

    copyUri = new QPushButton("Copy", this);
    connect(copyUri, &QAbstractButton::clicked, this, &TextEdit::on_copyUri_clicked);


    tb->addWidget(share);
    tb->addSeparator();
    tb->addWidget(uri);
    tb->addSeparator();
    tb->addWidget(copyUri);
    tb->addSeparator();
    tb->addWidget(homepage);

    addToolBarBreak(Qt::TopToolBarArea);


    //FORMAT ACTIONS

    QToolBar *tb = addToolBar(tr("Format Actions"));
    QMenu *menu = menuBar()->addMenu(tr("F&ormat"));
    tb->setFocusPolicy(Qt::NoFocus);
    //tb->setFixedHeight(75);
    tb->setMovable(false);


    const QIcon boldIcon = QIcon::fromTheme("format-text-bold", QIcon(rsrcPath + "/textbold.png"));

    actionTextBold = menu->addAction(boldIcon, tr("&Bold"), this, &TextEdit::textBold);

    actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
    actionTextBold->setPriority(QAction::LowPriority);
    QFont bold;
    bold.setBold(true);
    actionTextBold->setFont(bold);
    tb->addAction(actionTextBold);
    actionTextBold->setCheckable(true);

    const QIcon italicIcon = QIcon::fromTheme("format-text-italic", QIcon(rsrcPath + "/textitalic.png"));

    actionTextItalic = menu->addAction(italicIcon, tr("&Italic"), this, &TextEdit::textItalic);


    actionTextItalic->setPriority(QAction::LowPriority);
    actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
    QFont italic;
    italic.setItalic(true);
    actionTextItalic->setFont(italic);
    tb->addAction(actionTextItalic);
    actionTextItalic->setCheckable(true);

    const QIcon underlineIcon = QIcon::fromTheme("format-text-underline", QIcon(rsrcPath + "/textunder.png"));

    actionTextUnderline = menu->addAction(underlineIcon, tr("&Underline"), this, &TextEdit::textUnderline);


    actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
    actionTextUnderline->setPriority(QAction::LowPriority);
    QFont underline;
    underline.setUnderline(true);
    actionTextUnderline->setFont(underline);
    tb->addAction(actionTextUnderline);
    actionTextUnderline->setCheckable(true);

    menu->addSeparator();


    /*

    const QIcon leftIcon = QIcon::fromTheme("format-justify-left", QIcon(rsrcPath + "/textleft.png"));

    actionAlignLeft = new QAction(leftIcon, tr("&Left"), this);


    actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
    actionAlignLeft->setCheckable(true);
    actionAlignLeft->setPriority(QAction::LowPriority);
    const QIcon centerIcon = QIcon::fromTheme("format-justify-center", QIcon(rsrcPath + "/textcenter.png"));

    actionAlignCenter = new QAction(centerIcon, tr("C&enter"), this);


    actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
    actionAlignCenter->setCheckable(true);
    actionAlignCenter->setPriority(QAction::LowPriority);
    const QIcon rightIcon = QIcon::fromTheme("format-justify-right", QIcon(rsrcPath + "/textright.png"));

    actionAlignRight = new QAction(rightIcon, tr("&Right"), this);

    actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
    actionAlignRight->setCheckable(true);
    actionAlignRight->setPriority(QAction::LowPriority);
    const QIcon fillIcon = QIcon::fromTheme("format-justify-fill", QIcon(rsrcPath + "/textjustify.png"));

    actionAlignJustify = new QAction(fillIcon, tr("&Justify"), this);


    actionAlignJustify->setShortcut(Qt::CTRL + Qt::Key_J);
    actionAlignJustify->setCheckable(true);
    actionAlignJustify->setPriority(QAction::LowPriority);

    // Make sure the alignLeft  is always left of the alignRight

    QActionGroup *alignGroup = new QActionGroup(this);

    connect(alignGroup, &QActionGroup::triggered, this, &TextEdit::textAlign);


    if (QApplication::isLeftToRight()) {
        alignGroup->addAction(actionAlignLeft);
        alignGroup->addAction(actionAlignCenter);
        alignGroup->addAction(actionAlignRight);
    } else {
        alignGroup->addAction(actionAlignRight);
        alignGroup->addAction(actionAlignCenter);
        alignGroup->addAction(actionAlignLeft);
    }
    alignGroup->addAction(actionAlignJustify);

    tb->addActions(alignGroup->actions());
    menu->addActions(alignGroup->actions());

    menu->addSeparator();


    */

    /*

    QPixmap pix(16, 16);
    pix.fill(Qt::black);
    actionTextColor = menu->addAction(pix, tr("&Color..."), this, &TextEdit::textColor);

    tb->addAction(actionTextColor);

    */

    // TEXT ACTIONS


    tb = addToolBar(tr("Text Actions"));
    tb->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    //addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(tb);
    //tb->setFixedHeight(75);
    tb->setMovable(false);

    /*
    comboStyle = new QComboBox(tb);
    tb->addWidget(comboStyle);
    comboStyle->addItem("Standard");
    comboStyle->addItem("Bullet List (Disc)");
    comboStyle->addItem("Bullet List (Circle)");
    comboStyle->addItem("Bullet List (Square)");
    comboStyle->addItem("Ordered List (Decimal)");
    comboStyle->addItem("Ordered List (Alpha lower)");
    comboStyle->addItem("Ordered List (Alpha upper)");
    comboStyle->addItem("Ordered List (Roman lower)");
    comboStyle->addItem("Ordered List (Roman upper)");
    comboStyle->addItem("Heading 1");
    comboStyle->addItem("Heading 2");
    comboStyle->addItem("Heading 3");
    comboStyle->addItem("Heading 4");
    comboStyle->addItem("Heading 5");
    comboStyle->addItem("Heading 6");
    comboStyle->setFocusPolicy(Qt::NoFocus);

    connect(comboStyle, QOverload<int>::of(&QComboBox::activated), this, &TextEdit::textStyle);
    */

    comboFont = new QFontComboBox(tb);
    tb->addWidget(comboFont);
    connect(comboFont, QOverload<const QString &>::of(&QComboBox::activated), this, &TextEdit::textFamily);
    comboFont->setFocusPolicy(Qt::NoFocus);

    comboSize = new QComboBox(tb);
    comboSize->setObjectName("comboSize");
    tb->addWidget(comboSize);
    comboSize->setEditable(true);
    comboSize->setFocusPolicy(Qt::NoFocus);

    const QList<int> standardSizes = QFontDatabase::standardSizes();
    for (int size : standardSizes)
        comboSize->addItem(QString::number(size));
    comboSize->setCurrentIndex(standardSizes.indexOf(QApplication::font().pointSize()));

    connect(comboSize, QOverload<const QString &>::of(&QComboBox::activated), this, &TextEdit::textSize);


    tb->setMovable(false);

    //USERS

    /* LISTA UTENTI TOOLBAR CLASSICA

    ut = addToolBar(tr("Users Active"));
    //ut = addToolBar(tr("Users Active"));
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(ut);
    ut->setMovable(false);

    userLabel = new QLabel(tr("Users Connected: "), this);
    userActive = new QLabel(this);
    userActive->setNum(userNumber);
    ut->addWidget(userLabel);
    ut->addWidget(userActive);
    userActive->setFocusPolicy(Qt::NoFocus);
    userActive->setFixedSize(ut->size());
    */


    /*
    tb = addToolBar(tr("Users Active"));
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(tb);
    //tb->setFixedHeight(75);
    tb->setMovable(false);
    userLabel = new QLabel(tr("Users Connected: "));
    userActive = new QLabel();
    userActive->setNum(userNumber);
    tb->addWidget(userLabel);
    tb->addWidget(userActive);
    userActive->setFocusPolicy(Qt::NoFocus);
    userActive->setFixedSize(tb->size());
    */



    //LOWER TOOLBAR


    //riga, colonna, numero parole, numero caratteri,
    QToolBar *ltb = addToolBar(tr("Bottom ToolBar"));
    ltb->setFocusPolicy(Qt::NoFocus);
    ltb->setAllowedAreas(Qt::BottomToolBarArea);
    addToolBar(Qt::BottomToolBarArea, ltb);
    ltb->setMovable(false);
    //ltb->setFixedHeight(75);

    row = new QLabel (tr("Row: "), this);
    r = textEdit->document()->lineCount();
    numRow = new QLabel(this);
    numRow->setNum(r);
    ltb->addWidget(row);
    ltb->addWidget(numRow);
    ltb->addSeparator();

    column = new QLabel(tr("Column: "), this);
    c = textEdit->textCursor().columnNumber();
    numColumn = new QLabel(this);
    numColumn->setNum(c);
    ltb->addWidget(column);
    ltb->addWidget(numColumn);
    ltb->addSeparator();

    words = new QLabel(tr("Words: "), this);
    w = textEdit->toPlainText().split(QRegExp("(\\s|\\n|\\r)+") , QString::SkipEmptyParts).count();
    numWords = new QLabel(this);
    numWords->setNum(w);
    ltb->addWidget(words);
    ltb->addWidget(numWords);
    ltb->addSeparator();

    characters = new QLabel(tr("Characters: "), this);
    numC = textEdit->toPlainText().count();
    numCharacters = new QLabel(this);
    numCharacters->setNum(numC);
    ltb->addWidget(characters);
    ltb->addWidget(numCharacters);
    ltb->addSeparator();


    //dock per utenti a destra


    dock = new QDockWidget(tr("User Connected: "), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    userList = new QListWidget(dock);
    userList->addItem(MainWindow::username);
    qDebug()<<"colorsize"<<colors.size()<<"color 0";
    //userList->item(1)->setTextColor(colors.at(0));
    //userList->addItems(user);
    dock->setWidget(userList);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea,dock);
    dock->setFeatures(dock->features() & ~QDockWidget::DockWidgetClosable);
    dock->setFeatures(dock->features() & ~QDockWidget::DockWidgetMovable);
    dock->setFeatures(dock->features() & ~QDockWidget::DockWidgetFloatable);
    dock->setFocusPolicy(Qt::NoFocus);

}

bool TextEdit::load(const QByteArray arrayByte, const QByteArray arrayFont)
{
    //if (!QFile::exists(f))
    //    return false;
    //QFile file(f);
    //if (!file.open(QFile::ReadOnly))
    //    return false;
    QTextCodec *codec = Qt::codecForHtml(arrayByte);
    QString str = codec->toUnicode(arrayByte);

    QTextCodec *codecFont = Qt::codecForHtml(arrayFont);
    QString strFont = codecFont->toUnicode(arrayFont);

    if (Qt::mightBeRichText(str)) {
        textEdit->setHtml(str);
        //qDebug()<<"sono in HTML";
    } else {
        //sub_str = QString::fromLocal8Bit(arrayByte);
        //textEdit->setPlainText(str);
        QStringList charList = str.split(";,");
        QStringList fList = strFont.split(";");

        for(auto i=0; i<charList.size()-1; i++) {
            QStringList fontList = fList[i].split(",");
            QFont font = QFont();
            font.setFamily(fontList[0]);
            font.setPointSizeF(fontList[1].toDouble());
            font.setWeight(fontList[4].toInt());
            font.setItalic(fontList[5].toInt());
            font.setUnderline(fontList[6].toInt());
            textEdit->setFontFamily(font.family());
            textEdit->setFontPointSize(font.pointSize());
            textEdit->setFontItalic(font.italic());
            textEdit->setFontUnderline(font.underline());
            textEdit->setFontWeight(font.weight());
            //textEdit->setTextBackgroundColor(colors.at(fontList[10].toInt()-1));
            textEdit->setTextColor(colors.at(fontList[10].toInt()-1));
            textEdit->insertPlainText(charList[i]);
        }
    }
    //this->localInsert(index);

    textEdit->moveCursor(QTextCursor::End);

    //QByteArray data = f.readAll();
    //setCurrentFileName(f);
    return true;
}


bool TextEdit::maybeSave()
{
    if (!textEdit->document()->isModified())
        return true;

   const QMessageBox::StandardButton ret =
          QMessageBox::warning(this, QCoreApplication::applicationName(),
                             tr("The document has been modified.\n"
                                "Exit?"),
                             QMessageBox::Yes | QMessageBox::No);

   if(ret == QMessageBox::No){
    return false;
   }
   return true;

    /*if (ret == QMessageBox::Save)
        return fileSave();
    else if (ret == QMessageBox::Cancel)
        return false;
    return true;
    */
}

void TextEdit::setCurrentFileName(const QString &fileName)
{
    this->fileName = fileName;
    textEdit->document()->setModified(false);

    QString shownName;
    if (fileName.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = QFileInfo(fileName).fileName();

    setWindowTitle(tr("%1[*] - %2").arg(shownName, QCoreApplication::applicationName()));
    setWindowModified(false);


}
/*

void TextEdit::fileNew()
{
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFileName(QString());
    }
}
*/
/*
void TextEdit::fileOpen()
{
    QFileDialog fileDialog(this, tr("Open File..."));

    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setMimeTypeFilters(QStringList() << "text/html" << "text/plain");
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    const QString fn = fileDialog.selectedFiles().first();
    if (load(fn))
        statusBar()->showMessage(tr("Opened \"%1\"").arg(QDir::toNativeSeparators(fn)));
        //mainwindow->statusBar()->showMessage(tr("Opened \"%1\"").arg(QDir::toNativeSeparators(fn)));
    else
    statusBar()->showMessage(tr("Could not open \"%1\"").arg(QDir::toNativeSeparators(fn)));
    //mainwindow->statusBar()->showMessage(tr("Could not open \"%1\"").arg(QDir::toNativeSeparators(fn)));
}
*/
/*
bool TextEdit::fileSave()
{
    if (fileName.isEmpty())
        return fileSaveAs();
    if (fileName.startsWith(QStringLiteral(":/")))
        return fileSaveAs();

    QTextDocumentWriter writer(fileName);
    bool success = writer.write(textEdit->document());
    if (success) {
        textEdit->document()->setModified(false);
        statusBar()->showMessage(tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName)));
        //mainwindow->statusBar()->showMessage(tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName)));
    } else {
        statusBar()->showMessage(tr("Could not write to file \"%1\"").arg(QDir::toNativeSeparators(fileName)));
        //mainwindow->statusBar()->showMessage(tr("Could not write to file \"%1\"").arg(QDir::toNativeSeparators(fileName)));
    }
    return success;
}

bool TextEdit::fileSaveAs()
{
    QFileDialog fileDialog(this, tr("Save as..."));
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    QStringList mimeTypes;
    mimeTypes << "application/vnd.oasis.opendocument.text" << "text/html" << "text/plain";
    fileDialog.setMimeTypeFilters(mimeTypes);
    fileDialog.setDefaultSuffix("odt");
    if (fileDialog.exec() != QDialog::Accepted)
        return false;
    const QString fn = fileDialog.selectedFiles().first();
    setCurrentFileName(fn);
    return fileSave();
}

void TextEdit::filePrint()
{
#if QT_CONFIG(printdialog)
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog *dlg = new QPrintDialog(&printer, this);


    if (textEdit->textCursor().hasSelection())
        dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    dlg->setWindowTitle(tr("Print Document"));
    if (dlg->exec() == QDialog::Accepted)
        textEdit->print(&printer);
    delete dlg;
#endif
}

void TextEdit::filePrintPreview()
{
#if QT_CONFIG(printpreviewdialog)
    QPrinter printer(QPrinter::HighResolution);
   QPrintPreviewDialog preview(&printer, this);
   connect(&preview, &QPrintPreviewDialog::paintRequested, this, &TextEdit::printPreview);

    preview.exec();
#endif
}

void TextEdit::printPreview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    textEdit->print(printer);
#endif
}

*/
void TextEdit::filePrintPdf()
{
#ifndef QT_NO_PRINTER
//! [0]

    QFileDialog fileDialog(this, tr("Export PDF"));


    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setMimeTypeFilters(QStringList("application/pdf"));
    fileDialog.setDefaultSuffix("pdf");
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    QString fileName = fileDialog.selectedFiles().first();
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    textEdit->document()->print(&printer);
    statusBar()->showMessage(tr("Exported \"%1\"").arg(QDir::toNativeSeparators(fileName)));
    //mainwindow->statusBar()->showMessage(tr("Exported \"%1\"").arg(QDir::toNativeSeparators(fileName)));
//! [0]
#endif
}

void TextEdit::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(actionTextUnderline->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(actionTextItalic->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}
/*
void TextEdit::textStyle(int styleIndex)
{
    QTextCursor cursor = textEdit->textCursor();
    QTextListFormat::Style style = QTextListFormat::ListStyleUndefined;

    switch (styleIndex) {
    case 1:
        style = QTextListFormat::ListDisc;
        break;
    case 2:
        style = QTextListFormat::ListCircle;
        break;
    case 3:
        style = QTextListFormat::ListSquare;
        break;
    case 4:
        style = QTextListFormat::ListDecimal;
        break;
    case 5:
        style = QTextListFormat::ListLowerAlpha;
        break;
    case 6:
        style = QTextListFormat::ListUpperAlpha;
        break;
    case 7:
        style = QTextListFormat::ListLowerRoman;
        break;
    case 8:
        style = QTextListFormat::ListUpperRoman;
        break;
    default:
        break;
    }

    cursor.beginEditBlock();

    QTextBlockFormat blockFmt = cursor.blockFormat();

    if (style == QTextListFormat::ListStyleUndefined) {
        blockFmt.setObjectIndex(-1);
        int headingLevel = styleIndex >= 9 ? styleIndex - 9 + 1 : 0; // H1 to H6, or Standard
        blockFmt.setHeadingLevel(headingLevel);
        cursor.setBlockFormat(blockFmt);

        int sizeAdjustment = headingLevel ? 4 - headingLevel : 0; // H1 to H6: +3 to -2
        QTextCharFormat fmt;
        fmt.setFontWeight(headingLevel ? QFont::Bold : QFont::Normal);
        fmt.setProperty(QTextFormat::FontSizeAdjustment, sizeAdjustment);
        cursor.select(QTextCursor::LineUnderCursor);
        cursor.mergeCharFormat(fmt);
        textEdit->mergeCurrentCharFormat(fmt);
    } else {
        QTextListFormat listFmt;
        if (cursor.currentList()) {
            listFmt = cursor.currentList()->format();
        } else {
            listFmt.setIndent(blockFmt.indent() + 1);
            blockFmt.setIndent(0);
            cursor.setBlockFormat(blockFmt);
        }
        listFmt.setStyle(style);
        cursor.createList(listFmt);
    }

    cursor.endEditBlock();
}
*/
/*
void TextEdit::textColor()
{
   QColor col = QColorDialog::getColor(textEdit->textColor(), this);

    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    colorChanged(col);
}
*/

/*
void TextEdit::textAlign(QAction *a)
{

    if (a == actionAlignLeft)
        textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    else if (a == actionAlignCenter)
        textEdit->setAlignment(Qt::AlignHCenter);
    else if (a == actionAlignRight)
        textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    else if (a == actionAlignJustify)
        textEdit->setAlignment(Qt::AlignJustify);

    QString username = MainWindow::username;
    QTcpSocket *s = MyTcpSocket::socket;
    QString finalStr = username+";,"+this->windowTitle()+";,"+textEdit->alignment();

    auto size = finalStr.size();
    QString supportSize;
    if(size<100){
        if(size<10){
            supportSize = QString::number(size).insert(0,"0");
        }
        supportSize = QString::number(size).insert(0,"0");
    }

    QString msg = "CA      "+supportSize+finalStr;
    qDebug()<<msg;

    if(s->write(msg.toUtf8()) != msg.size())
        QMessageBox::warning(this,"Alignment Change","Impossible to communicate with server");

}
*/

void TextEdit::currentCharFormatChanged(const QTextCharFormat &format)
{
    //qDebug()<<"currentCharFormatChanged ";
    /*
    comboFont->setCurrentFont(format.font());
    comboSize->setCurrentText(QString::number(format.font().pointSize()));
    */
    comboFont->setEditText(format.font().family());
    comboSize->setEditText(QString::number(format.font().pointSize()));

    //selected text da fare
    if(textEdit->textCursor().hasSelection()){
        if(textEdit->textCursor().selectionStart() >= textEdit->textCursor().position()){
            QTextCursor tmpCursor;
            QTextCursor originalCursor = textEdit->textCursor();
            tmpCursor = textEdit->textCursor();
            tmpCursor.setPosition(textEdit->textCursor().position()+1);

            if(tmpCursor.charFormat().font().bold())
                actionTextBold->setChecked(true);
            else
                actionTextBold->setChecked(false);
            if(tmpCursor.charFormat().font().italic())
                actionTextItalic->setChecked(true);
            else
                actionTextItalic->setChecked(false);
            if(tmpCursor.charFormat().font().underline())
                actionTextUnderline->setChecked(true);
            else
                actionTextUnderline->setChecked(false);
        }
    }
    else{
        if(format.font().bold())
            actionTextBold->setChecked(true);
        else
            actionTextBold->setChecked(false);
        if(format.font().italic())
            actionTextItalic->setChecked(true);
        else
            actionTextItalic->setChecked(false);
        if(format.font().underline())
            actionTextUnderline->setChecked(true);
        else
            actionTextUnderline->setChecked(false);
    }

    //if(this->actionTextBold->isChecked() || this->actionTextItalic->isChecked() || this->actionTextUnderline->isChecked()
         //  || !this->comboFont->currentText().isEmpty() || !this->comboSize->currentText().isEmpty()){
    /*
    QColor color = textEdit->textColor();
    int r = color.red();
    int g = color.green();
    int b = color.blue();
    */
}

void TextEdit::cursorPositionChanged()
    {
        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::StartOfLine);

        int lines = 1;
        while(cursor.positionInBlock()>0) {
            cursor.movePosition(QTextCursor::Up);
            lines++;
        }
        QTextBlock block = cursor.block().previous();

        while(block.isValid()) {
            lines += block.lineCount();
            block = block.previous();
        }
        r = lines;
        w = textEdit->toPlainText().split(QRegExp("(\\s|\\n|\\r)+") , QString::SkipEmptyParts).count();
        numWords->setNum(w);
        numC = textEdit->toPlainText().count();
        numCharacters->setNum(numC);
}

void TextEdit::clipboardDataChanged()
{
#ifndef QT_NO_CLIPBOARD
    if (const QMimeData *md = QApplication::clipboard()->mimeData())
        actionPaste->setEnabled(md->hasText());
#endif
}

void TextEdit::about()
{

     QMessageBox::about(this, tr("About"), tr("This is the Politecnico Shared Editor made by: Ondesca Giacomo, Pelacà Rebecca and Rasicci Riccardo"));

}

void TextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    qDebug()<<"mergeFormattOnWordOrSelection";
    QTextCursor cursor = textEdit->textCursor();
    QFont currentFont = textEdit->currentFont();
    //qDebug()<<"current font"<<currentFont;

    if (!cursor.hasSelection()){
        cursor.select(QTextCursor::WordUnderCursor);
        textEdit->mergeCurrentCharFormat(format);
        qDebug()<<"no Selection";
        textEdit->currentFont() = format.font();
    }
    else{

        int s = textEdit->textCursor().selectedText().size();
        QString msg = "MULTI   "+supportSize(s);
        qDebug()<<msg;
        QTcpSocket *socket = MyTcpSocket::socket;

        if(socket->write(msg.toUtf8()) != msg.size())
            QMessageBox::warning(this,"Multi","Impossible to communicate with server");
        //QThread::msleep(2);
        socket->flush();

        if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
            //qDebug()<<"sinistra verso destra";
            //qDebug()<<"format"<< format.font().toString();
            fontChanged(format.font());
            //colorChanged(format.foreground().color());
            cursor.mergeCharFormat(format);
            cursor.charFormat();
            textEdit->mergeCurrentCharFormat(format);
            //qDebug()<<"merge "<<textEdit->currentCharFormat().font().toString();
            textEdit->currentFont() = format.font();

            for(int i = textEdit->textCursor().selectionStart(); i< textEdit->textCursor().selectionEnd(); i++){
                int finish = textEdit->textCursor().selectionEnd();
                Symbol currentSymbol = symbols.at(i);
                QTextCursor cursor1 (textEdit->textCursor());
                cursor1.setPosition(i+1,QTextCursor::MoveAnchor);
                textEdit->setTextCursor(cursor1);
                currentSymbol.setFont(textEdit->currentFont()); //textEdit->currentFont()
                symbols[i].setFont(textEdit->currentFont());
                QString finalStr = this->owner+";,"+this->windowTitle()+";,"+QString::number(i)+";,"+currentSymbol.toStr()+";,"+MainWindow::username;
                auto size = finalStr.toUtf8().size();
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
                QString msg = "TCFM    "+supportSize+finalStr;
                qDebug()<<"msg "<<msg;
                if(socket->write(msg.toUtf8()) != msg.toUtf8().size())
                    QMessageBox::warning(this,"Texteditor Format Change","Impossible to communicate with server");
                QThread::msleep(5);
                socket->flush();
                QTextCursor cursor2 (textEdit->textCursor());
                cursor2.setPosition(i+1,QTextCursor::MoveAnchor);
                cursor2.setPosition(finish,QTextCursor::KeepAnchor);
                textEdit->setTextCursor(cursor2);
            }
        }
        else{
            //qDebug()<<"destra verso sinistra";
            fontChanged(format.font());
            QTextCursor tmpCursor;
            tmpCursor = textEdit->textCursor();
            tmpCursor.setPosition(textEdit->textCursor().position()); //KeepAnchor
            tmpCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, s);
            textEdit->setTextCursor(tmpCursor);
            textEdit->textCursor().mergeCharFormat(format);
            textEdit->mergeCurrentCharFormat(format);
            textEdit->currentFont() = format.font();

            for(int i = textEdit->textCursor().selectionStart(); i< textEdit->textCursor().selectionEnd(); i++){
                int finish = textEdit->textCursor().selectionEnd();
                Symbol currentSymbol = symbols.at(i); //
                QTextCursor cursor1 (textEdit->textCursor());
                cursor1.setPosition(i+1,QTextCursor::MoveAnchor);
                textEdit->setTextCursor(cursor1);
                currentSymbol.setFont(textEdit->currentFont());
                symbols[i].setFont(textEdit->currentFont());
                QString finalStr = this->owner+";,"+this->windowTitle()+";,"+QString::number(i)+";,"+currentSymbol.toStr()+";,"+MainWindow::username;
                auto size = finalStr.toUtf8().size();
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
                QString msg = "TCFM    "+supportSize+finalStr;
                qDebug()<<"msg "<<msg;
                if(socket->write(msg.toUtf8()) != msg.toUtf8().size())
                    QMessageBox::warning(this,"Texteditor Format Change","Impossible to communicate with server");
                QThread::msleep(5);
                socket->flush();
                QTextCursor cursor2 (textEdit->textCursor());
                cursor2.setPosition(i+1,QTextCursor::MoveAnchor);
                cursor2.setPosition(finish,QTextCursor::KeepAnchor);
                textEdit->setTextCursor(cursor2);
            }
        }

    }
}

void TextEdit::fontChanged(const QFont &f)
{
    qDebug()<<"fontChanged";
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
    actionTextBold->setChecked(f.bold());
    actionTextItalic->setChecked(f.italic());
    actionTextUnderline->setChecked(f.underline());
}


/*
void TextEdit::colorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    pix.fill(c);
    actionTextColor->setIcon(pix);
}
*/

void TextEdit::setForm(QTextCharFormat f)
{
    textEdit->mergeCurrentCharFormat(f);
}

/*
void TextEdit::setAl(QString al)
{
    if (al == "\u0011")
       textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    else if (al == "\u0004")
       textEdit->setAlignment(Qt::AlignHCenter);
    else if (al == "\0012")
       textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    else if (al == "\b")
       textEdit->setAlignment(Qt::AlignJustify);

}

void TextEdit::alignmentChanged(Qt::Alignment a)
{

    if (a & Qt::AlignLeft)
        actionAlignLeft->setChecked(true);
    else if (a & Qt::AlignHCenter)
        actionAlignCenter->setChecked(true);
    else if (a & Qt::AlignRight)
        actionAlignRight->setChecked(true);
    else if (a & Qt::AlignJustify)
        actionAlignJustify->setChecked(true);


}
*/

void TextEdit::on_homepageButton_clicked(){
    if(maybeSave()){
        this->releaseKeyboard();
        uri->grabKeyboard();
        disconnect(0);
        h->show();
    }
}

std::vector<int> TextEdit::localInsert(unsigned int index)
{
    std::vector<int> pos;
    auto i = this->symbols.size();            //vedo il numero di elementi presenti in symbol

    qDebug()<<"INDEX"<< index <<"symbols size inizio"<<symbols.size();

    if(index > symbols.size()){
        //index = symbols.size() - 1;
        index = symbols.size();
        qDebug()<<"indexxx" << index;
    }

    if (i == 0) {
        pos.push_back(0);                    //nessun simbolo, metto 0
    }
    else
    if (index==0){
        pos.push_back(this->symbols[index].getPos()[0]-1);
    }
    else
    if (index==i) {
        pos.push_back(this->symbols[index - 1].getPos()[0] + 1);

    } else {                                                            //più simboli
        auto s = this->symbols[index - 1].getPos().size();     //metto la dimensione di sinistra in s
        auto d = this->symbols[index].getPos().size();
        for (unsigned int k=0; k<=s; k++){                               //scorro il vettore di sinistra
            if (k==s){                                                    //se finito metto l'elemento di destra-1
                if(k==d){
                    break;
                }
                pos.push_back(this->symbols[index].getPos()[k]-1);
                break;                                                  //esco dal for
            }
            else if ((this->symbols[index].getPos()[k]-this->symbols[index-1].getPos()[k])>1){      //distanza maggiore di 1
                pos.push_back((this->symbols[index].getPos()[k]+this->symbols[index-1].getPos()[k])/2);   //medio tra i due valori
                break;                                                                                     //esco
            }
            else if ((this->symbols[index].getPos()[k]-this->symbols[index-1].getPos()[k])==1){      //distanza uguale a 1
                pos.push_back(this->symbols[index-1].getPos()[k]);                       //inserisco il valore di sinistra
                if ((k+1)==s){                                                       //vedo se non ci sono sottolivelli a sinistra
                    pos.push_back(5);                                               //inserisco 5 ed esco eventualmente da togliere
                    break;
                }
                else{                                                               //altri casi
                    pos.push_back(this->symbols[index-1].getPos()[k+1]+1);         //prendo il valore del sottolivello di sinistra+1 ed esco
                    break;
                }
            }  //distanza uguale,quindi copio l'elemento di sinistra e riprendo il for con gli elementi nel sottolivello successivo
            pos.push_back(this->symbols[index-1].getPos()[k]);
        }
    }
    pos.push_back(this->getSiteID());
    QString fullText = textEdit->toPlainText();
    QString charValue = fullText.at(int(index));     //prendo l'ultimo carattere inserito

    this->counter++;
    Symbol symbol = Symbol(_siteId, counter, charValue, pos, textEdit->currentFont());
    //qDebug()<<"Symbol "<<symbol.getId()<<" "<<symbol.getCounter()<<" "<<symbol.getValue()<<" "<<symbol.getPos()<<" "<<symbol.getFont();
    /*
    if(index > symbols.size()){
        this->symbols.push_back(symbol);
        qDebug()<<"sonoif";
    }
    else
    */
        this->symbols.insert(this->symbols.cbegin()+index,symbol);

    QString username = MainWindow::username;
    QTcpSocket *s = MyTcpSocket::socket;
    QString finalStr = username+";,"+this->windowTitle()+";,"+ symbol.toStr()+";,"+this->getOwner()+";,"+QString::number(textEdit->cursorRect().bottomLeft().x())+";,"+QString::number(textEdit->cursorRect().topLeft().y());
    auto size = finalStr.toUtf8().size();
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
    QString msg = "LI      "+supportSize+finalStr;
    qDebug()<<msg<<" pos size: "<<pos.size();

    if( s->write(msg.toUtf8())!= msg.toUtf8().size()){
        QMessageBox::warning(this,"Local Insert","Impossible to communicate with server");
    }
    insertString.push_back(symbols[unsigned (index)].getPos().back());

    QThread::msleep(5);

    qDebug()<<"waitfobyteswritten: "<<s->waitForBytesWritten();

    s->flush();

    return pos;
}

std::vector<int> TextEdit::localRedo(unsigned int index, int id)
{
    std::vector<int> pos;
    auto i = this->symbols.size();            //vedo il numero di elementi presenti in symbol
    if (i == 0) {
        pos.push_back(0);                    //nessun simbolo, metto 0 o randomico
    }
    else
    if (index==0){
        pos.push_back(this->symbols[index].getPos()[0]-1);
    }
    else
    if (index==i) {
        pos.push_back(this->symbols[index - 1].getPos()[0] + 1);

    } else {                                                            //più simboli
        auto s = this->symbols[index - 1].getPos().size();     //metto la dimensione di sinistra in s
        auto d = this->symbols[index].getPos().size();
        for (unsigned int k=0; k<=s; k++){                               //scorro il vettore di sinistra
            if (k==s){                                                    //se finito metto l'elemento di destra-1
                if(k==d){
                    break;
                }
                pos.push_back(this->symbols[index].getPos()[k]-1);
                break;                                                  //esco dal for
            }
            else if ((this->symbols[index].getPos()[k]-this->symbols[index-1].getPos()[k])>1){      //distanza maggiore di 1
                pos.push_back((this->symbols[index].getPos()[k]+this->symbols[index-1].getPos()[k])/2);   //medio tra i due valori
                break;                                                                                     //esco
            }
            else if ((this->symbols[index].getPos()[k]-this->symbols[index-1].getPos()[k])==1){      //distanza uguale a 1
                pos.push_back(this->symbols[index-1].getPos()[k]);                       //inserisco il valore di sinistra
                if ((k+1)==s){                                                       //vedo se non ci sono sottolivelli a sinistra
                    pos.push_back(5);                                               //inserisco 5 ed esco eventualmente da togliere
                    break;
                }
                else{                                                               //altri casi
                    pos.push_back(this->symbols[index-1].getPos()[k+1]+1);         //prendo il valore del sottolivello di sinistra+1 ed esco
                    break;
                }
            }  //distanza uguale,quindi copio l'elemento di sinistra e riprendo il for con gli elementi nel sottolivello successivo
            pos.push_back(this->symbols[index-1].getPos()[k]);
        }
    }
    pos.push_back(id); ////////unicacosacambiata
    QString fullText = textEdit->toPlainText();
    QString charValue = fullText.at(int(index));     //prendo l'ultimo carattere inserito

    this->counter++;
    Symbol symbol = Symbol(id, counter, charValue, pos, textEdit->currentFont());
    //qDebug()<<"Symbol "<<symbol.getId()<<" "<<symbol.getCounter()<<" "<<symbol.getValue()<<" "<<symbol.getPos()<<" "<<symbol.getFont();
    this->symbols.insert(this->symbols.cbegin()+index,symbol);

    QString username = MainWindow::username;
    QTcpSocket *s = MyTcpSocket::socket;
    QString finalStr = username+";,"+this->windowTitle()+";,"+ symbol.toStr()+";,"+this->getOwner()+";,"+QString::number(textEdit->cursorRect().bottomLeft().x())+";,"+QString::number(textEdit->cursorRect().topLeft().y());
    auto size = finalStr.toUtf8().size();
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
    QString msg = "LI      "+supportSize+finalStr;
    qDebug()<<msg;

    if(s->write(msg.toUtf8()) != msg.toUtf8().size()){
        QMessageBox::warning(this,"Local Insert","Impossible to communicate with server");
    }
    QThread::msleep(5);

    s->flush();

    return pos;
}

std::vector<Symbol> TextEdit::getSymbol()
{
     return this->symbols;
}

void TextEdit::setSymbol(int index, Symbol s)
{
    this->symbols.insert(symbols.cbegin()+index,s);
}

void TextEdit::removeSymbol(int index, int id, int x, int y, int nChar)
{
    qDebug()<<"removeSymbol";

    this->symbols.erase(symbols.cbegin()+index);

    int textposition = textEdit->textCursor().position();

    bool select = false;
    int start=0;
    int finish=0;
    if (textEdit->textCursor().hasSelection()){
        select = true;
        start = textEdit->textCursor().selectionStart();
        finish = textEdit->textCursor().selectionEnd();
        QTextCursor cursor (textEdit->textCursor());
        cursor.setPosition(textposition,QTextCursor::MoveAnchor);
        textEdit->setTextCursor(cursor);
    }


    QTextCursor cursorTemp = QTextCursor(textEdit->textCursor());
    cursorTemp.setPosition(index, QTextCursor::MoveAnchor);
    textEdit->setTextCursor(cursorTemp);

    textEdit->textCursor().deleteChar();

    emit cursorPositionChanged();
    numRow->setNum(r);
    c = textEdit->textCursor().columnNumber();
    numColumn->setNum(c);

    QTextCursor cursor1 (textEdit->textCursor());

    if(select==true){
        if(index>=finish){
            cursor1.setPosition(start,QTextCursor::MoveAnchor);
            cursor1.setPosition(finish,QTextCursor::KeepAnchor);
        }
        else if(index<=start){
            cursor1.setPosition(start-1,QTextCursor::MoveAnchor);
            cursor1.setPosition(finish-1,QTextCursor::KeepAnchor);
        }
        else{
            cursor1.setPosition(start,QTextCursor::MoveAnchor);
            cursor1.setPosition(finish-1,QTextCursor::KeepAnchor);
        }
    }
    else{
        if (index<= textposition && textposition!=0) cursor1.setPosition(textposition-1,QTextCursor::MoveAnchor);
        else cursor1.setPosition(textposition,QTextCursor::MoveAnchor);
    }
    textEdit->setTextCursor(cursor1);

    if(nChar==1)
        this->updateLabel(id, x, y);

}

void TextEdit::setOwner(QString owner)
{
    this->owner = owner;
}

QString TextEdit::getOwner()
{
    return this->owner;
}

void TextEdit::setSymbolFont(int index, QFont f)
{
    qDebug()<<"setSymbolFont";
    symbols.at(index).setFont(f);

    bool select = false;
    int start = 0;
    int finish = 0;
    int textposition = textEdit->textCursor().position();
    //qDebug()<<"textposition"<<textposition;
    if(textEdit->textCursor().hasSelection()){
        select = true;
        start = textEdit->textCursor().selectionStart();
        finish = textEdit->textCursor().selectionEnd();
        QTextCursor cursor (textEdit->textCursor());
        cursor.setPosition(textposition,QTextCursor::MoveAnchor);
        //qDebug()<<"TextCursor dopo settext"<<textEdit->textCursor().position()<<" textposition "<<textposition;
        textEdit->setTextCursor(cursor);
        qDebug()<<"if";
    }
    //qDebug()<<"textposition"<<textposition;
    QTextCursor cursorTemp = QTextCursor(textEdit->textCursor());
    cursorTemp.setPosition(index, QTextCursor::MoveAnchor);
    textEdit->setTextCursor(cursorTemp);

    textEdit->moveCursor(QTextCursor::Right,QTextCursor::KeepAnchor);

    textEdit->setFontFamily(f.family());
    textEdit->setFontPointSize(f.pointSize());
    textEdit->setFontItalic(f.italic());
    textEdit->setFontUnderline(f.underline());
    textEdit->setFontWeight(f.weight());
    //symbols[index].setFont(f);
    QTextCursor cursor1 (textEdit->textCursor());
    if(select==true){
        cursor1.setPosition(start,QTextCursor::MoveAnchor);
        cursor1.setPosition(finish,QTextCursor::KeepAnchor);
    }
    else cursor1.setPosition(textposition,QTextCursor::MoveAnchor);
    //qDebug()<<"TextCursor dopo settext"<<textEdit->textCursor().position()<<" textposition "<<textposition;

    for(auto label : labelVector){
        label.first->setFixedSize(textEdit->cursorRect().size());
    }
    textEdit->setTextCursor(cursor1);

}

void TextEdit::setSymbolFontData(const QString &data)
{
    qDebug()<<"setSymbolFontData";
    QStringList msg = data.split(";,");
    QString owner = msg[0];
    QString title = msg[1];
    int index = msg[2].toInt();
    QStringList fontList = msg[3].split(",");
    QFont f = QFont();
    f.setFamily(fontList[0]);
    f.setPointSizeF(fontList[1].toDouble());
    f.setWeight(fontList[4].toInt());
    f.setItalic(fontList[5].toInt());
    f.setUnderline(fontList[6].toInt());

    symbols.at(index).setFont(f);

    bool select = false;
    int start = 0;
    int finish = 0;
    int textposition = textEdit->textCursor().position();
    //qDebug()<<"textposition"<<textposition;
    if(textEdit->textCursor().hasSelection()){
        select = true;
        start = textEdit->textCursor().selectionStart();
        finish = textEdit->textCursor().selectionEnd();
        QTextCursor cursor (textEdit->textCursor());
        cursor.setPosition(textposition,QTextCursor::MoveAnchor);
        //qDebug()<<"TextCursor dopo settext"<<textEdit->textCursor().position()<<" textposition "<<textposition;
        textEdit->setTextCursor(cursor);
    }
    //qDebug()<<"textposition"<<textposition;
    QTextCursor cursorTemp = QTextCursor(textEdit->textCursor());
    cursorTemp.setPosition(index, QTextCursor::MoveAnchor);
    textEdit->setTextCursor(cursorTemp);

    textEdit->moveCursor(QTextCursor::Right,QTextCursor::KeepAnchor);

    textEdit->setFontFamily(f.family());
    textEdit->setFontPointSize(f.pointSize());
    textEdit->setFontItalic(f.italic());
    textEdit->setFontUnderline(f.underline());
    textEdit->setFontWeight(f.weight());
    //symbols[index].setFont(f);
    QTextCursor cursor1 (textEdit->textCursor());
    if(select==true){
        cursor1.setPosition(start,QTextCursor::MoveAnchor);
        cursor1.setPosition(finish,QTextCursor::KeepAnchor);
    }
    else cursor1.setPosition(textposition,QTextCursor::MoveAnchor);
    //qDebug()<<"TextCursor dopo settext"<<textEdit->textCursor().position()<<" textposition "<<textposition;

    for(auto label : labelVector){
        label.first->setFixedSize(textEdit->cursorRect().size());
    }
    textEdit->setTextCursor(cursor1);

}

void TextEdit::disconnect(int flag)
{
    QTcpSocket *s = MyTcpSocket::socket;
    QString username = MainWindow::username;

    QString msg ="CLOSE   "+username+";"+this->owner+";"+this->windowTitle()+";"+QString::number(flag);
    qDebug()<<msg;
    if(s->write(msg.toUtf8()) != msg.toUtf8().size())
        QMessageBox::warning(this,"Disconnect","Impossible to communicate with server");
}

void TextEdit::localErase(int index)
{
    QString er;
    std::vector<int> deletedPos;
    auto i = 0;

    for(auto it = this->symbols.begin(); it != this->symbols.end(); it++) {
        if (i == index) {
            //Message m = Message(1, this->_siteId, _symbol.at(index));
            // this->_server.send(m);
            er = symbols.at(i).getValue();
            //er=symbols[i].getValue();
            deletedPos = symbols[i].getPos();
            this->symbols.erase(it);        //fare dopo?
            break;
        }
        i++;
    }

    QString fullText = textEdit->toPlainText();

    this->counter--;

    Symbol symbol = Symbol(_siteId, counter, er, deletedPos, textEdit->currentFont());

    QString username = MainWindow::username;
    QTcpSocket *s = MyTcpSocket::socket;
    //valutare velocità del server altrimenti togliere username
    QString finalStr = username+";,"+this->windowTitle()+";,"+ symbol.toStr()+";,"+this->getOwner()+";,"+QString::number(textEdit->cursorRect().left())+";,"+QString::number(textEdit->cursorRect().top());
    auto size = finalStr.toUtf8().size();
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
    QString msg = "LE      "+supportSize+finalStr;
    qDebug()<<msg;

    if(s->write(msg.toUtf8()) != msg.toUtf8().size())
        QMessageBox::warning(this,"Local Erase","Impossible to communicate with server");

    QThread::msleep(5);

    s->flush();
}

void TextEdit::insertCharacter(int index, Symbol symbol, int x, int y, int nChar)
{
   this->symbols.insert(symbols.cbegin()+index,symbol);
   int textposition = textEdit->textCursor().position();
   bool select = false;
   int start = 0;
   int finish = 0;
   if (textEdit->textCursor().hasSelection()){
       select = true;
       start= textEdit->textCursor().selectionStart();
       finish = textEdit->textCursor().selectionEnd();
       QTextCursor cursor (textEdit->textCursor());
       cursor.setPosition(textposition,QTextCursor::MoveAnchor);
       textEdit->setTextCursor(cursor);
   }
   QTextCursor cursorTemp = QTextCursor(textEdit->textCursor());
   cursorTemp.setPosition(index, QTextCursor::MoveAnchor);
   textEdit->setTextCursor(cursorTemp);

   textEdit->setFontFamily(symbol.getFont().family());
   textEdit->setFontPointSize(symbol.getFont().pointSize());
   textEdit->setFontItalic(symbol.getFont().italic());
   textEdit->setFontUnderline(symbol.getFont().underline());
   textEdit->setFontWeight(symbol.getFont().weight());

   int id = symbol.getId()-symbol.getCounter();
   textEdit->setTextColor(colors.at(id-1));
   textEdit->textCursor().insertText(symbol.getValue());

   emit cursorPositionChanged();
   numRow->setNum(r);
   c = textEdit->textCursor().columnNumber();
   numColumn->setNum(c);

   QTextCursor cursor1 (textEdit->textCursor());
   if(select==true){
       if(index>=finish){
           cursor1.setPosition(start,QTextCursor::MoveAnchor);
           cursor1.setPosition(finish,QTextCursor::KeepAnchor);
       }
       else if(index<=start){
           cursor1.setPosition(start+1,QTextCursor::MoveAnchor);
           cursor1.setPosition(finish+1,QTextCursor::KeepAnchor);
       }
       else{
           cursor1.setPosition(start,QTextCursor::MoveAnchor);
           cursor1.setPosition(finish+1,QTextCursor::KeepAnchor);
       }
   }
   else {
       if(index < textposition) cursor1.setPosition(textposition+1,QTextCursor::MoveAnchor);
       else cursor1.setPosition(textposition,QTextCursor::MoveAnchor);
   }

   textEdit->setTextCursor(cursor1);

   if(nChar==1)
       this->updateLabel(symbol.getId()-symbol.getCounter(), x, y);
}

void TextEdit::process(const QString& message)
{
    int textposition = textEdit->textCursor().position();
    qDebug()<<"process"<<message<<" "<<"textposition "<<textposition;
    QStringList data = message.split(";,");
    QString op = data[0];
    QString username = data[8];
    int id = data[1].toInt();
    int counter_message = data[2].toInt();
    QString charValue = data[3].toUtf8();
    QStringList posList = data[4].split("|");

    std::vector<int> pos_message;
    for(auto i = 0; i<posList.size(); i++)
        pos_message.push_back(posList[i].toInt());

    QStringList fontList = data[5].split(",");
    QFont font = QFont();
    font.setFamily(fontList[0]);
    font.setPointSizeF(fontList[1].toDouble());
    font.setWeight(fontList[4].toInt());
    font.setItalic(fontList[5].toInt());
    font.setUnderline(fontList[6].toInt());

    int left = data[6].toInt();
    int top = data[7].toInt();

    Symbol symbol = Symbol(id-counter_message, counter_message, charValue, pos_message, font);
    //qDebug()<<"simbolo da cancellare"<<symbol.toStr();
    //prendere dal socket il simbolo mandato dal server e il suo corrispettivo vettore di pos
    auto size_m = pos_message.size(); //numero di cifre nel vettore frazionario del simbolo da inserire
    auto size = this->symbols.size(); //numero totale di simboli presenti
    unsigned int index = 0; //indice del vettore _symbols in cui devo inserire il nuovo simbolo contenuto nel msg
    //auto idClientMessage = pos_message[size_m-1]; //ultimo elemento del vettore di pos (ID)
    std::vector<int> residualPos;
    for(size_t it = 0; it<size_m-1; it++){
        residualPos.push_back(pos_message.at(it));
    }

    bool flag=false;
    //int counter=0; //
    if(op == "LI") {
        // INSERT
        if(size==0){
            this->symbols.push_back(symbol); //devo inserire all'inizio se non ho inserito neanche un simbolo
            index = 0;
        }
        else
        if(size==1){ //se ho un solo simbolo
            if(size_m >= this->symbols[0].getPos().size()){
                //qDebug()<<"residual?"<<residualPos<<" "<<this->symbols[0].getResidualPos();
                if(size_m==this->symbols[0].getPos().size() && residualPos==this->symbols[0].getResidualPos()){
                    if(this->symbols[0].getPos().at(symbols[0].getPos().size()-1) < symbol.getPos().at(symbol.getPos().size()-1))
                        index=1;
                }
                else{
                    for(size_t i=0;i<this->symbols[0].getPos().size();i++){ //scorro l'indice frazionario dell'unico simbolo presente
                        if(pos_message[i]>this->symbols[0].getPos()[i]){
                            index=1;
                            break;
                        }
                    }
                }
            }
            else{
                for(size_t i=0;i<size_m;i++){
                    if(pos_message[i]>this->symbols[0].getPos()[i]){
                        index=1;
                        break;
                    }
                }
            }
            if(index==1) this->symbols.push_back(symbol); //inserisco dopo il primo carattere
            else this->symbols.insert(this->symbols.cbegin(),symbol); //altrimento lo inserisco all'inizio e sposto l'altro di una posizione
        }
        else if(size > 1){
            if(this->symbols[size-1].getPos()[0]<pos_message[0]){
                index= unsigned (symbols.size());
                this->symbols.push_back(symbol); //se il primo digit del mio simbolo ÃƒÂ¨ maggiore del primo dell'ultimo simbolo inserito(che ÃƒÂ¨ il maggiore) aggiungo in coda
            }
            else if(this->symbols[0].getPos()[0]>pos_message[0]){
                index=0;
                this->symbols.insert(this->symbols.cbegin(),symbol); //se l'ultimo simbolo(il piu piccolo) ha il primo digit maggiore del primo del simbolo da inserire, lo devo mettere in testa
                //else if(this->_symbols[0].pos[0]==pos_message[0] && size_m<this->_symbols[0].pos.size()) this->_symbols.insert(this->_symbols.cbegin(),m.getSymbol());
            }
            else{
                for(size_t i=0;i<size;i++){
                    if(this->symbols[i].getPos().size() > size_m){
                        for(size_t j=0;j<size_m && flag!=true ;j++){
                            if(pos_message[j]==this->symbols[i].getPos()[j]) continue;
                            else if(pos_message[j]<this->symbols[i].getPos()[j]){
                                index= unsigned (i);
                                flag=true;
                            }
                            else if(pos_message[j]>this->symbols[i].getPos()[j]) break;
                        }
                    }
                    if(flag==true) break;
                    if(this->symbols[i].getPos().size() <= size_m){
                        if(size_m==this->symbols[i].getPos().size() && residualPos==this->symbols[0].getResidualPos()){
                            if(this->symbols[0].getPos().at(symbols[0].getPos().size()-1) < symbol.getPos().at(symbol.getPos().size()-1)){
                                index= unsigned (i);
                                flag=true;
                            }
                        }
                        for(size_t j=0;j<this->symbols[i].getPos().size() && flag!=true ;j++){
                            if(pos_message[j]==this->symbols[i].getPos()[j]) continue;
                            else if(pos_message[j]<this->symbols[i].getPos()[j]){
                                index= unsigned (i);
                                flag=true;
                            }
                            else if(pos_message[j]>this->symbols[i].getPos()[j]) break;
                        }
                    }
                    if(flag==true) break;
                }
                if(flag==true) this->symbols.insert(this->symbols.cbegin()+index,symbol);
                else {
                    index = unsigned (symbols.size());
                    this->symbols.push_back(symbol);
                }
            }
        }
        bool select = false;
        int start = 0;
        int finish = 0;
        if (textEdit->textCursor().hasSelection()){
            select = true;
            start = textEdit->textCursor().selectionStart();
            finish = textEdit->textCursor().selectionEnd();
            QTextCursor cursor (textEdit->textCursor());
            cursor.setPosition(textposition,QTextCursor::MoveAnchor);
            textEdit->setTextCursor(cursor);
        }
        QTextCursor cursorTemp = QTextCursor(textEdit->textCursor());
        cursorTemp.setPosition(int (index), QTextCursor::MoveAnchor);
        textEdit->setTextCursor(cursorTemp);

        textEdit->setFontFamily(symbol.getFont().family());
        textEdit->setFontPointSize(symbol.getFont().pointSize());
        textEdit->setFontItalic(symbol.getFont().italic());
        textEdit->setFontUnderline(symbol.getFont().underline());
        textEdit->setFontWeight(symbol.getFont().weight());

        int id = symbol.getId()-symbol.getCounter();

        textEdit->setTextColor(colors.at(id-1));
        textEdit->textCursor().insertText(symbol.getValue());

        emit cursorPositionChanged();
        numRow->setNum(r);
        c = textEdit->textCursor().columnNumber();
        numColumn->setNum(c);

        QTextCursor cursor1 (textEdit->textCursor());
        if(select==true){
            if(index>=finish){
                cursor1.setPosition(start,QTextCursor::MoveAnchor);
                cursor1.setPosition(finish,QTextCursor::KeepAnchor);
            }
            else if(index<=start){
                cursor1.setPosition(start+1,QTextCursor::MoveAnchor);
                cursor1.setPosition(finish+1,QTextCursor::KeepAnchor);
            }
            else{
                cursor1.setPosition(start,QTextCursor::MoveAnchor);
                cursor1.setPosition(finish+1,QTextCursor::KeepAnchor);
            }
        }
        else {
            if(index < textposition) cursor1.setPosition(textposition+1,QTextCursor::MoveAnchor);
            else cursor1.setPosition(textposition,QTextCursor::MoveAnchor);
        }
        textEdit->setTextCursor(cursor1);
        insertString.push_back(id);
        this->updateLabel(symbol.getId()-symbol.getCounter(), left, top);
    }
    else{
        //DELETE
        for(size_t i=0;i<size;i++){
            //qDebug()<<"simblo nel for"<<symbols[i].toStr();
            if(pos_message==this->symbols[i].getPos()){
                index=i;

                this->symbols.erase(this->symbols.cbegin()+i);
                break;
            }
        }
        bool select = false;
        int start=0;
        int finish=0;
        if (textEdit->textCursor().hasSelection()){
            select = true;
            start = textEdit->textCursor().selectionStart();
            finish = textEdit->textCursor().selectionEnd();
            QTextCursor cursor (textEdit->textCursor());
            cursor.setPosition(textposition,QTextCursor::MoveAnchor);
            textEdit->setTextCursor(cursor);
        }

        QTextCursor cursorTemp = QTextCursor(textEdit->textCursor());
        cursorTemp.setPosition(index, QTextCursor::MoveAnchor);
        textEdit->setTextCursor(cursorTemp);
        textEdit->textCursor().deleteChar();

        emit cursorPositionChanged();
        numRow->setNum(r);
        c = textEdit->textCursor().columnNumber();
        numColumn->setNum(c);

        QTextCursor cursor1 (textEdit->textCursor());
        if(select==true){
            if(index>=finish){
                cursor1.setPosition(start,QTextCursor::MoveAnchor);
                cursor1.setPosition(finish,QTextCursor::KeepAnchor);
            }
            else if(index<=start){
                cursor1.setPosition(start-1,QTextCursor::MoveAnchor);
                cursor1.setPosition(finish-1,QTextCursor::KeepAnchor);
            }
            else{
                cursor1.setPosition(start,QTextCursor::MoveAnchor);
                cursor1.setPosition(finish-1,QTextCursor::KeepAnchor);
            }
        }
        else{
            if (index< textposition && textposition!=0) cursor1.setPosition(textposition-1,QTextCursor::MoveAnchor);
            else cursor1.setPosition(textposition,QTextCursor::MoveAnchor);
        }
        textEdit->setTextCursor(cursor1);
        this->updateLabel(id, left, top);
    }
}

void TextEdit::keyPressEvent(QKeyEvent *event){

    while(wait==true){
        QTime dieTime= QTime::currentTime().addMSecs(10000);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    QWidget::keyPressEvent(event);
    QMainWindow::keyPressEvent(event);

    if(event->modifiers() & Qt::AltModifier) { //solo alt (alt 1, alt 2, ...)
        if(event->key() == Qt::Key_1 || event->key() == Qt::Key_2 || event->key() == Qt::Key_3 || event->key() == Qt::Key_4|| event->key() == Qt::Key_5 || event->key() == Qt::Key_6 || event->key() == Qt::Key_7 || event->key() == Qt::Key_8 || event->key() == Qt::Key_9 || event->key() == Qt::Key_0)
            event->ignore();
        else {
           if(event->modifiers() & Qt::ControlModifier){ //control + alt
                if(event->key()==Qt::Key_At){
                    if(textEdit->textCursor().hasSelection()){
                        auto size = textEdit->textCursor().selectedText().size();
                        qDebug()<<"Selection";
                        int size2 = size+1;
                        QString msg = "MULTI   "+supportSize(size2);
                        qDebug()<<msg;
                        QTcpSocket *socket = MyTcpSocket::socket;

                        if(socket->write(msg.toUtf8()) != msg.size())
                            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

                        if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){   
                            textEdit->textCursor().removeSelectedText();
                            auto index = textEdit->textCursor().position();// - 1;
                            for(auto i = 0; i<size;i++){
                                localErase(index);
                            }
                        }
                        else{
                            textEdit->textCursor().removeSelectedText();
                            auto index = textEdit->textCursor().position();
                            for(auto i = 0; i<size;i++){
                                localErase(index);
                             }
                        }
                    }
                    //textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
                    textEdit->setTextColor(this->colors.at(_siteId-1));
                    textEdit->insertPlainText(event->text());
                    auto index = textEdit->textCursor().position()-1;
                    auto pos = this->localInsert(unsigned (index));
                    //textEdit->insertPlainText(event->text());
                }
                else if(event->key()==Qt::Key_BracketLeft){
                    if(textEdit->textCursor().hasSelection()){
                        auto size = textEdit->textCursor().selectedText().size();
                        qDebug()<<"Selection";
                        int size2 = size+1;
                        QString msg = "MULTI   "+supportSize(size2);
                        qDebug()<<msg;
                        QTcpSocket *socket = MyTcpSocket::socket;

                        if(socket->write(msg.toUtf8()) != msg.size())
                            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

                        if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){ 
                            textEdit->textCursor().removeSelectedText();
                            auto index = textEdit->textCursor().position();// - 1;
                            for(auto i = 0; i<size;i++){
                                localErase(index);
                            }
                        }
                        else{
                            textEdit->textCursor().removeSelectedText();
                            auto index = textEdit->textCursor().position();
                            for(auto i = 0; i<size;i++){
                                localErase(index);
                            }
                        }
                    }
                    //textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
                    textEdit->setTextColor(this->colors.at(_siteId-1));
                    textEdit->insertPlainText(event->text());
                    auto index = textEdit->textCursor().position()-1;
                    auto pos = this->localInsert(index);
                    //textEdit->insertPlainText(event->text());
                }
                else if(event->key()==Qt::Key_BracketRight){
                    if(textEdit->textCursor().hasSelection()){
                        auto size = textEdit->textCursor().selectedText().size();
                        qDebug()<<"Selection";
                        int size2 = size+1;
                        QString msg = "MULTI   "+supportSize(size2);
                        qDebug()<<msg;
                        QTcpSocket *socket = MyTcpSocket::socket;

                        if(socket->write(msg.toUtf8()) != msg.size())
                            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

                        if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){                          
                            textEdit->textCursor().removeSelectedText();
                            auto index = textEdit->textCursor().position();// - 1;
                            for(auto i = 0; i<size;i++){
                                localErase(index);
                            }
                        }
                        else{
                            textEdit->textCursor().removeSelectedText();
                            auto index = textEdit->textCursor().position();
                            for(auto i = 0; i<size;i++){
                                localErase(index);
                             }
                        }
                    }
                    //textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
                    textEdit->setTextColor(this->colors.at(_siteId-1));
                    textEdit->insertPlainText(event->text());
                    auto index = textEdit->textCursor().position()-1;
                    auto pos = this->localInsert(index);
               }
               else if(event->key()==Qt::Key_BraceLeft){
                    if(textEdit->textCursor().hasSelection()){
                        auto size = textEdit->textCursor().selectedText().size();
                        qDebug()<<"Selection";
                        int size2 = size+1;
                        QString msg = "MULTI   "+supportSize(size2);
                        qDebug()<<msg;
                        QTcpSocket *socket = MyTcpSocket::socket;

                        if(socket->write(msg.toUtf8()) != msg.size())
                            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

                        if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                            textEdit->textCursor().removeSelectedText();
                            auto index = textEdit->textCursor().position();// - 1;
                            for(auto i = 0; i<size;i++){
                                localErase(index);
                            }
                        }
                        else{
                            textEdit->textCursor().removeSelectedText();
                            auto index = textEdit->textCursor().position();
                            for(auto i = 0; i<size;i++){
                                localErase(index);
                             }
                        }
                    }
                    //textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
                    textEdit->setTextColor(this->colors.at(_siteId-1));
                    textEdit->insertPlainText(event->text());
                    auto index = textEdit->textCursor().position()-1;
                    auto pos = this->localInsert(index);
               }
               else if(event->key()==Qt::Key_BraceRight){
                    if(textEdit->textCursor().hasSelection()){
                        auto size = textEdit->textCursor().selectedText().size();
                        int size2 = size+1;
                        QString msg = "MULTI   "+supportSize(size2);
                        QTcpSocket *socket = MyTcpSocket::socket;

                        if(socket->write(msg.toUtf8()) != msg.size())
                            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

                        if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                            textEdit->textCursor().removeSelectedText();
                            auto index = textEdit->textCursor().position();// - 1;
                            for(auto i = 0; i<size;i++){
                                localErase(index);
                            }
                        }
                        else{
                            textEdit->textCursor().removeSelectedText();
                            auto index = textEdit->textCursor().position();
                            for(auto i = 0; i<size;i++){
                                localErase(index);
                             }
                        }
                    }
                    //textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
                    textEdit->setTextColor(this->colors.at(_siteId-1));
                    textEdit->insertPlainText(event->text());

                    auto index = textEdit->textCursor().position()-1;
                    auto pos = this->localInsert(index);
                }
                else if(event->key()==Qt::Key_NumberSign){ //cancelletto
                    if(textEdit->textCursor().hasSelection()){
                        auto size = textEdit->textCursor().selectedText().size();
                        qDebug()<<"Selection";
                        int size2 = size+1;
                        QString msg = "MULTI   "+supportSize(size2);
                        qDebug()<<msg;
                        QTcpSocket *socket = MyTcpSocket::socket;

                        if(socket->write(msg.toUtf8()) != msg.size())
                            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

                        if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                           textEdit->textCursor().removeSelectedText();
                           auto index = textEdit->textCursor().position();// - 1;
                           for(auto i = 0; i<size;i++){
                               localErase(index);
                           }
                        }
                        else{
                             textEdit->textCursor().removeSelectedText();
                             auto index = textEdit->textCursor().position();
                             for(auto i = 0; i<size;i++){
                                 localErase(index);
                              }
                        }
                    }
                    //textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
                    textEdit->setTextColor(this->colors.at(_siteId-1));
                    textEdit->insertPlainText(event->text());
                    auto index = textEdit->textCursor().position()-1;
                    auto pos = this->localInsert(index);
               }
               else if(event->key()==Qt::Key_AsciiTilde){
                    if(textEdit->textCursor().hasSelection()){
                        auto size = textEdit->textCursor().selectedText().size();
                        qDebug()<<"Selection";
                        int size2 = size+1;
                        QString msg = "MULTI   "+supportSize(size2);
                        qDebug()<<msg;
                        QTcpSocket *socket = MyTcpSocket::socket;

                        if(socket->write(msg.toUtf8()) != msg.size())
                            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

                        if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                           textEdit->textCursor().removeSelectedText();
                           auto index = textEdit->textCursor().position();// - 1;
                           for(auto i = 0; i<size;i++){
                               localErase(index);
                           }
                        }
                        else{
                            textEdit->textCursor().removeSelectedText();
                            auto index = textEdit->textCursor().position();
                            for(auto i = 0; i<size;i++){
                                localErase(index);
                             }
                        }
                    }
                    //textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
                    textEdit->setTextColor(this->colors.at(_siteId-1));
                    textEdit->insertPlainText(event->text());
                    auto index = textEdit->textCursor().position()-1;
                    auto pos = this->localInsert(index);
               }
           }
        }
    }
    else if (event->modifiers() & Qt::ControlModifier){ //solo control
        if (event->key() == Qt::Key_Plus){ //abilitare shortcut

        }
        else if(event->key() == Qt::Key_Minus){

        }
        else if(event->key()==Qt::Key_A){
            qDebug()<<"copio tutto";
            textEdit->selectAll();
        }
    }
   else{
       if(event->key()>=Qt::Key_Exclam && event->key()<=Qt::Key_ydiaeresis){ //tutti i caratteri
           if(textEdit->textCursor().hasSelection()){
               auto size = textEdit->textCursor().selectedText().size();
               qDebug()<<"Selection";
               int size2 = size+1;
               QString msg = "MULTI   "+supportSize(size2);
               qDebug()<<msg;
               QTcpSocket *socket = MyTcpSocket::socket;

               if(socket->write(msg.toUtf8()) != msg.size())
                   QMessageBox::warning(this,"Multi","Impossible to communicate with server");

               if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                  textEdit->textCursor().removeSelectedText();
                  auto index = textEdit->textCursor().position();// - 1;
                  for(auto i = 0; i<size;i++){
                      localErase(index);
                  }
               }
               else{
                   textEdit->textCursor().removeSelectedText();
                   auto index = textEdit->textCursor().position();
                   for(auto i = 0; i<size;i++){
                       localErase(index);
                    }
               }
           }
           //textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
           textEdit->setTextColor(colors.at(_siteId-1));
           textEdit->insertPlainText(event->text());
           auto index = textEdit->textCursor().position()-1;
           auto pos = this->localInsert(index);

        }
        else if (event->key() == Qt::Key_Backspace){
           if(textEdit->textCursor().hasSelection()){
               auto size = textEdit->textCursor().selectedText().size();
               qDebug()<<"Selection";
               int size2 = size;
               QString msg = "MULTI   "+supportSize(size2);
               qDebug()<<msg;
               QTcpSocket *socket = MyTcpSocket::socket;

               if(socket->write(msg.toUtf8()) != msg.size())
                   QMessageBox::warning(this,"Multi","Impossible to communicate with server");

               if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                  textEdit->textCursor().removeSelectedText();
                  auto index = textEdit->textCursor().position();// - 1;
                  for(auto i = 0; i<size;i++){
                      insertString.push_back(symbols[index].getPos().back());
                      localErase(index);
                  }
               }
               else{
                   textEdit->textCursor().removeSelectedText();
                   auto index = textEdit->textCursor().position();
                   for(auto i = 0; i<size;i++){
                       insertString.push_back(symbols[index].getPos().back());
                       qDebug()<<"id"<<symbols[index].getPos().back();
                       localErase(index);
                    }
               }
           }
           else if (textEdit->textCursor().position()!=0){
               qDebug()<<"no selection";
               auto index = textEdit->textCursor().position()-1;
               textEdit->textCursor().deletePreviousChar();
               insertString.push_back(symbols[index].getPos().back());
               qDebug()<<"id"<<symbols[index].getPos().back();
               localErase(index);
           }
           qDebug()<<"insertString"<<insertString;
       }
       else if(event->key()==Qt::Key_Space){
           if(textEdit->textCursor().hasSelection()){
               auto size = textEdit->textCursor().selectedText().size();
               qDebug()<<"Selection";
               int size2 = size+1;
               QString msg = "MULTI   "+supportSize(size2);
               qDebug()<<msg;
               QTcpSocket *socket = MyTcpSocket::socket;

               if(socket->write(msg.toUtf8()) != msg.size())
                   QMessageBox::warning(this,"Multi","Impossible to communicate with server");

               if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                   textEdit->textCursor().removeSelectedText();
                  auto index = textEdit->textCursor().position();// - 1;
                  for(auto i = 0; i<size;i++){
                      localErase(index);
                  }
               }
               else{
                   textEdit->textCursor().removeSelectedText();
                   auto index = textEdit->textCursor().position();
                   for(auto i = 0; i<size;i++){
                       localErase(index);
                    }
               }
           }
           //textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
           textEdit->setTextColor(this->colors.at(_siteId-1));
           textEdit->insertPlainText(event->text());
           auto index  = textEdit->textCursor().position()-1;
           auto pos = localInsert(index);
       }
       else if(event->key() == Qt::Key_Tab){
           if(textEdit->textCursor().hasSelection()){
               auto size = textEdit->textCursor().selectedText().size();
               qDebug()<<"Selection";
               int size2 = size+1;
               QString msg = "MULTI   "+supportSize(size2);
               qDebug()<<msg;
               QTcpSocket *socket = MyTcpSocket::socket;

               if(socket->write(msg.toUtf8()) != msg.size())
                   QMessageBox::warning(this,"Multi","Impossible to communicate with server");

               if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                   textEdit->textCursor().removeSelectedText();
                  auto index = textEdit->textCursor().position();// - 1;
                  for(auto i = 0; i<size;i++){
                      localErase(index);
                  }
               }
                else{
                   textEdit->textCursor().removeSelectedText();
                   auto index = textEdit->textCursor().position();
                   for(auto i = 0; i<size;i++){
                       localErase(index);
                   }
               }
           }
           //textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
           textEdit->setTextColor(this->colors.at(_siteId-1));
           textEdit->insertPlainText(event->text());
           //qDebug()<<textEdit->textCursor().position();
           auto index  = textEdit->textCursor().position()-1;
           auto pos = localInsert(index);
       }
       else if(event->key()==Qt::Key_Enter || event->key()==Qt::Key_Return){
           qDebug()<<"Enter";
           if(textEdit->textCursor().hasSelection()){
               auto size = textEdit->textCursor().selectedText().size();
               qDebug()<<"Selection";
               int size2 = size+1;
               QString msg = "MULTI   "+supportSize(size2);
               qDebug()<<msg;
               QTcpSocket *socket = MyTcpSocket::socket;

               if(socket->write(msg.toUtf8()) != msg.size())
                   QMessageBox::warning(this,"Multi","Impossible to communicate with server");

               if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                   textEdit->textCursor().removeSelectedText();
                  auto index = textEdit->textCursor().position();// - 1;
                  for(auto i = 0; i<size;i++){
                      localErase(index);
                  }
               }
                else{
                   textEdit->textCursor().removeSelectedText();
                   auto index = textEdit->textCursor().position();
                   for(auto i = 0; i<size;i++){
                       localErase(index);
                   }
               }
           }
           //textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
           textEdit->setTextColor(this->colors.at(_siteId-1));
           textEdit->insertPlainText(event->text());
           auto index  = textEdit->textCursor().position()-1;
           auto pos = localInsert(index);
       }
       else if(event->key() == Qt::Key_Left){
           if(combination!=0){
               QTextCursor tmpCursor = textEdit->textCursor();
               tmpCursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
               textEdit->setTextCursor(tmpCursor);
           }
           else{
               QTextCursor tmpCursor = textEdit->textCursor();
               tmpCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor);
               textEdit->setTextCursor(tmpCursor);
           }
       }
       else if(event->key() == Qt::Key_Right){
           if(combination !=0){
               QTextCursor tmpCursor = textEdit->textCursor();
               tmpCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
               textEdit->setTextCursor(tmpCursor);
           }
           else{
               QTextCursor tmpCursor = textEdit->textCursor();
               tmpCursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor);
               textEdit->setTextCursor(tmpCursor);
           }
       }
       else if(event->key() == Qt::Key_Up){
           if(combination!=0){
               QTextCursor tmpCursor = textEdit->textCursor();
               tmpCursor.movePosition(QTextCursor::Up, QTextCursor::KeepAnchor);
               textEdit->setTextCursor(tmpCursor);
           }
           else{
               QTextCursor tmpCursor = textEdit->textCursor();
               tmpCursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor);
               textEdit->setTextCursor(tmpCursor);
           }
       }
       else if(event->key() == Qt::Key_Down){
           if(combination!=0){
               QTextCursor tmpCursor = textEdit->textCursor();
               tmpCursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor);
               textEdit->setTextCursor(tmpCursor);
           }
           else{
               QTextCursor tmpCursor = textEdit->textCursor();
               tmpCursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor);
               textEdit->setTextCursor(tmpCursor);
           }
       }
       else if((event->key() == Qt::Key_Shift)){
           this->combination += event->key();
       }
       else if(event->key()==Qt::Key_Delete){
           if(textEdit->textCursor().hasSelection()){
               auto size = textEdit->textCursor().selectedText().size();
               int size2 = size+1;
               QString msg = "MULTI   "+supportSize(size2);
               qDebug()<<msg;
               QTcpSocket *socket = MyTcpSocket::socket;

               if(socket->write(msg.toUtf8()) != msg.size())
                   QMessageBox::warning(this,"Multi","Impossible to communicate with server");

               if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                   textEdit->textCursor().removeSelectedText();
                  auto index = textEdit->textCursor().position();// - 1;
                  for(auto i = 0; i<size;i++){
                      localErase(index);
                  }
               }
                else{
                   textEdit->textCursor().removeSelectedText();
                   auto index = textEdit->textCursor().position();
                   for(auto i = 0; i<size;i++){
                       localErase(index);
                    }
               }
           }
           else{
                   auto index = textEdit->textCursor().position();
                   if(!textEdit->textCursor().atEnd()){
                       textEdit->textCursor().deleteChar();
                       localErase(index);
                   }
                   else{
                       event->ignore();
                   }
           }
       }
       else if(event->key()==Qt::Key_Cancel){
       }
       else if(event->key()==Qt::Key_Control){
           this->keyReleaseEvent(event);
           event->ignore();
       }
       else{
           auto index = textEdit->textCursor().position()-1;
           event->ignore();
       }
}

       r = textEdit->document()->lineCount();
       numRow->setNum(r);
       c = textEdit->textCursor().columnNumber();
       numColumn->setNum(c);
       w = textEdit->toPlainText().split(QRegExp("(\\s|\\n|\\r)+") , QString::SkipEmptyParts).count();
       numWords->setNum(w);
       numC = textEdit->toPlainText().count();
       numCharacters->setNum(numC);

}

void TextEdit::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Shift){
        combination = 0;
    }

}


void TextEdit::myPaste()
{
    qDebug()<<"myPaste ";
    const QMimeData *md = QApplication::clipboard()->mimeData();

    int s = md->text().size();
    //qDebug()<<"Paste:" <<s;
    if (md) actionPaste->setEnabled(md->hasText());

    if(textEdit->textCursor().hasSelection()){
        int size = textEdit->textCursor().selectedText().size();
        QString msg = "MULTI   "+supportSize(size+s);
        qDebug()<<msg;
        QTcpSocket *socket = MyTcpSocket::socket;
        if(socket->write(msg.toUtf8()) != msg.size())
            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

        qDebug()<<"Selection";
        if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
            textEdit->textCursor().removeSelectedText();
           auto index = textEdit->textCursor().position();// - 1;
           for(auto i = 0; i<size;i++){
               localErase(index);
           }
        }
         else{
            textEdit->textCursor().removeSelectedText();
            auto index = textEdit->textCursor().position();
            for(auto i = 0; i<size;i++){
                localErase(index);
             }
        }
        //textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
        textEdit->setTextColor(this->colors.at(_siteId-1));
        //textEdit->insertPlainText(md->text());
        //auto index = textEdit->textCursor().position()-s;
        for(auto i = 0; i<s; i++){
            textEdit->insertPlainText(md->text().at(i));
            auto index = textEdit->textCursor().position()-1;
            auto pos = localInsert(index);
            index++;
        }
    }
    else{
        //textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
        textEdit->setTextColor(this->colors.at(_siteId-1));
        //textEdit->insertPlainText(md->text());
        //auto index = textEdit->textCursor().position()-s;
        QString msg = "MULTI   "+supportSize(s);
        qDebug()<<msg;
        QTcpSocket *socket = MyTcpSocket::socket;
        if(socket->write(msg.toUtf8()) != msg.size())
            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

        for(auto i = 0; i<s; i++){
            textEdit->insertPlainText(md->text().at(i));
            auto index = textEdit->textCursor().position()-1;
            auto pos = localInsert(unsigned (index));
            index++;
        }

    }

    r = textEdit->document()->lineCount();
    numRow->setNum(r);
    c = textEdit->textCursor().columnNumber();
    numColumn->setNum(c);
    w = textEdit->toPlainText().split(QRegExp("(\\s|\\n|\\r)+") , QString::SkipEmptyParts).count();
    numWords->setNum(w);
    numC = textEdit->toPlainText().count();
    numCharacters->setNum(numC);

}

void TextEdit::myCut(){

    qDebug()<<"myCut ";

    if(textEdit->textCursor().hasSelection()){
        auto size = textEdit->textCursor().selectedText().size();
        QString msg = "MULTI   "+supportSize(size);
        qDebug()<<msg;
        QTcpSocket *socket = MyTcpSocket::socket;

        if(socket->write(msg.toUtf8()) != msg.size())
            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

        QApplication::clipboard()->setText(textEdit->textCursor().selectedText());
        if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
            textEdit->textCursor().removeSelectedText();
           auto index = textEdit->textCursor().position();
           for(auto i = 0; i<size;i++){
               localErase(index);
           }
        }
         else{
            textEdit->textCursor().removeSelectedText();
            auto index = textEdit->textCursor().position();
            for(auto i = 0; i<size;i++){
                localErase(index);
            }
        }
    }
    r = textEdit->document()->lineCount();
    numRow->setNum(r);
    c = textEdit->textCursor().columnNumber();
    numColumn->setNum(c);
    w = textEdit->toPlainText().split(QRegExp("(\\s|\\n|\\r)+") , QString::SkipEmptyParts).count();
    numWords->setNum(w);
    numC = textEdit->toPlainText().count();
    numCharacters->setNum(numC);

}

void TextEdit::myUndo()
{
    qDebug()<<"MyUndo ";
    int previousIndex = textEdit->toPlainText().size();
    textEdit->undo();
    int actualIndex = textEdit->toPlainText().size();
    int cursorPosition = textEdit->textCursor().position();
    qDebug()<<"cursorposition"<<cursorPosition;
    if(previousIndex > actualIndex){
        //precedente maggiore dell'attuale quindi vado verso sinistra => local erase
        qDebug()<<"undo1";
        int size = previousIndex - actualIndex;
        QString msg = "MULTI   "+supportSize(size);
        //QString msg = "UNDOREDO"+supportSize(size);
        qDebug()<<msg;
        QTcpSocket *socket = MyTcpSocket::socket;

        if(socket->write(msg.toUtf8()) != msg.size())
            QMessageBox::warning(this,"Multi","Impossible to communicate with server");
        for (auto i = 0; i<size;i++){
            //previousIndex--;
            undoString.push_back(symbols[cursorPosition].getPos().back());
            qDebug()<<"id"<<symbols[cursorPosition].getPos().back();
            localErase(cursorPosition);
        }
        qDebug()<<"undostring "<<undoString;
    }
    else{
        //vado verso destra local insert
        int size = actualIndex - previousIndex;
        qDebug()<<"actualIndex"<<actualIndex;
        qDebug()<<"previousIndex"<<previousIndex;
        qDebug()<<"insertString"<<insertString;
        QString msg = "MULTI   "+supportSize(size);
        qDebug()<<msg;
        QTcpSocket *socket = MyTcpSocket::socket;

        if(socket->write(msg.toUtf8()) != msg.size())
            QMessageBox::warning(this,"Multi","Impossible to communicate with server");
        int initialSize = insertString.size();
        for(auto i = 0; i<size;i++){
            //auto pos = localInsert(previousIndex);
            auto pos = localRedo(previousIndex, insertString[initialSize-size]);
            previousIndex++;
            qDebug()<<"undo2"<<pos;
            insertString.removeAt(initialSize-size);
        }
        qDebug()<<"insertString"<<insertString;

    }
    r = textEdit->document()->lineCount();
    numRow->setNum(r);
    c = textEdit->textCursor().columnNumber();
    numColumn->setNum(c);
    w = textEdit->toPlainText().split(QRegExp("(\\s|\\n|\\r)+") , QString::SkipEmptyParts).count();
    numWords->setNum(w);
    numC = textEdit->toPlainText().count();
    numCharacters->setNum(numC);

}

void TextEdit::myRedo()
{
    qDebug()<<"myRedo";
    qDebug()<<"insertString"<<insertString;
    //int previousIndex = textEdit->textCursor().position();
    int previousIndex = textEdit->toPlainText().size();
    //textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
    textEdit->setTextColor(this->colors.at(_siteId-1));

    textEdit->redo();
    int cursorPosition = textEdit->textCursor().position();
    int actualIndex = textEdit->toPlainText().size();
    if(previousIndex > actualIndex){
        //precedente maggiore dell'attuale quindi vado verso sinistra => local erase
        int size = previousIndex - actualIndex;
        QString msg = "MULTI   "+supportSize(size);
        qDebug()<<msg;
        QTcpSocket *socket = MyTcpSocket::socket;

        if(socket->write(msg.toUtf8()) != msg.size())
            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

        for (auto i = 0; i<size;i++){
            insertString.push_back(symbols.at(cursorPosition).getPos().back());
            localErase(cursorPosition);
            qDebug()<<"redo1"<<i;
        }
        qDebug()<<"insertString"<<insertString;
    }
    else{
        //vado verso destra local insert
        int size = actualIndex - previousIndex;
        QString msg = "MULTI   "+supportSize(size);
        qDebug()<<msg;
        QTcpSocket *socket = MyTcpSocket::socket;

        if(socket->write(msg.toUtf8()) != msg.size())
            QMessageBox::warning(this,"Multi","Impossible to communicate with server");
        int j = cursorPosition-size;
        for(auto i = 0; i<size;i++){
            //auto pos = localInsert(j+i);
            //auto pos = localRedo(j+i,undoString[i]);
            auto pos = localRedo(j+i,undoString.back());
            previousIndex++;
            qDebug()<<"redo2"<<pos;
            undoString.pop_back();
        }
        qDebug()<<"undostring "<<undoString;
    }
    r = textEdit->document()->lineCount();
    numRow->setNum(r);
    c = textEdit->textCursor().columnNumber();
    numColumn->setNum(c);
    w = textEdit->toPlainText().split(QRegExp("(\\s|\\n|\\r)+") , QString::SkipEmptyParts).count();
    numWords->setNum(w);
    numC = textEdit->toPlainText().count();
    numCharacters->setNum(numC);

}

void TextEdit::printPos()
{
    auto s = symbols.size();
    //size_t
    for(size_t i = 0; i<s;i++){
        qDebug()<<symbols[i].getPos();
    }

}

void TextEdit::showUri(QString str)
{
    uri->clear();
    uri->setText(str);
}

void TextEdit::on_share_clicked()
{
    QString username = MainWindow::username;
    QTcpSocket *s = MyTcpSocket::socket;
    QString msg = "GETURI  "+this->owner+";"+this->windowTitle();
    qDebug()<<msg;

    if(s->write(msg.toUtf8()) != msg.size())
        QMessageBox::warning(this,"URI","Impossible to communicate with server");

}

void TextEdit::on_copyUri_clicked()
{
    if(uri->text().length()==32){
    QApplication::clipboard()->setText(uri->text());
    QMessageBox::information(this,"URI"," URI copied");
    }
    else{
        QMessageBox::information(this,"URI", "Click on Share before Copy");
    }

}

void TextEdit::addUserLabel(QString username)
{
    qDebug()<<"addUserLabel ";

    QLabel *tempLabel = new QLabel(username, nullptr);
    //tempLabel->setStyleSheet("QLabel{color: blue;" "background-color: light-grey;}");
    tempLabel->setAutoFillBackground(true);
    QColor color = colors.at(_siteId-1);
    QString values = QString::number(color.red())+","+QString::number(color.green())+","+QString::number(color.blue());

    userList->item(0)->setTextColor(colors.at(_siteId-1));

    tempLabel->setStyleSheet("QLabel { background-color: rgba("+values+"); }");
    tempLabel->setFocusPolicy(Qt::NoFocus);
    //ut->addWidget(tempLabel);
    usersVector.push_back(tempLabel);

}

void TextEdit::addUser(QString& username, int id)
{
    qDebug()<<"addUser ";
    userConnessi.push_back(QPair<QString,int>(username,id));
    QLabel *tempLabel = new QLabel(username, nullptr);
    //tempLabel->setAutoFillBackground(true);
    QColor color = colors.at(id-1);
    QString values = QString::number(color.red())+","+QString::number(color.green())+","+QString::number(color.blue());
    tempLabel->setStyleSheet("QLabel { background-color: rgba("+values+"); }");
    tempLabel->setFocusPolicy(Qt::NoFocus);
    //ut->addWidget(tempLabel);
    usersVector.push_back(tempLabel);

    QLabel* cursorLabel = new QLabel(textEdit);
    labelFixedSize = QSize(1,33);
    QColor color2 = colors.at(id-1);
    QString values2 = QString::number(color.red())+","+QString::number(color.green())+","+QString::number(color.blue());
    //cursorLabel = new QLabel(textEdit);
    cursorLabel->setStyleSheet("QLabel { background-color: rgba("+values2+"); }"); //assegnargli il colore
    //cursorLabel->setFixedSize(textEdit->cursorRect().size().width()*20, textEdit->cursorRect().size().height());
    cursorLabel->setFixedSize(labelFixedSize.width()*3, labelFixedSize.height());
    //cursorLabel->setGeometry(textEdit->size().width()-1, textEdit->size().height()-1, textEdit->cursor().pixmap().width(), textEdit->cursor().pixmap().height());
    cursorLabel->move(textEdit->cursorRect().left(), textEdit->cursorRect().top());
    cursorLabel->setVisible(true);
    cursorLabel->show();

    //this->userNumber++;
    //this->userActive->setNum(userNumber);

    labelVector.push_back(QPair<QLabel*,int>(cursorLabel,id));

    userList->addItem(username);
    if(_siteId<id)
        userList->item(id-1)->setTextColor(colors.at(id-1));
    else
        userList->item(id)->setTextColor(colors.at(id-1));

    timer->start(250);
    //labelFixedSize = QSize(1,33);

}

void TextEdit::setSiteID(int siteID)
{
    this->_siteId = siteID;
}

int TextEdit::getSiteID()
{
    return this->_siteId;
}

