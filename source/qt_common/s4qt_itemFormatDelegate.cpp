#include "qt_common/s4qt_itemFormatDelegate.h"
#include "qt_common/s4qt_colorpalette.h"
#include <QPainter>
#include <QDateTime>
#include <QApplication>
#include <QDebug>

namespace S4
{

static qt_colorpalette_t colorpalette;

itemFormatDelegate::itemFormatDelegate(QObject * parent) :
    QStyledItemDelegate(parent)
{}

void itemFormatDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option,
                     const QModelIndex & index) const
{


    QAbstractTableModel * db_model  = (QAbstractTableModel *)index.model();
    QVariant v = db_model->data( index, ItemChangeNoticeRole ); //TODO: 自定义填色结构

    painter->save();

    // QRect leftRect = QRect(option.rect.left(), option.rect.top(),
    //                        option.rect.width() / 2, option.rect.height());
    // QBrush leftBrush = QBrush(Qt::red);
    // painter->fillRect(leftRect, leftBrush);

    // QRect rightRect = QRect(option.rect.left() + option.rect.width() / 2,
    //                         option.rect.top(), option.rect.width() / 2,
    //                         option.rect.height());
    // QBrush rightBrush = QBrush(Qt::blue);
    // painter->fillRect(rightRect, rightBrush);

    if( !v.isNull() ){
		painter->fillRect(option.rect, v.value<QColor>());
	}

    painter->restore();

    QStyleOptionViewItem adjustedOption(option);
    adjustedOption.backgroundBrush = QBrush(Qt::NoBrush);
    QStyledItemDelegate::paint(painter, adjustedOption, index);
}


}