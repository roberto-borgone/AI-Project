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

//#include <algorithm>
//#include <random>

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

    //mainwindow = new QMainWindow(this);
    this->counter = 0;
    combination = 0;
    //this->_siteId = 1;
    this->userNumber = 1;
    this->percZoom = 100;
    this->flag = true;

    //this->_siteId = 1;
    //this->userNumber = this->_siteId;

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
         //QMenu *helpMenu = mainwindow->menuBar()->addMenu(tr("Help"));

        helpMenu->addAction(tr("About"), this, &TextEdit::about);

        helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
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

    /*
    qDebug()<<"before shuffle"<<colors;
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(colors), std::end(colors), rng);
    qDebug()<<"after shuffle"<<colors;
    */
}

TextEdit::~TextEdit()
{
    //this->releaseKeyboard();
    //textEdit->releaseKeyboard();
    //disconnect();
    qDebug()<<"destroying";

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
    //QLabel* tempLabel;
    for(auto pair : labelVector){
        if(pair.second==id){
            qDebug()<<"x e y "<<pair.first->x()<<" "<<pair.first->y();
            pair.first->move(x, y);
            qDebug()<<"x e y "<<pair.first->x()<<" "<<pair.first->y();
            break;
            //tempLabel=pair.first;
        }
    }
    timer->start(500);

    //tempLabel->move(tempLabel->width()*2,tempLabel->height());
    //tempLabel->move(x,y);


}

void TextEdit::removeLabel(QString &username, int id) //, int x, int y
{
    qDebug()<<"sono in removelabel "<<usersVector.size();
    //tolgo le label da uservector e labelvector
    for(auto it=usersVector.begin(); it!= usersVector.end();it++){
        if((*it)->text()==username){
            qDebug()<<"nell'il"<<(*it)->text();
            delete (*it);
            (*it) = nullptr;
            usersVector.erase(it);
            qDebug()<<"esco dall'if";
            break;
        }
    }
    int i = 0;
    qDebug()<<"è colpa del labelvector?";
    qDebug()<<"prima del secondo for "<<labelVector.size();
    for(auto pair : labelVector){
        if(pair.second == id){
            labelVector.remove(i);
            delete pair.first;
            pair.first = nullptr;

        }
        i++;
    }
    qDebug()<<"sono prima dell'ultimo for ";
    int j = 0;
    for(auto pair : userConnessi){
        if(pair.first == username){
            userConnessi.remove(j);
        }
        j++;
    }
    this->userNumber--;
    this->userActive->setNum(userNumber);
    qDebug()<<"esco dal removelabel";

    //this->updateLabel(id, x, y);
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
        qDebug()<<"sono in closeEvent";
        if (maybeSave()){            
            this->disconnect();
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

    //METTERE LENTE INGRANDIMENTO PIU E MENO
    const QIcon ZoomInIcon = QIcon::fromTheme("zoom-in", QIcon(rsrcPath + "/zoomin.png"));
    QAction *actionZoomIn;
    actionZoomIn = menu->addAction(ZoomInIcon, tr("&Zoom In"),this, &TextEdit::myZoomIn);
    actionZoomIn->setShortcut(QKeySequence::ZoomIn);
    tb->addAction(actionZoomIn);
    const QIcon ZoomOutIcon = QIcon::fromTheme("zoom-out", QIcon(rsrcPath + "/zoomout.png"));
    QAction *actionZoomOut;
    actionZoomOut = menu->addAction(ZoomOutIcon, tr("&Zoom Out"),this, &TextEdit::myZoomOut);
    actionZoomOut->setShortcut(QKeySequence::ZoomOut);
    tb->addAction(actionZoomOut);

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
    share = new QPushButton("Share",nullptr);
    share->setFocusPolicy(Qt::NoFocus);

    connect(share, &QAbstractButton::clicked, this, &TextEdit::on_share_clicked);

    homepage = new QPushButton("Go to Homepage",nullptr);
    homepage->setFocusPolicy(Qt::NoFocus);

    connect(homepage, &QAbstractButton::clicked,this, &TextEdit::on_homepageButton_clicked);

    uri = new QLabel("Click on Share to get the URI", nullptr);
    //uri->setReadOnly(true);
    uri->setFocusPolicy(Qt::NoFocus);

    copyUri = new QPushButton("Copy", nullptr);
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

    ut = addToolBar(tr("Users Active"));
    //ut = addToolBar(tr("Users Active"));
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(ut);
    ut->setMovable(false);

    userLabel = new QLabel(tr("Users Connected: "));
    userActive = new QLabel();
    userActive->setNum(userNumber);
    ut->addWidget(userLabel);
    ut->addWidget(userActive);
    userActive->setFocusPolicy(Qt::NoFocus);
    userActive->setFixedSize(ut->size());

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

    row = new QLabel (tr("Row: "));
    r = textEdit->document()->lineCount();
    numRow = new QLabel();
    numRow->setNum(r);
    ltb->addWidget(row);
    ltb->addWidget(numRow);
    ltb->addSeparator();

    column = new QLabel(tr("Column: "));
    c = textEdit->textCursor().columnNumber();
    numColumn = new QLabel();
    numColumn->setNum(c);
    ltb->addWidget(column);
    ltb->addWidget(numColumn);
    ltb->addSeparator();

    words = new QLabel(tr("Words: "));
    w = textEdit->toPlainText().split(QRegExp("(\\s|\\n|\\r)+") , QString::SkipEmptyParts).count();
    numWords = new QLabel();
    numWords->setNum(w);
    ltb->addWidget(words);
    ltb->addWidget(numWords);
    ltb->addSeparator();

    characters = new QLabel(tr("Characters: "));
    numC = textEdit->document()->characterCount();
    numCharacters = new QLabel();
    numCharacters->setNum(numC);
    ltb->addWidget(characters);
    ltb->addWidget(numCharacters);
    ltb->addSeparator();

    zoom = new QLabel(tr("Zoom: "));
    numZoom = new QLabel();
    numZoom->setNum(percZoom);
    ltb->addWidget(zoom);
    ltb->addWidget(numZoom);
    ltb->addSeparator();

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
        qDebug()<<"sono in HTML";
    } else {
        //sub_str = QString::fromLocal8Bit(arrayByte);
        //textEdit->setPlainText(str);
        QStringList charList = str.split(";,");
        QStringList fList = strFont.split(";");

        for(auto i=0; i<charList.size()-1; i++) {
            QStringList fontList = fList[i].split(",");
            QFont font = QFont(fontList[0],fontList[1].toDouble(),fontList[4].toInt(),fontList[5].toInt());
            font.setUnderline(fontList[6].toInt());
            textEdit->setFontFamily(font.family());
            textEdit->setFontPointSize(font.pointSize());
            textEdit->setFontItalic(font.italic());
            textEdit->setFontUnderline(font.underline());
            textEdit->setFontWeight(font.weight());
            textEdit->setTextBackgroundColor(colors.at(fontList[10].toInt()-1));
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
    qDebug()<<"change format";
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

    //alignmentChanged(textEdit->alignment());
    /*
    this->updateLabel(this->getSiteID(), textEdit->cursorRect().left(), textEdit->cursorRect().top());
    qDebug()<<"textcursor position"<<textEdit->textCursor().position();
    QString finalStr = MainWindow::username+";,"+this->windowTitle()+";,"+QString::number(getSiteID())+";,"+QString::number(textEdit->cursorRect().left())+";,"+QString::number(textEdit->cursorRect().top())+";,"+";,"+";,"+";,"+this->getOwner();
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
    qDebug()<<"size"<<supportSize;
    QString msg = "CHANGE  "+supportSize+finalStr;
    qDebug()<<msg;

    QTcpSocket *socket = MyTcpSocket::socket;

    if(socket->write(msg.toUtf8()) != msg.size())
        QMessageBox::warning(this,"Multi","Impossible to communicate with server");
    QTime dieTime= QTime::currentTime().addMSecs(100); //5
         while (QTime::currentTime() < dieTime)
             QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    */



    /*
    QTextList *list = textEdit->textCursor().currentList();
    if (list) {
        switch (list->format().style()) {
        case QTextListFormat::ListDisc:
            comboStyle->setCurrentIndex(1);
            break;
        case QTextListFormat::ListCircle:
            comboStyle->setCurrentIndex(2);
            break;
        case QTextListFormat::ListSquare:
            comboStyle->setCurrentIndex(3);
            break;
        case QTextListFormat::ListDecimal:
            comboStyle->setCurrentIndex(4);
            break;
        case QTextListFormat::ListLowerAlpha:
            comboStyle->setCurrentIndex(5);
            break;
        case QTextListFormat::ListUpperAlpha:
            comboStyle->setCurrentIndex(6);
            break;
        case QTextListFormat::ListLowerRoman:
            comboStyle->setCurrentIndex(7);
            break;
        case QTextListFormat::ListUpperRoman:
            comboStyle->setCurrentIndex(8);
            break;
        default:
            comboStyle->setCurrentIndex(-1);
            break;
        }
    } else {
        int headingLevel = textEdit->textCursor().blockFormat().headingLevel();
        comboStyle->setCurrentIndex(headingLevel ? headingLevel + 8 : 0);
    }
    */
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

     QMessageBox::about(this, tr("About"), tr("This example demonstrates Qt's "
        "rich text editing facilities in action, providing an example "
        "document for you to experiment with."));

}

void TextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    qDebug()<<"mergeFormat";
    QTextCursor cursor = textEdit->textCursor();
    QFont currentFont = textEdit->currentFont();
    qDebug()<<"current font"<<currentFont;

    if (!cursor.hasSelection()){
        cursor.select(QTextCursor::WordUnderCursor);
        textEdit->mergeCurrentCharFormat(format);
        qDebug()<<"no Selection";
        textEdit->currentFont() = format.font();
    }
    else{

        int s = textEdit->textCursor().selectedText().size();
        QString supportSize;
        if(s<10){
            supportSize = QString::number(s).insert(0,"00");
        }
        else if(s<100){
            supportSize = QString::number(s).insert(0,"0");
        }
        else {
            supportSize = QString::number(s);
        }
        QString msg = "MULTI   "+supportSize;
        qDebug()<<msg;
        QTcpSocket *socket = MyTcpSocket::socket;

        if(socket->write(msg.toUtf8()) != msg.size())
            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

        if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
            qDebug()<<"sinistra verso destra";
            qDebug()<<"format"<< format.font().toString();
            fontChanged(format.font());
            //colorChanged(format.foreground().color());
            cursor.mergeCharFormat(format);
            cursor.charFormat();
            textEdit->mergeCurrentCharFormat(format);
            qDebug()<<"merge "<<textEdit->currentCharFormat().font().toString();
            textEdit->currentFont() = format.font();

            for(int i = textEdit->textCursor().selectionStart(); i< textEdit->textCursor().selectionEnd(); i++){
                Symbol currentSymbol = symbols.at(i);
                currentSymbol.setFont(textEdit->currentFont());
                symbols[i].setFont(textEdit->currentFont());
                //qDebug()<<"currentFont1 "<<currentSymbol.getFont().toString();
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
            }
        }
        else{
            qDebug()<<"destra verso sinistra";
            qDebug()<<"format"<< format.font().toString();
            fontChanged(format.font());
            //colorChanged(format.foreground().color());

            qDebug()<<"position iniziale"<<textEdit->textCursor().position();
            qDebug()<<"anchor iniziale"<<textEdit->textCursor().anchor();

            QTextCursor tmpCursor;
            tmpCursor = textEdit->textCursor();
            tmpCursor.setPosition(textEdit->textCursor().position()); //KeepAnchor
            tmpCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, s);
            qDebug()<<"position finale "<<tmpCursor.position();
            qDebug()<<"anchor finale "<<tmpCursor.anchor();
            textEdit->setTextCursor(tmpCursor);

            textEdit->textCursor().mergeCharFormat(format);
            textEdit->mergeCurrentCharFormat(format);
            qDebug()<<"merge "<<textEdit->currentCharFormat().font().toString();
            textEdit->currentFont() = format.font();

            for(int i = textEdit->textCursor().selectionStart(); i< textEdit->textCursor().selectionEnd(); i++){
                //qDebug()<<"currentFont2 "<<textEdit->currentFont();
                Symbol currentSymbol = symbols.at(i);           
                currentSymbol.setFont(textEdit->currentFont());
                symbols[i].setFont(textEdit->currentFont());
                //qDebug()<<"currentSymbol "<<currentSymbol.getFont().toString();
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
        //flag = false;
        this->releaseKeyboard();
        uri->grabKeyboard();
        disconnect();
        //this->close();
        h->show();
        //flag = true;

    }
}


std::vector<int> TextEdit::localInsert(unsigned int index)
{
    std::vector<int> pos;
    auto i = this->symbols.size();            //vedo il numero di elementi presenti in symbol
    //if (index > i) throw std::exception();
    //if(index < 0) throw std::exception();
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
    pos.push_back(this->getSiteID());
    //QString s = textEdit->toHtml();
    QString fullText = textEdit->toPlainText();
    QString charValue = fullText.at(int(index));     //prendo l'ultimo carattere inserito
    //QString charValue = text;
    //passo alla local insert un secondo parametro event->text() e da questo mi ricavo il carattere inserito

    qDebug()<<"Full Text"<<fullText;
    qDebug()<<"Last char"<<charValue;

    this->counter++;
    //Symbol symbol = Symbol(_siteId, counter, charValue, pos, font);
    //Symbol symbol = Symbol(MainWindow::username.toInt(), counter, s.toStdString(), pos);
    Symbol symbol = Symbol(_siteId, counter, charValue, pos, textEdit->currentFont());
    qDebug()<<"Symbol "<<symbol.getId()<<" "<<symbol.getCounter()<<" "<<symbol.getValue()<<" "<<symbol.getPos()<<" "<<symbol.getFont();
    this->symbols.insert(this->symbols.cbegin()+index,symbol);

    printPos();

    QString username = MainWindow::username;
    QTcpSocket *s = MyTcpSocket::socket;
    //valutare velocità del server altrimenti togliere username
    //QString finalStr = username+";;"+this->windowTitle()+";;"+ symbol.toStr();
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
    //qDebug()<<"size"<<supportSize;
    QString msg = "LI      "+supportSize+finalStr;
    qDebug()<<msg;

    if(s->write(msg.toUtf8()) != msg.toUtf8().size())
        QMessageBox::warning(this,"Local Insert","Impossible to communicate with server");


    //qDebug()<<"update? ";

    return pos;
}

std::vector<Symbol> TextEdit::getSymbol()
{
     return this->symbols;
}

void TextEdit::setSymbol(int index, Symbol s)
{
    //this->symbols.push_back(s);
    this->symbols.insert(symbols.cbegin()+index,s);
}

void TextEdit::removeSymbol(int index, int id, int x, int y){

    qDebug()<<"Sono nella removeSymbol";
    qDebug()<<"Index: "<<index;

    this->symbols.erase(symbols.cbegin()+index);
    for(auto s : symbols)
        qDebug()<<s.toStr();
    int textposition = textEdit->textCursor().position();
    qDebug()<<"Textposition prima: "<<textposition;

    /*
    QString buffer = textEdit->toPlainText();
    buffer.remove(index,1);
    for(auto c : buffer) {
        textEdit->setTextBackgroundColor(colors.at(id-1));
        textEdit->insertPlainText(c);
    }
    textEdit->setText(buffer);*/
    if (index>textposition) {
        for(int i=0;i<index-textposition;i++){
            textEdit->moveCursor(QTextCursor::Right,QTextCursor::MoveAnchor);
        }
    }
    else if (index<textposition) {
        //for(int j=0;j<index-textposition;j++){
        for(int j=0;j<textposition-index;j++){
            textEdit->moveCursor(QTextCursor::Left,QTextCursor::MoveAnchor);
        }
    }
    qDebug()<<"Textposition dopo: "<<textEdit->textCursor().position();

    //textEdit->setTextBackgroundColor(colors.at(id-1));
    textEdit->textCursor().deleteChar();

    QTextCursor cursor (textEdit->textCursor());
    //if(textposition >= index) cursor.setPosition(textposition-1,QTextCursor::MoveAnchor);
    //else cursor.setPosition(textposition,QTextCursor::MoveAnchor);
    //if(textposition == int (symbols.size())) cursor.setPosition(textposition-1,QTextCursor::MoveAnchor);
    //else cursor.setPosition(textposition,QTextCursor::MoveAnchor);
    cursor.setPosition(textposition,QTextCursor::MoveAnchor);
    qDebug()<<"TextCursor dopo settext"<<textEdit->textCursor().position()<<" textposition "<<textposition;
    textEdit->setTextCursor(cursor);
    if (index < textposition && index!= int (symbols.size())){
        textEdit->moveCursor(QTextCursor::Left);
    }

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
    qDebug()<<"sono nel setfontsymbol";
    symbols.at(index).setFont(f);
    //symbols[index].setFont(f);
    /*QTextCharFormat format;
    format.setFont(f);
    textEdit->mergeCurrentCharFormat(format);
    textEdit->currentFont()=format.font();*/
    //for(auto i=0; i<symbols.size()-1; i++) {
        //QStringList fontList = fList[i].split(",");
        //QFont font = QFont(fontList[0],fontList[1].toDouble(),fontList[4].toInt(),fontList[5].toInt());
        //font.setUnderline(fontList[6].toInt());
    textEdit->setFontFamily(f.family());
    textEdit->setFontPointSize(f.pointSize());
    textEdit->setFontItalic(f.italic());
    textEdit->setFontUnderline(f.underline());
    textEdit->setFontWeight(f.weight());
    //symbols[index].setFont(f);
}

void TextEdit::disconnect()
{
    QTcpSocket *s = MyTcpSocket::socket;
    QString username = MainWindow::username;

    QString msg ="CLOSE   "+username+";"+this->owner+";"+this->windowTitle();
    qDebug()<<"DISCO "<<msg;
    if(s->write(msg.toUtf8()) != msg.toUtf8().size())
        QMessageBox::warning(this,"Disconnect","Impossible to communicate with server");
}

void TextEdit::localErase(int index){
    QString er;
    std::vector<int> deletedPos;
    auto i = 0;
    if (index > int (symbols.size())) throw std::exception();
    else if(index < 0) throw  std::exception();
    else
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
    //QString c = s.at(s.size()-1);
    qDebug()<<"erase off char "<<er<<"remaining text:  "<<fullText;
    //this->counter++;
    //int counter = 1;
    //QFont f = textEdit->textCursor().charFormat().font();
    //qDebug()<<"font"<<f;
    //Symbol symbol = Symbol(MainWindow::username.toInt(), counter, er, deletedPos,f);

    this->counter--;

    Symbol symbol = Symbol(_siteId, counter, er, deletedPos, textEdit->currentFont());
    //qDebug()<<"symbol "<<symbol.getId()<<" "<<symbol.getCounter()<<" "<<symbol.getValue()<<" "<<symbol.getPos()<<" "<<symbol.getFont();
    qDebug()<<"symbol "<<symbol.getId()<<" "<<symbol.getCounter()<<" "<<symbol.getValue()<<" "<<symbol.getPos()<<" "<<symbol.getFont();

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
    qDebug()<<"size"<<supportSize;
    QString msg = "LE      "+supportSize+finalStr;
    qDebug()<<msg;

    if(s->write(msg.toUtf8()) != msg.toUtf8().size())
        QMessageBox::warning(this,"Local Erase","Impossible to communicate with server");

    //this->updateLabel(this->getSiteID(),textEdit->cursorRect().left(),textEdit->cursorRect().top());

}

/*
    QStringList fontList = data[7].split(",");
    QFont font = QFont(fontList[0],fontList[1].toInt(),fontList[4].toInt(),fontList[5].toInt());
    font.setUnderline(fontList[6].toInt());
 */


void TextEdit::insertCharacter(int index, QString value, int x, int y){

    int textposition = textEdit->textCursor().position();
    qDebug()<<"sono nella insertCharacter"<<" "<<"textposition "<<textposition;
    //QString buffer = textEdit->toPlainText();
    qDebug()<<"sono nell'ultima parte op"<<index<<" "<<value;
    //buffer.insert(index,value);
    /*QTextCursor cursortmp (textEdit->textCursor());
    cursortmp.setPosition(index,QTextCursor::MoveAnchor);
    qDebug()<<"TextCursor dopo settext"<<textEdit->textCursor().position()<<" textposition "<<textposition;
    textEdit->setTextCursor(cursor);*/
    //if(textEdit->toPlainText().size()<=index) textEdit->textCursor().setPosition(textEdit->toPlainText().size());
    //else textEdit->textCursor().setPosition(index);
    if (index>textposition) {
        for(int i=0;i<index-textposition;i++){
            textEdit->moveCursor(QTextCursor::Right,QTextCursor::MoveAnchor);
        }
    }
    else if (index<textposition) {
        //for(int j=0;j<index-textposition;j++){
        for(int j=0;j<textposition-index;j++){
            textEdit->moveCursor(QTextCursor::Left,QTextCursor::MoveAnchor);
        }
    }
    qDebug()<<"posizione testo "<<textEdit->textCursor().position();
    //textEdit->textCursor().setPosition(index+1,QTextCursor::MoveAnchor);
    textEdit->setFontFamily(symbols.at(index).getFont().family());
    textEdit->setFontPointSize(symbols.at(index).getFont().pointSize());
    textEdit->setFontItalic(symbols.at(index).getFont().italic());
    textEdit->setFontUnderline(symbols.at(index).getFont().underline());
    textEdit->setFontWeight(symbols.at(index).getFont().weight());
    int id = symbols.at(index).getId()-symbols.at(index).getCounter();
    textEdit->setTextBackgroundColor(colors.at(id-1));
    textEdit->insertPlainText(symbols.at(index).getValue());
    //textEdit->setText(buffer);
    QTextCursor cursor (textEdit->textCursor());
    cursor.setPosition(textposition,QTextCursor::MoveAnchor);
    qDebug()<<"TextCursor dopo settext"<<textEdit->textCursor().position()<<" textposition "<<textposition;
    textEdit->setTextCursor(cursor);
    if (index < textposition){
        textEdit->moveCursor(QTextCursor::Right);
    }

    this->updateLabel(symbols.at(index).getId()-symbols.at(index).getCounter(),x,y);

    r = textEdit->document()->lineCount();
       numRow->setNum(r);
       c = textEdit->textCursor().columnNumber();
       numColumn->setNum(c);
       w = textEdit->toPlainText().split(QRegExp("(\\s|\\n|\\r)+") , QString::SkipEmptyParts).count();
       numWords->setNum(w);
       numC = textEdit->document()->characterCount();

       textEdit->update();

    printPos();
}

void TextEdit::process(const QString& message){

    //QTextCursor cursor(textEdit->textCursor());
    int textposition = textEdit->textCursor().position();
    qDebug()<<"sono nella process"<<message<<" "<<"textposition "<<textposition;
    QStringList data = message.split(";,");
    QString op = data[0];
    QString username = data[1];
    //QString uri = data[2]; //??
    //QString title = data[3];
    int id = data[3].toInt();
    int counter_message = data[4].toInt();
    QString charValue = data[5];
    QStringList posList = data[6].split("|");

    std::vector<int> pos_message;
    for(auto i = 0; i<posList.size(); i++){

        pos_message.push_back(posList[i].toInt());
        qDebug()<<" entra nel primo for"<<posList[i].toInt();
    }
    QStringList fontList = data[7].split(",");
    QFont font = QFont(fontList[0],fontList[1].toInt(),fontList[4].toInt(),fontList[5].toInt());
    font.setUnderline(fontList[6].toInt());

    int left = data[9].toInt();
    int top = data[10].toInt();

    Symbol symbol = Symbol(id-counter_message, counter_message, charValue, pos_message, font);
    qDebug()<<"simbolo da cancellare"<<symbol.toStr();
    //prendere dal socket il simbolo mandato dal server e il suo corrispettivo vettore di pos
    auto size_m = pos_message.size(); //numero di cifre nel vettore frazionario del simbolo da inserire
    auto size = this->symbols.size(); //numero totale di simboli presenti
    unsigned int index = 0; //indice del vettore _symbols in cui devo inserire il nuovo simbolo contenuto nel msg
    auto idClientMessage = pos_message[size_m-1]; //ultimo elemento del vettore di pos (ID)
    std::vector<int> residualPos;
    for(size_t it = 0; it<size_m-1; it++){
        residualPos.push_back(pos_message.at(it));
    }

    bool flag=false;
    //int counter=0; //???????????
    if(op == "LI") {
        // INSERT
        qDebug()<<"entro il LI?"<<size;
        if(size==0){
            qDebug()<<"sono nel primo if op=LI";
            this->symbols.push_back(symbol); //devo inserire all'inizio se non ho inserito neanche un simbolo
        }
        else
        if(size==1){ //se ho un solo simbolo
            qDebug()<<"sono nel caso s==1"<<size_m<<" "<<this->symbols[0].getPos().size()<<" resid"<<this->symbols[0].getResidualPos();
            if(size_m >= this->symbols[0].getPos().size()){
                qDebug()<<"residual?"<<residualPos<<" "<<this->symbols[0].getResidualPos();
                if(size_m==this->symbols[0].getPos().size() && residualPos==this->symbols[0].getResidualPos()){
                    if(this->symbols[0].getPos().at(symbols[0].getPos().size()-1) < symbol.getPos().at(symbol.getPos().size()-1)) index=1;
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
                index=symbols.size();
                this->symbols.push_back(symbol); //se il primo digit del mio simbolo ÃƒÂ¨ maggiore del primo dell'ultimo simbolo inserito(che ÃƒÂ¨ il maggiore) aggiungo in coda
            }
            else if(this->symbols[0].getPos()[0]>pos_message[0]){
                index=0;
                this->symbols.insert(this->symbols.cbegin(),symbol); //se l'ultimo simbolo(il piu piccolo) ha il primo digit maggiore del primo del simbolo da inserire, lo devo mettere in testa
                //else if(this->_symbols[0].pos[0]==pos_message[0] && size_m<this->_symbols[0].pos.size()) this->_symbols.insert(this->_symbols.cbegin(),m.getSymbol());
            }
            /*
            else if(size_m==this->symbols[0].getPos().size() && residualPos==this->symbols[0].getResidualPos() &&
                    this->symbols[0].getPos().at(symbols[0].getPos().size()-1)>symbol.getPos().at(symbol.getPos().size()-1))
                this->symbols.insert(this->symbols.cbegin(),symbol);
            */
                // if(this->_symbols[0].getId()>m.getSymbol().getId()) this->_symbols.insert(this->_symbols.cbegin(),m.getSymbol());
                // else this->_symbols.insert(this->_symbols.cbegin(),m.getSymbol());
            else{
                for(size_t i=0;i<size;i++){
                    if(this->symbols[i].getPos().size() > size_m){
                        for(size_t j=0;j<size_m && flag!=true ;j++){
                            if(pos_message[j]==this->symbols[i].getPos()[j]) continue;
                            else if(pos_message[j]<this->symbols[i].getPos()[j]){
                                index=int(i);
                                flag=true;
                            }
                            else if(pos_message[j]>this->symbols[i].getPos()[j]) break;
                        }
                    }
                    if(flag==true) break;
                    if(this->symbols[i].getPos().size() <= size_m){
                        if(size_m==this->symbols[i].getPos().size() && residualPos==this->symbols[0].getResidualPos()){
                            if(this->symbols[0].getPos().at(symbols[0].getPos().size()-1) < symbol.getPos().at(symbol.getPos().size()-1)){
                                index=int(i);
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
                        for(size_t j=0;j<this->symbols[i].getPos().size() && flag!=true ;j++){
                            if(pos_message[j]==this->symbols[i].getPos()[j]) continue;
                            else if(pos_message[j]<this->symbols[i].getPos()[j]){
                                index=int (i);
                                flag=true;
                            }
                            else if(pos_message[j]>this->symbols[i].getPos()[j]) break;
                        }
                    }
                    if(flag==true) break;
                }
                if(flag==true) this->symbols.insert(this->symbols.cbegin()+index,symbol);
                else this->symbols.push_back(symbol);
            }
      }

       //inserire il carattere nel textedit alla posizione index
       //muovere il cursore e inserire
        /*
        muovendo il cursore
       int initialCursorPosition =  textEdit->textCursor().position();
       textEdit->textCursor().setPosition(index);
       textEdit->insertPlainText(symbol.getValue());
       if(initialCursorPosition > index){
            textEdit->textCursor().setPosition(initialCursorPosition);
       }
       else{
            textEdit->textCursor().setPosition(initialCursorPosition+1);
       }

       */

       // mi salvo una copia di quello che Ã¨ stato scritto nel buffer e faccio la insert


       QString buffer = textEdit->toPlainText();
       qDebug()<<"sono nell'ultima parte op"<<index<<" "<<symbol.getValue();
       buffer.insert(index,symbol.getValue());
       textEdit->setText(buffer);
       QTextCursor cursor (textEdit->textCursor());
       cursor.setPosition(textposition,QTextCursor::MoveAnchor);
       qDebug()<<"TextCursor dopo settext"<<textEdit->textCursor().position()<<" textposition "<<textposition;
       textEdit->setTextCursor(cursor);
       if (index < textposition){
           textEdit->moveCursor(QTextCursor::Right);
       }

       //textEdit->setTextCursor(cursor);
       //textEdit->textCursor().setPosition(textposition, QTextCursor::MoveAnchor);
       //textEdit->textCursor().movePosition(QTextCursor::MoveOperation() ,QTextCursor::MoveAnchor,textposition);
    }
    else{
        //DELETE
        qDebug()<<"sono nella delete della process";
        for(size_t i=0;i<size;i++){
            qDebug()<<"simblo nel for"<<symbols[i].toStr();
            //if(pos_message==this->symbols[i].getPos() && symbol.getId()==this->symbols[i].getId() && symbol.getCounter()==this->symbols[i].getCounter() ){
            if(pos_message==this->symbols[i].getPos()){
                index=i;
                qDebug()<<"indice "<<index<<"simbols "<<symbols[i].getValue();
                this->symbols.erase(this->symbols.cbegin()+i);
                break;
            }

        }
        // mi salvo una copia di quello che Ã¨ stato scritto nel buffer e faccio la remove
        QString buffer = textEdit->toPlainText();
        buffer.remove(index,1);
        textEdit->setText(buffer);
        QTextCursor cursor (textEdit->textCursor());
        if(textposition == symbols.size()) cursor.setPosition(textposition-1,QTextCursor::MoveAnchor);
        else cursor.setPosition(textposition,QTextCursor::MoveAnchor);
        cursor.setPosition(textposition,QTextCursor::MoveAnchor);
        qDebug()<<"TextCursor dopo settext"<<textEdit->textCursor().position()<<" textposition "<<textposition;
        textEdit->setTextCursor(cursor);
        if (index < textposition && index!= symbols.size()){
            textEdit->moveCursor(QTextCursor::Left);
        }

        //textEdit->setTextCursor(cursor);
    }
    this->updateLabel(id,left,top);

}

void TextEdit::keyPressEvent(QKeyEvent *event){

    //QTextEdit::keyPressEvent(event);
    QWidget::keyPressEvent(event);
    QMainWindow::keyPressEvent(event);
    qDebug()<<"keypressevent";

       //if((event->key()>=Qt::Key_A && event->key()<=Qt::Key_Z)){// || (event->key()>=Qt::Key_Exclam && event->key()<=Qt::Key_ydiaeresis)){
    if(event->modifiers() & Qt::AltModifier) { //solo alt (alt 1, alt 2, ...)
        if(event->key() == Qt::Key_1 || event->key() == Qt::Key_2 || event->key() == Qt::Key_3 || event->key() == Qt::Key_4|| event->key() == Qt::Key_5 || event->key() == Qt::Key_6 || event->key() == Qt::Key_7 || event->key() == Qt::Key_8 || event->key() == Qt::Key_9 || event->key() == Qt::Key_0)
            event->ignore();
        else {
           if(event->modifiers() & Qt::ControlModifier){ //control + alt
                if(event->key()==Qt::Key_At){
                    if(textEdit->textCursor().hasSelection()){
                        auto size = textEdit->textCursor().selectedText().size();
                        qDebug()<<"Selection";

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
                        QString msg = "MULTI   "+supportSize;
                        qDebug()<<msg;
                        QTcpSocket *socket = MyTcpSocket::socket;

                        if(socket->write(msg.toUtf8()) != msg.size())
                            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

                        if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                           textEdit->textCursor().removeSelectedText();
                           auto index = textEdit->textCursor().position();// - 1;
                           for(auto i = 0; i<size;i++){
                               qDebug()<<"sinistra verso destra"<<i<<" "<<index;
                               localErase(index);
                               //index--;
                           }
                        }
                        else{
                            textEdit->textCursor().removeSelectedText();
                            auto index = textEdit->textCursor().position();
                            for(auto i = 0; i<size;i++){
                                qDebug()<<"destra verso sinistra"<<i<<index;
                                localErase(index);
                             }
                        }
                    }
                    qDebug()<<"----------------Site id: "<<_siteId;
                    textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
                    textEdit->insertPlainText(event->text());
                    auto index = textEdit->textCursor().position()-1;
                    auto pos = this->localInsert(index);
                    //textEdit->insertPlainText(event->text());
                    qDebug()<<index<<" "<<pos<<"event: "<<event->key();
                }
                else if(event->key()==Qt::Key_BracketLeft){
                    if(textEdit->textCursor().hasSelection()){
                        auto size = textEdit->textCursor().selectedText().size();
                        qDebug()<<"Selection";

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
                        QString msg = "MULTI   "+supportSize;
                        qDebug()<<msg;
                        QTcpSocket *socket = MyTcpSocket::socket;

                        if(socket->write(msg.toUtf8()) != msg.size())
                            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

                        if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                           textEdit->textCursor().removeSelectedText();
                           auto index = textEdit->textCursor().position();// - 1;
                           for(auto i = 0; i<size;i++){
                               qDebug()<<"sinistra verso destra"<<i<<" "<<index;
                               localErase(index);
                               //index--;
                           }
                        }
                         else{
                            textEdit->textCursor().removeSelectedText();
                            auto index = textEdit->textCursor().position();
                            for(auto i = 0; i<size;i++){
                                qDebug()<<"destra verso sinistra"<<i<<index;
                                localErase(index);
                            }
                        }
                    }
                    qDebug()<<"----------------Site id: "<<_siteId;
                    textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
                    textEdit->insertPlainText(event->text());
                    auto index = textEdit->textCursor().position()-1;
                    auto pos = this->localInsert(index);
                    //textEdit->insertPlainText(event->text());
                    qDebug()<<index<<" "<<pos<<"event: "<<event->key();
                }
                else if(event->key()==Qt::Key_BracketRight){
                    if(textEdit->textCursor().hasSelection()){
                        auto size = textEdit->textCursor().selectedText().size();
                        qDebug()<<"Selection";
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
                        QString msg = "MULTI   "+supportSize;
                        qDebug()<<msg;
                        QTcpSocket *socket = MyTcpSocket::socket;

                        if(socket->write(msg.toUtf8()) != msg.size())
                            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

                        if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                           textEdit->textCursor().removeSelectedText();
                           auto index = textEdit->textCursor().position();// - 1;
                           for(auto i = 0; i<size;i++){
                               qDebug()<<"sinistra verso destra"<<i<<" "<<index;
                               localErase(index);
                               //index--;
                           }
                        }
                        else{
                            textEdit->textCursor().removeSelectedText();
                            auto index = textEdit->textCursor().position();
                            for(auto i = 0; i<size;i++){
                                qDebug()<<"destra verso sinistra"<<i<<index;
                                localErase(index);
                             }
                        }
                    }
                    qDebug()<<"----------------Site id: "<<_siteId;
                    textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
                    textEdit->insertPlainText(event->text());
                    auto index = textEdit->textCursor().position()-1;
                    auto pos = this->localInsert(index);
                    //textEdit->insertPlainText(event->text());
                    qDebug()<<index<<" "<<pos<<"event: "<<event->key();
               }
               else if(event->key()==Qt::Key_BraceLeft){
                    if(textEdit->textCursor().hasSelection()){
                        auto size = textEdit->textCursor().selectedText().size();
                        qDebug()<<"Selection";
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
                        QString msg = "MULTI   "+supportSize;
                        qDebug()<<msg;
                        QTcpSocket *socket = MyTcpSocket::socket;

                        if(socket->write(msg.toUtf8()) != msg.size())
                            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

                        if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                           textEdit->textCursor().removeSelectedText();
                           auto index = textEdit->textCursor().position();// - 1;
                           for(auto i = 0; i<size;i++){
                               qDebug()<<"sinistra verso destra"<<i<<" "<<index;
                               localErase(index);
                               //index--;
                           }
                        }
                        else{
                            textEdit->textCursor().removeSelectedText();
                            auto index = textEdit->textCursor().position();
                            for(auto i = 0; i<size;i++){
                                qDebug()<<"destra verso sinistra"<<i<<index;
                                localErase(index);
                             }
                        }
                    }
                    qDebug()<<"----------------Site id: "<<_siteId;
                    textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
                    textEdit->insertPlainText(event->text());
                    auto index = textEdit->textCursor().position()-1;
                    auto pos = this->localInsert(index);
                    //textEdit->insertPlainText(event->text());
                    qDebug()<<index<<" "<<pos<<"event: "<<event->key();
               }
               else if(event->key()==Qt::Key_BraceRight){
                    if(textEdit->textCursor().hasSelection()){
                        auto size = textEdit->textCursor().selectedText().size();
                        qDebug()<<"Selection";
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
                        QString msg = "MULTI   "+supportSize;
                        qDebug()<<msg;
                        QTcpSocket *socket = MyTcpSocket::socket;

                        if(socket->write(msg.toUtf8()) != msg.size())
                            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

                        if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                           textEdit->textCursor().removeSelectedText();
                           auto index = textEdit->textCursor().position();// - 1;
                           for(auto i = 0; i<size;i++){
                               qDebug()<<"sinistra verso destra"<<i<<" "<<index;
                               localErase(index);
                               //index--;
                           }
                        }
                        else{
                            textEdit->textCursor().removeSelectedText();
                            auto index = textEdit->textCursor().position();
                            for(auto i = 0; i<size;i++){
                                qDebug()<<"destra verso sinistra"<<i<<index;
                                localErase(index);
                             }
                        }
                    }
                    qDebug()<<"----------------Site id: "<<_siteId;
                    textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
                    textEdit->insertPlainText(event->text());
                    auto index = textEdit->textCursor().position()-1;
                    auto pos = this->localInsert(index);
                    //textEdit->insertPlainText(event->text());
                    qDebug()<<index<<" "<<pos<<"event: "<<event->key();
                }
                else if(event->key()==Qt::Key_NumberSign){ //cancelletto
                    if(textEdit->textCursor().hasSelection()){
                        auto size = textEdit->textCursor().selectedText().size();
                        qDebug()<<"Selection";
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
                        QString msg = "MULTI   "+supportSize;
                        qDebug()<<msg;
                        QTcpSocket *socket = MyTcpSocket::socket;

                        if(socket->write(msg.toUtf8()) != msg.size())
                            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

                        if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                           textEdit->textCursor().removeSelectedText();
                           auto index = textEdit->textCursor().position();// - 1;
                           for(auto i = 0; i<size;i++){
                               qDebug()<<"sinistra verso destra"<<i<<" "<<index;
                               localErase(index);
                               //index--;
                           }
                        }
                         else{
                            textEdit->textCursor().removeSelectedText();
                            auto index = textEdit->textCursor().position();
                            for(auto i = 0; i<size;i++){
                                qDebug()<<"destra verso sinistra"<<i<<index;
                                localErase(index);
                             }
                        }
                    }
                    qDebug()<<"----------------Site id: "<<_siteId;
                    textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
                    textEdit->insertPlainText(event->text());
                    auto index = textEdit->textCursor().position()-1;
                    auto pos = this->localInsert(index);
                    //textEdit->insertPlainText(event->text());
                    qDebug()<<index<<" "<<pos<<"event: "<<event->key();
               }
               else if(event->key()==Qt::Key_AsciiTilde){
                    if(textEdit->textCursor().hasSelection()){
                        auto size = textEdit->textCursor().selectedText().size();
                        qDebug()<<"Selection";
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
                        QString msg = "MULTI   "+supportSize;
                        qDebug()<<msg;
                        QTcpSocket *socket = MyTcpSocket::socket;

                        if(socket->write(msg.toUtf8()) != msg.size())
                            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

                        if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                           textEdit->textCursor().removeSelectedText();
                           auto index = textEdit->textCursor().position();// - 1;
                           for(auto i = 0; i<size;i++){
                               qDebug()<<"sinistra verso destra"<<i<<" "<<index;
                               localErase(index);
                               //index--;
                           }
                        }
                        else{
                            textEdit->textCursor().removeSelectedText();
                            auto index = textEdit->textCursor().position();
                            for(auto i = 0; i<size;i++){
                                qDebug()<<"destra verso sinistra"<<i<<index;
                                localErase(index);
                             }
                        }
                    }
                    qDebug()<<"----------------Site id: "<<_siteId;
                    textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
                    textEdit->insertPlainText(event->text());
                    auto index = textEdit->textCursor().position()-1;
                    auto pos = this->localInsert(index);
                    //textEdit->insertPlaainText(event->text());
                    qDebug()<<index<<" "<<pos<<"event: "<<event->key();
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
           qDebug()<<"character or number";
           if(textEdit->textCursor().hasSelection()){
               auto size = textEdit->textCursor().selectedText().size();
               qDebug()<<"Selection";
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
               QString msg = "MULTI   "+supportSize;
               qDebug()<<msg;
               QTcpSocket *socket = MyTcpSocket::socket;

               if(socket->write(msg.toUtf8()) != msg.size())
                   QMessageBox::warning(this,"Multi","Impossible to communicate with server");

               if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                  textEdit->textCursor().removeSelectedText();
                  auto index = textEdit->textCursor().position();// - 1;
                  for(auto i = 0; i<size;i++){
                      qDebug()<<"sinistra verso destra"<<i<<" "<<index;
                      localErase(index);
                      //index--;
                  }
               }
               else{
                   textEdit->textCursor().removeSelectedText();
                   auto index = textEdit->textCursor().position();
                   for(auto i = 0; i<size;i++){
                       qDebug()<<"destra verso sinistra"<<i<<index;
                       localErase(index);
                    }
               }
           }
           qDebug()<<"----------------Site id: "<<_siteId;
           textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
           textEdit->insertPlainText(event->text());
           auto index = textEdit->textCursor().position()-1;
           auto pos = this->localInsert(index);
           //textEdit->insertPlainText(event->text());
           qDebug()<<index<<" "<<pos<<"event: "<<event->key();

        }
        else if (event->key() == Qt::Key_Backspace){
           if(textEdit->textCursor().hasSelection()){
               auto size = textEdit->textCursor().selectedText().size();
               qDebug()<<"Selection";
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
               QString msg = "MULTI   "+supportSize;
               qDebug()<<msg;
               QTcpSocket *socket = MyTcpSocket::socket;

               if(socket->write(msg.toUtf8()) != msg.size())
                   QMessageBox::warning(this,"Multi","Impossible to communicate with server");

               if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                   textEdit->textCursor().removeSelectedText();
                  auto index = textEdit->textCursor().position();// - 1;
                  for(auto i = 0; i<size;i++){
                      qDebug()<<"sinistra verso destra"<<i<<" "<<index;
                      localErase(index);
                      //index--;
                  }
               }
               else{
                   textEdit->textCursor().removeSelectedText();
                   auto index = textEdit->textCursor().position();
                   for(auto i = 0; i<size;i++){
                       qDebug()<<"destra verso sinistra"<<i<<index;
                       localErase(index);
                    }
               }
           }
           else if (textEdit->textCursor().position()!=0){
               qDebug()<<"no selection";
               auto index = textEdit->textCursor().position()-1;
               textEdit->textCursor().deletePreviousChar();
               localErase(index);
               qDebug()<<index;
           }
       }
       else if(event->key()==Qt::Key_Space){
           if(textEdit->textCursor().hasSelection()){
               auto size = textEdit->textCursor().selectedText().size();
               qDebug()<<"Selection";
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
               QString msg = "MULTI   "+supportSize;
               qDebug()<<msg;
               QTcpSocket *socket = MyTcpSocket::socket;

               if(socket->write(msg.toUtf8()) != msg.size())
                   QMessageBox::warning(this,"Multi","Impossible to communicate with server");

               if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                   textEdit->textCursor().removeSelectedText();
                  auto index = textEdit->textCursor().position();// - 1;
                  for(auto i = 0; i<size;i++){
                      qDebug()<<"sinistra verso destra"<<i<<" "<<index;
                      localErase(index);
                      //index--;
                  }
               }
               else{
                   textEdit->textCursor().removeSelectedText();
                   auto index = textEdit->textCursor().position();
                   for(auto i = 0; i<size;i++){
                       qDebug()<<"destra verso sinistra"<<i<<index;
                       localErase(index);
                    }
               }
           }
           qDebug()<<"----------------Site id: "<<_siteId;
           textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
           textEdit->insertPlainText(event->text());
           auto index  = textEdit->textCursor().position()-1;
           auto pos = localInsert(index);
           qDebug()<<"space"<< event->text()<<pos;
       }
       else if(event->key() == Qt::Key_Tab){
           if(textEdit->textCursor().hasSelection()){
               auto size = textEdit->textCursor().selectedText().size();
               qDebug()<<"Selection";
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
               QString msg = "MULTI   "+supportSize;
               qDebug()<<msg;
               QTcpSocket *socket = MyTcpSocket::socket;

               if(socket->write(msg.toUtf8()) != msg.size())
                   QMessageBox::warning(this,"Multi","Impossible to communicate with server");

               if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                   textEdit->textCursor().removeSelectedText();
                  auto index = textEdit->textCursor().position();// - 1;
                  for(auto i = 0; i<size;i++){
                      qDebug()<<"sinistra verso destra"<<i<<" "<<index;
                      localErase(index);
                      //index--;
                  }
               }
                else{
                   textEdit->textCursor().removeSelectedText();
                   auto index = textEdit->textCursor().position();
                   for(auto i = 0; i<size;i++){
                       qDebug()<<"destra verso sinistra"<<i<<index;
                       localErase(index);
                    }
               }
           }
           qDebug()<<"tab";
           qDebug()<<"----------------Site id: "<<_siteId;
           textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
           textEdit->insertPlainText(event->text());
           qDebug()<<textEdit->textCursor().position();
           auto index  = textEdit->textCursor().position()-1;
           auto pos = localInsert(index);
           qDebug()<<index<< pos;
       }
       else if(event->key()==Qt::Key_Enter || event->key()==Qt::Key_Return){
           qDebug()<<"Enter";
           if(textEdit->textCursor().hasSelection()){
               auto size = textEdit->textCursor().selectedText().size();
               qDebug()<<"Selection";

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
               QString msg = "MULTI   "+supportSize;
               qDebug()<<msg;
               QTcpSocket *socket = MyTcpSocket::socket;

               if(socket->write(msg.toUtf8()) != msg.size())
                   QMessageBox::warning(this,"Multi","Impossible to communicate with server");

               if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                   textEdit->textCursor().removeSelectedText();
                  auto index = textEdit->textCursor().position();// - 1;
                  for(auto i = 0; i<size;i++){
                      qDebug()<<"sinistra verso destra"<<i<<" "<<index;
                      localErase(index);
                      //index--;
                  }
               }
                else{
                   textEdit->textCursor().removeSelectedText();
                   auto index = textEdit->textCursor().position();
                   for(auto i = 0; i<size;i++){
                       qDebug()<<"destra verso sinistra"<<i<<index;
                       localErase(index);
                   }
               }
           }
           qDebug()<<"----------------Site id: "<<_siteId;
           textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
           textEdit->insertPlainText(event->text());
           qDebug()<<textEdit->textCursor().position();
           auto index  = textEdit->textCursor().position()-1;
           auto pos = localInsert(index);
           qDebug()<<index<< pos;
       }
       else if(event->key() == Qt::Key_Left){
           if(combination!=0){
               qDebug()<<"select previous char";
               QTextCursor tmpCursor = textEdit->textCursor();
               tmpCursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
               textEdit->setTextCursor(tmpCursor);
               qDebug()<<textEdit->textCursor().position();
           }
           else{
               qDebug()<<"left";
               QTextCursor tmpCursor = textEdit->textCursor();
               tmpCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor);
               textEdit->setTextCursor(tmpCursor);
               qDebug()<<textEdit->textCursor().position();
           }
       }
       else if(event->key() == Qt::Key_Right){
           if(combination !=0){
               qDebug()<<"select next char";
               //this->combination += event->key();
              // textEdit->textCursor().select(QTextCursor::SelectionType::BlockUnderCursor);

               QTextCursor tmpCursor = textEdit->textCursor();
               tmpCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
               textEdit->setTextCursor(tmpCursor);
               qDebug()<<textEdit->textCursor().position();
           }
           else{
               qDebug()<<"right";
               QTextCursor tmpCursor = textEdit->textCursor();
               tmpCursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor);
               textEdit->setTextCursor(tmpCursor);
               qDebug()<<textEdit->textCursor().position();
           }
       }
       else if(event->key() == Qt::Key_Up){
           if(combination!=0){
               qDebug()<<"line up";
               QTextCursor tmpCursor = textEdit->textCursor();
               tmpCursor.movePosition(QTextCursor::Up, QTextCursor::KeepAnchor);
               textEdit->setTextCursor(tmpCursor);
               qDebug()<<textEdit->textCursor().position();
           }
           else{
               qDebug()<<"up";
               QTextCursor tmpCursor = textEdit->textCursor();
               tmpCursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor);
               textEdit->setTextCursor(tmpCursor);
               qDebug()<<textEdit->textCursor().position();
           }
       }
       else if(event->key() == Qt::Key_Down){
           if(combination!=0){
               qDebug()<<"line down";
               QTextCursor tmpCursor = textEdit->textCursor();
               tmpCursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor);
               textEdit->setTextCursor(tmpCursor);
               qDebug()<<textEdit->textCursor().position();
           }
           else{
               qDebug()<<"down";
               QTextCursor tmpCursor = textEdit->textCursor();
               tmpCursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor);
               textEdit->setTextCursor(tmpCursor);
               qDebug()<<textEdit->textCursor().position();
           }
       }
       else if((event->key() == Qt::Key_Shift)){
           //qDebug()<<"select next char";
           this->combination += event->key();
           qDebug()<<"shift"<<event->key();
           //textEdit->textCursor().select(QTextCursor::SelectionType::BlockUnderCursor);
       }
       else if(event->key()==Qt::Key_Delete){
           qDebug()<<"delete";
           if(textEdit->textCursor().hasSelection()){
               auto size = textEdit->textCursor().selectedText().size();
               qDebug()<<"Selection";
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
               QString msg = "MULTI   "+supportSize;
               qDebug()<<msg;
               QTcpSocket *socket = MyTcpSocket::socket;

               if(socket->write(msg.toUtf8()) != msg.size())
                   QMessageBox::warning(this,"Multi","Impossible to communicate with server");

               if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
                   textEdit->textCursor().removeSelectedText();
                  auto index = textEdit->textCursor().position();// - 1;
                  for(auto i = 0; i<size;i++){
                      qDebug()<<"sinistra verso destra"<<i<<" "<<index;
                      localErase(index);
                      //index--;
                  }
               }
                else{
                   textEdit->textCursor().removeSelectedText();
                   auto index = textEdit->textCursor().position();
                   for(auto i = 0; i<size;i++){
                       qDebug()<<"destra verso sinistra"<<i<<index;
                       localErase(index);
                    }
               }
           }
           else{
                   qDebug()<<"no selection";
                   auto index = textEdit->textCursor().position();
                   if(!textEdit->textCursor().atEnd()){
                       textEdit->textCursor().deleteChar();
                       localErase(index);
                   }
                   else{
                       event->ignore();
                   }
                   qDebug()<<index;
           }
       }
       else if(event->key()==Qt::Key_Cancel){
           qDebug()<<"cancel";
       }
       else if(event->key()==Qt::Key_Control){
           this->keyReleaseEvent(event);
           event->ignore();
       }
       else{
           qDebug()<<event->key();
           auto index = textEdit->textCursor().position()-1;
           //textEdit->releaseKeyboard();
           //this->keyReleaseEvent(event);
           qDebug()<<index;
           event->ignore();
       }
}

       r = textEdit->document()->lineCount();
       numRow->setNum(r);
       c = textEdit->textCursor().columnNumber();
       numColumn->setNum(c);
       w = textEdit->toPlainText().split(QRegExp("(\\s|\\n|\\r)+") , QString::SkipEmptyParts).count();
       numWords->setNum(w);
       numC = textEdit->document()->characterCount();
       numCharacters->setNum(numC);


}

void TextEdit::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Shift){
        combination = 0;
    }
    //if(event->key() == Qt::Key_Alt) event->ignore();

}


void TextEdit::myPaste()
{
      const QMimeData *md = QApplication::clipboard()->mimeData();
      int s = md->text().size();
      qDebug()<<"Paste:" <<s;
      if (md) actionPaste->setEnabled(md->hasText());

      if(textEdit->textCursor().hasSelection()){
          int size = textEdit->textCursor().selectedText().size();
          QString supportSize;
          if(size+s<10){
              supportSize = QString::number(size+s).insert(0,"00");
          }
          else if(size+s<100){
              supportSize = QString::number(size+s).insert(0,"0");
          }
          else {
              supportSize = QString::number(size+s);
          }
          QString msg = "MULTI   "+supportSize;
          qDebug()<<msg;
          QTcpSocket *socket = MyTcpSocket::socket;
          if(socket->write(msg.toUtf8()) != msg.size())
              QMessageBox::warning(this,"Multi","Impossible to communicate with server");

          qDebug()<<"Selection";
          if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
              textEdit->textCursor().removeSelectedText();
             auto index = textEdit->textCursor().position();// - 1;
             for(auto i = 0; i<size;i++){
                 qDebug()<<"sinistra verso destra"<<i<<" "<<index;
                 localErase(index);
                 //index--;
             }
          }
           else{
              textEdit->textCursor().removeSelectedText();
              auto index = textEdit->textCursor().position();
              for(auto i = 0; i<size;i++){
                  qDebug()<<"destra verso sinistra"<<i<<index;
                  localErase(index);
               }
          }
          qDebug()<<"----------------Site id: "<<_siteId;
          textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
          textEdit->insertPlainText(md->text());
          auto index = textEdit->textCursor().position()-s;
          for(auto i = 0; i<s; i++){
              auto pos = localInsert(index);
              qDebug()<<index<<" "<<pos;
              index++;
          }
      }
      else {
          QString supportSize;
          if(s<10){
              supportSize = QString::number(s).insert(0,"00");
          }
          else if(s<100){
              supportSize = QString::number(s).insert(0,"0");
          }
          else {
              supportSize = QString::number(s);
          }
          qDebug()<<"----------------Site id: "<<_siteId;
          textEdit->setTextBackgroundColor(this->colors.at(_siteId-1));
          textEdit->insertPlainText(md->text());
          auto index = textEdit->textCursor().position()-s;
          QString msg = "MULTI   "+supportSize;
          qDebug()<<msg;
          QTcpSocket *socket = MyTcpSocket::socket;
          if(socket->write(msg.toUtf8()) != msg.size())
              QMessageBox::warning(this,"Multi","Impossible to communicate with server");

          for(auto i = 0; i<s; i++){
              auto pos = localInsert(index);
              qDebug()<<index<<" "<<pos;
              index++;
          }
      }

      r = textEdit->document()->lineCount();
      numRow->setNum(r);
      c = textEdit->textCursor().columnNumber();
      numColumn->setNum(c);
      w = textEdit->toPlainText().split(QRegExp("(\\s|\\n|\\r)+") , QString::SkipEmptyParts).count();
      numWords->setNum(w);
      numC = textEdit->document()->characterCount();
      numCharacters->setNum(numC);

}

void TextEdit::myCut(){

    qDebug()<<"Cutting: ";

    if(textEdit->textCursor().hasSelection()){
        auto size = textEdit->textCursor().selectedText().size();

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
        QString msg = "MULTI   "+supportSize;

        qDebug()<<msg;
        QTcpSocket *socket = MyTcpSocket::socket;

        if(socket->write(msg.toUtf8()) != msg.size())
            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

        QApplication::clipboard()->setText(textEdit->textCursor().selectedText());
        qDebug()<<"Selection";
        if(textEdit->textCursor().selectionStart() < textEdit->textCursor().position()){
            textEdit->textCursor().removeSelectedText();
           auto index = textEdit->textCursor().position();
           for(auto i = 0; i<size;i++){
               qDebug()<<"lol"<<i<<" "<<index;
               localErase(index);
           }
        }
         else{
            textEdit->textCursor().removeSelectedText();
            auto index = textEdit->textCursor().position();
            for(auto i = 0; i<size;i++){
                qDebug()<<i<<" "<<index;
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
    numC = textEdit->document()->characterCount();
    numCharacters->setNum(numC);

}

void TextEdit::myUndo() {
    //int previousIndex = textEdit->textCursor().position();
    int previousIndex = textEdit->toPlainText().size();
    textEdit->undo();
    int actualIndex = textEdit->toPlainText().size();
    //int actualIndex = textEdit->textCursor().position();
    if(previousIndex > actualIndex){
        //precedente maggiore dell'attuale quindi vado verso sinistra => local erase
        int size = previousIndex - actualIndex;

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
        QString msg = "MULTI   "+supportSize;
        qDebug()<<msg;
        QTcpSocket *socket = MyTcpSocket::socket;

        if(socket->write(msg.toUtf8()) != msg.size())
            QMessageBox::warning(this,"Multi","Impossible to communicate with server");
        for (auto i = 0; i<size;i++){
            previousIndex--;
            localErase(previousIndex);
            qDebug()<<"undo1"<<i;
        }
    }
    else{
        //vado verso destra local insert
        int size = actualIndex - previousIndex;
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
        QString msg = "MULTI   "+supportSize;
        qDebug()<<msg;
        QTcpSocket *socket = MyTcpSocket::socket;

        if(socket->write(msg.toUtf8()) != msg.size())
            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

        for (auto i = 0; i<size;i++){
            auto pos = localInsert(previousIndex);
            previousIndex++;
            qDebug()<<"undo2"<<pos;
        }
    }
    r = textEdit->document()->lineCount();
    numRow->setNum(r);
    c = textEdit->textCursor().columnNumber();
    numColumn->setNum(c);
    w = textEdit->toPlainText().split(QRegExp("(\\s|\\n|\\r)+") , QString::SkipEmptyParts).count();
    numWords->setNum(w);
    numC = textEdit->document()->characterCount();
    numCharacters->setNum(numC);
}

void TextEdit::myRedo()
{
    //int previousIndex = textEdit->textCursor().position();
    int previousIndex = textEdit->toPlainText().size();
    textEdit->redo();
    int actualIndex = textEdit->toPlainText().size();
    //int actualIndex = textEdit->textCursor().position();
    if(previousIndex > actualIndex){
        //precedente maggiore dell'attuale quindi vado verso sinistra => local erase
        int size = previousIndex - actualIndex;
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
        QString msg = "MULTI   "+supportSize;
        qDebug()<<msg;
        QTcpSocket *socket = MyTcpSocket::socket;

        if(socket->write(msg.toUtf8()) != msg.size())
            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

        for (auto i = 0; i<size;i++){
            previousIndex--;
            localErase(previousIndex);
            qDebug()<<"redo1"<<i;
        }
    }
    else{
        //vado verso destra local insert
        int size = actualIndex - previousIndex;
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
        QString msg = "MULTI   "+supportSize;
        qDebug()<<msg;
        QTcpSocket *socket = MyTcpSocket::socket;

        if(socket->write(msg.toUtf8()) != msg.size())
            QMessageBox::warning(this,"Multi","Impossible to communicate with server");

        for (auto i = 0; i<size;i++){
            auto pos = localInsert(previousIndex);
            previousIndex++;
            qDebug()<<"redo2"<<pos;
        }
    }
    r = textEdit->document()->lineCount();
    numRow->setNum(r);
    c = textEdit->textCursor().columnNumber();
    numColumn->setNum(c);
    w = textEdit->toPlainText().split(QRegExp("(\\s|\\n|\\r)+") , QString::SkipEmptyParts).count();
    numWords->setNum(w);
    numC = textEdit->document()->characterCount();
    numCharacters->setNum(numC);
}

void TextEdit::myZoomIn()
{
    qDebug()<<"Zoom in"<<percZoom;
    if(percZoom == 250) percZoom = 250;
    else {
        textEdit->zoomIn(10);
        percZoom = percZoom + 10;
    }
    numZoom->setNum(percZoom);
}

void TextEdit::myZoomOut()
{
    qDebug()<<"Zoom Out"<<percZoom;

    if(percZoom==100) percZoom = 100;
    else{
        textEdit->zoomOut(10);
        percZoom = percZoom - 10;
    }
    numZoom->setNum(percZoom);
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
    //uri->insert(str);
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
    //abilitare la copia dell'uri
    //mette l'uri nella clipboard

    //Da mettere in myctpsocket.cpp:
    /*
    QApplication::clipboard()->setText(uri->text());
    qDebug()<<"uri copiato: "<< uri->text();
    // finestra popup che ti dice uri copiato
    */

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

void TextEdit::addUserLabel(int numUser, QString username)
{
    qDebug()<<"labeladd";
    //mappa username label per facilitare eliminazione?
    //oppure eliminazione tramite id del client
    //for che itera sul numero di client connessi e che aggiunge le label
    QLabel *tempLabel = new QLabel(username,nullptr);
    //tempLabel->setStyleSheet("QLabel{color: blue;" "background-color: light-grey;}");
    tempLabel->setAutoFillBackground(true);
    QColor color = colors.at(_siteId-1);
    QString values = QString::number(color.red())+","+QString::number(color.green())+","+QString::number(color.blue());
    tempLabel->setStyleSheet("QLabel { background-color: rgba("+values+"); }");
    tempLabel->setFocusPolicy(Qt::NoFocus);
    ut->addWidget(tempLabel);
    usersVector.push_back(tempLabel);

    //da commentare
/*
    QLabel* cursorLabel = new QLabel(textEdit);
    labelFixedSize = QSize(1,33);
    //cursorLabel = new QLabel(textEdit);
    QColor color2 = colors.at(_siteId-1);
    QString values2 = QString::number(color2.red())+","+QString::number(color2.green())+","+QString::number(color2.blue());
    cursorLabel->setStyleSheet("QLabel { background-color: rgba("+values2+"); }");
    //cursorLabel->setStyleSheet("QLabel { background-color: rgba(253,97,125); }");
    //cursorLabel->setFixedSize(textEdit->cursorRect().size().width()*20, textEdit->cursorRect().size().height());
    cursorLabel->setFixedSize(labelFixedSize.width()*20, labelFixedSize.height());
    qDebug()<<"setfixedsize"<<labelFixedSize;
    qDebug()<<"weight "<<textEdit->cursorRect().size().width()*20<<"HEIGHT" <<textEdit->cursorRect().size().height();
    //cursorLabel->setGeometry(textEdit->size().width()-1, textEdit->size().height()-1, textEdit->cursor().pixmap().width(), textEdit->cursor().pixmap().height());
    cursorLabel->move(textEdit->cursorRect().left(), textEdit->cursorRect().top());
    cursorLabel->setCursor(textEdit->cursor());
    cursorLabel->setVisible(true);
    cursorLabel->show();
    labelVector.push_back(QPair<QLabel*,int>(cursorLabel,this->getSiteID()));

    timer->start(500);*/


}

void TextEdit::addUser(QString& username, int id)
{
    userConnessi.push_back(QPair<QString,int>(username,id));

    QLabel *tempLabel = new QLabel(username,nullptr);
    tempLabel->setAutoFillBackground(true);
    QColor color = colors.at(id-1);
    QString values = QString::number(color.red())+","+QString::number(color.green())+","+QString::number(color.blue());
    tempLabel->setStyleSheet("QLabel { background-color: rgba("+values+"); }");
    tempLabel->setFocusPolicy(Qt::NoFocus);
    ut->addWidget(tempLabel);
    usersVector.push_back(tempLabel);

    QLabel* cursorLabel = new QLabel(textEdit);
    labelFixedSize = QSize(1,33);
    QColor color2 = colors.at(id-1);
    QString values2 = QString::number(color.red())+","+QString::number(color.green())+","+QString::number(color.blue());
    //cursorLabel = new QLabel(textEdit);
    cursorLabel->setStyleSheet("QLabel { background-color: rgba("+values2+"); }"); //assegnargli il colore
    //cursorLabel->setFixedSize(textEdit->cursorRect().size().width()*20, textEdit->cursorRect().size().height());
    cursorLabel->setFixedSize(labelFixedSize.width()*2, labelFixedSize.height());
    //cursorLabel->setGeometry(textEdit->size().width()-1, textEdit->size().height()-1, textEdit->cursor().pixmap().width(), textEdit->cursor().pixmap().height());
    cursorLabel->move(textEdit->cursorRect().left(), textEdit->cursorRect().top());
    cursorLabel->setVisible(true);
    cursorLabel->show();

    this->userNumber++;
    this->userActive->setNum(userNumber);
    labelVector.push_back(QPair<QLabel*,int>(cursorLabel,id));

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

