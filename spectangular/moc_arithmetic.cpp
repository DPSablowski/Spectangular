/****************************************************************************
** Meta object code from reading C++ file 'arithmetic.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "arithmetic.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'arithmetic.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Arithmetic_t {
    QByteArrayData data[12];
    char stringdata0[203];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Arithmetic_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Arithmetic_t qt_meta_stringdata_Arithmetic = {
    {
QT_MOC_LITERAL(0, 0, 10), // "Arithmetic"
QT_MOC_LITERAL(1, 11, 6), // "seData"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 3), // "str"
QT_MOC_LITERAL(4, 23, 23), // "on_pushButton_2_clicked"
QT_MOC_LITERAL(5, 47, 25), // "on_lineEdit_4_textChanged"
QT_MOC_LITERAL(6, 73, 19), // "on_checkBox_clicked"
QT_MOC_LITERAL(7, 93, 21), // "on_checkBox_2_clicked"
QT_MOC_LITERAL(8, 115, 21), // "on_checkBox_3_clicked"
QT_MOC_LITERAL(9, 137, 21), // "on_checkBox_4_clicked"
QT_MOC_LITERAL(10, 159, 21), // "on_checkBox_5_clicked"
QT_MOC_LITERAL(11, 181, 21) // "on_checkBox_6_clicked"

    },
    "Arithmetic\0seData\0\0str\0on_pushButton_2_clicked\0"
    "on_lineEdit_4_textChanged\0on_checkBox_clicked\0"
    "on_checkBox_2_clicked\0on_checkBox_3_clicked\0"
    "on_checkBox_4_clicked\0on_checkBox_5_clicked\0"
    "on_checkBox_6_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Arithmetic[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x0a /* Public */,
       4,    0,   62,    2, 0x08 /* Private */,
       5,    0,   63,    2, 0x08 /* Private */,
       6,    0,   64,    2, 0x08 /* Private */,
       7,    0,   65,    2, 0x08 /* Private */,
       8,    0,   66,    2, 0x08 /* Private */,
       9,    0,   67,    2, 0x08 /* Private */,
      10,    0,   68,    2, 0x08 /* Private */,
      11,    0,   69,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Arithmetic::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Arithmetic *_t = static_cast<Arithmetic *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->seData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->on_pushButton_2_clicked(); break;
        case 2: _t->on_lineEdit_4_textChanged(); break;
        case 3: _t->on_checkBox_clicked(); break;
        case 4: _t->on_checkBox_2_clicked(); break;
        case 5: _t->on_checkBox_3_clicked(); break;
        case 6: _t->on_checkBox_4_clicked(); break;
        case 7: _t->on_checkBox_5_clicked(); break;
        case 8: _t->on_checkBox_6_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject Arithmetic::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Arithmetic.data,
      qt_meta_data_Arithmetic,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Arithmetic::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Arithmetic::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Arithmetic.stringdata0))
        return static_cast<void*>(const_cast< Arithmetic*>(this));
    return QDialog::qt_metacast(_clname);
}

int Arithmetic::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
