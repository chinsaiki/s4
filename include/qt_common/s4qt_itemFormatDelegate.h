#ifndef DELEGATE_H
#define DELEGATE_H

#include <QStyledItemDelegate>

namespace S4
{

enum itemFormatDelegateRole {
    itemFormatDelegateRole = Qt::UserRole + 1024
};

class itemFormatDelegate: public QStyledItemDelegate
{
public:
    itemFormatDelegate(QObject * parent = 0);

    virtual void paint(QPainter * painter,
                       const QStyleOptionViewItem & option,
                       const QModelIndex & index) const;
	static const int update_scope = 3000;
	static const int update_nb = 10;
private:

};

}

#endif // DELEGATE_H
