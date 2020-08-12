/****************************************************************************
** Meta object code from reading C++ file 'textedit.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../textedit.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'textedit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TextEdit_t {
    QByteArrayData data[26];
    char stringdata0[290];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TextEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TextEdit_t qt_meta_stringdata_TextEdit = {
    {
QT_MOC_LITERAL(0, 0, 8), // "TextEdit"
QT_MOC_LITERAL(1, 9, 25), // "on_homepageButton_clicked"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 7), // "myPaste"
QT_MOC_LITERAL(4, 44, 5), // "myCut"
QT_MOC_LITERAL(5, 50, 6), // "myRedo"
QT_MOC_LITERAL(6, 57, 6), // "myUndo"
QT_MOC_LITERAL(7, 64, 5), // "blink"
QT_MOC_LITERAL(8, 70, 7), // "process"
QT_MOC_LITERAL(9, 78, 7), // "message"
QT_MOC_LITERAL(10, 86, 12), // "filePrintPdf"
QT_MOC_LITERAL(11, 99, 8), // "textBold"
QT_MOC_LITERAL(12, 108, 13), // "textUnderline"
QT_MOC_LITERAL(13, 122, 10), // "textItalic"
QT_MOC_LITERAL(14, 133, 10), // "textFamily"
QT_MOC_LITERAL(15, 144, 1), // "f"
QT_MOC_LITERAL(16, 146, 8), // "textSize"
QT_MOC_LITERAL(17, 155, 1), // "p"
QT_MOC_LITERAL(18, 157, 24), // "currentCharFormatChanged"
QT_MOC_LITERAL(19, 182, 15), // "QTextCharFormat"
QT_MOC_LITERAL(20, 198, 6), // "format"
QT_MOC_LITERAL(21, 205, 21), // "cursorPositionChanged"
QT_MOC_LITERAL(22, 227, 20), // "clipboardDataChanged"
QT_MOC_LITERAL(23, 248, 5), // "about"
QT_MOC_LITERAL(24, 254, 16), // "on_share_clicked"
QT_MOC_LITERAL(25, 271, 18) // "on_copyUri_clicked"

    },
    "TextEdit\0on_homepageButton_clicked\0\0"
    "myPaste\0myCut\0myRedo\0myUndo\0blink\0"
    "process\0message\0filePrintPdf\0textBold\0"
    "textUnderline\0textItalic\0textFamily\0"
    "f\0textSize\0p\0currentCharFormatChanged\0"
    "QTextCharFormat\0format\0cursorPositionChanged\0"
    "clipboardDataChanged\0about\0on_share_clicked\0"
    "on_copyUri_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TextEdit[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  109,    2, 0x0a /* Public */,
       3,    0,  110,    2, 0x0a /* Public */,
       4,    0,  111,    2, 0x0a /* Public */,
       5,    0,  112,    2, 0x0a /* Public */,
       6,    0,  113,    2, 0x0a /* Public */,
       7,    0,  114,    2, 0x0a /* Public */,
       8,    1,  115,    2, 0x0a /* Public */,
      10,    0,  118,    2, 0x08 /* Private */,
      11,    0,  119,    2, 0x08 /* Private */,
      12,    0,  120,    2, 0x08 /* Private */,
      13,    0,  121,    2, 0x08 /* Private */,
      14,    1,  122,    2, 0x08 /* Private */,
      16,    1,  125,    2, 0x08 /* Private */,
      18,    1,  128,    2, 0x08 /* Private */,
      21,    0,  131,    2, 0x08 /* Private */,
      22,    0,  132,    2, 0x08 /* Private */,
      23,    0,  133,    2, 0x08 /* Private */,
      24,    0,  134,    2, 0x08 /* Private */,
      25,    0,  135,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TextEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TextEdit *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_homepageButton_clicked(); break;
        case 1: _t->myPaste(); break;
        case 2: _t->myCut(); break;
        case 3: _t->myRedo(); break;
        case 4: _t->myUndo(); break;
        case 5: _t->blink(); break;
        case 6: _t->process((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->filePrintPdf(); break;
        case 8: _t->textBold(); break;
        case 9: _t->textUnderline(); break;
        case 10: _t->textItalic(); break;
        case 11: _t->textFamily((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: _t->textSize((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->currentCharFormatChanged((*reinterpret_cast< const QTextCharFormat(*)>(_a[1]))); break;
        case 14: _t->cursorPositionChanged(); break;
        case 15: _t->clipboardDataChanged(); break;
        case 16: _t->about(); break;
        case 17: _t->on_share_clicked(); break;
        case 18: _t->on_copyUri_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TextEdit::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_TextEdit.data,
    qt_meta_data_TextEdit,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TextEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TextEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TextEdit.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int TextEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
