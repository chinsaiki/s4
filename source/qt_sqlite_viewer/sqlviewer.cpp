#include "qt_sqlite_viewer/sqlviewer.h"
#include "ui_sqlviewer.h"
#include "common/s4conf.h"
#include "qt_common/Utils.h"
#include <QMessageBox>
#include <QStyleFactory>
#include <QSplitter>
#include <QSortFilterProxyModel>
#include <QTableWidget>

#include "trade/s4_history_trade.h"
#include "jsonTypes/nw_load_instrument_t.h"

namespace S4{

#define HISTORY_ORDER_TREE_ROOT "history-orders"

using asio::ip::tcp;

SqlViewer::SqlViewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SqlViewer)
{
    ui->setupUi(this);

    connect(ui->actionOpen,&QAction::triggered,this,&SqlViewer::onOpen);
    connect(ui->dbTree,&QTreeView::doubleClicked,this,&SqlViewer::openTableTab);
//    Subclassing is needed for mouse events
//    connect(ui->tabWidget,&QTabWidget::mousePressEvent,this,&tabClickHandler);
    connect(ui->tabWidget,&QTabWidget::tabCloseRequested,this,&SqlViewer::tabCloseRequestHandler);

    ui->statusbar->showMessage("Use \"Ctrl + O\" to open S4 configure json file.");


}

SqlViewer::~SqlViewer()
{
    if (_pTcp_json_server)
        _pTcp_json_server->stop();

    delete dbHandler;
    delete ui;
}

void SqlViewer::onTcpSetup()
{
    if (!_tcp_json_server_running) {
        std::thread t([this]() {
            asio::io_context io_context;
            tcp::endpoint endpoint(tcp::v4(), std::atoi(glb_conf::pInstance()->nw().db_viewer_port.c_str()));

            _pTcp_json_server = std::make_shared<NW::tcp_json_server>(io_context, endpoint);

            _tcp_json_server_running = true;
            io_context.run();
            _tcp_json_server_running = false;
        });

        t.detach();
    }
}

void SqlViewer::onOpen()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Open S4 configure json"), "../worksapce", tr("Json files (*.json)"));

    if(!Utils::fileCanBeOpened(path)){
        QMessageBox::warning(NULL, "warning", "file is not readable!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }

    if (!glb_conf::pInstance()->load(path.toStdString()))
    {
        QMessageBox::warning(NULL, "warning", "file format error!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    ui->statusbar->showMessage(path);

    onOpenDBs();

    onTcpSetup();
}

void SqlViewer::onLoadConf(void)
{
    //Not used
}

void SqlViewer::onOpenDBs(void)
{
    _dbTree_model = new QStandardItemModel(this);
    _dbTree_model->setHorizontalHeaderLabels(QStringList() << QStringLiteral("数据库"));
    onOpenDB_orders();
    ui->dbTree->setModel(_dbTree_model);
    ui->dbTree->setStyle(QStyleFactory::create("windows"));
    ui->dbTree->setSortingEnabled(true);
}

void SqlViewer::onOpenDB_orders(void)
{

    glb_conf_ctx_t::db_t db = glb_conf::pInstance()->db();
    std::filesystem::path db_root_path = db.root;
    std::filesystem::path db_history_path = db_root_path / db.history_trade;
    _pHistory_db = std::make_shared<sqlite::DB_t>(db_history_path.string());

    std::vector < std::string> history_tables = _pHistory_db->get_table_list();

    QStandardItem* orderRoot = new QStandardItem;
    orderRoot->setText(QStringLiteral(HISTORY_ORDER_TREE_ROOT));
    for (auto& tbl : history_tables) {
        QStandardItem* child = new QStandardItem;
        child->setText(tbl.c_str());
        orderRoot->appendRow(child);
    }
    _dbTree_model->appendRow(orderRoot);

}




void SqlViewer::openTableTab_orders(const std::string& table_name)
{
    S4::sqlite::s4_history_trade_t_dbTbl history_tbl;
    //std::string condition = " WHERE mktCodeStr = '" + stkName + "'";
    //if (stgName.size()) {
    //    condition += " AND stgName = '" + stgName + "'";
    //}
    std::vector<struct s4_history_trade_t> history_trade_data;
    _pHistory_db->read_table<S4::sqlite::s4_history_trade_t_dbTbl::data_t>(&history_tbl, table_name, history_trade_data);
    std::vector<std::string> col_name = _pHistory_db->get_colum_list(table_name);

    orderModel* model = new orderModel(this);
    model->setTitle(col_name);
    for (auto& order : history_trade_data) {
        model->append(order);
    }
    _order_models[table_name] = model;

    if(tabAlreadyExists(table_name.c_str())){
        QWidget *tab = tableMap.find(table_name.c_str()).value();
        int i = ui->tabWidget->indexOf(tab);
        ui->tabWidget->setCurrentIndex(i);
    }
    else{
        QTableView* tv = new QTableView(this);
        tableMap.insert(table_name.c_str(),tv);
        //QSqlQueryModel *model = dbHandler->getTableData(table_name.c_str());
        QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(model);
        tv->setModel(proxyModel);
        tv->setSortingEnabled(true);
        tv->setSelectionBehavior(QAbstractItemView::SelectRows);
        _order_views[table_name] = tv;
        connect(tv, SIGNAL(doubleClicked(const QModelIndex & )), this, SLOT(orderDoubleClicked(const QModelIndex&)));

        int i = ui->tabWidget->addTab(tv, table_name.c_str());
        ui->tabWidget->setCurrentIndex(i);
    }
}

void SqlViewer::orderDoubleClicked(const QModelIndex& index)
{
    const QString tabName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    QTableView* tv = (QTableWidget*)ui->tabWidget->currentWidget();//_order_views[tabName.toStdString()];
    int row = tv->currentIndex().row();
    QAbstractItemModel* model = tv->model();
    QModelIndex indexCode = model->index(row, 3);
    const std::string mktCode = model->data(indexCode).toString().toStdString();

    ui->statusbar->showMessage(mktCode.c_str());

    if (_tcp_json_server_running) {
        if (_pTcp_json_server && _pTcp_json_server->client_nb()) {
            nw_load_instrument_t cmd;
            cmd.seq = _cmd_seq++;
            cmd.mktCode = mktCode;
            cmd.stgName = "";
            cmd.tableName = tabName.toStdString();
            json_ptr_t pJ = make_json_ptr();
            nw_load_instrument_t::to_json(*pJ, cmd);
            _pTcp_json_server->write_broadcast(pJ);
        }
    }

}


void SqlViewer::openTableTab(const QModelIndex &index){
    QString str;
    str += QStringLiteral("当前选中：%1\nrow:%2,column:%3\n").arg(index.data().toString())
        .arg(index.row()).arg(index.column());
    str += QStringLiteral("父级：%1\n").arg(index.parent().data().toString());
    ui->statusbar->showMessage(str);

    if (index.parent().data().toString() == QStringLiteral(HISTORY_ORDER_TREE_ROOT) && _pHistory_db) {
        std::string table_name = index.data().toString().toStdString();
        openTableTab_orders(table_name);
    }

    //const QString tabName = index.data(Qt::DisplayRole).toString();
    //if(tabAlreadyExists(tabName)){
    //    QWidget *tab = tableMap.find(tabName).value();
    //    int i = ui->tabWidget->indexOf(tab);
    //    ui->tabWidget->setCurrentIndex(i);
    //}
    //else{
    //    QTableView* tv = new QTableView(this);
    //    tableMap.insert(tabName,tv);
    //    QSqlQueryModel *model = dbHandler->getTableData(tabName);
    //    tv->setModel(model);


    //    int i = ui->tabWidget->addTab(tv,tabName);
    //    ui->tabWidget->setCurrentIndex(i);
    //}

}

void SqlViewer::connectDbDialog()
{
    connectionDialog = new DbConnectDialog(this);

    connect(connectionDialog,&QDialog::accepted,this,&SqlViewer::onConnectionDialogAccepted);
    connect(connectionDialog,&QDialog::rejected,this,&SqlViewer::onConnectionDialogRejected);

    connectionDialog->show();
}

void SqlViewer::onConnectionDialogAccepted()
{
    if(dbHandler){
        delete dbHandler;
    }
    this->dbHandler = connectionDialog->getDBHandler();

    for (int i = ui->tabWidget->count() - 1; i >= 0; --i) {
       tabCloseRequestHandler(i);
    }

    QAbstractListModel *model = new TableList(dbHandler->getTables());
    //ui->dbTree->setFlow(QListView::Flow::TopToBottom);
    ui->dbTree->setModel(model);

    delete connectionDialog;
}

void SqlViewer::onConnectionDialogRejected()
{
    delete connectionDialog;
}

bool SqlViewer::tabAlreadyExists(const QString &tabName) const
{
    if(tableMap.size() == 0) return false;

    auto it = tableMap.find(tabName);

    return it != tableMap.end();
}

void SqlViewer::tabCloseRequestHandler(int index)
{
    const QString tabName = ui->tabWidget->tabText(index);
    ui->tabWidget->removeTab(index);

    auto it = tableMap.find(tabName);
    if(it != tableMap.end()){
        delete it.value();
        tableMap.remove(tabName);
    }
}

}