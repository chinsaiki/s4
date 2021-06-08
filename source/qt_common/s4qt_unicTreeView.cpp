#include "qt_common/s4qt_unicTreeView.h"
#include <QDebug>
#include <QMouseEvent>
#include <QStandardItem>

namespace S4
{
namespace QT
{


unicTreeView::unicTreeView(QWidget* parent):
    QTreeView(parent)
{
	connect(this, &QTreeView::doubleClicked, this, &unicTreeView::onTreeDoubleClick);
	connect(this, &unicTreeView::signal_mouseDoubleClick, this, &unicTreeView::onBlankDoubleClick);
	//connect(this, &QTreeView::mouseDoubleClickEvent, this, &unicTreeView::onMouseDoubleClick);
}
void unicTreeView::onSetCurrentRoot(QStandardItem* root)
{
	_current_root = root;
	connect((QStandardItemModel*)model(), &QStandardItemModel::itemChanged, this, &unicTreeView::onItemChanged);
}
void unicTreeView::onSetTextFormater(unicTreeView::textFormater_t* textFormater)
{
	_textFormater = textFormater;
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

void unicTreeView::onBlankDoubleClick() {
	//if (!index.parent().isValid())
	//	return;
	//if (!index.parent().parent().isValid()) return;

    QStandardItem * item = new QStandardItem;
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
	_current_root->appendRow(item);
	this->expandAll();
	this->edit(item->index());
}

void unicTreeView::onItemChanged(QStandardItem *item)
{
	if (_textFormater){
		QString fmt_text;
		if (!(*_textFormater)(item->text(),fmt_text)){
			this->edit(item->index());
			qDebug() << "error format, please retry!";
			return;
		}
		item->setText(fmt_text);
	}
	item->setFlags(item->flags() & (~Qt::ItemIsEditable));
	emit signal_newItem(item->text());
	return;
}
    
} // namespace QT
} // namespace S4
