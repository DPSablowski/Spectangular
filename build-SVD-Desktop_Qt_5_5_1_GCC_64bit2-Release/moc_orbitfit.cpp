/****************************************************************************
** Meta object code from reading C++ file 'orbitfit.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../spectangular/orbitfit.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'orbitfit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_OrbitFit_t {
    QByteArrayData data[6];
    char stringdata0[106];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OrbitFit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OrbitFit_t qt_meta_stringdata_OrbitFit = {
    {
QT_MOC_LITERAL(0, 0, 8), // "OrbitFit"
QT_MOC_LITERAL(1, 9, 23), // "on_pushButton_2_clicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 21), // "on_checkBox_2_clicked"
QT_MOC_LITERAL(4, 56, 19), // "on_checkBox_clicked"
QT_MOC_LITERAL(5, 76, 29) // "on_lineEdit_2_editingFinished"

    },
    "OrbitFit\0on_pushButton_2_clicked\0\0"
    "on_checkBox_2_clicked\0on_checkBox_clicked\0"
    "on_lineEdit_2_editingFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OrbitFit[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    0,   36,    2, 0x08 /* Private */,
       5,    0,   37,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void OrbitFit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        OrbitFit *_t = static_cast<OrbitFit *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pushButton_2_clicked(); break;
        case 1: _t->on_checkBox_2_clicked(); break;
        case 2: _t->on_checkBox_clicked(); break;
        case 3: _t->on_lineEdit_2_editingFinished(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject OrbitFit::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_OrbitFit.data,
      qt_meta_data_OrbitFit,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *OrbitFit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OrbitFit::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_OrbitFit.stringdata0))
        return static_cast<void*>(const_cast< OrbitFit*>(this));
    return QDialog::qt_metacast(_clname);
}

int OrbitFit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
