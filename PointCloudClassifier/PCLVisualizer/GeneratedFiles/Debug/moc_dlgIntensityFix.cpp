/****************************************************************************
** Meta object code from reading C++ file 'dlgIntensityFix.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../dlgIntensityFix.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dlgIntensityFix.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_dlgIntensityFix_t {
    QByteArrayData data[9];
    char stringdata0[91];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_dlgIntensityFix_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_dlgIntensityFix_t qt_meta_stringdata_dlgIntensityFix = {
    {
QT_MOC_LITERAL(0, 0, 15), // "dlgIntensityFix"
QT_MOC_LITERAL(1, 16, 8), // "startFix"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 8), // "QString&"
QT_MOC_LITERAL(4, 35, 6), // "float&"
QT_MOC_LITERAL(5, 42, 4), // "int&"
QT_MOC_LITERAL(6, 47, 8), // "openFile"
QT_MOC_LITERAL(7, 56, 17), // "on_btnYes_clicked"
QT_MOC_LITERAL(8, 74, 16) // "on_btnNo_clicked"

    },
    "dlgIntensityFix\0startFix\0\0QString&\0"
    "float&\0int&\0openFile\0on_btnYes_clicked\0"
    "on_btnNo_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_dlgIntensityFix[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    5,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   45,    2, 0x0a /* Public */,
       7,    0,   46,    2, 0x0a /* Public */,
       8,    0,   47,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4, 0x80000000 | 5, 0x80000000 | 4, 0x80000000 | 4,    2,    2,    2,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void dlgIntensityFix::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        dlgIntensityFix *_t = static_cast<dlgIntensityFix *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startFix((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4])),(*reinterpret_cast< float(*)>(_a[5]))); break;
        case 1: _t->openFile(); break;
        case 2: _t->on_btnYes_clicked(); break;
        case 3: _t->on_btnNo_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (dlgIntensityFix::*_t)(QString & , float & , int & , float & , float & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&dlgIntensityFix::startFix)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject dlgIntensityFix::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_dlgIntensityFix.data,
      qt_meta_data_dlgIntensityFix,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *dlgIntensityFix::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *dlgIntensityFix::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_dlgIntensityFix.stringdata0))
        return static_cast<void*>(const_cast< dlgIntensityFix*>(this));
    return QDialog::qt_metacast(_clname);
}

int dlgIntensityFix::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void dlgIntensityFix::startFix(QString & _t1, float & _t2, int & _t3, float & _t4, float & _t5)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
