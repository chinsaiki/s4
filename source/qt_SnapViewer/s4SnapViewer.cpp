#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4189) 
#endif

#include "qt_SnapViewer/s4SnapViewer.h"
#include "ui_s4SnapViewer.h"
#include "common/s4logger.h"
#include "qt_common/Utils.h"
#include "qt_Kviewer/s4Kinstrument.h"
#include "jsonTypes/nw_load_instrument_t.h"
#include "jsonTypes/tdx_snap_t_dbTbl.h"

#include <QSplitter>
#include <QScrollArea>
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaType>
#include <QStyleFactory>

#ifdef max
#undef max
#endif

using namespace std;

namespace S4 {
namespace QT {

CREATE_LOCAL_LOGGER("qt_SnapViewer")

s4SnapViewer::s4SnapViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::s4SnapViewer)
{   
    ui->setupUi(this);
	ui->centralwidget->setMouseTracking(true);
	connect(ui->actionOpen_DB, &QAction::triggered, this, &s4SnapViewer::onOpenDB);
	//connect(ui->actionCallConsole, &QAction::triggered, this, &s4SnapViewer::onCallConsole);

	//this->setMouseTracking(true);
	//_instrument_tab = new Kviewer_instrumentTab(this);

	//
 //   QSplitter *splitterMain = new QSplitter(Qt::Vertical, 0); //新建主分割窗口，水平分割
	//if (!splitterMain->hasMouseTracking()) {
	//	splitterMain->setMouseTracking(true);
	//}

 //   QSplitter *splitterV1 = new QSplitter(Qt::Vertical, splitterMain);

 //   splitterMain->setHandleWidth(1);

 //   splitterV1->addWidget(_instrument_tab);
	//if (!splitterV1->hasMouseTracking()) {
	//	splitterV1->setMouseTracking(true);
	//}

	//QList<int> list;
	//list << 100;//v1
	//list << 50;	//v2
	//list << 20;	//v3
	//list << 20;
	//list << 20;
	//splitterMain->setSizes(list);

 //   //创建滚动区域。
 //   QScrollArea *scrollArea = new QScrollArea;
 //   //把label控件放进滚动区域中.注意只能设置一个控件,一个一个控件往里面加,只会显示最后一个加入的控件.
 //   scrollArea->setWidget(splitterMain);
 //   //设置对齐格式.
 //   scrollArea->setAlignment(Qt::AlignCenter);


 //   //设置水平和垂直滚动条的策略.默认是如下策略.
 //   //scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
 //   //scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
 //   //设置是否自动调整部件的大小.默认是false.
 //   scrollArea->setWidgetResizable(true);

	//scrollArea->resize(1200, 800);

	//button_last_trade = new QPushButton(scrollArea);
	//button_last_trade->setGeometry(QRect(50, 50, 25, 25));	// x, y, w, h
	//button_last_trade->setText("<");
	//connect(button_last_trade, SIGNAL(pressed(void)), this, SLOT(onButton_last_trade(void)));
	//button_next_trade = new QPushButton(scrollArea);
	//button_next_trade->setGeometry(QRect(75, 50, 25, 25));	// x, y, w, h
	//button_next_trade->setText(">");
	//connect(button_next_trade, SIGNAL(pressed(void)), this, SLOT(onButton_next_trade(void)));

	//this->setCentralWidget(scrollArea);

	//resize(1200, 800);

}

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

		std::vector <std::string> dates = snap_db.get_table_list();

		if(!_dbTree_model)
			_dbTree_model = new QStandardItemModel(this);
		_dbTree_model->setHorizontalHeaderLabels(QStringList() << QStringLiteral("快照数据库"));

		QStandardItem* treeRoot = new QStandardItem;
		treeRoot->setText(fileInfo.fileName());
		for (auto& tbl : dates) {
			QStandardItem* child = new QStandardItem;
			child->setText(tbl.c_str());
			treeRoot->appendRow(child);
		}
		_dbTree_model->appendRow(treeRoot);

		ui->dbTree->setModel(_dbTree_model);
		ui->dbTree->setStyle(QStyleFactory::create("windows"));
		ui->dbTree->setSortingEnabled(true);


		//if (dates.size() == 0) {
		//	LCL_ERR("No snap to read out!");
		//	QMessageBox::warning(NULL, "warning", "snap db is empty!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		//	return;
		//}
		//LCL_INFO("Going to load snap of {}", dates.back());

		//S4::sqlite::tdx_snap_t_dbTbl snap_tbl;
		//std::vector<tdx_snap_t> snaps;

		//// snap_db.read_table<S4::sqlite::tdx_snap_t_dbTbl::data_t>(&snap_tbl, dates.back(), snaps);
		//snap_db.read_table_v2(&snap_tbl, dates.back(), snaps, "WHERE minuSec>=91500");
		//LCL_INFO("{} snaps has been loaded:", snaps.size());

		//if (!glb_conf::pInstance()->load(path.toStdString()))
		//{
		//	QMessageBox::warning(NULL, "warning", "file format error!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		//	return;
		//}

		//_data_if = std::make_shared<S4::QT::s4qt_data_if>();

		//connect(this, SIGNAL(signal_getInfo(const std::string&, const struct S4::stkInfoReq_t&, class S4::stkInfo_t*&)),
		//	_data_if.get(), SLOT(getInfo(const std::string&, const struct S4::stkInfoReq_t&, class S4::stkInfo_t*&)));
		//connect(this, SIGNAL(signal_loadOrdres(const std::string&, const std::string&, const std::string&, std::vector<S4::s4_history_trade_t>&)),
		//	_data_if.get(), SLOT(loadOrdres(const std::string&, const std::string&, const std::string&, std::vector<S4::s4_history_trade_t>&)));

		//onTcpSetup();

		//onLoadConf();
	}
	catch (std::exception& e) {
		QMessageBox::warning(NULL, "warning", "load snap db error: " + QString::fromStdString(e.what()) + "!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
}

void s4SnapViewer::onCallConsole()
{
	_console = new s4console(this);
	connect(_console, SIGNAL(signal_load(const std::string&, const std::string&, const std::string&)),
		this, SLOT(load(const std::string&, const std::string&, const std::string&)));
	_console->setModal(false);
	_console->show();
	_console->setGeometry(this->x() + this->width(),
		this->y(), 200, this->height());
}

void s4SnapViewer::onTcpSetup()
{
	_pTcp_json_client = std::make_shared<qt_tcp_json_client>(glb_conf::pInstance()->nw().db_viewer_port.c_str());
	_pTcp_json_client->start();

	qRegisterMetaType<std::shared_ptr<nlohmann::json>>("std::shared_ptr<nlohmann::json>");
	qRegisterMetaType<std::shared_ptr<nlohmann::json>>("std::shared_ptr<nlohmann::json>&");
	connect(_pTcp_json_client.get(), SIGNAL(signal_onRecv(const std::shared_ptr<nlohmann::json>&)),
		this, SLOT(onTcpRecvJson(const std::shared_ptr<nlohmann::json>&)));
}

void s4SnapViewer::load(const std::string& stkName, const std::string& stgName, const std::string& orderTblName)
{

	S4::stkInfoReq_t infoReq;
	S4::stkInfo_t* pInfo;
	std::vector<S4::s4_history_trade_t> history_trade_data;

	infoReq.endDate = _DOOMSDAY_;
	infoReq.nbDay_preEndDate = std::numeric_limits<int>::max();

	//infoReq.cyc_scope_list = vector<int>{ __CYC_S1__, __CYC_M1__, __CYC_L1__, __CYC_X1__ };
	// infoReq.ma_scope_list = vector<int>{ 15,60 };
	infoReq.ma_scope_list = vector<int>{5, 20, 60, 120};


	emit signal_getInfo(stkName, infoReq, pInfo);

	if (orderTblName.size() != 0) {
		emit signal_loadOrdres(stkName, stgName, orderTblName, history_trade_data);
	}

	if (!pInfo) {
		LCL_WARN("load nothing to show");
		return;
	}

	_data_panel.infoReq = infoReq;
	_data_panel.history = history_trade_data;
	_data_panel.info = *pInfo;
	showData();
}

void s4SnapViewer::onButton_next_trade(void)
{
	_instrument_tab->slot_next_trade(1);
}

void s4SnapViewer::onButton_last_trade(void)
{
	_instrument_tab->slot_next_trade(-1);
}

void s4SnapViewer::onTcpRecvJson(const std::shared_ptr<nlohmann::json>& pJ)
{
	LCL_INFO("RecvJson: {:}", pJ->dump(4));

	int command = pJ->at("command").get<int>();

	if (command == 1) {
		nw_load_instrument_t command_load;
		nw_load_instrument_t::from_json(*pJ, command_load);
		load(command_load.mktCode, command_load.stgName, command_load.tableName);
	}
	else {
		LCL_ERR("unknown command = {:}", command);
	}

}


void s4SnapViewer::showData()
{
	_instrument_tab->addInstrument(_data_panel);

	// Kinstrument* K = new Kinstrument(_instrument_tab);
	// int i = _instrument_tab->addTab(K, _data_panel.info.name().c_str());
	// _instrument_tab->setCurrentIndex(i);
	// if (!hasMouseTracking()) {
	// 	setMouseTracking(true);
	// }
	// if (!_instrument_tab->hasMouseTracking()) {
	// 	_instrument_tab->setMouseTracking(true);
	// }
	// if (!K->hasMouseTracking()) {
	// 	K->setMouseTracking(true);
	// }
}



s4SnapViewer::~s4SnapViewer()
{
}

}
}
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
