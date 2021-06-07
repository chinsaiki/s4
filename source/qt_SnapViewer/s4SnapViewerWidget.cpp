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
	_treeView = new QTreeView(this);
	_treeView->setStyle(QStyleFactory::create("windows"));
	_treeView->setSortingEnabled(true);
	_treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_treeView->setMaximumWidth(300);

	_tabWidget = new QTabWidget(this);
    //网格分割
	QGridLayout *pLayout = new QGridLayout(this);
    pLayout->addWidget(_treeView, 0, 0);
    pLayout->addWidget(_tabWidget, 0, 2);
	pLayout->setColumnStretch(0, 1);			//0列的拉伸系数
	pLayout->setColumnStretch(2, 3);			//6列的拉伸系数 (0=不拉伸)

	setLayout(pLayout);

	setMouseTracking(true);
	connect(_tabWidget, &QTabWidget::tabCloseRequested, this, &s4SnapViewerWidget::closeSnapTab);
}

void s4SnapViewerWidget::addTreeItem(const QString& root_name, const std::vector<QString>& leaf_names)
{
	if (!_tree_model) {
		_tree_model = new QStandardItemModel(this);
		_tree_model->setHorizontalHeaderLabels(QStringList() << DBTREE_ROOT_NAME);
		_treeView->setModel(_tree_model);
	}

	if (_tree_model->findItems(root_name).count()) {
		_tree_model->removeRow(_tree_model->findItems(root_name).first()->row());
	}

	QStandardItem* treeRoot = new QStandardItem;
	treeRoot->setText(root_name);
	for (auto& tbl : leaf_names) {
		QStandardItem* child = new QStandardItem;
		child->setText(tbl);
		treeRoot->appendRow(child);
	}
	_tree_model->appendRow(treeRoot);
}

void s4SnapViewerWidget::delTreeItem(const QString& root_name)
{

	while (_tree_model->findItems(root_name).count()) {
		_tree_model->removeRow(_tree_model->findItems(root_name).first()->row());
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
