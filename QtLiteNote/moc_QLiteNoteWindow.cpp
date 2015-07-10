/****************************************************************************
** Meta object code from reading C++ file 'QLiteNoteWindow.h'
**
** Created: Thu Jul 9 17:37:49 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "QLiteNoteWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QLiteNoteWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QLiteNoteWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   17,   16,   16, 0x08,
      55,   17,   16,   16, 0x08,
      88,   17,   16,   16, 0x08,
     121,   17,   16,   16, 0x08,
     154,   17,   16,   16, 0x08,
     192,   17,   16,   16, 0x08,
     225,   17,   16,   16, 0x08,
     266,  261,   16,   16, 0x08,
     286,   16,   16,   16, 0x08,
     299,   16,   16,   16, 0x08,
     311,   16,   16,   16, 0x08,
     324,   16,   16,   16, 0x08,
     335,   16,   16,   16, 0x08,
     348,   16,   16,   16, 0x08,
     363,   16,   16,   16, 0x08,
     376,   16,   16,   16, 0x08,
     389,   16,   16,   16, 0x08,
     405,   16,   16,   16, 0x08,
     419,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QLiteNoteWindow[] = {
    "QLiteNoteWindow\0\0item\0"
    "TreeItemSelect(QTreeWidgetItem*)\0"
    "TreeItemDelete(QTreeWidgetItem*)\0"
    "TreeItemEdited(QTreeWidgetItem*)\0"
    "TreeRightClick(QTreeWidgetItem*)\0"
    "TreeItemDoubleClick(QTreeWidgetItem*)\0"
    "TreeItemExpand(QTreeWidgetItem*)\0"
    "TreeItemCollapsed(QTreeWidgetItem*)\0"
    "html\0ConvertEnd(QString)\0AddNewNote()\0"
    "AddNewDir()\0DeleteItem()\0EditNote()\0"
    "RenameItem()\0OpenExplorer()\0RefreshAll()\0"
    "NewRootDir()\0ShowTreeCheck()\0ResumeTrash()\0"
    "ShowAbout()\0"
};

void QLiteNoteWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QLiteNoteWindow *_t = static_cast<QLiteNoteWindow *>(_o);
        switch (_id) {
        case 0: _t->TreeItemSelect((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->TreeItemDelete((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 2: _t->TreeItemEdited((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->TreeRightClick((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->TreeItemDoubleClick((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 5: _t->TreeItemExpand((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 6: _t->TreeItemCollapsed((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 7: _t->ConvertEnd((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->AddNewNote(); break;
        case 9: _t->AddNewDir(); break;
        case 10: _t->DeleteItem(); break;
        case 11: _t->EditNote(); break;
        case 12: _t->RenameItem(); break;
        case 13: _t->OpenExplorer(); break;
        case 14: _t->RefreshAll(); break;
        case 15: _t->NewRootDir(); break;
        case 16: _t->ShowTreeCheck(); break;
        case 17: _t->ResumeTrash(); break;
        case 18: _t->ShowAbout(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QLiteNoteWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QLiteNoteWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QLiteNoteWindow,
      qt_meta_data_QLiteNoteWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QLiteNoteWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QLiteNoteWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QLiteNoteWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QLiteNoteWindow))
        return static_cast<void*>(const_cast< QLiteNoteWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QLiteNoteWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
