/****************************************************************************
** Meta object code from reading C++ file 'barycorrdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../barycorrdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'barycorrdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BaryCorrDialog_t {
    QByteArrayData data[19];
    char stringdata0[285];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BaryCorrDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BaryCorrDialog_t qt_meta_stringdata_BaryCorrDialog = {
    {
QT_MOC_LITERAL(0, 0, 14), // "BaryCorrDialog"
QT_MOC_LITERAL(1, 15, 9), // "useInMain"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 6), // "prefix"
QT_MOC_LITERAL(4, 33, 3), // "ext"
QT_MOC_LITERAL(5, 37, 6), // "rvFile"
QT_MOC_LITERAL(6, 44, 9), // "useBcFile"
QT_MOC_LITERAL(7, 54, 4), // "file"
QT_MOC_LITERAL(8, 59, 9), // "corrected"
QT_MOC_LITERAL(9, 69, 14), // "useAirmassFile"
QT_MOC_LITERAL(10, 84, 15), // "spectraRejected"
QT_MOC_LITERAL(11, 100, 4), // "last"
QT_MOC_LITERAL(12, 105, 4), // "note"
QT_MOC_LITERAL(13, 110, 29), // "on_pushButton_compute_clicked"
QT_MOC_LITERAL(14, 140, 27), // "on_pushButton_write_clicked"
QT_MOC_LITERAL(15, 168, 25), // "on_pushButton_use_clicked"
QT_MOC_LITERAL(16, 194, 29), // "on_pushButton_writebc_clicked"
QT_MOC_LITERAL(17, 224, 31), // "on_pushButton_tellcheck_clicked"
QT_MOC_LITERAL(18, 256, 28) // "on_pushButton_reject_clicked"

    },
    "BaryCorrDialog\0useInMain\0\0prefix\0ext\0"
    "rvFile\0useBcFile\0file\0corrected\0"
    "useAirmassFile\0spectraRejected\0last\0"
    "note\0on_pushButton_compute_clicked\0"
    "on_pushButton_write_clicked\0"
    "on_pushButton_use_clicked\0"
    "on_pushButton_writebc_clicked\0"
    "on_pushButton_tellcheck_clicked\0"
    "on_pushButton_reject_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BaryCorrDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   64,    2, 0x06 /* Public */,
       6,    2,   71,    2, 0x06 /* Public */,
       9,    1,   76,    2, 0x06 /* Public */,
      10,    2,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    0,   84,    2, 0x0a /* Public */,
      14,    0,   85,    2, 0x0a /* Public */,
      15,    0,   86,    2, 0x0a /* Public */,
      16,    0,   87,    2, 0x0a /* Public */,
      17,    0,   88,    2, 0x0a /* Public */,
      18,    0,   89,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    4,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    7,    8,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   11,   12,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void BaryCorrDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BaryCorrDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->useInMain((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 1: _t->useBcFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->useAirmassFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->spectraRejected((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->on_pushButton_compute_clicked(); break;
        case 5: _t->on_pushButton_write_clicked(); break;
        case 6: _t->on_pushButton_use_clicked(); break;
        case 7: _t->on_pushButton_writebc_clicked(); break;
        case 8: _t->on_pushButton_tellcheck_clicked(); break;
        case 9: _t->on_pushButton_reject_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BaryCorrDialog::*)(const QString & , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaryCorrDialog::useInMain)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BaryCorrDialog::*)(const QString & , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaryCorrDialog::useBcFile)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BaryCorrDialog::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaryCorrDialog::useAirmassFile)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (BaryCorrDialog::*)(int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaryCorrDialog::spectraRejected)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BaryCorrDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_BaryCorrDialog.data,
    qt_meta_data_BaryCorrDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BaryCorrDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BaryCorrDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BaryCorrDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int BaryCorrDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void BaryCorrDialog::useInMain(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BaryCorrDialog::useBcFile(const QString & _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BaryCorrDialog::useAirmassFile(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BaryCorrDialog::spectraRejected(int _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
