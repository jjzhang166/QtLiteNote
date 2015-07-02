/****************************************************************************
** Meta object code from reading C++ file 'QTreeWidgetEx.h'
**
** Created: Thu Jul 2 16:27:49 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "QTreeWidgetEx.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QTreeWidgetEx.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QTreeWidgetEx[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   15,   14,   14, 0x05,
      49,   15,   14,   14, 0x05,
      78,   15,   14,   14, 0x05,
     107,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     137,   15,   14,   14, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_QTreeWidgetEx[] = {
    "QTreeWidgetEx\0\0item\0itemSelect(QTreeWidgetItem*)\0"
    "itemDelete(QTreeWidgetItem*)\0"
    "rightClick(QTreeWidgetItem*)\0"
    "doubleClick(QTreeWidgetItem*)\0"
    "UpdateEdit(QTreeWidgetItem*)\0"
};

void QTreeWidgetEx::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QTreeWidgetEx *_t = static_cast<QTreeWidgetEx *>(_o);
        switch (_id) {
        case 0: _t->itemSelect((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->itemDelete((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 2: _t->rightClick((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->doubleClick((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->UpdateEdit((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QTreeWidgetEx::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QTreeWidgetEx::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_QTreeWidgetEx,
      qt_meta_data_QTreeWidgetEx, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QTreeWidgetEx::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QTreeWidgetEx::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QTreeWidgetEx::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QTreeWidgetEx))
        return static_cast<void*>(const_cast< QTreeWidgetEx*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int QTreeWidgetEx::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void QTreeWidgetEx::itemSelect(QTreeWidgetItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QTreeWidgetEx::itemDelete(QTreeWidgetItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QTreeWidgetEx::rightClick(QTreeWidgetItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QTreeWidgetEx::doubleClick(QTreeWidgetItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
