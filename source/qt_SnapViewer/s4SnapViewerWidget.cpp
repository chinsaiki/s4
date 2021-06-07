#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4189) 
#endif

#include "qt_SnapViewer/s4SnapViewerWidget.h"

#include <QSplitter>
#include <QMessageBox>
#include <QTableWidget>
#include <QStyleFactory>
#include <QGridLayout>
//
//#ifdef max
//#undef max
//#endif

using namespace std;

namespace S4 {
namespace QT {

//CREATE_LOCAL_LOGGER("qt_SnapViewer")

#define DBTREE_ROOT_NAME QStringLiteral("实时数据源")

s4SnapViewerWidget::s4SnapViewerWidget(QWidget *parent) :
	QWidget(parent)
{   
	setMouseTracking(true);
}

void s4SnapViewerWidget::newTree(const QString& root_name, const std::vector<QString>& leaf_names)
{
	if (!_tree_model) {
		_tree_model = new QStandardItemModel(this);
		_tree_model->setHorizontalHeaderLabels(QStringList() << DBTREE_ROOT_NAME);
		_treeView->setModel(_tree_model);
	}

	delTree(root_name);

	QStandardItem* treeRoot = new QStandardItem;
	treeRoot->setText(root_name);
	for (auto& tbl : leaf_names) {
		QStandardItem* child = new QStandardItem;
		child->setText(tbl);
		treeRoot->appendRow(child);
	}
	_tree_model->appendRow(treeRoot);
}

void s4SnapViewerWidget::delTree(const QString& root_name)
{

	while (_tree_model->findItems(root_name).count()) {
		_tree_model->removeRow(_tree_model->findItems(root_name).first()->row());
	}
}


void s4SnapViewerWidget::appendTreeItems(const QString& root_name, const std::vector<QString>& leaf_names)
{
	if (!_tree_model) {
		_tree_model = new QStandardItemModel(this);
		_tree_model->setHorizontalHeaderLabels(QStringList() << DBTREE_ROOT_NAME);
		_treeView->setModel(_tree_model);
	}

	if (_tree_model->findItems(root_name).count()==0){
		newTree(root_name, leaf_names);
	}
	else {
		QStandardItem* root = _tree_model->findItems(root_name).first();
		for (auto& leaf: leaf_names){
			QStandardItem* child = new QStandardItem;
			child->setText(leaf);
			root->appendRow(child);
		}
	}

}

void s4SnapViewerWidget::removeTreeItems(const QString& root_name, const std::vector<QString>& leaf_names)
{
	if (!_tree_model || _tree_model->findItems(root_name).count() == 0) {
		return;
	}

	QStandardItem* root = _tree_model->findItems(root_name).first();
	for (auto& leaf : leaf_names) {
		for (int i = 0; i < root->rowCount(); ++i)
		{
			if (root->child(i)->text() == leaf)
			{
				root->removeRow(i);
				break;
			}
		}
	}
}
void s4SnapViewerWidget::openSnapTab(const QString& snap_tab_name, snapInstrument* pInstrument)
{

	try {
		//drop old data
		if (tabAlreadyExists(snap_tab_name)) {
			QWidget* tab = _instrument_view_cargo.at(snap_tab_name);
			int i = _tabWidget->indexOf(tab);
			_tabWidget->removeTab(i);
			_instrument_view_cargo.erase(snap_tab_name);
		}

		// snapInstrument* pInstrument = new snapInstrument(5, this);

		int i = _tabWidget->addTab(pInstrument, snap_tab_name);
		_tabWidget->setCurrentIndex(i);

		//存储
		_instrument_view_cargo[snap_tab_name] = pInstrument;

	}
	catch (std::exception& e) {
		QMessageBox::warning(NULL, "warning", "openSnapTab error: " + QString::fromStdString(e.what()) + "!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
}



void s4SnapViewerWidget::closeSnapTab(int index)
{
	const QString tabName = _tabWidget->tabText(index);
	_tabWidget->removeTab(index);

	auto it = _instrument_view_cargo.find(tabName);
	if (it != _instrument_view_cargo.end()) {
		_instrument_view_cargo.erase(tabName);
	}
}


bool s4SnapViewerWidget::tabAlreadyExists(const QString& tabName) const
{

	auto it = _instrument_view_cargo.find(tabName);

	return it != _instrument_view_cargo.end();
}

s4SnapViewerWidget::~s4SnapViewerWidget()
{
}

}
}
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
