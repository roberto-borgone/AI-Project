/****************************************************************************
** Meta object code from reading C++ file 'homepage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../client/homepage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'homepage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Window_t {
    QByteArrayData data[17];
    char stringdata0[244];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Window_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Window_t qt_meta_stringdata_Window = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Window"
QT_MOC_LITERAL(1, 7, 4), // "find"
QT_MOC_LITERAL(2, 12, 0), // ""
QT_MOC_LITERAL(3, 13, 23), // "on_modifyButton_clicked"
QT_MOC_LITERAL(4, 37, 23), // "on_removeButton_clicked"
QT_MOC_LITERAL(5, 61, 25), // "on_downloadButton_clicked"
QT_MOC_LITERAL(6, 87, 16), // "animateFindClick"
QT_MOC_LITERAL(7, 104, 14), // "openFileOfItem"
QT_MOC_LITERAL(8, 119, 3), // "row"
QT_MOC_LITERAL(9, 123, 6), // "column"
QT_MOC_LITERAL(10, 130, 11), // "contextMenu"
QT_MOC_LITERAL(11, 142, 3), // "pos"
QT_MOC_LITERAL(12, 146, 23), // "on_createButton_clicked"
QT_MOC_LITERAL(13, 170, 22), // "on_editProfile_clicked"
QT_MOC_LITERAL(14, 193, 19), // "on_okButton_clicked"
QT_MOC_LITERAL(15, 213, 23), // "on_deleteButton_clicked"
QT_MOC_LITERAL(16, 237, 6) // "logout"

    },
    "Window\0find\0\0on_modifyButton_clicked\0"
    "on_removeButton_clicked\0"
    "on_downloadButton_clicked\0animateFindClick\0"
    "openFileOfItem\0row\0column\0contextMenu\0"
    "pos\0on_createButton_clicked\0"
    "on_editProfile_clicked\0on_okButton_clicked\0"
    "on_deleteButton_clicked\0logout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Window[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x08 /* Private */,
       3,    0,   75,    2, 0x08 /* Private */,
       4,    0,   76,    2, 0x08 /* Private */,
       5,    0,   77,    2, 0x08 /* Private */,
       6,    0,   78,    2, 0x08 /* Private */,
       7,    2,   79,    2, 0x08 /* Private */,
      10,    1,   84,    2, 0x08 /* Private */,
      12,    0,   87,    2, 0x08 /* Private */,
      13,    0,   88,    2, 0x08 /* Private */,
      14,    0,   89,    2, 0x08 /* Private */,
      15,    0,   90,    2, 0x08 /* Private */,
      16,    0,   91,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    8,    9,
    QMetaType::Void, QMetaType::QPoint,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Window::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Window *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->find(); break;
        case 1: _t->on_modifyButton_clicked(); break;
        case 2: _t->on_removeButton_clicked(); break;
        case 3: _t->on_downloadButton_clicked(); break;
        case 4: _t->animateFindClick(); break;
        case 5: _t->openFileOfItem((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->contextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 7: _t->on_createButton_clicked(); break;
        case 8: _t->on_editProfile_clicked(); break;
        case 9: _t->on_okButton_clicked(); break;
        case 10: _t->on_deleteButton_clicked(); break;
        case 11: _t->logout(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Window::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_Window.data,
    qt_meta_data_Window,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Window::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Window::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Window.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
