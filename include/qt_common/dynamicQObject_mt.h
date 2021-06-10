/****************************************************************************
**
** Copyright (C) 2006 Trolltech AS. All rights reserved.
**
** This file is part of the documentation of Qt. It was originally
** published as part of Qt Quarterly.
**
** This file may be used under the terms of the GNU General Public License
** version 2.0 as published by the Free Software Foundation or under the
** terms of the Qt Commercial License Agreement. The respective license
** texts for these are provided with the open source and commercial
** editions of Qt.
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef DYNAMICQOBJECT_MT_H
#define DYNAMICQOBJECT_MT_H

#include <QHash>
#include <QList>
#include <QMetaObject>
#include <QObject>

#include "sharedCharArray_ptr.h"

namespace S4{

class DynamicQObject_mt: public QObject
{
public:
    DynamicQObject_mt(QObject *parent = 0) : QObject(parent) { }

    bool emitDynamicSignal(const char *signal, sharedCharArray_ptr _t1);
    bool connectDynamicSignal(const char *signal, QObject *obj, const char *slot);

    //TODO: disconnect
private:
    QHash<QByteArray, int> signalIndices;
    QHash<QByteArray, QObject *> recverIndices; //TODO: list of QObject
    QHash<QByteArray, QString> slotIndices;     //TODO: list of slot
};

}
#endif
