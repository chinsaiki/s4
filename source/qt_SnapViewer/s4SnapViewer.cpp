#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4189) 
#endif

#include "qt_SnapViewer/s4SnapViewer.h"
#include "ui_s4SnapViewer.h"
#include "common/s4logger.h"
#include "qt_common/Utils.h"
#include "jsonTypes/nw_load_instrument_t.h"
#include "db_sqlite/db.h"
#include "jsonTypes/tdx_snap_t_dbTbl.h"

#include <QSplitter>
#include <QScrollArea>
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaType>
#include <QStyleFactory>
#include <QSortFilterProxyModel>
#include <QTableWidget>

#ifdef max
#undef max
#endif

using namespace std;

namespace S4 {
namespace QT {

CREATE_LOCAL_LOGGER("qt_SnapViewer")

#define DBTREE_ROOT_NAME QStringLiteral("快照数据库")

s4SnapViewer::s4SnapViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::s4SnapViewer)
{   
	ui->setupUi(this);

	ui->dbTree->setStyle(QStyleFactory::create("windows"));
	ui->dbTree->setSortingEnabled(true);

	ui->centralwidget->setMouseTracking(true);

	connect(ui->actionOpen_DB, &QAction::triggered, this, &s4SnapViewer::onOpenDB);
	connect(ui->dbTree, &QTreeView::doubleClicked, this, &s4SnapViewer::dbTree_doubleClicked);

	connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &s4SnapViewer::snapTab_closeRequest);

}

//打开一个sqlite数据库，并把table添加到dbTree中
void s4SnapViewer::onOpenDB()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Open snap database"), "../db", tr("sqlite db files (*.db)"));

	if (!Utils::fileCanBeOpened(path)) {
		QMessageBox::warning(NULL, "warning", "file is not readable!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}

	QFileInfo fileInfo;
	fileInfo = QFileInfo(path);
	ui->statusbar->showMessage(path);

	try {

		sqlite::DB_t snap_db(path.toStdString());

		std::set<std::string> dates = snap_db.get_table_list();

		//读取一页，检查数据结构 【这里不做】
		// if (dates.size() == 0) {
		// 	LCL_ERR("No snap to read out!");
		// 	QMessageBox::warning(NULL, "warning", "snap db is empty!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		// 	return;
		// }

		// S4::sqlite::tdx_snap_t_dbTbl snap_tbl;
		// std::vector<tdx_snap_t> snaps;

		// snap_db.read_table_v2(&snap_tbl, dates.back(), snaps);
		// LCL_INFO("{} snaps has been loaded:", snaps.size());


		if (!_dbTree_model) {
			_dbTree_model = new QStandardItemModel(this);
			_dbTree_model->setHorizontalHeaderLabels(QStringList() << DBTREE_ROOT_NAME);
			ui->dbTree->setModel(_dbTree_model);
		}

		if (_dbTree_model->findItems(fileInfo.baseName()).count()) {
			_dbTree_model->removeRow(_dbTree_model->findItems(fileInfo.baseName()).first()->row());
		}

		QStandardItem* treeRoot = new QStandardItem;
		treeRoot->setText(fileInfo.baseName());
		for (auto& tbl : dates) {
			QStandardItem* child = new QStandardItem;
			child->setText(tbl.c_str());
			treeRoot->appendRow(child);
		}
		_dbTree_model->appendRow(treeRoot);

		_db_list[fileInfo.baseName().toStdString()] = path.toStdString();
	}
	catch (std::exception& e) {
		QMessageBox::warning(NULL, "warning", "load snap db error: " + QString::fromStdString(e.what()) + "!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
}

void s4SnapViewer::dbTree_doubleClicked(const QModelIndex& index) {
	if (!index.parent().isValid()) return;
	//if (!index.parent().parent().isValid()) return;

	QString tableName = index.data().toString();
	QString dbName = index.parent().data().toString();

	QString str;
	str += QStringLiteral("当前选中：%1/%2\n").arg(dbName).arg(tableName);
	ui->statusbar->showMessage(str);

	openSnapTab(dbName.toStdString(), tableName.toStdString());
}

void s4SnapViewer::openSnapTab(const std::string& db_name, const std::string& table_name)
{
	if (_db_list.count(db_name) == 0) {
		return;
	}

	std::string path = _db_list.at(db_name);

	try {
		sqlite::DB_t snap_db(path);
		std::set<std::string> dates = snap_db.get_table_list();
		if (dates.count(table_name) == 0) {
			return;
		}

		S4::sqlite::tdx_snap_t_dbTbl snap_tbl;
		std::vector<tdx_snap_t> snaps;

		snap_db.read_table_v2(&snap_tbl, table_name, snaps);

		std::string snap_tab_name_s = db_name + "-" + table_name;
		QString snap_tab_name(snap_tab_name_s.c_str());

		//drop old data
		if (tabAlreadyExists(snap_tab_name)) {
			QWidget* tab = _snap_cargo.at(snap_tab_name).levels_view;
			int i = ui->tabWidget->indexOf(tab);
			ui->tabWidget->removeTab(i);
			_snap_cargo.erase(snap_tab_name);
		}

		//create new data
		//QAbstractTableModel
		snapLevels* levels = new snapLevels(5, this);
		QTableView* levels_tv = new QTableView(this);
		//QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
		//proxyModel->setSourceModel(levels);
		levels_tv->setModel(levels);
		levels_tv->setSortingEnabled(true);
		levels_tv->setSelectionBehavior(QAbstractItemView::SelectRows);
		connect(levels_tv, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(snap_level_doubleClicked(const QModelIndex&)));

		int i = ui->tabWidget->addTab(tv, snap_tab_name);
		ui->tabWidget->setCurrentIndex(i);

		//存储
		_snap_cargo[snap_tab_name] = { snaps, 0, tv };

		if (snaps.size()) {
			levels->refresh(snaps.front());
			_snap_cargo[snap_tab_name].curse = 1;
		}
	}
	catch (std::exception& e) {
		QMessageBox::warning(NULL, "warning", "read snap table error: " + QString::fromStdString(e.what()) + "!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
}


void s4SnapViewer::snap_level_doubleClicked(const QModelIndex& index)
{
	const QString snap_tab_name = ui->tabWidget->tabText(ui->tabWidget->currentIndex());


	QTableView* levels_tv = (QTableWidget*)ui->tabWidget->currentWidget();
	int row = levels_tv->currentIndex().row();
	QAbstractItemModel* model = levels_tv->model();

	QModelIndex indexCode = model->index(row, 0);
	const QString side = model->data(indexCode).toString();

	ui->statusbar->showMessage(side);

	if (_snap_cargo.count(snap_tab_name)) {
		if (_snap_cargo[snap_tab_name].curse < _snap_cargo[snap_tab_name].snaps.size()) {
			((snapLevels*)model)->refresh(_snap_cargo[snap_tab_name].snaps[_snap_cargo[snap_tab_name].curse++]);
		}
	}
}

void s4SnapViewer::snapTab_closeRequest(int index)
{
	const QString tabName = ui->tabWidget->tabText(index);
	ui->tabWidget->removeTab(index);

	auto it = _snap_cargo.find(tabName);
	if (it != _snap_cargo.end()) {
		_snap_cargo.erase(tabName);
	}
}


bool s4SnapViewer::tabAlreadyExists(const QString& tabName) const
{

	auto it = _snap_cargo.find(tabName);

	return it != _snap_cargo.end();
}

s4SnapViewer::~s4SnapViewer()
{
}

}
}
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
