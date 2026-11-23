/****************************************************************************
** Meta object code from reading C++ file 'qcustomplot.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qcustomplot.h"
#include <QtGui/qtextcursor.h>
#include <QtGui/qscreen.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qcustomplot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN15QCPScatterStyleE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPScatterStyle::qt_create_metaobjectdata<qt_meta_tag_ZN15QCPScatterStyleE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPScatterStyle",
        "ScatterShape",
        "ssNone",
        "ssDot",
        "ssCross",
        "ssPlus",
        "ssCircle",
        "ssDisc",
        "ssSquare",
        "ssDiamond",
        "ssStar",
        "ssTriangle",
        "ssTriangleInverted",
        "ssCrossSquare",
        "ssPlusSquare",
        "ssCrossCircle",
        "ssPlusCircle",
        "ssPeace",
        "ssPixmap",
        "ssCustom"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'ScatterShape'
        QtMocHelpers::EnumData<enum ScatterShape>(1, 1, QMC::EnumFlags{}).add({
            {    2, ScatterShape::ssNone },
            {    3, ScatterShape::ssDot },
            {    4, ScatterShape::ssCross },
            {    5, ScatterShape::ssPlus },
            {    6, ScatterShape::ssCircle },
            {    7, ScatterShape::ssDisc },
            {    8, ScatterShape::ssSquare },
            {    9, ScatterShape::ssDiamond },
            {   10, ScatterShape::ssStar },
            {   11, ScatterShape::ssTriangle },
            {   12, ScatterShape::ssTriangleInverted },
            {   13, ScatterShape::ssCrossSquare },
            {   14, ScatterShape::ssPlusSquare },
            {   15, ScatterShape::ssCrossCircle },
            {   16, ScatterShape::ssPlusCircle },
            {   17, ScatterShape::ssPeace },
            {   18, ScatterShape::ssPixmap },
            {   19, ScatterShape::ssCustom },
        }),
    };
    return QtMocHelpers::metaObjectData<QCPScatterStyle, qt_meta_tag_ZN15QCPScatterStyleE_t>(QMC::PropertyAccessInStaticMetaCall, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPScatterStyle::staticMetaObject = { {
    nullptr,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15QCPScatterStyleE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15QCPScatterStyleE_t>.data,
    nullptr,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15QCPScatterStyleE_t>.metaTypes,
    nullptr
} };

namespace {
struct qt_meta_tag_ZN10QCPPainterE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPPainter::qt_create_metaobjectdata<qt_meta_tag_ZN10QCPPainterE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPPainter",
        "PainterMode",
        "pmDefault",
        "pmVectorized",
        "pmNoCaching",
        "pmNonCosmetic",
        "PainterModes"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'PainterMode'
        QtMocHelpers::EnumData<enum PainterMode>(1, 1, QMC::EnumFlags{}).add({
            {    2, PainterMode::pmDefault },
            {    3, PainterMode::pmVectorized },
            {    4, PainterMode::pmNoCaching },
            {    5, PainterMode::pmNonCosmetic },
        }),
        // flag 'PainterModes'
        QtMocHelpers::EnumData<PainterModes>(6, 1, QMC::EnumIsFlag).add({
            {    2, PainterMode::pmDefault },
            {    3, PainterMode::pmVectorized },
            {    4, PainterMode::pmNoCaching },
            {    5, PainterMode::pmNonCosmetic },
        }),
    };
    return QtMocHelpers::metaObjectData<QCPPainter, qt_meta_tag_ZN10QCPPainterE_t>(QMC::PropertyAccessInStaticMetaCall, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPPainter::staticMetaObject = { {
    QtPrivate::MetaObjectForType<QPainter>::value,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10QCPPainterE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10QCPPainterE_t>.data,
    nullptr,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10QCPPainterE_t>.metaTypes,
    nullptr
} };

namespace {
struct qt_meta_tag_ZN8QCPLayerE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPLayer::qt_create_metaobjectdata<qt_meta_tag_ZN8QCPLayerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPLayer",
        "parentPlot",
        "QCustomPlot*",
        "name",
        "index",
        "children",
        "QList<QCPLayerable*>",
        "visible"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'parentPlot'
        QtMocHelpers::PropertyData<QCustomPlot*>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'name'
        QtMocHelpers::PropertyData<QString>(3, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'index'
        QtMocHelpers::PropertyData<int>(4, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'children'
        QtMocHelpers::PropertyData<QList<QCPLayerable*>>(5, 0x80000000 | 6, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'visible'
        QtMocHelpers::PropertyData<bool>(7, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPLayer, qt_meta_tag_ZN8QCPLayerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPLayer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8QCPLayerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8QCPLayerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8QCPLayerE_t>.metaTypes,
    nullptr
} };

void QCPLayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPLayer *>(_o);
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCustomPlot* >(); break;
        case 3:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QCPLayerable*> >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QCustomPlot**>(_v) = _t->parentPlot(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->name(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->index(); break;
        case 3: *reinterpret_cast<QList<QCPLayerable*>*>(_v) = _t->children(); break;
        case 4: *reinterpret_cast<bool*>(_v) = _t->visible(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 4: _t->setVisible(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPLayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPLayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8QCPLayerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QCPLayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN12QCPLayerableE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPLayerable::qt_create_metaobjectdata<qt_meta_tag_ZN12QCPLayerableE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPLayerable",
        "layerChanged",
        "",
        "QCPLayer*",
        "newLayer",
        "setLayer",
        "layer",
        "visible",
        "parentPlot",
        "QCustomPlot*",
        "parentLayerable",
        "QCPLayerable*",
        "antialiased"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'layerChanged'
        QtMocHelpers::SignalData<void(QCPLayer *)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'setLayer'
        QtMocHelpers::SlotData<bool(QCPLayer *)>(5, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 3, 6 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'visible'
        QtMocHelpers::PropertyData<bool>(7, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'parentPlot'
        QtMocHelpers::PropertyData<QCustomPlot*>(8, 0x80000000 | 9, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'parentLayerable'
        QtMocHelpers::PropertyData<QCPLayerable*>(10, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'layer'
        QtMocHelpers::PropertyData<QCPLayer*>(6, 0x80000000 | 3, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 0),
        // property 'antialiased'
        QtMocHelpers::PropertyData<bool>(12, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPLayerable, qt_meta_tag_ZN12QCPLayerableE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPLayerable::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12QCPLayerableE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12QCPLayerableE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12QCPLayerableE_t>.metaTypes,
    nullptr
} };

void QCPLayerable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPLayerable *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->layerChanged((*reinterpret_cast<std::add_pointer_t<QCPLayer*>>(_a[1]))); break;
        case 1: { bool _r = _t->setLayer((*reinterpret_cast<std::add_pointer_t<QCPLayer*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QCPLayer* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QCPLayer* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (QCPLayerable::*)(QCPLayer * )>(_a, &QCPLayerable::layerChanged, 0))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPLayer* >(); break;
        case 2:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPLayerable* >(); break;
        case 1:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCustomPlot* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<bool*>(_v) = _t->visible(); break;
        case 1: *reinterpret_cast<QCustomPlot**>(_v) = _t->parentPlot(); break;
        case 2: *reinterpret_cast<QCPLayerable**>(_v) = _t->parentLayerable(); break;
        case 3: *reinterpret_cast<QCPLayer**>(_v) = _t->layer(); break;
        case 4: *reinterpret_cast<bool*>(_v) = _t->antialiased(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setVisible(*reinterpret_cast<bool*>(_v)); break;
        case 3: _t->setLayer(*reinterpret_cast<QCPLayer**>(_v)); break;
        case 4: _t->setAntialiased(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPLayerable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPLayerable::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12QCPLayerableE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QCPLayerable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void QCPLayerable::layerChanged(QCPLayer * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN14QCPMarginGroupE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPMarginGroup::qt_create_metaobjectdata<qt_meta_tag_ZN14QCPMarginGroupE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPMarginGroup"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPMarginGroup, qt_meta_tag_ZN14QCPMarginGroupE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPMarginGroup::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14QCPMarginGroupE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14QCPMarginGroupE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14QCPMarginGroupE_t>.metaTypes,
    nullptr
} };

void QCPMarginGroup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPMarginGroup *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *QCPMarginGroup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPMarginGroup::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14QCPMarginGroupE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QCPMarginGroup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN16QCPLayoutElementE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPLayoutElement::qt_create_metaobjectdata<qt_meta_tag_ZN16QCPLayoutElementE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPLayoutElement",
        "layout",
        "QCPLayout*",
        "rect",
        "QRect",
        "outerRect",
        "margins",
        "QMargins",
        "minimumMargins",
        "minimumSize",
        "QSize",
        "maximumSize",
        "UpdatePhase",
        "upPreparation",
        "upMargins",
        "upLayout"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'layout'
        QtMocHelpers::PropertyData<QCPLayout*>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'rect'
        QtMocHelpers::PropertyData<QRect>(3, 0x80000000 | 4, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'outerRect'
        QtMocHelpers::PropertyData<QRect>(5, 0x80000000 | 4, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'margins'
        QtMocHelpers::PropertyData<QMargins>(6, 0x80000000 | 7, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'minimumMargins'
        QtMocHelpers::PropertyData<QMargins>(8, 0x80000000 | 7, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'minimumSize'
        QtMocHelpers::PropertyData<QSize>(9, 0x80000000 | 10, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'maximumSize'
        QtMocHelpers::PropertyData<QSize>(11, 0x80000000 | 10, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'UpdatePhase'
        QtMocHelpers::EnumData<enum UpdatePhase>(12, 12, QMC::EnumFlags{}).add({
            {   13, UpdatePhase::upPreparation },
            {   14, UpdatePhase::upMargins },
            {   15, UpdatePhase::upLayout },
        }),
    };
    return QtMocHelpers::metaObjectData<QCPLayoutElement, qt_meta_tag_ZN16QCPLayoutElementE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPLayoutElement::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPLayerable::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16QCPLayoutElementE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16QCPLayoutElementE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN16QCPLayoutElementE_t>.metaTypes,
    nullptr
} };

void QCPLayoutElement::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPLayoutElement *>(_o);
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPLayout* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QCPLayout**>(_v) = _t->layout(); break;
        case 1: *reinterpret_cast<QRect*>(_v) = _t->rect(); break;
        case 2: *reinterpret_cast<QRect*>(_v) = _t->outerRect(); break;
        case 3: *reinterpret_cast<QMargins*>(_v) = _t->margins(); break;
        case 4: *reinterpret_cast<QMargins*>(_v) = _t->minimumMargins(); break;
        case 5: *reinterpret_cast<QSize*>(_v) = _t->minimumSize(); break;
        case 6: *reinterpret_cast<QSize*>(_v) = _t->maximumSize(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 2: _t->setOuterRect(*reinterpret_cast<QRect*>(_v)); break;
        case 3: _t->setMargins(*reinterpret_cast<QMargins*>(_v)); break;
        case 4: _t->setMinimumMargins(*reinterpret_cast<QMargins*>(_v)); break;
        case 5: _t->setMinimumSize(*reinterpret_cast<QSize*>(_v)); break;
        case 6: _t->setMaximumSize(*reinterpret_cast<QSize*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPLayoutElement::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPLayoutElement::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16QCPLayoutElementE_t>.strings))
        return static_cast<void*>(this);
    return QCPLayerable::qt_metacast(_clname);
}

int QCPLayoutElement::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPLayerable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN9QCPLayoutE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPLayout::qt_create_metaobjectdata<qt_meta_tag_ZN9QCPLayoutE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPLayout"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPLayout, qt_meta_tag_ZN9QCPLayoutE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPLayout::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPLayoutElement::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9QCPLayoutE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9QCPLayoutE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9QCPLayoutE_t>.metaTypes,
    nullptr
} };

void QCPLayout::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPLayout *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *QCPLayout::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPLayout::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9QCPLayoutE_t>.strings))
        return static_cast<void*>(this);
    return QCPLayoutElement::qt_metacast(_clname);
}

int QCPLayout::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPLayoutElement::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN13QCPLayoutGridE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPLayoutGrid::qt_create_metaobjectdata<qt_meta_tag_ZN13QCPLayoutGridE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPLayoutGrid",
        "rowCount",
        "columnCount",
        "columnStretchFactors",
        "QList<double>",
        "rowStretchFactors",
        "columnSpacing",
        "rowSpacing"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'rowCount'
        QtMocHelpers::PropertyData<int>(1, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'columnCount'
        QtMocHelpers::PropertyData<int>(2, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'columnStretchFactors'
        QtMocHelpers::PropertyData<QList<double>>(3, 0x80000000 | 4, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'rowStretchFactors'
        QtMocHelpers::PropertyData<QList<double>>(5, 0x80000000 | 4, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'columnSpacing'
        QtMocHelpers::PropertyData<int>(6, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'rowSpacing'
        QtMocHelpers::PropertyData<int>(7, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPLayoutGrid, qt_meta_tag_ZN13QCPLayoutGridE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPLayoutGrid::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPLayout::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13QCPLayoutGridE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13QCPLayoutGridE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13QCPLayoutGridE_t>.metaTypes,
    nullptr
} };

void QCPLayoutGrid::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPLayoutGrid *>(_o);
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
        case 2:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<double> >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->rowCount(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->columnCount(); break;
        case 2: *reinterpret_cast<QList<double>*>(_v) = _t->columnStretchFactors(); break;
        case 3: *reinterpret_cast<QList<double>*>(_v) = _t->rowStretchFactors(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->columnSpacing(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->rowSpacing(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 2: _t->setColumnStretchFactors(*reinterpret_cast<QList<double>*>(_v)); break;
        case 3: _t->setRowStretchFactors(*reinterpret_cast<QList<double>*>(_v)); break;
        case 4: _t->setColumnSpacing(*reinterpret_cast<int*>(_v)); break;
        case 5: _t->setRowSpacing(*reinterpret_cast<int*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPLayoutGrid::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPLayoutGrid::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13QCPLayoutGridE_t>.strings))
        return static_cast<void*>(this);
    return QCPLayout::qt_metacast(_clname);
}

int QCPLayoutGrid::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPLayout::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN14QCPLayoutInsetE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPLayoutInset::qt_create_metaobjectdata<qt_meta_tag_ZN14QCPLayoutInsetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPLayoutInset"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPLayoutInset, qt_meta_tag_ZN14QCPLayoutInsetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPLayoutInset::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPLayout::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14QCPLayoutInsetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14QCPLayoutInsetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14QCPLayoutInsetE_t>.metaTypes,
    nullptr
} };

void QCPLayoutInset::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPLayoutInset *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *QCPLayoutInset::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPLayoutInset::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14QCPLayoutInsetE_t>.strings))
        return static_cast<void*>(this);
    return QCPLayout::qt_metacast(_clname);
}

int QCPLayoutInset::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPLayout::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN13QCPLineEndingE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPLineEnding::qt_create_metaobjectdata<qt_meta_tag_ZN13QCPLineEndingE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPLineEnding",
        "EndingStyle",
        "esNone",
        "esFlatArrow",
        "esSpikeArrow",
        "esLineArrow",
        "esDisc",
        "esSquare",
        "esDiamond",
        "esBar",
        "esHalfBar",
        "esSkewedBar"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'EndingStyle'
        QtMocHelpers::EnumData<enum EndingStyle>(1, 1, QMC::EnumFlags{}).add({
            {    2, EndingStyle::esNone },
            {    3, EndingStyle::esFlatArrow },
            {    4, EndingStyle::esSpikeArrow },
            {    5, EndingStyle::esLineArrow },
            {    6, EndingStyle::esDisc },
            {    7, EndingStyle::esSquare },
            {    8, EndingStyle::esDiamond },
            {    9, EndingStyle::esBar },
            {   10, EndingStyle::esHalfBar },
            {   11, EndingStyle::esSkewedBar },
        }),
    };
    return QtMocHelpers::metaObjectData<QCPLineEnding, qt_meta_tag_ZN13QCPLineEndingE_t>(QMC::PropertyAccessInStaticMetaCall, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPLineEnding::staticMetaObject = { {
    nullptr,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13QCPLineEndingE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13QCPLineEndingE_t>.data,
    nullptr,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13QCPLineEndingE_t>.metaTypes,
    nullptr
} };

namespace {
struct qt_meta_tag_ZN7QCPGridE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPGrid::qt_create_metaobjectdata<qt_meta_tag_ZN7QCPGridE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPGrid",
        "subGridVisible",
        "antialiasedSubGrid",
        "antialiasedZeroLine",
        "pen",
        "QPen",
        "subGridPen",
        "zeroLinePen"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'subGridVisible'
        QtMocHelpers::PropertyData<bool>(1, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'antialiasedSubGrid'
        QtMocHelpers::PropertyData<bool>(2, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'antialiasedZeroLine'
        QtMocHelpers::PropertyData<bool>(3, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'pen'
        QtMocHelpers::PropertyData<QPen>(4, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'subGridPen'
        QtMocHelpers::PropertyData<QPen>(6, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'zeroLinePen'
        QtMocHelpers::PropertyData<QPen>(7, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPGrid, qt_meta_tag_ZN7QCPGridE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPGrid::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPLayerable::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7QCPGridE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7QCPGridE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7QCPGridE_t>.metaTypes,
    nullptr
} };

void QCPGrid::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPGrid *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<bool*>(_v) = _t->subGridVisible(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->antialiasedSubGrid(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->antialiasedZeroLine(); break;
        case 3: *reinterpret_cast<QPen*>(_v) = _t->pen(); break;
        case 4: *reinterpret_cast<QPen*>(_v) = _t->subGridPen(); break;
        case 5: *reinterpret_cast<QPen*>(_v) = _t->zeroLinePen(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setSubGridVisible(*reinterpret_cast<bool*>(_v)); break;
        case 1: _t->setAntialiasedSubGrid(*reinterpret_cast<bool*>(_v)); break;
        case 2: _t->setAntialiasedZeroLine(*reinterpret_cast<bool*>(_v)); break;
        case 3: _t->setPen(*reinterpret_cast<QPen*>(_v)); break;
        case 4: _t->setSubGridPen(*reinterpret_cast<QPen*>(_v)); break;
        case 5: _t->setZeroLinePen(*reinterpret_cast<QPen*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPGrid::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPGrid::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7QCPGridE_t>.strings))
        return static_cast<void*>(this);
    return QCPLayerable::qt_metacast(_clname);
}

int QCPGrid::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPLayerable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN7QCPAxisE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPAxis::qt_create_metaobjectdata<qt_meta_tag_ZN7QCPAxisE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPAxis",
        "ticksRequest",
        "",
        "rangeChanged",
        "QCPRange",
        "newRange",
        "oldRange",
        "scaleTypeChanged",
        "QCPAxis::ScaleType",
        "scaleType",
        "selectionChanged",
        "QCPAxis::SelectableParts",
        "parts",
        "selectableChanged",
        "setScaleType",
        "type",
        "setRange",
        "range",
        "setSelectableParts",
        "selectableParts",
        "setSelectedParts",
        "selectedParts",
        "axisType",
        "AxisType",
        "axisRect",
        "QCPAxisRect*",
        "ScaleType",
        "scaleLogBase",
        "rangeReversed",
        "autoTicks",
        "autoTickCount",
        "autoTickLabels",
        "autoTickStep",
        "autoSubTicks",
        "ticks",
        "tickLabels",
        "tickLabelPadding",
        "tickLabelType",
        "LabelType",
        "tickLabelFont",
        "QFont",
        "tickLabelColor",
        "QColor",
        "tickLabelRotation",
        "tickLabelSide",
        "LabelSide",
        "dateTimeFormat",
        "dateTimeSpec",
        "Qt::TimeSpec",
        "numberFormat",
        "numberPrecision",
        "tickStep",
        "tickVector",
        "QList<double>",
        "tickVectorLabels",
        "tickLengthIn",
        "tickLengthOut",
        "subTickCount",
        "subTickLengthIn",
        "subTickLengthOut",
        "basePen",
        "QPen",
        "tickPen",
        "subTickPen",
        "labelFont",
        "labelColor",
        "label",
        "labelPadding",
        "padding",
        "offset",
        "SelectableParts",
        "selectedTickLabelFont",
        "selectedLabelFont",
        "selectedTickLabelColor",
        "selectedLabelColor",
        "selectedBasePen",
        "selectedTickPen",
        "selectedSubTickPen",
        "lowerEnding",
        "QCPLineEnding",
        "upperEnding",
        "grid",
        "QCPGrid*",
        "atLeft",
        "atRight",
        "atTop",
        "atBottom",
        "AxisTypes",
        "ltNumber",
        "ltDateTime",
        "lsInside",
        "lsOutside",
        "stLinear",
        "stLogarithmic",
        "SelectablePart",
        "spNone",
        "spAxis",
        "spTickLabels",
        "spAxisLabel"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'ticksRequest'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'rangeChanged'
        QtMocHelpers::SignalData<void(const QCPRange &)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 },
        }}),
        // Signal 'rangeChanged'
        QtMocHelpers::SignalData<void(const QCPRange &, const QCPRange &)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 }, { 0x80000000 | 4, 6 },
        }}),
        // Signal 'scaleTypeChanged'
        QtMocHelpers::SignalData<void(QCPAxis::ScaleType)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Signal 'selectionChanged'
        QtMocHelpers::SignalData<void(const QCPAxis::SelectableParts &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
        // Signal 'selectableChanged'
        QtMocHelpers::SignalData<void(const QCPAxis::SelectableParts &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
        // Slot 'setScaleType'
        QtMocHelpers::SlotData<void(QCPAxis::ScaleType)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 15 },
        }}),
        // Slot 'setRange'
        QtMocHelpers::SlotData<void(const QCPRange &)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 17 },
        }}),
        // Slot 'setSelectableParts'
        QtMocHelpers::SlotData<void(const QCPAxis::SelectableParts &)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 11, 19 },
        }}),
        // Slot 'setSelectedParts'
        QtMocHelpers::SlotData<void(const QCPAxis::SelectableParts &)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 11, 21 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'axisType'
        QtMocHelpers::PropertyData<enum AxisType>(22, 0x80000000 | 23, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'axisRect'
        QtMocHelpers::PropertyData<QCPAxisRect*>(24, 0x80000000 | 25, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'scaleType'
        QtMocHelpers::PropertyData<enum ScaleType>(9, 0x80000000 | 26, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 3),
        // property 'scaleLogBase'
        QtMocHelpers::PropertyData<double>(27, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'range'
        QtMocHelpers::PropertyData<QCPRange>(17, 0x80000000 | 4, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 1),
        // property 'rangeReversed'
        QtMocHelpers::PropertyData<bool>(28, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'autoTicks'
        QtMocHelpers::PropertyData<bool>(29, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'autoTickCount'
        QtMocHelpers::PropertyData<int>(30, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'autoTickLabels'
        QtMocHelpers::PropertyData<bool>(31, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'autoTickStep'
        QtMocHelpers::PropertyData<bool>(32, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'autoSubTicks'
        QtMocHelpers::PropertyData<bool>(33, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'ticks'
        QtMocHelpers::PropertyData<bool>(34, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'tickLabels'
        QtMocHelpers::PropertyData<bool>(35, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'tickLabelPadding'
        QtMocHelpers::PropertyData<int>(36, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'tickLabelType'
        QtMocHelpers::PropertyData<enum LabelType>(37, 0x80000000 | 38, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'tickLabelFont'
        QtMocHelpers::PropertyData<QFont>(39, 0x80000000 | 40, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'tickLabelColor'
        QtMocHelpers::PropertyData<QColor>(41, 0x80000000 | 42, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'tickLabelRotation'
        QtMocHelpers::PropertyData<double>(43, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'tickLabelSide'
        QtMocHelpers::PropertyData<enum LabelSide>(44, 0x80000000 | 45, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'dateTimeFormat'
        QtMocHelpers::PropertyData<QString>(46, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'dateTimeSpec'
        QtMocHelpers::PropertyData<Qt::TimeSpec>(47, 0x80000000 | 48, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'numberFormat'
        QtMocHelpers::PropertyData<QString>(49, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'numberPrecision'
        QtMocHelpers::PropertyData<int>(50, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'tickStep'
        QtMocHelpers::PropertyData<double>(51, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'tickVector'
        QtMocHelpers::PropertyData<QList<double>>(52, 0x80000000 | 53, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'tickVectorLabels'
        QtMocHelpers::PropertyData<QList<QString>>(54, QMetaType::QStringList, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'tickLengthIn'
        QtMocHelpers::PropertyData<int>(55, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'tickLengthOut'
        QtMocHelpers::PropertyData<int>(56, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'subTickCount'
        QtMocHelpers::PropertyData<int>(57, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'subTickLengthIn'
        QtMocHelpers::PropertyData<int>(58, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'subTickLengthOut'
        QtMocHelpers::PropertyData<int>(59, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'basePen'
        QtMocHelpers::PropertyData<QPen>(60, 0x80000000 | 61, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'tickPen'
        QtMocHelpers::PropertyData<QPen>(62, 0x80000000 | 61, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'subTickPen'
        QtMocHelpers::PropertyData<QPen>(63, 0x80000000 | 61, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'labelFont'
        QtMocHelpers::PropertyData<QFont>(64, 0x80000000 | 40, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'labelColor'
        QtMocHelpers::PropertyData<QColor>(65, 0x80000000 | 42, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'label'
        QtMocHelpers::PropertyData<QString>(66, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'labelPadding'
        QtMocHelpers::PropertyData<int>(67, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'padding'
        QtMocHelpers::PropertyData<int>(68, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'offset'
        QtMocHelpers::PropertyData<int>(69, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'selectedParts'
        QtMocHelpers::PropertyData<SelectableParts>(21, 0x80000000 | 70, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 4),
        // property 'selectableParts'
        QtMocHelpers::PropertyData<SelectableParts>(19, 0x80000000 | 70, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 5),
        // property 'selectedTickLabelFont'
        QtMocHelpers::PropertyData<QFont>(71, 0x80000000 | 40, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedLabelFont'
        QtMocHelpers::PropertyData<QFont>(72, 0x80000000 | 40, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedTickLabelColor'
        QtMocHelpers::PropertyData<QColor>(73, 0x80000000 | 42, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedLabelColor'
        QtMocHelpers::PropertyData<QColor>(74, 0x80000000 | 42, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedBasePen'
        QtMocHelpers::PropertyData<QPen>(75, 0x80000000 | 61, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedTickPen'
        QtMocHelpers::PropertyData<QPen>(76, 0x80000000 | 61, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedSubTickPen'
        QtMocHelpers::PropertyData<QPen>(77, 0x80000000 | 61, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'lowerEnding'
        QtMocHelpers::PropertyData<QCPLineEnding>(78, 0x80000000 | 79, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'upperEnding'
        QtMocHelpers::PropertyData<QCPLineEnding>(80, 0x80000000 | 79, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'grid'
        QtMocHelpers::PropertyData<QCPGrid*>(81, 0x80000000 | 82, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'AxisType'
        QtMocHelpers::EnumData<enum AxisType>(23, 23, QMC::EnumFlags{}).add({
            {   83, AxisType::atLeft },
            {   84, AxisType::atRight },
            {   85, AxisType::atTop },
            {   86, AxisType::atBottom },
        }),
        // flag 'AxisTypes'
        QtMocHelpers::EnumData<AxisTypes>(87, 23, QMC::EnumIsFlag).add({
            {   83, AxisType::atLeft },
            {   84, AxisType::atRight },
            {   85, AxisType::atTop },
            {   86, AxisType::atBottom },
        }),
        // enum 'LabelType'
        QtMocHelpers::EnumData<enum LabelType>(38, 38, QMC::EnumFlags{}).add({
            {   88, LabelType::ltNumber },
            {   89, LabelType::ltDateTime },
        }),
        // enum 'LabelSide'
        QtMocHelpers::EnumData<enum LabelSide>(45, 45, QMC::EnumFlags{}).add({
            {   90, LabelSide::lsInside },
            {   91, LabelSide::lsOutside },
        }),
        // enum 'ScaleType'
        QtMocHelpers::EnumData<enum ScaleType>(26, 26, QMC::EnumFlags{}).add({
            {   92, ScaleType::stLinear },
            {   93, ScaleType::stLogarithmic },
        }),
        // enum 'SelectablePart'
        QtMocHelpers::EnumData<enum SelectablePart>(94, 94, QMC::EnumFlags{}).add({
            {   95, SelectablePart::spNone },
            {   96, SelectablePart::spAxis },
            {   97, SelectablePart::spTickLabels },
            {   98, SelectablePart::spAxisLabel },
        }),
        // flag 'SelectableParts'
        QtMocHelpers::EnumData<SelectableParts>(70, 94, QMC::EnumIsFlag).add({
            {   95, SelectablePart::spNone },
            {   96, SelectablePart::spAxis },
            {   97, SelectablePart::spTickLabels },
            {   98, SelectablePart::spAxisLabel },
        }),
    };
    return QtMocHelpers::metaObjectData<QCPAxis, qt_meta_tag_ZN7QCPAxisE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPAxis::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPLayerable::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7QCPAxisE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7QCPAxisE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7QCPAxisE_t>.metaTypes,
    nullptr
} };

void QCPAxis::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPAxis *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->ticksRequest(); break;
        case 1: _t->rangeChanged((*reinterpret_cast<std::add_pointer_t<QCPRange>>(_a[1]))); break;
        case 2: _t->rangeChanged((*reinterpret_cast<std::add_pointer_t<QCPRange>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QCPRange>>(_a[2]))); break;
        case 3: _t->scaleTypeChanged((*reinterpret_cast<std::add_pointer_t<QCPAxis::ScaleType>>(_a[1]))); break;
        case 4: _t->selectionChanged((*reinterpret_cast<std::add_pointer_t<QCPAxis::SelectableParts>>(_a[1]))); break;
        case 5: _t->selectableChanged((*reinterpret_cast<std::add_pointer_t<QCPAxis::SelectableParts>>(_a[1]))); break;
        case 6: _t->setScaleType((*reinterpret_cast<std::add_pointer_t<QCPAxis::ScaleType>>(_a[1]))); break;
        case 7: _t->setRange((*reinterpret_cast<std::add_pointer_t<QCPRange>>(_a[1]))); break;
        case 8: _t->setSelectableParts((*reinterpret_cast<std::add_pointer_t<QCPAxis::SelectableParts>>(_a[1]))); break;
        case 9: _t->setSelectedParts((*reinterpret_cast<std::add_pointer_t<QCPAxis::SelectableParts>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (QCPAxis::*)()>(_a, &QCPAxis::ticksRequest, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCPAxis::*)(const QCPRange & )>(_a, &QCPAxis::rangeChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCPAxis::*)(const QCPRange & , const QCPRange & )>(_a, &QCPAxis::rangeChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCPAxis::*)(QCPAxis::ScaleType )>(_a, &QCPAxis::scaleTypeChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCPAxis::*)(const QCPAxis::SelectableParts & )>(_a, &QCPAxis::selectionChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCPAxis::*)(const QCPAxis::SelectableParts & )>(_a, &QCPAxis::selectableChanged, 5))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPAxisRect* >(); break;
        case 51:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPGrid* >(); break;
        case 24:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<double> >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: QtMocHelpers::assignFlags<enum AxisType>(_v, _t->axisType()); break;
        case 1: *reinterpret_cast<QCPAxisRect**>(_v) = _t->axisRect(); break;
        case 2: *reinterpret_cast<enum ScaleType*>(_v) = _t->scaleType(); break;
        case 3: *reinterpret_cast<double*>(_v) = _t->scaleLogBase(); break;
        case 4: *reinterpret_cast<QCPRange*>(_v) = _t->range(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->rangeReversed(); break;
        case 6: *reinterpret_cast<bool*>(_v) = _t->autoTicks(); break;
        case 7: *reinterpret_cast<int*>(_v) = _t->autoTickCount(); break;
        case 8: *reinterpret_cast<bool*>(_v) = _t->autoTickLabels(); break;
        case 9: *reinterpret_cast<bool*>(_v) = _t->autoTickStep(); break;
        case 10: *reinterpret_cast<bool*>(_v) = _t->autoSubTicks(); break;
        case 11: *reinterpret_cast<bool*>(_v) = _t->ticks(); break;
        case 12: *reinterpret_cast<bool*>(_v) = _t->tickLabels(); break;
        case 13: *reinterpret_cast<int*>(_v) = _t->tickLabelPadding(); break;
        case 14: *reinterpret_cast<enum LabelType*>(_v) = _t->tickLabelType(); break;
        case 15: *reinterpret_cast<QFont*>(_v) = _t->tickLabelFont(); break;
        case 16: *reinterpret_cast<QColor*>(_v) = _t->tickLabelColor(); break;
        case 17: *reinterpret_cast<double*>(_v) = _t->tickLabelRotation(); break;
        case 18: *reinterpret_cast<enum LabelSide*>(_v) = _t->tickLabelSide(); break;
        case 19: *reinterpret_cast<QString*>(_v) = _t->dateTimeFormat(); break;
        case 20: *reinterpret_cast<Qt::TimeSpec*>(_v) = _t->dateTimeSpec(); break;
        case 21: *reinterpret_cast<QString*>(_v) = _t->numberFormat(); break;
        case 22: *reinterpret_cast<int*>(_v) = _t->numberPrecision(); break;
        case 23: *reinterpret_cast<double*>(_v) = _t->tickStep(); break;
        case 24: *reinterpret_cast<QList<double>*>(_v) = _t->tickVector(); break;
        case 25: *reinterpret_cast<QList<QString>*>(_v) = _t->tickVectorLabels(); break;
        case 26: *reinterpret_cast<int*>(_v) = _t->tickLengthIn(); break;
        case 27: *reinterpret_cast<int*>(_v) = _t->tickLengthOut(); break;
        case 28: *reinterpret_cast<int*>(_v) = _t->subTickCount(); break;
        case 29: *reinterpret_cast<int*>(_v) = _t->subTickLengthIn(); break;
        case 30: *reinterpret_cast<int*>(_v) = _t->subTickLengthOut(); break;
        case 31: *reinterpret_cast<QPen*>(_v) = _t->basePen(); break;
        case 32: *reinterpret_cast<QPen*>(_v) = _t->tickPen(); break;
        case 33: *reinterpret_cast<QPen*>(_v) = _t->subTickPen(); break;
        case 34: *reinterpret_cast<QFont*>(_v) = _t->labelFont(); break;
        case 35: *reinterpret_cast<QColor*>(_v) = _t->labelColor(); break;
        case 36: *reinterpret_cast<QString*>(_v) = _t->label(); break;
        case 37: *reinterpret_cast<int*>(_v) = _t->labelPadding(); break;
        case 38: *reinterpret_cast<int*>(_v) = _t->padding(); break;
        case 39: *reinterpret_cast<int*>(_v) = _t->offset(); break;
        case 40: QtMocHelpers::assignFlags<SelectableParts>(_v, _t->selectedParts()); break;
        case 41: QtMocHelpers::assignFlags<SelectableParts>(_v, _t->selectableParts()); break;
        case 42: *reinterpret_cast<QFont*>(_v) = _t->selectedTickLabelFont(); break;
        case 43: *reinterpret_cast<QFont*>(_v) = _t->selectedLabelFont(); break;
        case 44: *reinterpret_cast<QColor*>(_v) = _t->selectedTickLabelColor(); break;
        case 45: *reinterpret_cast<QColor*>(_v) = _t->selectedLabelColor(); break;
        case 46: *reinterpret_cast<QPen*>(_v) = _t->selectedBasePen(); break;
        case 47: *reinterpret_cast<QPen*>(_v) = _t->selectedTickPen(); break;
        case 48: *reinterpret_cast<QPen*>(_v) = _t->selectedSubTickPen(); break;
        case 49: *reinterpret_cast<QCPLineEnding*>(_v) = _t->lowerEnding(); break;
        case 50: *reinterpret_cast<QCPLineEnding*>(_v) = _t->upperEnding(); break;
        case 51: *reinterpret_cast<QCPGrid**>(_v) = _t->grid(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 2: _t->setScaleType(*reinterpret_cast<enum ScaleType*>(_v)); break;
        case 3: _t->setScaleLogBase(*reinterpret_cast<double*>(_v)); break;
        case 4: _t->setRange(*reinterpret_cast<QCPRange*>(_v)); break;
        case 5: _t->setRangeReversed(*reinterpret_cast<bool*>(_v)); break;
        case 6: _t->setAutoTicks(*reinterpret_cast<bool*>(_v)); break;
        case 7: _t->setAutoTickCount(*reinterpret_cast<int*>(_v)); break;
        case 8: _t->setAutoTickLabels(*reinterpret_cast<bool*>(_v)); break;
        case 9: _t->setAutoTickStep(*reinterpret_cast<bool*>(_v)); break;
        case 10: _t->setAutoSubTicks(*reinterpret_cast<bool*>(_v)); break;
        case 11: _t->setTicks(*reinterpret_cast<bool*>(_v)); break;
        case 12: _t->setTickLabels(*reinterpret_cast<bool*>(_v)); break;
        case 13: _t->setTickLabelPadding(*reinterpret_cast<int*>(_v)); break;
        case 14: _t->setTickLabelType(*reinterpret_cast<enum LabelType*>(_v)); break;
        case 15: _t->setTickLabelFont(*reinterpret_cast<QFont*>(_v)); break;
        case 16: _t->setTickLabelColor(*reinterpret_cast<QColor*>(_v)); break;
        case 17: _t->setTickLabelRotation(*reinterpret_cast<double*>(_v)); break;
        case 18: _t->setTickLabelSide(*reinterpret_cast<enum LabelSide*>(_v)); break;
        case 19: _t->setDateTimeFormat(*reinterpret_cast<QString*>(_v)); break;
        case 20: _t->setDateTimeSpec(*reinterpret_cast<Qt::TimeSpec*>(_v)); break;
        case 21: _t->setNumberFormat(*reinterpret_cast<QString*>(_v)); break;
        case 22: _t->setNumberPrecision(*reinterpret_cast<int*>(_v)); break;
        case 23: _t->setTickStep(*reinterpret_cast<double*>(_v)); break;
        case 24: _t->setTickVector(*reinterpret_cast<QList<double>*>(_v)); break;
        case 25: _t->setTickVectorLabels(*reinterpret_cast<QList<QString>*>(_v)); break;
        case 26: _t->setTickLengthIn(*reinterpret_cast<int*>(_v)); break;
        case 27: _t->setTickLengthOut(*reinterpret_cast<int*>(_v)); break;
        case 28: _t->setSubTickCount(*reinterpret_cast<int*>(_v)); break;
        case 29: _t->setSubTickLengthIn(*reinterpret_cast<int*>(_v)); break;
        case 30: _t->setSubTickLengthOut(*reinterpret_cast<int*>(_v)); break;
        case 31: _t->setBasePen(*reinterpret_cast<QPen*>(_v)); break;
        case 32: _t->setTickPen(*reinterpret_cast<QPen*>(_v)); break;
        case 33: _t->setSubTickPen(*reinterpret_cast<QPen*>(_v)); break;
        case 34: _t->setLabelFont(*reinterpret_cast<QFont*>(_v)); break;
        case 35: _t->setLabelColor(*reinterpret_cast<QColor*>(_v)); break;
        case 36: _t->setLabel(*reinterpret_cast<QString*>(_v)); break;
        case 37: _t->setLabelPadding(*reinterpret_cast<int*>(_v)); break;
        case 38: _t->setPadding(*reinterpret_cast<int*>(_v)); break;
        case 39: _t->setOffset(*reinterpret_cast<int*>(_v)); break;
        case 40: _t->setSelectedParts(*reinterpret_cast<SelectableParts*>(_v)); break;
        case 41: _t->setSelectableParts(*reinterpret_cast<SelectableParts*>(_v)); break;
        case 42: _t->setSelectedTickLabelFont(*reinterpret_cast<QFont*>(_v)); break;
        case 43: _t->setSelectedLabelFont(*reinterpret_cast<QFont*>(_v)); break;
        case 44: _t->setSelectedTickLabelColor(*reinterpret_cast<QColor*>(_v)); break;
        case 45: _t->setSelectedLabelColor(*reinterpret_cast<QColor*>(_v)); break;
        case 46: _t->setSelectedBasePen(*reinterpret_cast<QPen*>(_v)); break;
        case 47: _t->setSelectedTickPen(*reinterpret_cast<QPen*>(_v)); break;
        case 48: _t->setSelectedSubTickPen(*reinterpret_cast<QPen*>(_v)); break;
        case 49: _t->setLowerEnding(*reinterpret_cast<QCPLineEnding*>(_v)); break;
        case 50: _t->setUpperEnding(*reinterpret_cast<QCPLineEnding*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPAxis::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPAxis::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7QCPAxisE_t>.strings))
        return static_cast<void*>(this);
    return QCPLayerable::qt_metacast(_clname);
}

int QCPAxis::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPLayerable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 52;
    }
    return _id;
}

// SIGNAL 0
void QCPAxis::ticksRequest()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QCPAxis::rangeChanged(const QCPRange & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void QCPAxis::rangeChanged(const QCPRange & _t1, const QCPRange & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void QCPAxis::scaleTypeChanged(QCPAxis::ScaleType _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void QCPAxis::selectionChanged(const QCPAxis::SelectableParts & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void QCPAxis::selectableChanged(const QCPAxis::SelectableParts & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN20QCPAbstractPlottableE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPAbstractPlottable::qt_create_metaobjectdata<qt_meta_tag_ZN20QCPAbstractPlottableE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPAbstractPlottable",
        "selectionChanged",
        "",
        "selected",
        "selectableChanged",
        "selectable",
        "setSelectable",
        "setSelected",
        "name",
        "antialiasedFill",
        "antialiasedScatters",
        "antialiasedErrorBars",
        "pen",
        "QPen",
        "selectedPen",
        "brush",
        "QBrush",
        "selectedBrush",
        "keyAxis",
        "QCPAxis*",
        "valueAxis"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'selectionChanged'
        QtMocHelpers::SignalData<void(bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Signal 'selectableChanged'
        QtMocHelpers::SignalData<void(bool)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 5 },
        }}),
        // Slot 'setSelectable'
        QtMocHelpers::SlotData<void(bool)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 5 },
        }}),
        // Slot 'setSelected'
        QtMocHelpers::SlotData<void(bool)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'name'
        QtMocHelpers::PropertyData<QString>(8, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'antialiasedFill'
        QtMocHelpers::PropertyData<bool>(9, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'antialiasedScatters'
        QtMocHelpers::PropertyData<bool>(10, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'antialiasedErrorBars'
        QtMocHelpers::PropertyData<bool>(11, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'pen'
        QtMocHelpers::PropertyData<QPen>(12, 0x80000000 | 13, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedPen'
        QtMocHelpers::PropertyData<QPen>(14, 0x80000000 | 13, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'brush'
        QtMocHelpers::PropertyData<QBrush>(15, 0x80000000 | 16, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedBrush'
        QtMocHelpers::PropertyData<QBrush>(17, 0x80000000 | 16, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'keyAxis'
        QtMocHelpers::PropertyData<QCPAxis*>(18, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'valueAxis'
        QtMocHelpers::PropertyData<QCPAxis*>(20, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectable'
        QtMocHelpers::PropertyData<bool>(5, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
        // property 'selected'
        QtMocHelpers::PropertyData<bool>(3, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPAbstractPlottable, qt_meta_tag_ZN20QCPAbstractPlottableE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPAbstractPlottable::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPLayerable::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20QCPAbstractPlottableE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20QCPAbstractPlottableE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN20QCPAbstractPlottableE_t>.metaTypes,
    nullptr
} };

void QCPAbstractPlottable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPAbstractPlottable *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->selectionChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->selectableChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->setSelectable((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->setSelected((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (QCPAbstractPlottable::*)(bool )>(_a, &QCPAbstractPlottable::selectionChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCPAbstractPlottable::*)(bool )>(_a, &QCPAbstractPlottable::selectableChanged, 1))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 9:
        case 8:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPAxis* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->name(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->antialiasedFill(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->antialiasedScatters(); break;
        case 3: *reinterpret_cast<bool*>(_v) = _t->antialiasedErrorBars(); break;
        case 4: *reinterpret_cast<QPen*>(_v) = _t->pen(); break;
        case 5: *reinterpret_cast<QPen*>(_v) = _t->selectedPen(); break;
        case 6: *reinterpret_cast<QBrush*>(_v) = _t->brush(); break;
        case 7: *reinterpret_cast<QBrush*>(_v) = _t->selectedBrush(); break;
        case 8: *reinterpret_cast<QCPAxis**>(_v) = _t->keyAxis(); break;
        case 9: *reinterpret_cast<QCPAxis**>(_v) = _t->valueAxis(); break;
        case 10: *reinterpret_cast<bool*>(_v) = _t->selectable(); break;
        case 11: *reinterpret_cast<bool*>(_v) = _t->selected(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setName(*reinterpret_cast<QString*>(_v)); break;
        case 1: _t->setAntialiasedFill(*reinterpret_cast<bool*>(_v)); break;
        case 2: _t->setAntialiasedScatters(*reinterpret_cast<bool*>(_v)); break;
        case 3: _t->setAntialiasedErrorBars(*reinterpret_cast<bool*>(_v)); break;
        case 4: _t->setPen(*reinterpret_cast<QPen*>(_v)); break;
        case 5: _t->setSelectedPen(*reinterpret_cast<QPen*>(_v)); break;
        case 6: _t->setBrush(*reinterpret_cast<QBrush*>(_v)); break;
        case 7: _t->setSelectedBrush(*reinterpret_cast<QBrush*>(_v)); break;
        case 8: _t->setKeyAxis(*reinterpret_cast<QCPAxis**>(_v)); break;
        case 9: _t->setValueAxis(*reinterpret_cast<QCPAxis**>(_v)); break;
        case 10: _t->setSelectable(*reinterpret_cast<bool*>(_v)); break;
        case 11: _t->setSelected(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPAbstractPlottable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPAbstractPlottable::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20QCPAbstractPlottableE_t>.strings))
        return static_cast<void*>(this);
    return QCPLayerable::qt_metacast(_clname);
}

int QCPAbstractPlottable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPLayerable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void QCPAbstractPlottable::selectionChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void QCPAbstractPlottable::selectableChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN15QCPAbstractItemE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPAbstractItem::qt_create_metaobjectdata<qt_meta_tag_ZN15QCPAbstractItemE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPAbstractItem",
        "selectionChanged",
        "",
        "selected",
        "selectableChanged",
        "selectable",
        "setSelectable",
        "setSelected",
        "clipToAxisRect",
        "clipAxisRect",
        "QCPAxisRect*"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'selectionChanged'
        QtMocHelpers::SignalData<void(bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Signal 'selectableChanged'
        QtMocHelpers::SignalData<void(bool)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 5 },
        }}),
        // Slot 'setSelectable'
        QtMocHelpers::SlotData<void(bool)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 5 },
        }}),
        // Slot 'setSelected'
        QtMocHelpers::SlotData<void(bool)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'clipToAxisRect'
        QtMocHelpers::PropertyData<bool>(8, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'clipAxisRect'
        QtMocHelpers::PropertyData<QCPAxisRect*>(9, 0x80000000 | 10, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectable'
        QtMocHelpers::PropertyData<bool>(5, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
        // property 'selected'
        QtMocHelpers::PropertyData<bool>(3, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPAbstractItem, qt_meta_tag_ZN15QCPAbstractItemE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPAbstractItem::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPLayerable::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15QCPAbstractItemE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15QCPAbstractItemE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15QCPAbstractItemE_t>.metaTypes,
    nullptr
} };

void QCPAbstractItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPAbstractItem *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->selectionChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->selectableChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->setSelectable((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->setSelected((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (QCPAbstractItem::*)(bool )>(_a, &QCPAbstractItem::selectionChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCPAbstractItem::*)(bool )>(_a, &QCPAbstractItem::selectableChanged, 1))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPAxisRect* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<bool*>(_v) = _t->clipToAxisRect(); break;
        case 1: *reinterpret_cast<QCPAxisRect**>(_v) = _t->clipAxisRect(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->selectable(); break;
        case 3: *reinterpret_cast<bool*>(_v) = _t->selected(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setClipToAxisRect(*reinterpret_cast<bool*>(_v)); break;
        case 1: _t->setClipAxisRect(*reinterpret_cast<QCPAxisRect**>(_v)); break;
        case 2: _t->setSelectable(*reinterpret_cast<bool*>(_v)); break;
        case 3: _t->setSelected(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPAbstractItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPAbstractItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15QCPAbstractItemE_t>.strings))
        return static_cast<void*>(this);
    return QCPLayerable::qt_metacast(_clname);
}

int QCPAbstractItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPLayerable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void QCPAbstractItem::selectionChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void QCPAbstractItem::selectableChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN11QCustomPlotE_t {};
} // unnamed namespace

template <> constexpr inline auto QCustomPlot::qt_create_metaobjectdata<qt_meta_tag_ZN11QCustomPlotE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCustomPlot",
        "mouseDoubleClick",
        "",
        "QMouseEvent*",
        "event",
        "mousePress",
        "mouseMove",
        "mouseRelease",
        "mouseWheel",
        "QWheelEvent*",
        "plottableClick",
        "QCPAbstractPlottable*",
        "plottable",
        "plottableDoubleClick",
        "itemClick",
        "QCPAbstractItem*",
        "item",
        "itemDoubleClick",
        "axisClick",
        "QCPAxis*",
        "axis",
        "QCPAxis::SelectablePart",
        "part",
        "axisDoubleClick",
        "legendClick",
        "QCPLegend*",
        "legend",
        "QCPAbstractLegendItem*",
        "legendDoubleClick",
        "titleClick",
        "QCPPlotTitle*",
        "title",
        "titleDoubleClick",
        "selectionChangedByUser",
        "beforeReplot",
        "afterReplot",
        "rescaleAxes",
        "onlyVisiblePlottables",
        "deselectAll",
        "replot",
        "QCustomPlot::RefreshPriority",
        "refreshPriority",
        "viewport",
        "QRect",
        "background",
        "QPixmap",
        "backgroundScaled",
        "backgroundScaledMode",
        "Qt::AspectRatioMode",
        "plotLayout",
        "QCPLayoutGrid*",
        "autoAddPlottableToLegend",
        "selectionTolerance",
        "noAntialiasingOnDrag",
        "multiSelectModifier",
        "Qt::KeyboardModifier",
        "LayerInsertMode",
        "limBelow",
        "limAbove"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'mouseDoubleClick'
        QtMocHelpers::SignalData<void(QMouseEvent *)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'mousePress'
        QtMocHelpers::SignalData<void(QMouseEvent *)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'mouseMove'
        QtMocHelpers::SignalData<void(QMouseEvent *)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'mouseRelease'
        QtMocHelpers::SignalData<void(QMouseEvent *)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'mouseWheel'
        QtMocHelpers::SignalData<void(QWheelEvent *)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 9, 4 },
        }}),
        // Signal 'plottableClick'
        QtMocHelpers::SignalData<void(QCPAbstractPlottable *, QMouseEvent *)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 11, 12 }, { 0x80000000 | 3, 4 },
        }}),
        // Signal 'plottableDoubleClick'
        QtMocHelpers::SignalData<void(QCPAbstractPlottable *, QMouseEvent *)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 11, 12 }, { 0x80000000 | 3, 4 },
        }}),
        // Signal 'itemClick'
        QtMocHelpers::SignalData<void(QCPAbstractItem *, QMouseEvent *)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 15, 16 }, { 0x80000000 | 3, 4 },
        }}),
        // Signal 'itemDoubleClick'
        QtMocHelpers::SignalData<void(QCPAbstractItem *, QMouseEvent *)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 15, 16 }, { 0x80000000 | 3, 4 },
        }}),
        // Signal 'axisClick'
        QtMocHelpers::SignalData<void(QCPAxis *, QCPAxis::SelectablePart, QMouseEvent *)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 19, 20 }, { 0x80000000 | 21, 22 }, { 0x80000000 | 3, 4 },
        }}),
        // Signal 'axisDoubleClick'
        QtMocHelpers::SignalData<void(QCPAxis *, QCPAxis::SelectablePart, QMouseEvent *)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 19, 20 }, { 0x80000000 | 21, 22 }, { 0x80000000 | 3, 4 },
        }}),
        // Signal 'legendClick'
        QtMocHelpers::SignalData<void(QCPLegend *, QCPAbstractLegendItem *, QMouseEvent *)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 25, 26 }, { 0x80000000 | 27, 16 }, { 0x80000000 | 3, 4 },
        }}),
        // Signal 'legendDoubleClick'
        QtMocHelpers::SignalData<void(QCPLegend *, QCPAbstractLegendItem *, QMouseEvent *)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 25, 26 }, { 0x80000000 | 27, 16 }, { 0x80000000 | 3, 4 },
        }}),
        // Signal 'titleClick'
        QtMocHelpers::SignalData<void(QMouseEvent *, QCPPlotTitle *)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { 0x80000000 | 30, 31 },
        }}),
        // Signal 'titleDoubleClick'
        QtMocHelpers::SignalData<void(QMouseEvent *, QCPPlotTitle *)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { 0x80000000 | 30, 31 },
        }}),
        // Signal 'selectionChangedByUser'
        QtMocHelpers::SignalData<void()>(33, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'beforeReplot'
        QtMocHelpers::SignalData<void()>(34, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'afterReplot'
        QtMocHelpers::SignalData<void()>(35, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'rescaleAxes'
        QtMocHelpers::SlotData<void(bool)>(36, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 37 },
        }}),
        // Slot 'rescaleAxes'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Slot 'deselectAll'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'replot'
        QtMocHelpers::SlotData<void(QCustomPlot::RefreshPriority)>(39, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 40, 41 },
        }}),
        // Slot 'replot'
        QtMocHelpers::SlotData<void()>(39, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'viewport'
        QtMocHelpers::PropertyData<QRect>(42, 0x80000000 | 43, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'background'
        QtMocHelpers::PropertyData<QPixmap>(44, 0x80000000 | 45, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'backgroundScaled'
        QtMocHelpers::PropertyData<bool>(46, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'backgroundScaledMode'
        QtMocHelpers::PropertyData<Qt::AspectRatioMode>(47, 0x80000000 | 48, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'plotLayout'
        QtMocHelpers::PropertyData<QCPLayoutGrid*>(49, 0x80000000 | 50, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'autoAddPlottableToLegend'
        QtMocHelpers::PropertyData<bool>(51, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'selectionTolerance'
        QtMocHelpers::PropertyData<int>(52, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'noAntialiasingOnDrag'
        QtMocHelpers::PropertyData<bool>(53, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'multiSelectModifier'
        QtMocHelpers::PropertyData<Qt::KeyboardModifier>(54, 0x80000000 | 55, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'LayerInsertMode'
        QtMocHelpers::EnumData<enum LayerInsertMode>(56, 56, QMC::EnumFlags{}).add({
            {   57, LayerInsertMode::limBelow },
            {   58, LayerInsertMode::limAbove },
        }),
    };
    return QtMocHelpers::metaObjectData<QCustomPlot, qt_meta_tag_ZN11QCustomPlotE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCustomPlot::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QCustomPlotE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QCustomPlotE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11QCustomPlotE_t>.metaTypes,
    nullptr
} };

void QCustomPlot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCustomPlot *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->mouseDoubleClick((*reinterpret_cast<std::add_pointer_t<QMouseEvent*>>(_a[1]))); break;
        case 1: _t->mousePress((*reinterpret_cast<std::add_pointer_t<QMouseEvent*>>(_a[1]))); break;
        case 2: _t->mouseMove((*reinterpret_cast<std::add_pointer_t<QMouseEvent*>>(_a[1]))); break;
        case 3: _t->mouseRelease((*reinterpret_cast<std::add_pointer_t<QMouseEvent*>>(_a[1]))); break;
        case 4: _t->mouseWheel((*reinterpret_cast<std::add_pointer_t<QWheelEvent*>>(_a[1]))); break;
        case 5: _t->plottableClick((*reinterpret_cast<std::add_pointer_t<QCPAbstractPlottable*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QMouseEvent*>>(_a[2]))); break;
        case 6: _t->plottableDoubleClick((*reinterpret_cast<std::add_pointer_t<QCPAbstractPlottable*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QMouseEvent*>>(_a[2]))); break;
        case 7: _t->itemClick((*reinterpret_cast<std::add_pointer_t<QCPAbstractItem*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QMouseEvent*>>(_a[2]))); break;
        case 8: _t->itemDoubleClick((*reinterpret_cast<std::add_pointer_t<QCPAbstractItem*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QMouseEvent*>>(_a[2]))); break;
        case 9: _t->axisClick((*reinterpret_cast<std::add_pointer_t<QCPAxis*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QCPAxis::SelectablePart>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QMouseEvent*>>(_a[3]))); break;
        case 10: _t->axisDoubleClick((*reinterpret_cast<std::add_pointer_t<QCPAxis*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QCPAxis::SelectablePart>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QMouseEvent*>>(_a[3]))); break;
        case 11: _t->legendClick((*reinterpret_cast<std::add_pointer_t<QCPLegend*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QCPAbstractLegendItem*>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QMouseEvent*>>(_a[3]))); break;
        case 12: _t->legendDoubleClick((*reinterpret_cast<std::add_pointer_t<QCPLegend*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QCPAbstractLegendItem*>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QMouseEvent*>>(_a[3]))); break;
        case 13: _t->titleClick((*reinterpret_cast<std::add_pointer_t<QMouseEvent*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QCPPlotTitle*>>(_a[2]))); break;
        case 14: _t->titleDoubleClick((*reinterpret_cast<std::add_pointer_t<QMouseEvent*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QCPPlotTitle*>>(_a[2]))); break;
        case 15: _t->selectionChangedByUser(); break;
        case 16: _t->beforeReplot(); break;
        case 17: _t->afterReplot(); break;
        case 18: _t->rescaleAxes((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 19: _t->rescaleAxes(); break;
        case 20: _t->deselectAll(); break;
        case 21: _t->replot((*reinterpret_cast<std::add_pointer_t<QCustomPlot::RefreshPriority>>(_a[1]))); break;
        case 22: _t->replot(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QCPAbstractPlottable* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QCPAbstractPlottable* >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QCPAbstractItem* >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QCPAbstractItem* >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QCPAxis* >(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QCPAxis::SelectablePart >(); break;
            }
            break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QCPAxis* >(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QCPAxis::SelectablePart >(); break;
            }
            break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QCPAbstractLegendItem* >(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QCPLegend* >(); break;
            }
            break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QCPAbstractLegendItem* >(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QCPLegend* >(); break;
            }
            break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QCPPlotTitle* >(); break;
            }
            break;
        case 14:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QCPPlotTitle* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (QCustomPlot::*)(QMouseEvent * )>(_a, &QCustomPlot::mouseDoubleClick, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCustomPlot::*)(QMouseEvent * )>(_a, &QCustomPlot::mousePress, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCustomPlot::*)(QMouseEvent * )>(_a, &QCustomPlot::mouseMove, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCustomPlot::*)(QMouseEvent * )>(_a, &QCustomPlot::mouseRelease, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCustomPlot::*)(QWheelEvent * )>(_a, &QCustomPlot::mouseWheel, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCustomPlot::*)(QCPAbstractPlottable * , QMouseEvent * )>(_a, &QCustomPlot::plottableClick, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCustomPlot::*)(QCPAbstractPlottable * , QMouseEvent * )>(_a, &QCustomPlot::plottableDoubleClick, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCustomPlot::*)(QCPAbstractItem * , QMouseEvent * )>(_a, &QCustomPlot::itemClick, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCustomPlot::*)(QCPAbstractItem * , QMouseEvent * )>(_a, &QCustomPlot::itemDoubleClick, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCustomPlot::*)(QCPAxis * , QCPAxis::SelectablePart , QMouseEvent * )>(_a, &QCustomPlot::axisClick, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCustomPlot::*)(QCPAxis * , QCPAxis::SelectablePart , QMouseEvent * )>(_a, &QCustomPlot::axisDoubleClick, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCustomPlot::*)(QCPLegend * , QCPAbstractLegendItem * , QMouseEvent * )>(_a, &QCustomPlot::legendClick, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCustomPlot::*)(QCPLegend * , QCPAbstractLegendItem * , QMouseEvent * )>(_a, &QCustomPlot::legendDoubleClick, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCustomPlot::*)(QMouseEvent * , QCPPlotTitle * )>(_a, &QCustomPlot::titleClick, 13))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCustomPlot::*)(QMouseEvent * , QCPPlotTitle * )>(_a, &QCustomPlot::titleDoubleClick, 14))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCustomPlot::*)()>(_a, &QCustomPlot::selectionChangedByUser, 15))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCustomPlot::*)()>(_a, &QCustomPlot::beforeReplot, 16))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCustomPlot::*)()>(_a, &QCustomPlot::afterReplot, 17))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPLayoutGrid* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QRect*>(_v) = _t->viewport(); break;
        case 1: *reinterpret_cast<QPixmap*>(_v) = _t->background(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->backgroundScaled(); break;
        case 3: *reinterpret_cast<Qt::AspectRatioMode*>(_v) = _t->backgroundScaledMode(); break;
        case 4: *reinterpret_cast<QCPLayoutGrid**>(_v) = _t->plotLayout(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->autoAddPlottableToLegend(); break;
        case 6: *reinterpret_cast<int*>(_v) = _t->selectionTolerance(); break;
        case 7: *reinterpret_cast<bool*>(_v) = _t->noAntialiasingOnDrag(); break;
        case 8: *reinterpret_cast<Qt::KeyboardModifier*>(_v) = _t->multiSelectModifier(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setViewport(*reinterpret_cast<QRect*>(_v)); break;
        case 1: _t->setBackground(*reinterpret_cast<QPixmap*>(_v)); break;
        case 2: _t->setBackgroundScaled(*reinterpret_cast<bool*>(_v)); break;
        case 3: _t->setBackgroundScaledMode(*reinterpret_cast<Qt::AspectRatioMode*>(_v)); break;
        case 5: _t->setAutoAddPlottableToLegend(*reinterpret_cast<bool*>(_v)); break;
        case 6: _t->setSelectionTolerance(*reinterpret_cast<int*>(_v)); break;
        case 7: _t->setNoAntialiasingOnDrag(*reinterpret_cast<bool*>(_v)); break;
        case 8: _t->setMultiSelectModifier(*reinterpret_cast<Qt::KeyboardModifier*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCustomPlot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCustomPlot::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QCustomPlotE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int QCustomPlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void QCustomPlot::mouseDoubleClick(QMouseEvent * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void QCustomPlot::mousePress(QMouseEvent * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void QCustomPlot::mouseMove(QMouseEvent * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void QCustomPlot::mouseRelease(QMouseEvent * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void QCustomPlot::mouseWheel(QWheelEvent * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void QCustomPlot::plottableClick(QCPAbstractPlottable * _t1, QMouseEvent * _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2);
}

// SIGNAL 6
void QCustomPlot::plottableDoubleClick(QCPAbstractPlottable * _t1, QMouseEvent * _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1, _t2);
}

// SIGNAL 7
void QCustomPlot::itemClick(QCPAbstractItem * _t1, QMouseEvent * _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1, _t2);
}

// SIGNAL 8
void QCustomPlot::itemDoubleClick(QCPAbstractItem * _t1, QMouseEvent * _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1, _t2);
}

// SIGNAL 9
void QCustomPlot::axisClick(QCPAxis * _t1, QCPAxis::SelectablePart _t2, QMouseEvent * _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1, _t2, _t3);
}

// SIGNAL 10
void QCustomPlot::axisDoubleClick(QCPAxis * _t1, QCPAxis::SelectablePart _t2, QMouseEvent * _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1, _t2, _t3);
}

// SIGNAL 11
void QCustomPlot::legendClick(QCPLegend * _t1, QCPAbstractLegendItem * _t2, QMouseEvent * _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 11, nullptr, _t1, _t2, _t3);
}

// SIGNAL 12
void QCustomPlot::legendDoubleClick(QCPLegend * _t1, QCPAbstractLegendItem * _t2, QMouseEvent * _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 12, nullptr, _t1, _t2, _t3);
}

// SIGNAL 13
void QCustomPlot::titleClick(QMouseEvent * _t1, QCPPlotTitle * _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 13, nullptr, _t1, _t2);
}

// SIGNAL 14
void QCustomPlot::titleDoubleClick(QMouseEvent * _t1, QCPPlotTitle * _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 14, nullptr, _t1, _t2);
}

// SIGNAL 15
void QCustomPlot::selectionChangedByUser()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void QCustomPlot::beforeReplot()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void QCustomPlot::afterReplot()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}
namespace {
struct qt_meta_tag_ZN16QCPColorGradientE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPColorGradient::qt_create_metaobjectdata<qt_meta_tag_ZN16QCPColorGradientE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPColorGradient",
        "ColorInterpolation",
        "ciRGB",
        "ciHSV",
        "GradientPreset",
        "gpGrayscale",
        "gpHot",
        "gpCold",
        "gpNight",
        "gpCandy",
        "gpGeography",
        "gpIon",
        "gpThermal",
        "gpPolar",
        "gpSpectrum",
        "gpJet",
        "gpHues"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'ColorInterpolation'
        QtMocHelpers::EnumData<enum ColorInterpolation>(1, 1, QMC::EnumFlags{}).add({
            {    2, ColorInterpolation::ciRGB },
            {    3, ColorInterpolation::ciHSV },
        }),
        // enum 'GradientPreset'
        QtMocHelpers::EnumData<enum GradientPreset>(4, 4, QMC::EnumFlags{}).add({
            {    5, GradientPreset::gpGrayscale },
            {    6, GradientPreset::gpHot },
            {    7, GradientPreset::gpCold },
            {    8, GradientPreset::gpNight },
            {    9, GradientPreset::gpCandy },
            {   10, GradientPreset::gpGeography },
            {   11, GradientPreset::gpIon },
            {   12, GradientPreset::gpThermal },
            {   13, GradientPreset::gpPolar },
            {   14, GradientPreset::gpSpectrum },
            {   15, GradientPreset::gpJet },
            {   16, GradientPreset::gpHues },
        }),
    };
    return QtMocHelpers::metaObjectData<QCPColorGradient, qt_meta_tag_ZN16QCPColorGradientE_t>(QMC::PropertyAccessInStaticMetaCall, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPColorGradient::staticMetaObject = { {
    nullptr,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16QCPColorGradientE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16QCPColorGradientE_t>.data,
    nullptr,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN16QCPColorGradientE_t>.metaTypes,
    nullptr
} };

namespace {
struct qt_meta_tag_ZN11QCPAxisRectE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPAxisRect::qt_create_metaobjectdata<qt_meta_tag_ZN11QCPAxisRectE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPAxisRect",
        "background",
        "QPixmap",
        "backgroundScaled",
        "backgroundScaledMode",
        "Qt::AspectRatioMode",
        "rangeDrag",
        "Qt::Orientations",
        "rangeZoom"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'background'
        QtMocHelpers::PropertyData<QPixmap>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'backgroundScaled'
        QtMocHelpers::PropertyData<bool>(3, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'backgroundScaledMode'
        QtMocHelpers::PropertyData<Qt::AspectRatioMode>(4, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'rangeDrag'
        QtMocHelpers::PropertyData<Qt::Orientations>(6, 0x80000000 | 7, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'rangeZoom'
        QtMocHelpers::PropertyData<Qt::Orientations>(8, 0x80000000 | 7, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPAxisRect, qt_meta_tag_ZN11QCPAxisRectE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPAxisRect::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPLayoutElement::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QCPAxisRectE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QCPAxisRectE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11QCPAxisRectE_t>.metaTypes,
    nullptr
} };

void QCPAxisRect::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPAxisRect *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QPixmap*>(_v) = _t->background(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->backgroundScaled(); break;
        case 2: *reinterpret_cast<Qt::AspectRatioMode*>(_v) = _t->backgroundScaledMode(); break;
        case 3: *reinterpret_cast<Qt::Orientations*>(_v) = _t->rangeDrag(); break;
        case 4: *reinterpret_cast<Qt::Orientations*>(_v) = _t->rangeZoom(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setBackground(*reinterpret_cast<QPixmap*>(_v)); break;
        case 1: _t->setBackgroundScaled(*reinterpret_cast<bool*>(_v)); break;
        case 2: _t->setBackgroundScaledMode(*reinterpret_cast<Qt::AspectRatioMode*>(_v)); break;
        case 3: _t->setRangeDrag(*reinterpret_cast<Qt::Orientations*>(_v)); break;
        case 4: _t->setRangeZoom(*reinterpret_cast<Qt::Orientations*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPAxisRect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPAxisRect::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QCPAxisRectE_t>.strings))
        return static_cast<void*>(this);
    return QCPLayoutElement::qt_metacast(_clname);
}

int QCPAxisRect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPLayoutElement::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN21QCPAbstractLegendItemE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPAbstractLegendItem::qt_create_metaobjectdata<qt_meta_tag_ZN21QCPAbstractLegendItemE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPAbstractLegendItem",
        "selectionChanged",
        "",
        "selected",
        "selectableChanged",
        "selectable",
        "setSelectable",
        "setSelected",
        "parentLegend",
        "QCPLegend*",
        "font",
        "QFont",
        "textColor",
        "QColor",
        "selectedFont",
        "selectedTextColor"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'selectionChanged'
        QtMocHelpers::SignalData<void(bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Signal 'selectableChanged'
        QtMocHelpers::SignalData<void(bool)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 5 },
        }}),
        // Slot 'setSelectable'
        QtMocHelpers::SlotData<void(bool)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 5 },
        }}),
        // Slot 'setSelected'
        QtMocHelpers::SlotData<void(bool)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'parentLegend'
        QtMocHelpers::PropertyData<QCPLegend*>(8, 0x80000000 | 9, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'font'
        QtMocHelpers::PropertyData<QFont>(10, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'textColor'
        QtMocHelpers::PropertyData<QColor>(12, 0x80000000 | 13, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedFont'
        QtMocHelpers::PropertyData<QFont>(14, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedTextColor'
        QtMocHelpers::PropertyData<QColor>(15, 0x80000000 | 13, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectable'
        QtMocHelpers::PropertyData<bool>(5, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'selected'
        QtMocHelpers::PropertyData<bool>(3, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPAbstractLegendItem, qt_meta_tag_ZN21QCPAbstractLegendItemE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPAbstractLegendItem::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPLayoutElement::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21QCPAbstractLegendItemE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21QCPAbstractLegendItemE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN21QCPAbstractLegendItemE_t>.metaTypes,
    nullptr
} };

void QCPAbstractLegendItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPAbstractLegendItem *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->selectionChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->selectableChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->setSelectable((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->setSelected((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (QCPAbstractLegendItem::*)(bool )>(_a, &QCPAbstractLegendItem::selectionChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCPAbstractLegendItem::*)(bool )>(_a, &QCPAbstractLegendItem::selectableChanged, 1))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPLegend* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QCPLegend**>(_v) = _t->parentLegend(); break;
        case 1: *reinterpret_cast<QFont*>(_v) = _t->font(); break;
        case 2: *reinterpret_cast<QColor*>(_v) = _t->textColor(); break;
        case 3: *reinterpret_cast<QFont*>(_v) = _t->selectedFont(); break;
        case 4: *reinterpret_cast<QColor*>(_v) = _t->selectedTextColor(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->selectable(); break;
        case 6: *reinterpret_cast<bool*>(_v) = _t->selected(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setFont(*reinterpret_cast<QFont*>(_v)); break;
        case 2: _t->setTextColor(*reinterpret_cast<QColor*>(_v)); break;
        case 3: _t->setSelectedFont(*reinterpret_cast<QFont*>(_v)); break;
        case 4: _t->setSelectedTextColor(*reinterpret_cast<QColor*>(_v)); break;
        case 5: _t->setSelectable(*reinterpret_cast<bool*>(_v)); break;
        case 6: _t->setSelected(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPAbstractLegendItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPAbstractLegendItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21QCPAbstractLegendItemE_t>.strings))
        return static_cast<void*>(this);
    return QCPLayoutElement::qt_metacast(_clname);
}

int QCPAbstractLegendItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPLayoutElement::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void QCPAbstractLegendItem::selectionChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void QCPAbstractLegendItem::selectableChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN22QCPPlottableLegendItemE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPPlottableLegendItem::qt_create_metaobjectdata<qt_meta_tag_ZN22QCPPlottableLegendItemE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPPlottableLegendItem"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPPlottableLegendItem, qt_meta_tag_ZN22QCPPlottableLegendItemE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPPlottableLegendItem::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPAbstractLegendItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22QCPPlottableLegendItemE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22QCPPlottableLegendItemE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN22QCPPlottableLegendItemE_t>.metaTypes,
    nullptr
} };

void QCPPlottableLegendItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPPlottableLegendItem *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *QCPPlottableLegendItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPPlottableLegendItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22QCPPlottableLegendItemE_t>.strings))
        return static_cast<void*>(this);
    return QCPAbstractLegendItem::qt_metacast(_clname);
}

int QCPPlottableLegendItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPAbstractLegendItem::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN9QCPLegendE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPLegend::qt_create_metaobjectdata<qt_meta_tag_ZN9QCPLegendE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPLegend",
        "selectionChanged",
        "",
        "QCPLegend::SelectableParts",
        "parts",
        "selectableChanged",
        "setSelectableParts",
        "SelectableParts",
        "selectableParts",
        "setSelectedParts",
        "selectedParts",
        "borderPen",
        "QPen",
        "brush",
        "QBrush",
        "font",
        "QFont",
        "textColor",
        "QColor",
        "iconSize",
        "QSize",
        "iconTextPadding",
        "iconBorderPen",
        "selectedBorderPen",
        "selectedIconBorderPen",
        "selectedBrush",
        "selectedFont",
        "selectedTextColor",
        "SelectablePart",
        "spNone",
        "spLegendBox",
        "spItems"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'selectionChanged'
        QtMocHelpers::SignalData<void(QCPLegend::SelectableParts)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'selectableChanged'
        QtMocHelpers::SignalData<void(QCPLegend::SelectableParts)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'setSelectableParts'
        QtMocHelpers::SlotData<void(const SelectableParts &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Slot 'setSelectedParts'
        QtMocHelpers::SlotData<void(const SelectableParts &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 10 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'borderPen'
        QtMocHelpers::PropertyData<QPen>(11, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'brush'
        QtMocHelpers::PropertyData<QBrush>(13, 0x80000000 | 14, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'font'
        QtMocHelpers::PropertyData<QFont>(15, 0x80000000 | 16, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'textColor'
        QtMocHelpers::PropertyData<QColor>(17, 0x80000000 | 18, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'iconSize'
        QtMocHelpers::PropertyData<QSize>(19, 0x80000000 | 20, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'iconTextPadding'
        QtMocHelpers::PropertyData<int>(21, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'iconBorderPen'
        QtMocHelpers::PropertyData<QPen>(22, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectableParts'
        QtMocHelpers::PropertyData<SelectableParts>(8, 0x80000000 | 7, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 0),
        // property 'selectedParts'
        QtMocHelpers::PropertyData<SelectableParts>(10, 0x80000000 | 7, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 1),
        // property 'selectedBorderPen'
        QtMocHelpers::PropertyData<QPen>(23, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedIconBorderPen'
        QtMocHelpers::PropertyData<QPen>(24, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedBrush'
        QtMocHelpers::PropertyData<QBrush>(25, 0x80000000 | 14, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedFont'
        QtMocHelpers::PropertyData<QFont>(26, 0x80000000 | 16, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedTextColor'
        QtMocHelpers::PropertyData<QColor>(27, 0x80000000 | 18, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'SelectablePart'
        QtMocHelpers::EnumData<enum SelectablePart>(28, 28, QMC::EnumFlags{}).add({
            {   29, SelectablePart::spNone },
            {   30, SelectablePart::spLegendBox },
            {   31, SelectablePart::spItems },
        }),
        // flag 'SelectableParts'
        QtMocHelpers::EnumData<SelectableParts>(7, 28, QMC::EnumIsFlag).add({
            {   29, SelectablePart::spNone },
            {   30, SelectablePart::spLegendBox },
            {   31, SelectablePart::spItems },
        }),
    };
    return QtMocHelpers::metaObjectData<QCPLegend, qt_meta_tag_ZN9QCPLegendE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPLegend::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPLayoutGrid::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9QCPLegendE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9QCPLegendE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9QCPLegendE_t>.metaTypes,
    nullptr
} };

void QCPLegend::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPLegend *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->selectionChanged((*reinterpret_cast<std::add_pointer_t<QCPLegend::SelectableParts>>(_a[1]))); break;
        case 1: _t->selectableChanged((*reinterpret_cast<std::add_pointer_t<QCPLegend::SelectableParts>>(_a[1]))); break;
        case 2: _t->setSelectableParts((*reinterpret_cast<std::add_pointer_t<SelectableParts>>(_a[1]))); break;
        case 3: _t->setSelectedParts((*reinterpret_cast<std::add_pointer_t<SelectableParts>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (QCPLegend::*)(QCPLegend::SelectableParts )>(_a, &QCPLegend::selectionChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCPLegend::*)(QCPLegend::SelectableParts )>(_a, &QCPLegend::selectableChanged, 1))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QPen*>(_v) = _t->borderPen(); break;
        case 1: *reinterpret_cast<QBrush*>(_v) = _t->brush(); break;
        case 2: *reinterpret_cast<QFont*>(_v) = _t->font(); break;
        case 3: *reinterpret_cast<QColor*>(_v) = _t->textColor(); break;
        case 4: *reinterpret_cast<QSize*>(_v) = _t->iconSize(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->iconTextPadding(); break;
        case 6: *reinterpret_cast<QPen*>(_v) = _t->iconBorderPen(); break;
        case 7: QtMocHelpers::assignFlags<SelectableParts>(_v, _t->selectableParts()); break;
        case 8: QtMocHelpers::assignFlags<SelectableParts>(_v, _t->selectedParts()); break;
        case 9: *reinterpret_cast<QPen*>(_v) = _t->selectedBorderPen(); break;
        case 10: *reinterpret_cast<QPen*>(_v) = _t->selectedIconBorderPen(); break;
        case 11: *reinterpret_cast<QBrush*>(_v) = _t->selectedBrush(); break;
        case 12: *reinterpret_cast<QFont*>(_v) = _t->selectedFont(); break;
        case 13: *reinterpret_cast<QColor*>(_v) = _t->selectedTextColor(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setBorderPen(*reinterpret_cast<QPen*>(_v)); break;
        case 1: _t->setBrush(*reinterpret_cast<QBrush*>(_v)); break;
        case 2: _t->setFont(*reinterpret_cast<QFont*>(_v)); break;
        case 3: _t->setTextColor(*reinterpret_cast<QColor*>(_v)); break;
        case 4: _t->setIconSize(*reinterpret_cast<QSize*>(_v)); break;
        case 5: _t->setIconTextPadding(*reinterpret_cast<int*>(_v)); break;
        case 6: _t->setIconBorderPen(*reinterpret_cast<QPen*>(_v)); break;
        case 7: _t->setSelectableParts(*reinterpret_cast<SelectableParts*>(_v)); break;
        case 8: _t->setSelectedParts(*reinterpret_cast<SelectableParts*>(_v)); break;
        case 9: _t->setSelectedBorderPen(*reinterpret_cast<QPen*>(_v)); break;
        case 10: _t->setSelectedIconBorderPen(*reinterpret_cast<QPen*>(_v)); break;
        case 11: _t->setSelectedBrush(*reinterpret_cast<QBrush*>(_v)); break;
        case 12: _t->setSelectedFont(*reinterpret_cast<QFont*>(_v)); break;
        case 13: _t->setSelectedTextColor(*reinterpret_cast<QColor*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPLegend::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPLegend::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9QCPLegendE_t>.strings))
        return static_cast<void*>(this);
    return QCPLayoutGrid::qt_metacast(_clname);
}

int QCPLegend::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPLayoutGrid::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void QCPLegend::selectionChanged(QCPLegend::SelectableParts _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void QCPLegend::selectableChanged(QCPLegend::SelectableParts _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN12QCPPlotTitleE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPPlotTitle::qt_create_metaobjectdata<qt_meta_tag_ZN12QCPPlotTitleE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPPlotTitle",
        "selectionChanged",
        "",
        "selected",
        "selectableChanged",
        "selectable",
        "setSelectable",
        "setSelected",
        "text",
        "font",
        "QFont",
        "textColor",
        "QColor",
        "selectedFont",
        "selectedTextColor"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'selectionChanged'
        QtMocHelpers::SignalData<void(bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Signal 'selectableChanged'
        QtMocHelpers::SignalData<void(bool)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 5 },
        }}),
        // Slot 'setSelectable'
        QtMocHelpers::SlotData<void(bool)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 5 },
        }}),
        // Slot 'setSelected'
        QtMocHelpers::SlotData<void(bool)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'text'
        QtMocHelpers::PropertyData<QString>(8, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'font'
        QtMocHelpers::PropertyData<QFont>(9, 0x80000000 | 10, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'textColor'
        QtMocHelpers::PropertyData<QColor>(11, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedFont'
        QtMocHelpers::PropertyData<QFont>(13, 0x80000000 | 10, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedTextColor'
        QtMocHelpers::PropertyData<QColor>(14, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectable'
        QtMocHelpers::PropertyData<bool>(5, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
        // property 'selected'
        QtMocHelpers::PropertyData<bool>(3, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPPlotTitle, qt_meta_tag_ZN12QCPPlotTitleE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPPlotTitle::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPLayoutElement::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12QCPPlotTitleE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12QCPPlotTitleE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12QCPPlotTitleE_t>.metaTypes,
    nullptr
} };

void QCPPlotTitle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPPlotTitle *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->selectionChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->selectableChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->setSelectable((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->setSelected((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (QCPPlotTitle::*)(bool )>(_a, &QCPPlotTitle::selectionChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCPPlotTitle::*)(bool )>(_a, &QCPPlotTitle::selectableChanged, 1))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->text(); break;
        case 1: *reinterpret_cast<QFont*>(_v) = _t->font(); break;
        case 2: *reinterpret_cast<QColor*>(_v) = _t->textColor(); break;
        case 3: *reinterpret_cast<QFont*>(_v) = _t->selectedFont(); break;
        case 4: *reinterpret_cast<QColor*>(_v) = _t->selectedTextColor(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->selectable(); break;
        case 6: *reinterpret_cast<bool*>(_v) = _t->selected(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setText(*reinterpret_cast<QString*>(_v)); break;
        case 1: _t->setFont(*reinterpret_cast<QFont*>(_v)); break;
        case 2: _t->setTextColor(*reinterpret_cast<QColor*>(_v)); break;
        case 3: _t->setSelectedFont(*reinterpret_cast<QFont*>(_v)); break;
        case 4: _t->setSelectedTextColor(*reinterpret_cast<QColor*>(_v)); break;
        case 5: _t->setSelectable(*reinterpret_cast<bool*>(_v)); break;
        case 6: _t->setSelected(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPPlotTitle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPPlotTitle::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12QCPPlotTitleE_t>.strings))
        return static_cast<void*>(this);
    return QCPLayoutElement::qt_metacast(_clname);
}

int QCPPlotTitle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPLayoutElement::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void QCPPlotTitle::selectionChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void QCPPlotTitle::selectableChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN28QCPColorScaleAxisRectPrivateE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPColorScaleAxisRectPrivate::qt_create_metaobjectdata<qt_meta_tag_ZN28QCPColorScaleAxisRectPrivateE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPColorScaleAxisRectPrivate",
        "axisSelectionChanged",
        "",
        "QCPAxis::SelectableParts",
        "selectedParts",
        "axisSelectableChanged",
        "selectableParts"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'axisSelectionChanged'
        QtMocHelpers::SlotData<void(QCPAxis::SelectableParts)>(1, 2, QMC::AccessProtected, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'axisSelectableChanged'
        QtMocHelpers::SlotData<void(QCPAxis::SelectableParts)>(5, 2, QMC::AccessProtected, QMetaType::Void, {{
            { 0x80000000 | 3, 6 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPColorScaleAxisRectPrivate, qt_meta_tag_ZN28QCPColorScaleAxisRectPrivateE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPColorScaleAxisRectPrivate::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPAxisRect::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN28QCPColorScaleAxisRectPrivateE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN28QCPColorScaleAxisRectPrivateE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN28QCPColorScaleAxisRectPrivateE_t>.metaTypes,
    nullptr
} };

void QCPColorScaleAxisRectPrivate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPColorScaleAxisRectPrivate *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->axisSelectionChanged((*reinterpret_cast<std::add_pointer_t<QCPAxis::SelectableParts>>(_a[1]))); break;
        case 1: _t->axisSelectableChanged((*reinterpret_cast<std::add_pointer_t<QCPAxis::SelectableParts>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *QCPColorScaleAxisRectPrivate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPColorScaleAxisRectPrivate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN28QCPColorScaleAxisRectPrivateE_t>.strings))
        return static_cast<void*>(this);
    return QCPAxisRect::qt_metacast(_clname);
}

int QCPColorScaleAxisRectPrivate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPAxisRect::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN13QCPColorScaleE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPColorScale::qt_create_metaobjectdata<qt_meta_tag_ZN13QCPColorScaleE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPColorScale",
        "dataRangeChanged",
        "",
        "QCPRange",
        "newRange",
        "dataScaleTypeChanged",
        "QCPAxis::ScaleType",
        "scaleType",
        "gradientChanged",
        "QCPColorGradient",
        "newGradient",
        "setDataRange",
        "dataRange",
        "setDataScaleType",
        "setGradient",
        "gradient",
        "type",
        "QCPAxis::AxisType",
        "dataScaleType",
        "label",
        "barWidth",
        "rangeDrag",
        "rangeZoom"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'dataRangeChanged'
        QtMocHelpers::SignalData<void(QCPRange)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'dataScaleTypeChanged'
        QtMocHelpers::SignalData<void(QCPAxis::ScaleType)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Signal 'gradientChanged'
        QtMocHelpers::SignalData<void(QCPColorGradient)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 9, 10 },
        }}),
        // Slot 'setDataRange'
        QtMocHelpers::SlotData<void(const QCPRange &)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 12 },
        }}),
        // Slot 'setDataScaleType'
        QtMocHelpers::SlotData<void(QCPAxis::ScaleType)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Slot 'setGradient'
        QtMocHelpers::SlotData<void(const QCPColorGradient &)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 9, 15 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'type'
        QtMocHelpers::PropertyData<QCPAxis::AxisType>(16, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'dataRange'
        QtMocHelpers::PropertyData<QCPRange>(12, 0x80000000 | 3, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 0),
        // property 'dataScaleType'
        QtMocHelpers::PropertyData<QCPAxis::ScaleType>(18, 0x80000000 | 6, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 1),
        // property 'gradient'
        QtMocHelpers::PropertyData<QCPColorGradient>(15, 0x80000000 | 9, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 2),
        // property 'label'
        QtMocHelpers::PropertyData<QString>(19, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'barWidth'
        QtMocHelpers::PropertyData<int>(20, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'rangeDrag'
        QtMocHelpers::PropertyData<bool>(21, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'rangeZoom'
        QtMocHelpers::PropertyData<bool>(22, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPColorScale, qt_meta_tag_ZN13QCPColorScaleE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT static const QMetaObject::SuperData qt_meta_extradata_ZN13QCPColorScaleE[] = {
    QMetaObject::SuperData::link<QCPAxis::staticMetaObject>(),
    nullptr
};

Q_CONSTINIT const QMetaObject QCPColorScale::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPLayoutElement::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13QCPColorScaleE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13QCPColorScaleE_t>.data,
    qt_static_metacall,
    qt_meta_extradata_ZN13QCPColorScaleE,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13QCPColorScaleE_t>.metaTypes,
    nullptr
} };

void QCPColorScale::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPColorScale *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->dataRangeChanged((*reinterpret_cast<std::add_pointer_t<QCPRange>>(_a[1]))); break;
        case 1: _t->dataScaleTypeChanged((*reinterpret_cast<std::add_pointer_t<QCPAxis::ScaleType>>(_a[1]))); break;
        case 2: _t->gradientChanged((*reinterpret_cast<std::add_pointer_t<QCPColorGradient>>(_a[1]))); break;
        case 3: _t->setDataRange((*reinterpret_cast<std::add_pointer_t<QCPRange>>(_a[1]))); break;
        case 4: _t->setDataScaleType((*reinterpret_cast<std::add_pointer_t<QCPAxis::ScaleType>>(_a[1]))); break;
        case 5: _t->setGradient((*reinterpret_cast<std::add_pointer_t<QCPColorGradient>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (QCPColorScale::*)(QCPRange )>(_a, &QCPColorScale::dataRangeChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCPColorScale::*)(QCPAxis::ScaleType )>(_a, &QCPColorScale::dataScaleTypeChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCPColorScale::*)(QCPColorGradient )>(_a, &QCPColorScale::gradientChanged, 2))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QCPAxis::AxisType*>(_v) = _t->type(); break;
        case 1: *reinterpret_cast<QCPRange*>(_v) = _t->dataRange(); break;
        case 2: *reinterpret_cast<QCPAxis::ScaleType*>(_v) = _t->dataScaleType(); break;
        case 3: *reinterpret_cast<QCPColorGradient*>(_v) = _t->gradient(); break;
        case 4: *reinterpret_cast<QString*>(_v) = _t->label(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->barWidth(); break;
        case 6: *reinterpret_cast<bool*>(_v) = _t->rangeDrag(); break;
        case 7: *reinterpret_cast<bool*>(_v) = _t->rangeZoom(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setType(*reinterpret_cast<QCPAxis::AxisType*>(_v)); break;
        case 1: _t->setDataRange(*reinterpret_cast<QCPRange*>(_v)); break;
        case 2: _t->setDataScaleType(*reinterpret_cast<QCPAxis::ScaleType*>(_v)); break;
        case 3: _t->setGradient(*reinterpret_cast<QCPColorGradient*>(_v)); break;
        case 4: _t->setLabel(*reinterpret_cast<QString*>(_v)); break;
        case 5: _t->setBarWidth(*reinterpret_cast<int*>(_v)); break;
        case 6: _t->setRangeDrag(*reinterpret_cast<bool*>(_v)); break;
        case 7: _t->setRangeZoom(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPColorScale::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPColorScale::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13QCPColorScaleE_t>.strings))
        return static_cast<void*>(this);
    return QCPLayoutElement::qt_metacast(_clname);
}

int QCPColorScale::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPLayoutElement::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void QCPColorScale::dataRangeChanged(QCPRange _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void QCPColorScale::dataScaleTypeChanged(QCPAxis::ScaleType _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void QCPColorScale::gradientChanged(QCPColorGradient _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN8QCPGraphE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPGraph::qt_create_metaobjectdata<qt_meta_tag_ZN8QCPGraphE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPGraph",
        "lineStyle",
        "LineStyle",
        "scatterStyle",
        "QCPScatterStyle",
        "errorType",
        "ErrorType",
        "errorPen",
        "QPen",
        "errorBarSize",
        "errorBarSkipSymbol",
        "channelFillGraph",
        "QCPGraph*",
        "adaptiveSampling",
        "lsNone",
        "lsLine",
        "lsStepLeft",
        "lsStepRight",
        "lsStepCenter",
        "lsImpulse",
        "etNone",
        "etKey",
        "etValue",
        "etBoth"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'lineStyle'
        QtMocHelpers::PropertyData<enum LineStyle>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'scatterStyle'
        QtMocHelpers::PropertyData<QCPScatterStyle>(3, 0x80000000 | 4, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'errorType'
        QtMocHelpers::PropertyData<enum ErrorType>(5, 0x80000000 | 6, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'errorPen'
        QtMocHelpers::PropertyData<QPen>(7, 0x80000000 | 8, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'errorBarSize'
        QtMocHelpers::PropertyData<double>(9, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'errorBarSkipSymbol'
        QtMocHelpers::PropertyData<bool>(10, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'channelFillGraph'
        QtMocHelpers::PropertyData<QCPGraph*>(11, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'adaptiveSampling'
        QtMocHelpers::PropertyData<bool>(13, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'LineStyle'
        QtMocHelpers::EnumData<enum LineStyle>(2, 2, QMC::EnumFlags{}).add({
            {   14, LineStyle::lsNone },
            {   15, LineStyle::lsLine },
            {   16, LineStyle::lsStepLeft },
            {   17, LineStyle::lsStepRight },
            {   18, LineStyle::lsStepCenter },
            {   19, LineStyle::lsImpulse },
        }),
        // enum 'ErrorType'
        QtMocHelpers::EnumData<enum ErrorType>(6, 6, QMC::EnumFlags{}).add({
            {   20, ErrorType::etNone },
            {   21, ErrorType::etKey },
            {   22, ErrorType::etValue },
            {   23, ErrorType::etBoth },
        }),
    };
    return QtMocHelpers::metaObjectData<QCPGraph, qt_meta_tag_ZN8QCPGraphE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPGraph::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPAbstractPlottable::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8QCPGraphE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8QCPGraphE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8QCPGraphE_t>.metaTypes,
    nullptr
} };

void QCPGraph::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPGraph *>(_o);
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPGraph* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<enum LineStyle*>(_v) = _t->lineStyle(); break;
        case 1: *reinterpret_cast<QCPScatterStyle*>(_v) = _t->scatterStyle(); break;
        case 2: *reinterpret_cast<enum ErrorType*>(_v) = _t->errorType(); break;
        case 3: *reinterpret_cast<QPen*>(_v) = _t->errorPen(); break;
        case 4: *reinterpret_cast<double*>(_v) = _t->errorBarSize(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->errorBarSkipSymbol(); break;
        case 6: *reinterpret_cast<QCPGraph**>(_v) = _t->channelFillGraph(); break;
        case 7: *reinterpret_cast<bool*>(_v) = _t->adaptiveSampling(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setLineStyle(*reinterpret_cast<enum LineStyle*>(_v)); break;
        case 1: _t->setScatterStyle(*reinterpret_cast<QCPScatterStyle*>(_v)); break;
        case 2: _t->setErrorType(*reinterpret_cast<enum ErrorType*>(_v)); break;
        case 3: _t->setErrorPen(*reinterpret_cast<QPen*>(_v)); break;
        case 4: _t->setErrorBarSize(*reinterpret_cast<double*>(_v)); break;
        case 5: _t->setErrorBarSkipSymbol(*reinterpret_cast<bool*>(_v)); break;
        case 6: _t->setChannelFillGraph(*reinterpret_cast<QCPGraph**>(_v)); break;
        case 7: _t->setAdaptiveSampling(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPGraph::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPGraph::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8QCPGraphE_t>.strings))
        return static_cast<void*>(this);
    return QCPAbstractPlottable::qt_metacast(_clname);
}

int QCPGraph::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPAbstractPlottable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN8QCPCurveE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPCurve::qt_create_metaobjectdata<qt_meta_tag_ZN8QCPCurveE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPCurve",
        "scatterStyle",
        "QCPScatterStyle",
        "lineStyle",
        "LineStyle"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'scatterStyle'
        QtMocHelpers::PropertyData<QCPScatterStyle>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'lineStyle'
        QtMocHelpers::PropertyData<enum LineStyle>(3, 0x80000000 | 4, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPCurve, qt_meta_tag_ZN8QCPCurveE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPCurve::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPAbstractPlottable::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8QCPCurveE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8QCPCurveE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8QCPCurveE_t>.metaTypes,
    nullptr
} };

void QCPCurve::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPCurve *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QCPScatterStyle*>(_v) = _t->scatterStyle(); break;
        case 1: *reinterpret_cast<enum LineStyle*>(_v) = _t->lineStyle(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setScatterStyle(*reinterpret_cast<QCPScatterStyle*>(_v)); break;
        case 1: _t->setLineStyle(*reinterpret_cast<enum LineStyle*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPCurve::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPCurve::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8QCPCurveE_t>.strings))
        return static_cast<void*>(this);
    return QCPAbstractPlottable::qt_metacast(_clname);
}

int QCPCurve::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPAbstractPlottable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN12QCPBarsGroupE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPBarsGroup::qt_create_metaobjectdata<qt_meta_tag_ZN12QCPBarsGroupE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPBarsGroup",
        "spacingType",
        "SpacingType",
        "spacing"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'spacingType'
        QtMocHelpers::PropertyData<enum SpacingType>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'spacing'
        QtMocHelpers::PropertyData<double>(3, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPBarsGroup, qt_meta_tag_ZN12QCPBarsGroupE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPBarsGroup::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12QCPBarsGroupE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12QCPBarsGroupE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12QCPBarsGroupE_t>.metaTypes,
    nullptr
} };

void QCPBarsGroup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPBarsGroup *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<enum SpacingType*>(_v) = _t->spacingType(); break;
        case 1: *reinterpret_cast<double*>(_v) = _t->spacing(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setSpacingType(*reinterpret_cast<enum SpacingType*>(_v)); break;
        case 1: _t->setSpacing(*reinterpret_cast<double*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPBarsGroup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPBarsGroup::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12QCPBarsGroupE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QCPBarsGroup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN7QCPBarsE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPBars::qt_create_metaobjectdata<qt_meta_tag_ZN7QCPBarsE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPBars",
        "width",
        "widthType",
        "WidthType",
        "barsGroup",
        "QCPBarsGroup*",
        "baseValue",
        "barBelow",
        "QCPBars*",
        "barAbove",
        "wtAbsolute",
        "wtAxisRectRatio",
        "wtPlotCoords"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'width'
        QtMocHelpers::PropertyData<double>(1, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'widthType'
        QtMocHelpers::PropertyData<enum WidthType>(2, 0x80000000 | 3, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'barsGroup'
        QtMocHelpers::PropertyData<QCPBarsGroup*>(4, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'baseValue'
        QtMocHelpers::PropertyData<double>(6, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'barBelow'
        QtMocHelpers::PropertyData<QCPBars*>(7, 0x80000000 | 8, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'barAbove'
        QtMocHelpers::PropertyData<QCPBars*>(9, 0x80000000 | 8, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'WidthType'
        QtMocHelpers::EnumData<enum WidthType>(3, 3, QMC::EnumFlags{}).add({
            {   10, WidthType::wtAbsolute },
            {   11, WidthType::wtAxisRectRatio },
            {   12, WidthType::wtPlotCoords },
        }),
    };
    return QtMocHelpers::metaObjectData<QCPBars, qt_meta_tag_ZN7QCPBarsE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPBars::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPAbstractPlottable::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7QCPBarsE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7QCPBarsE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7QCPBarsE_t>.metaTypes,
    nullptr
} };

void QCPBars::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPBars *>(_o);
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
        case 4:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPBars* >(); break;
        case 2:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPBarsGroup* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<double*>(_v) = _t->width(); break;
        case 1: *reinterpret_cast<enum WidthType*>(_v) = _t->widthType(); break;
        case 2: *reinterpret_cast<QCPBarsGroup**>(_v) = _t->barsGroup(); break;
        case 3: *reinterpret_cast<double*>(_v) = _t->baseValue(); break;
        case 4: *reinterpret_cast<QCPBars**>(_v) = _t->barBelow(); break;
        case 5: *reinterpret_cast<QCPBars**>(_v) = _t->barAbove(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setWidth(*reinterpret_cast<double*>(_v)); break;
        case 1: _t->setWidthType(*reinterpret_cast<enum WidthType*>(_v)); break;
        case 2: _t->setBarsGroup(*reinterpret_cast<QCPBarsGroup**>(_v)); break;
        case 3: _t->setBaseValue(*reinterpret_cast<double*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPBars::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPBars::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7QCPBarsE_t>.strings))
        return static_cast<void*>(this);
    return QCPAbstractPlottable::qt_metacast(_clname);
}

int QCPBars::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPAbstractPlottable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN17QCPStatisticalBoxE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPStatisticalBox::qt_create_metaobjectdata<qt_meta_tag_ZN17QCPStatisticalBoxE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPStatisticalBox",
        "key",
        "minimum",
        "lowerQuartile",
        "median",
        "upperQuartile",
        "maximum",
        "outliers",
        "QList<double>",
        "width",
        "whiskerWidth",
        "whiskerPen",
        "QPen",
        "whiskerBarPen",
        "medianPen",
        "outlierStyle",
        "QCPScatterStyle"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'key'
        QtMocHelpers::PropertyData<double>(1, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'minimum'
        QtMocHelpers::PropertyData<double>(2, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'lowerQuartile'
        QtMocHelpers::PropertyData<double>(3, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'median'
        QtMocHelpers::PropertyData<double>(4, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'upperQuartile'
        QtMocHelpers::PropertyData<double>(5, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'maximum'
        QtMocHelpers::PropertyData<double>(6, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'outliers'
        QtMocHelpers::PropertyData<QList<double>>(7, 0x80000000 | 8, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'width'
        QtMocHelpers::PropertyData<double>(9, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'whiskerWidth'
        QtMocHelpers::PropertyData<double>(10, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'whiskerPen'
        QtMocHelpers::PropertyData<QPen>(11, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'whiskerBarPen'
        QtMocHelpers::PropertyData<QPen>(13, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'medianPen'
        QtMocHelpers::PropertyData<QPen>(14, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'outlierStyle'
        QtMocHelpers::PropertyData<QCPScatterStyle>(15, 0x80000000 | 16, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPStatisticalBox, qt_meta_tag_ZN17QCPStatisticalBoxE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPStatisticalBox::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPAbstractPlottable::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17QCPStatisticalBoxE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17QCPStatisticalBoxE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17QCPStatisticalBoxE_t>.metaTypes,
    nullptr
} };

void QCPStatisticalBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPStatisticalBox *>(_o);
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<double> >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<double*>(_v) = _t->key(); break;
        case 1: *reinterpret_cast<double*>(_v) = _t->minimum(); break;
        case 2: *reinterpret_cast<double*>(_v) = _t->lowerQuartile(); break;
        case 3: *reinterpret_cast<double*>(_v) = _t->median(); break;
        case 4: *reinterpret_cast<double*>(_v) = _t->upperQuartile(); break;
        case 5: *reinterpret_cast<double*>(_v) = _t->maximum(); break;
        case 6: *reinterpret_cast<QList<double>*>(_v) = _t->outliers(); break;
        case 7: *reinterpret_cast<double*>(_v) = _t->width(); break;
        case 8: *reinterpret_cast<double*>(_v) = _t->whiskerWidth(); break;
        case 9: *reinterpret_cast<QPen*>(_v) = _t->whiskerPen(); break;
        case 10: *reinterpret_cast<QPen*>(_v) = _t->whiskerBarPen(); break;
        case 11: *reinterpret_cast<QPen*>(_v) = _t->medianPen(); break;
        case 12: *reinterpret_cast<QCPScatterStyle*>(_v) = _t->outlierStyle(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setKey(*reinterpret_cast<double*>(_v)); break;
        case 1: _t->setMinimum(*reinterpret_cast<double*>(_v)); break;
        case 2: _t->setLowerQuartile(*reinterpret_cast<double*>(_v)); break;
        case 3: _t->setMedian(*reinterpret_cast<double*>(_v)); break;
        case 4: _t->setUpperQuartile(*reinterpret_cast<double*>(_v)); break;
        case 5: _t->setMaximum(*reinterpret_cast<double*>(_v)); break;
        case 6: _t->setOutliers(*reinterpret_cast<QList<double>*>(_v)); break;
        case 7: _t->setWidth(*reinterpret_cast<double*>(_v)); break;
        case 8: _t->setWhiskerWidth(*reinterpret_cast<double*>(_v)); break;
        case 9: _t->setWhiskerPen(*reinterpret_cast<QPen*>(_v)); break;
        case 10: _t->setWhiskerBarPen(*reinterpret_cast<QPen*>(_v)); break;
        case 11: _t->setMedianPen(*reinterpret_cast<QPen*>(_v)); break;
        case 12: _t->setOutlierStyle(*reinterpret_cast<QCPScatterStyle*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPStatisticalBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPStatisticalBox::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17QCPStatisticalBoxE_t>.strings))
        return static_cast<void*>(this);
    return QCPAbstractPlottable::qt_metacast(_clname);
}

int QCPStatisticalBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPAbstractPlottable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN11QCPColorMapE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPColorMap::qt_create_metaobjectdata<qt_meta_tag_ZN11QCPColorMapE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPColorMap",
        "dataRangeChanged",
        "",
        "QCPRange",
        "newRange",
        "dataScaleTypeChanged",
        "QCPAxis::ScaleType",
        "scaleType",
        "gradientChanged",
        "QCPColorGradient",
        "newGradient",
        "setDataRange",
        "dataRange",
        "setDataScaleType",
        "setGradient",
        "gradient",
        "updateLegendIcon",
        "Qt::TransformationMode",
        "transformMode",
        "QSize",
        "thumbSize",
        "dataScaleType",
        "interpolate",
        "tightBoundary",
        "colorScale",
        "QCPColorScale*"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'dataRangeChanged'
        QtMocHelpers::SignalData<void(QCPRange)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'dataScaleTypeChanged'
        QtMocHelpers::SignalData<void(QCPAxis::ScaleType)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Signal 'gradientChanged'
        QtMocHelpers::SignalData<void(QCPColorGradient)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 9, 10 },
        }}),
        // Slot 'setDataRange'
        QtMocHelpers::SlotData<void(const QCPRange &)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 12 },
        }}),
        // Slot 'setDataScaleType'
        QtMocHelpers::SlotData<void(QCPAxis::ScaleType)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Slot 'setGradient'
        QtMocHelpers::SlotData<void(const QCPColorGradient &)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 9, 15 },
        }}),
        // Slot 'updateLegendIcon'
        QtMocHelpers::SlotData<void(Qt::TransformationMode, const QSize &)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 17, 18 }, { 0x80000000 | 19, 20 },
        }}),
        // Slot 'updateLegendIcon'
        QtMocHelpers::SlotData<void(Qt::TransformationMode)>(16, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { 0x80000000 | 17, 18 },
        }}),
        // Slot 'updateLegendIcon'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'dataRange'
        QtMocHelpers::PropertyData<QCPRange>(12, 0x80000000 | 3, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 0),
        // property 'dataScaleType'
        QtMocHelpers::PropertyData<QCPAxis::ScaleType>(21, 0x80000000 | 6, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 1),
        // property 'gradient'
        QtMocHelpers::PropertyData<QCPColorGradient>(15, 0x80000000 | 9, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 2),
        // property 'interpolate'
        QtMocHelpers::PropertyData<bool>(22, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'tightBoundary'
        QtMocHelpers::PropertyData<bool>(23, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'colorScale'
        QtMocHelpers::PropertyData<QCPColorScale*>(24, 0x80000000 | 25, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPColorMap, qt_meta_tag_ZN11QCPColorMapE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT static const QMetaObject::SuperData qt_meta_extradata_ZN11QCPColorMapE[] = {
    QMetaObject::SuperData::link<QCPAxis::staticMetaObject>(),
    nullptr
};

Q_CONSTINIT const QMetaObject QCPColorMap::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPAbstractPlottable::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QCPColorMapE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QCPColorMapE_t>.data,
    qt_static_metacall,
    qt_meta_extradata_ZN11QCPColorMapE,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11QCPColorMapE_t>.metaTypes,
    nullptr
} };

void QCPColorMap::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPColorMap *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->dataRangeChanged((*reinterpret_cast<std::add_pointer_t<QCPRange>>(_a[1]))); break;
        case 1: _t->dataScaleTypeChanged((*reinterpret_cast<std::add_pointer_t<QCPAxis::ScaleType>>(_a[1]))); break;
        case 2: _t->gradientChanged((*reinterpret_cast<std::add_pointer_t<QCPColorGradient>>(_a[1]))); break;
        case 3: _t->setDataRange((*reinterpret_cast<std::add_pointer_t<QCPRange>>(_a[1]))); break;
        case 4: _t->setDataScaleType((*reinterpret_cast<std::add_pointer_t<QCPAxis::ScaleType>>(_a[1]))); break;
        case 5: _t->setGradient((*reinterpret_cast<std::add_pointer_t<QCPColorGradient>>(_a[1]))); break;
        case 6: _t->updateLegendIcon((*reinterpret_cast<std::add_pointer_t<Qt::TransformationMode>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QSize>>(_a[2]))); break;
        case 7: _t->updateLegendIcon((*reinterpret_cast<std::add_pointer_t<Qt::TransformationMode>>(_a[1]))); break;
        case 8: _t->updateLegendIcon(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (QCPColorMap::*)(QCPRange )>(_a, &QCPColorMap::dataRangeChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCPColorMap::*)(QCPAxis::ScaleType )>(_a, &QCPColorMap::dataScaleTypeChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (QCPColorMap::*)(QCPColorGradient )>(_a, &QCPColorMap::gradientChanged, 2))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPColorScale* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QCPRange*>(_v) = _t->dataRange(); break;
        case 1: *reinterpret_cast<QCPAxis::ScaleType*>(_v) = _t->dataScaleType(); break;
        case 2: *reinterpret_cast<QCPColorGradient*>(_v) = _t->gradient(); break;
        case 3: *reinterpret_cast<bool*>(_v) = _t->interpolate(); break;
        case 4: *reinterpret_cast<bool*>(_v) = _t->tightBoundary(); break;
        case 5: *reinterpret_cast<QCPColorScale**>(_v) = _t->colorScale(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setDataRange(*reinterpret_cast<QCPRange*>(_v)); break;
        case 1: _t->setDataScaleType(*reinterpret_cast<QCPAxis::ScaleType*>(_v)); break;
        case 2: _t->setGradient(*reinterpret_cast<QCPColorGradient*>(_v)); break;
        case 3: _t->setInterpolate(*reinterpret_cast<bool*>(_v)); break;
        case 4: _t->setTightBoundary(*reinterpret_cast<bool*>(_v)); break;
        case 5: _t->setColorScale(*reinterpret_cast<QCPColorScale**>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPColorMap::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPColorMap::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QCPColorMapE_t>.strings))
        return static_cast<void*>(this);
    return QCPAbstractPlottable::qt_metacast(_clname);
}

int QCPColorMap::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPAbstractPlottable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void QCPColorMap::dataRangeChanged(QCPRange _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void QCPColorMap::dataScaleTypeChanged(QCPAxis::ScaleType _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void QCPColorMap::gradientChanged(QCPColorGradient _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN12QCPFinancialE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPFinancial::qt_create_metaobjectdata<qt_meta_tag_ZN12QCPFinancialE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPFinancial",
        "chartStyle",
        "ChartStyle",
        "width",
        "twoColored",
        "brushPositive",
        "QBrush",
        "brushNegative",
        "penPositive",
        "QPen",
        "penNegative",
        "csOhlc",
        "csCandlestick"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'chartStyle'
        QtMocHelpers::PropertyData<enum ChartStyle>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'width'
        QtMocHelpers::PropertyData<double>(3, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'twoColored'
        QtMocHelpers::PropertyData<bool>(4, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'brushPositive'
        QtMocHelpers::PropertyData<QBrush>(5, 0x80000000 | 6, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'brushNegative'
        QtMocHelpers::PropertyData<QBrush>(7, 0x80000000 | 6, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'penPositive'
        QtMocHelpers::PropertyData<QPen>(8, 0x80000000 | 9, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'penNegative'
        QtMocHelpers::PropertyData<QPen>(10, 0x80000000 | 9, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'ChartStyle'
        QtMocHelpers::EnumData<enum ChartStyle>(2, 2, QMC::EnumFlags{}).add({
            {   11, ChartStyle::csOhlc },
            {   12, ChartStyle::csCandlestick },
        }),
    };
    return QtMocHelpers::metaObjectData<QCPFinancial, qt_meta_tag_ZN12QCPFinancialE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPFinancial::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPAbstractPlottable::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12QCPFinancialE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12QCPFinancialE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12QCPFinancialE_t>.metaTypes,
    nullptr
} };

void QCPFinancial::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPFinancial *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<enum ChartStyle*>(_v) = _t->chartStyle(); break;
        case 1: *reinterpret_cast<double*>(_v) = _t->width(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->twoColored(); break;
        case 3: *reinterpret_cast<QBrush*>(_v) = _t->brushPositive(); break;
        case 4: *reinterpret_cast<QBrush*>(_v) = _t->brushNegative(); break;
        case 5: *reinterpret_cast<QPen*>(_v) = _t->penPositive(); break;
        case 6: *reinterpret_cast<QPen*>(_v) = _t->penNegative(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setChartStyle(*reinterpret_cast<enum ChartStyle*>(_v)); break;
        case 1: _t->setWidth(*reinterpret_cast<double*>(_v)); break;
        case 2: _t->setTwoColored(*reinterpret_cast<bool*>(_v)); break;
        case 3: _t->setBrushPositive(*reinterpret_cast<QBrush*>(_v)); break;
        case 4: _t->setBrushNegative(*reinterpret_cast<QBrush*>(_v)); break;
        case 5: _t->setPenPositive(*reinterpret_cast<QPen*>(_v)); break;
        case 6: _t->setPenNegative(*reinterpret_cast<QPen*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPFinancial::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPFinancial::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12QCPFinancialE_t>.strings))
        return static_cast<void*>(this);
    return QCPAbstractPlottable::qt_metacast(_clname);
}

int QCPFinancial::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPAbstractPlottable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN19QCPItemStraightLineE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPItemStraightLine::qt_create_metaobjectdata<qt_meta_tag_ZN19QCPItemStraightLineE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPItemStraightLine",
        "pen",
        "QPen",
        "selectedPen"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'pen'
        QtMocHelpers::PropertyData<QPen>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedPen'
        QtMocHelpers::PropertyData<QPen>(3, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPItemStraightLine, qt_meta_tag_ZN19QCPItemStraightLineE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPItemStraightLine::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPAbstractItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19QCPItemStraightLineE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19QCPItemStraightLineE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN19QCPItemStraightLineE_t>.metaTypes,
    nullptr
} };

void QCPItemStraightLine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPItemStraightLine *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QPen*>(_v) = _t->pen(); break;
        case 1: *reinterpret_cast<QPen*>(_v) = _t->selectedPen(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setPen(*reinterpret_cast<QPen*>(_v)); break;
        case 1: _t->setSelectedPen(*reinterpret_cast<QPen*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPItemStraightLine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPItemStraightLine::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19QCPItemStraightLineE_t>.strings))
        return static_cast<void*>(this);
    return QCPAbstractItem::qt_metacast(_clname);
}

int QCPItemStraightLine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPAbstractItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN11QCPItemLineE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPItemLine::qt_create_metaobjectdata<qt_meta_tag_ZN11QCPItemLineE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPItemLine",
        "pen",
        "QPen",
        "selectedPen",
        "head",
        "QCPLineEnding",
        "tail"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'pen'
        QtMocHelpers::PropertyData<QPen>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedPen'
        QtMocHelpers::PropertyData<QPen>(3, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'head'
        QtMocHelpers::PropertyData<QCPLineEnding>(4, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'tail'
        QtMocHelpers::PropertyData<QCPLineEnding>(6, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPItemLine, qt_meta_tag_ZN11QCPItemLineE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPItemLine::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPAbstractItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QCPItemLineE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QCPItemLineE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11QCPItemLineE_t>.metaTypes,
    nullptr
} };

void QCPItemLine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPItemLine *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QPen*>(_v) = _t->pen(); break;
        case 1: *reinterpret_cast<QPen*>(_v) = _t->selectedPen(); break;
        case 2: *reinterpret_cast<QCPLineEnding*>(_v) = _t->head(); break;
        case 3: *reinterpret_cast<QCPLineEnding*>(_v) = _t->tail(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setPen(*reinterpret_cast<QPen*>(_v)); break;
        case 1: _t->setSelectedPen(*reinterpret_cast<QPen*>(_v)); break;
        case 2: _t->setHead(*reinterpret_cast<QCPLineEnding*>(_v)); break;
        case 3: _t->setTail(*reinterpret_cast<QCPLineEnding*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPItemLine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPItemLine::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QCPItemLineE_t>.strings))
        return static_cast<void*>(this);
    return QCPAbstractItem::qt_metacast(_clname);
}

int QCPItemLine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPAbstractItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN12QCPItemCurveE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPItemCurve::qt_create_metaobjectdata<qt_meta_tag_ZN12QCPItemCurveE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPItemCurve",
        "pen",
        "QPen",
        "selectedPen",
        "head",
        "QCPLineEnding",
        "tail"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'pen'
        QtMocHelpers::PropertyData<QPen>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedPen'
        QtMocHelpers::PropertyData<QPen>(3, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'head'
        QtMocHelpers::PropertyData<QCPLineEnding>(4, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'tail'
        QtMocHelpers::PropertyData<QCPLineEnding>(6, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPItemCurve, qt_meta_tag_ZN12QCPItemCurveE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPItemCurve::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPAbstractItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12QCPItemCurveE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12QCPItemCurveE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12QCPItemCurveE_t>.metaTypes,
    nullptr
} };

void QCPItemCurve::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPItemCurve *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QPen*>(_v) = _t->pen(); break;
        case 1: *reinterpret_cast<QPen*>(_v) = _t->selectedPen(); break;
        case 2: *reinterpret_cast<QCPLineEnding*>(_v) = _t->head(); break;
        case 3: *reinterpret_cast<QCPLineEnding*>(_v) = _t->tail(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setPen(*reinterpret_cast<QPen*>(_v)); break;
        case 1: _t->setSelectedPen(*reinterpret_cast<QPen*>(_v)); break;
        case 2: _t->setHead(*reinterpret_cast<QCPLineEnding*>(_v)); break;
        case 3: _t->setTail(*reinterpret_cast<QCPLineEnding*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPItemCurve::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPItemCurve::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12QCPItemCurveE_t>.strings))
        return static_cast<void*>(this);
    return QCPAbstractItem::qt_metacast(_clname);
}

int QCPItemCurve::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPAbstractItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN11QCPItemRectE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPItemRect::qt_create_metaobjectdata<qt_meta_tag_ZN11QCPItemRectE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPItemRect",
        "pen",
        "QPen",
        "selectedPen",
        "brush",
        "QBrush",
        "selectedBrush"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'pen'
        QtMocHelpers::PropertyData<QPen>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedPen'
        QtMocHelpers::PropertyData<QPen>(3, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'brush'
        QtMocHelpers::PropertyData<QBrush>(4, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedBrush'
        QtMocHelpers::PropertyData<QBrush>(6, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPItemRect, qt_meta_tag_ZN11QCPItemRectE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPItemRect::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPAbstractItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QCPItemRectE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QCPItemRectE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11QCPItemRectE_t>.metaTypes,
    nullptr
} };

void QCPItemRect::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPItemRect *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QPen*>(_v) = _t->pen(); break;
        case 1: *reinterpret_cast<QPen*>(_v) = _t->selectedPen(); break;
        case 2: *reinterpret_cast<QBrush*>(_v) = _t->brush(); break;
        case 3: *reinterpret_cast<QBrush*>(_v) = _t->selectedBrush(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setPen(*reinterpret_cast<QPen*>(_v)); break;
        case 1: _t->setSelectedPen(*reinterpret_cast<QPen*>(_v)); break;
        case 2: _t->setBrush(*reinterpret_cast<QBrush*>(_v)); break;
        case 3: _t->setSelectedBrush(*reinterpret_cast<QBrush*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPItemRect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPItemRect::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QCPItemRectE_t>.strings))
        return static_cast<void*>(this);
    return QCPAbstractItem::qt_metacast(_clname);
}

int QCPItemRect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPAbstractItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN11QCPItemTextE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPItemText::qt_create_metaobjectdata<qt_meta_tag_ZN11QCPItemTextE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPItemText",
        "color",
        "QColor",
        "selectedColor",
        "pen",
        "QPen",
        "selectedPen",
        "brush",
        "QBrush",
        "selectedBrush",
        "font",
        "QFont",
        "selectedFont",
        "text",
        "positionAlignment",
        "Qt::Alignment",
        "textAlignment",
        "rotation",
        "padding",
        "QMargins"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'color'
        QtMocHelpers::PropertyData<QColor>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedColor'
        QtMocHelpers::PropertyData<QColor>(3, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'pen'
        QtMocHelpers::PropertyData<QPen>(4, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedPen'
        QtMocHelpers::PropertyData<QPen>(6, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'brush'
        QtMocHelpers::PropertyData<QBrush>(7, 0x80000000 | 8, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedBrush'
        QtMocHelpers::PropertyData<QBrush>(9, 0x80000000 | 8, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'font'
        QtMocHelpers::PropertyData<QFont>(10, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedFont'
        QtMocHelpers::PropertyData<QFont>(12, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'text'
        QtMocHelpers::PropertyData<QString>(13, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'positionAlignment'
        QtMocHelpers::PropertyData<Qt::Alignment>(14, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'textAlignment'
        QtMocHelpers::PropertyData<Qt::Alignment>(16, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'rotation'
        QtMocHelpers::PropertyData<double>(17, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'padding'
        QtMocHelpers::PropertyData<QMargins>(18, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPItemText, qt_meta_tag_ZN11QCPItemTextE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPItemText::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPAbstractItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QCPItemTextE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QCPItemTextE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11QCPItemTextE_t>.metaTypes,
    nullptr
} };

void QCPItemText::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPItemText *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QColor*>(_v) = _t->color(); break;
        case 1: *reinterpret_cast<QColor*>(_v) = _t->selectedColor(); break;
        case 2: *reinterpret_cast<QPen*>(_v) = _t->pen(); break;
        case 3: *reinterpret_cast<QPen*>(_v) = _t->selectedPen(); break;
        case 4: *reinterpret_cast<QBrush*>(_v) = _t->brush(); break;
        case 5: *reinterpret_cast<QBrush*>(_v) = _t->selectedBrush(); break;
        case 6: *reinterpret_cast<QFont*>(_v) = _t->font(); break;
        case 7: *reinterpret_cast<QFont*>(_v) = _t->selectedFont(); break;
        case 8: *reinterpret_cast<QString*>(_v) = _t->text(); break;
        case 9: *reinterpret_cast<Qt::Alignment*>(_v) = _t->positionAlignment(); break;
        case 10: *reinterpret_cast<Qt::Alignment*>(_v) = _t->textAlignment(); break;
        case 11: *reinterpret_cast<double*>(_v) = _t->rotation(); break;
        case 12: *reinterpret_cast<QMargins*>(_v) = _t->padding(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setColor(*reinterpret_cast<QColor*>(_v)); break;
        case 1: _t->setSelectedColor(*reinterpret_cast<QColor*>(_v)); break;
        case 2: _t->setPen(*reinterpret_cast<QPen*>(_v)); break;
        case 3: _t->setSelectedPen(*reinterpret_cast<QPen*>(_v)); break;
        case 4: _t->setBrush(*reinterpret_cast<QBrush*>(_v)); break;
        case 5: _t->setSelectedBrush(*reinterpret_cast<QBrush*>(_v)); break;
        case 6: _t->setFont(*reinterpret_cast<QFont*>(_v)); break;
        case 7: _t->setSelectedFont(*reinterpret_cast<QFont*>(_v)); break;
        case 8: _t->setText(*reinterpret_cast<QString*>(_v)); break;
        case 9: _t->setPositionAlignment(*reinterpret_cast<Qt::Alignment*>(_v)); break;
        case 10: _t->setTextAlignment(*reinterpret_cast<Qt::Alignment*>(_v)); break;
        case 11: _t->setRotation(*reinterpret_cast<double*>(_v)); break;
        case 12: _t->setPadding(*reinterpret_cast<QMargins*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPItemText::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPItemText::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QCPItemTextE_t>.strings))
        return static_cast<void*>(this);
    return QCPAbstractItem::qt_metacast(_clname);
}

int QCPItemText::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPAbstractItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN14QCPItemEllipseE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPItemEllipse::qt_create_metaobjectdata<qt_meta_tag_ZN14QCPItemEllipseE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPItemEllipse",
        "pen",
        "QPen",
        "selectedPen",
        "brush",
        "QBrush",
        "selectedBrush"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'pen'
        QtMocHelpers::PropertyData<QPen>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedPen'
        QtMocHelpers::PropertyData<QPen>(3, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'brush'
        QtMocHelpers::PropertyData<QBrush>(4, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedBrush'
        QtMocHelpers::PropertyData<QBrush>(6, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPItemEllipse, qt_meta_tag_ZN14QCPItemEllipseE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPItemEllipse::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPAbstractItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14QCPItemEllipseE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14QCPItemEllipseE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14QCPItemEllipseE_t>.metaTypes,
    nullptr
} };

void QCPItemEllipse::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPItemEllipse *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QPen*>(_v) = _t->pen(); break;
        case 1: *reinterpret_cast<QPen*>(_v) = _t->selectedPen(); break;
        case 2: *reinterpret_cast<QBrush*>(_v) = _t->brush(); break;
        case 3: *reinterpret_cast<QBrush*>(_v) = _t->selectedBrush(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setPen(*reinterpret_cast<QPen*>(_v)); break;
        case 1: _t->setSelectedPen(*reinterpret_cast<QPen*>(_v)); break;
        case 2: _t->setBrush(*reinterpret_cast<QBrush*>(_v)); break;
        case 3: _t->setSelectedBrush(*reinterpret_cast<QBrush*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPItemEllipse::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPItemEllipse::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14QCPItemEllipseE_t>.strings))
        return static_cast<void*>(this);
    return QCPAbstractItem::qt_metacast(_clname);
}

int QCPItemEllipse::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPAbstractItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN13QCPItemPixmapE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPItemPixmap::qt_create_metaobjectdata<qt_meta_tag_ZN13QCPItemPixmapE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPItemPixmap",
        "pixmap",
        "QPixmap",
        "scaled",
        "aspectRatioMode",
        "Qt::AspectRatioMode",
        "transformationMode",
        "Qt::TransformationMode",
        "pen",
        "QPen",
        "selectedPen"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'pixmap'
        QtMocHelpers::PropertyData<QPixmap>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'scaled'
        QtMocHelpers::PropertyData<bool>(3, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'aspectRatioMode'
        QtMocHelpers::PropertyData<Qt::AspectRatioMode>(4, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'transformationMode'
        QtMocHelpers::PropertyData<Qt::TransformationMode>(6, 0x80000000 | 7, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'pen'
        QtMocHelpers::PropertyData<QPen>(8, 0x80000000 | 9, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedPen'
        QtMocHelpers::PropertyData<QPen>(10, 0x80000000 | 9, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPItemPixmap, qt_meta_tag_ZN13QCPItemPixmapE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPItemPixmap::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPAbstractItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13QCPItemPixmapE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13QCPItemPixmapE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13QCPItemPixmapE_t>.metaTypes,
    nullptr
} };

void QCPItemPixmap::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPItemPixmap *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QPixmap*>(_v) = _t->pixmap(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->scaled(); break;
        case 2: *reinterpret_cast<Qt::AspectRatioMode*>(_v) = _t->aspectRatioMode(); break;
        case 3: *reinterpret_cast<Qt::TransformationMode*>(_v) = _t->transformationMode(); break;
        case 4: *reinterpret_cast<QPen*>(_v) = _t->pen(); break;
        case 5: *reinterpret_cast<QPen*>(_v) = _t->selectedPen(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setPixmap(*reinterpret_cast<QPixmap*>(_v)); break;
        case 1: _t->setScaled(*reinterpret_cast<bool*>(_v)); break;
        case 4: _t->setPen(*reinterpret_cast<QPen*>(_v)); break;
        case 5: _t->setSelectedPen(*reinterpret_cast<QPen*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPItemPixmap::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPItemPixmap::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13QCPItemPixmapE_t>.strings))
        return static_cast<void*>(this);
    return QCPAbstractItem::qt_metacast(_clname);
}

int QCPItemPixmap::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPAbstractItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN13QCPItemTracerE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPItemTracer::qt_create_metaobjectdata<qt_meta_tag_ZN13QCPItemTracerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPItemTracer",
        "pen",
        "QPen",
        "selectedPen",
        "brush",
        "QBrush",
        "selectedBrush",
        "size",
        "style",
        "TracerStyle",
        "graph",
        "QCPGraph*",
        "graphKey",
        "interpolating",
        "tsNone",
        "tsPlus",
        "tsCrosshair",
        "tsCircle",
        "tsSquare"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'pen'
        QtMocHelpers::PropertyData<QPen>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedPen'
        QtMocHelpers::PropertyData<QPen>(3, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'brush'
        QtMocHelpers::PropertyData<QBrush>(4, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedBrush'
        QtMocHelpers::PropertyData<QBrush>(6, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'size'
        QtMocHelpers::PropertyData<double>(7, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'style'
        QtMocHelpers::PropertyData<enum TracerStyle>(8, 0x80000000 | 9, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'graph'
        QtMocHelpers::PropertyData<QCPGraph*>(10, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'graphKey'
        QtMocHelpers::PropertyData<double>(12, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'interpolating'
        QtMocHelpers::PropertyData<bool>(13, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'TracerStyle'
        QtMocHelpers::EnumData<enum TracerStyle>(9, 9, QMC::EnumFlags{}).add({
            {   14, TracerStyle::tsNone },
            {   15, TracerStyle::tsPlus },
            {   16, TracerStyle::tsCrosshair },
            {   17, TracerStyle::tsCircle },
            {   18, TracerStyle::tsSquare },
        }),
    };
    return QtMocHelpers::metaObjectData<QCPItemTracer, qt_meta_tag_ZN13QCPItemTracerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPItemTracer::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPAbstractItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13QCPItemTracerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13QCPItemTracerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13QCPItemTracerE_t>.metaTypes,
    nullptr
} };

void QCPItemTracer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPItemTracer *>(_o);
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPGraph* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QPen*>(_v) = _t->pen(); break;
        case 1: *reinterpret_cast<QPen*>(_v) = _t->selectedPen(); break;
        case 2: *reinterpret_cast<QBrush*>(_v) = _t->brush(); break;
        case 3: *reinterpret_cast<QBrush*>(_v) = _t->selectedBrush(); break;
        case 4: *reinterpret_cast<double*>(_v) = _t->size(); break;
        case 5: *reinterpret_cast<enum TracerStyle*>(_v) = _t->style(); break;
        case 6: *reinterpret_cast<QCPGraph**>(_v) = _t->graph(); break;
        case 7: *reinterpret_cast<double*>(_v) = _t->graphKey(); break;
        case 8: *reinterpret_cast<bool*>(_v) = _t->interpolating(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setPen(*reinterpret_cast<QPen*>(_v)); break;
        case 1: _t->setSelectedPen(*reinterpret_cast<QPen*>(_v)); break;
        case 2: _t->setBrush(*reinterpret_cast<QBrush*>(_v)); break;
        case 3: _t->setSelectedBrush(*reinterpret_cast<QBrush*>(_v)); break;
        case 4: _t->setSize(*reinterpret_cast<double*>(_v)); break;
        case 5: _t->setStyle(*reinterpret_cast<enum TracerStyle*>(_v)); break;
        case 6: _t->setGraph(*reinterpret_cast<QCPGraph**>(_v)); break;
        case 7: _t->setGraphKey(*reinterpret_cast<double*>(_v)); break;
        case 8: _t->setInterpolating(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPItemTracer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPItemTracer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13QCPItemTracerE_t>.strings))
        return static_cast<void*>(this);
    return QCPAbstractItem::qt_metacast(_clname);
}

int QCPItemTracer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPAbstractItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN14QCPItemBracketE_t {};
} // unnamed namespace

template <> constexpr inline auto QCPItemBracket::qt_create_metaobjectdata<qt_meta_tag_ZN14QCPItemBracketE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QCPItemBracket",
        "pen",
        "QPen",
        "selectedPen",
        "length",
        "style",
        "BracketStyle"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'pen'
        QtMocHelpers::PropertyData<QPen>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'selectedPen'
        QtMocHelpers::PropertyData<QPen>(3, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'length'
        QtMocHelpers::PropertyData<double>(4, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'style'
        QtMocHelpers::PropertyData<enum BracketStyle>(5, 0x80000000 | 6, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QCPItemBracket, qt_meta_tag_ZN14QCPItemBracketE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QCPItemBracket::staticMetaObject = { {
    QMetaObject::SuperData::link<QCPAbstractItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14QCPItemBracketE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14QCPItemBracketE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14QCPItemBracketE_t>.metaTypes,
    nullptr
} };

void QCPItemBracket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QCPItemBracket *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QPen*>(_v) = _t->pen(); break;
        case 1: *reinterpret_cast<QPen*>(_v) = _t->selectedPen(); break;
        case 2: *reinterpret_cast<double*>(_v) = _t->length(); break;
        case 3: *reinterpret_cast<enum BracketStyle*>(_v) = _t->style(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setPen(*reinterpret_cast<QPen*>(_v)); break;
        case 1: _t->setSelectedPen(*reinterpret_cast<QPen*>(_v)); break;
        case 2: _t->setLength(*reinterpret_cast<double*>(_v)); break;
        case 3: _t->setStyle(*reinterpret_cast<enum BracketStyle*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QCPItemBracket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QCPItemBracket::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14QCPItemBracketE_t>.strings))
        return static_cast<void*>(this);
    return QCPAbstractItem::qt_metacast(_clname);
}

int QCPItemBracket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCPAbstractItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
