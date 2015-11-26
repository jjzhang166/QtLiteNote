/****************************************************************************
** Meta object code from reading C++ file 'QTreeWidgetEx.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "QTreeWidgetEx.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QTreeWidgetEx.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QTreeWidgetEx_t {
    QByteArrayData data[11];
    char stringdata0[117];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QTreeWidgetEx_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QTreeWidgetEx_t qt_meta_stringdata_QTreeWidgetEx = {
    {
QT_MOC_LITERAL(0, 0, 13), // "QTreeWidgetEx"
QT_MOC_LITERAL(1, 14, 10), // "itemSelect"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(4, 43, 4), // "item"
QT_MOC_LITERAL(5, 48, 10), // "itemDelete"
QT_MOC_LITERAL(6, 59, 10), // "itemRename"
QT_MOC_LITERAL(7, 70, 10), // "rightClick"
QT_MOC_LITERAL(8, 81, 11), // "doubleClick"
QT_MOC_LITERAL(9, 93, 12), // "spaceKeyItem"
QT_MOC_LITERAL(10, 106, 10) // "UpdateEdit"

    },
    "QTreeWidgetEx\0itemSelect\0\0QTreeWidgetItem*\0"
    "item\0itemDelete\0itemRename\0rightClick\0"
    "doubleClick\0spaceKeyItem\0UpdateEdit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QTreeWidgetEx[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       5,    1,   52,    2, 0x06 /* Public */,
       6,    1,   55,    2, 0x06 /* Public */,
       7,    1,   58,    2, 0x06 /* Public */,
       8,    1,   61,    2, 0x06 /* Public */,
       9,    1,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    1,   67,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void QTreeWidgetEx::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QTreeWidgetEx *_t = static_cast<QTreeWidgetEx *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->itemSelect((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->itemDelete((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 2: _t->itemRename((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->rightClick((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->doubleClick((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 5: _t->spaceKeyItem((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 6: _t->UpdateEdit((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QTreeWidgetEx::*_t)(QTreeWidgetItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QTreeWidgetEx::itemSelect)) {
                *result = 0;
            }
        }
        {
            typedef void (QTreeWidgetEx::*_t)(QTreeWidgetItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QTreeWidgetEx::itemDelete)) {
                *result = 1;
            }
        }
        {
            typedef void (QTreeWidgetEx::*_t)(QTreeWidgetItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QTreeWidgetEx::itemRename)) {
                *result = 2;
            }
        }
        {
            typedef void (QTreeWidgetEx::*_t)(QTreeWidgetItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QTreeWidgetEx::rightClick)) {
                *result = 3;
            }
        }
        {
            typedef void (QTreeWidgetEx::*_t)(QTreeWidgetItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QTreeWidgetEx::doubleClick)) {
                *result = 4;
            }
        }
        {
            typedef void (QTreeWidgetEx::*_t)(QTreeWidgetItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QTreeWidgetEx::spaceKeyItem)) {
                *result = 5;
            }
        }
    }
}

const QMetaObject QTreeWidgetEx::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_QTreeWidgetEx.data,
      qt_meta_data_QTreeWidgetEx,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QTreeWidgetEx::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QTreeWidgetEx::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QTreeWidgetEx.stringdata0))
        return static_cast<void*>(const_cast< QTreeWidgetEx*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int QTreeWidgetEx::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void QTreeWidgetEx::itemSelect(QTreeWidgetItem * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QTreeWidgetEx::itemDelete(QTreeWidgetItem * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QTreeWidgetEx::itemRename(QTreeWidgetItem * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QTreeWidgetEx::rightClick(QTreeWidgetItem * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QTreeWidgetEx::doubleClick(QTreeWidgetItem * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QTreeWidgetEx::spaceKeyItem(QTreeWidgetItem * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
