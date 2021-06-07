#include "qt_common/s4qt_unicTreeView.h"
#include <QDebug>
#include <QMouseEvent>

namespace S4
{
namespace QT
{
    
unicTreeView::unicTreeView(QWidget* parent):
    QTreeView(parent)
{

	connect(this, &QTreeView::doubleClicked, this, &unicTreeView::onTreeDoubleClick);
	//connect(this, &QTreeView::mouseDoubleClickEvent, this, &unicTreeView::onMouseDoubleClick);
}

void unicTreeView::mouseDoubleClickEvent(QMouseEvent* event)
{
	QTreeView::mouseDoubleClickEvent(event);

	QModelIndex index = this->indexAt(event->pos());
	//QAbstractItemModel *model = this->model();
	//QMap<int, QVariant> ItemData = model->itemData(index);

	if (index.row() < 0) {
		qDebug() << "onMouseDoubleClick";
		emit signal_mouseDoubleClick();
	}
}

void unicTreeView::onTreeDoubleClick(const QModelIndex& index)
{
    qDebug() << "onTreeDoubleClick";
    emit signal_treeDoubleClick(index);
}

    
    
} // namespace QT
} // namespace S4
