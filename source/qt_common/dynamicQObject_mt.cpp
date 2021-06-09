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

#include "qt_common/dynamicQObject_mt.h"

namespace S4{

Q_DECLARE_METATYPE(sharedCharArray_ptr);

bool DynamicQObject_mt::connectDynamicSignal(char *signal, QObject *obj, char *slot)
{
    QString _signal(signal);
    QString _slot(slot);
    if (!_signal.endsWith("(sharedCharArray_ptr)"))
        return false;

    QByteArray theSignal = QMetaObject::normalizedSignature(signal);
    QByteArray theSlot = QMetaObject::normalizedSignature(slot);
    if (!QMetaObject::checkConnectArgs(theSignal, theSlot))
        return false;

    int slotId = obj->metaObject()->indexOfSlot(theSlot);
    if (slotId < 0) 
        return false;    

    int signalId = signalIndices.value(theSignal, -1);
    if (signalId < 0) {
        signalId = signalIndices.size();
        signalIndices[theSignal] = signalId;
        recverIndices[theSignal] = obj;
        _slot.remove("(sharedCharArray_ptr)");
        slotIndices[theSignal] = _slot;
        return true;
    }

    return false;
}



bool DynamicQObject_mt::emitDynamicSignal(char *signal, sharedCharArray_ptr _t1)
{
	qRegisterMetaType<sharedCharArray_ptr>();

    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };

    QByteArray theSignal = QMetaObject::normalizedSignature(signal);
    int signalId = signalIndices.value(theSignal, -1);
    if (signalId >= 0) {
          return QMetaObject::invokeMethod(
                      recverIndices.value(theSignal),
                      slotIndices.value(theSignal).toStdString().data(),
                      Qt::QueuedConnection,
                      Q_ARG(sharedCharArray_ptr, _t1)
          );
    } else {
        return false;
    }
}


}