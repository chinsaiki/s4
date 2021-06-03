#pragma once
#include "network/tcp_server.h"

#include <QMainWindow>
#include <QModelIndex>
#include <QTableView>
#include <QTreeView>
#include <QStandardItemModel>

#include "dbconnectdialog.h"
#include "DBHandler.h"
#include "tablelist.h"
#include "jsonTypes/s4_history_trade_t_dbTbl.h"
#include "db_sqlite/db.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SqlViewer; }
QT_END_NAMESPACE

namespace S4{
class orderModel;
class SqlViewer : public QMainWindow
{
    Q_OBJECT
public:
    SqlViewer(QWidget *parent = nullptr);
    ~SqlViewer();

public slots:
    void connectDbDialog();

    void onConnectionDialogAccepted();
    void onConnectionDialogRejected();

//    void updateTableList();
    void openTableTab(const QModelIndex &index);

    void orderDoubleClicked(const QModelIndex&);
private:
    Ui::SqlViewer *ui;
    DbConnectDialog *connectionDialog = nullptr;
    DBHandler *dbHandler = nullptr;

    QStandardItemModel* _dbTree_model;

    bool tabAlreadyExists(const QString &tableName) const;
    void tabCloseRequestHandler(int index);

private:
    std::shared_ptr<sqlite::DB_t> _pHistory_db;
    QMap<QString, QTableView *> tableMap;
    std::map<std::string, orderModel*> _order_models;

    volatile bool _tcp_json_server_running = false;
    std::shared_ptr<NW::tcp_json_server> _pTcp_json_server;
    void onTcpSetup();
    int _cmd_seq = 0;

    void onOpen();
    void onLoadConf(void);
    void onOpenDBs(void);
    void onOpenDB_orders(void);

    void openTableTab_orders(const std::string& table_name);

};


class orderModel : public QAbstractTableModel {
    QList<s4_history_trade_t> m_data;
    std::vector<std::string> _title;
public:
    orderModel(QObject* parent = {}) : QAbstractTableModel{ parent } {}

    void setTitle(const std::vector<std::string>& title) {
        _title = title;
    }

    int rowCount(const QModelIndex&) const override { return m_data.count(); }
    int columnCount(const QModelIndex&) const override { return (int)_title.size(); }
    QVariant data(const QModelIndex& index, int role) const override {
        if (role != Qt::DisplayRole) return {}; // && role != Qt::EditRole
        const auto& order = m_data[index.row()];
        switch (index.column()) {
        case 0: return QVariant::fromValue(order.id);
        case 1: return order.date;
        case 2: return order.stgName.c_str();
        case 3: return order.mktCodeStr.c_str();
        case 4: return QVariant::fromValue(order.time_utcSec);
        case 5: return order.datetime.c_str();
        case 6: return trade_opt_t_toString((order.optType)).c_str();
        case 7: return order.position.c_str();
        case 8: return order.status.c_str();
        case 9: return order.order_open;
        case 10: return order.order_take;
        case 11: return order.order_stop;
        case 12: return order.order_close;
        case 13: return order.order_vol;
        case 14: return order.deal_open;
        case 15: return order.deal_close;
        case 16: return order.deal_vol;
        case 17: return order.deal_amt;
        case 18: return order.commission;
        case 19: return order.stamp_duty;
        case 20: return order.transfer_fee;
        case 21: return order.other_fees;
        case 22: return QString::fromLocal8Bit(order.remarks.data());
        default: return {};
        };
    }
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
        if ((size_t)section >= _title.size()) return {};
        //switch (section) {
        //case 0: return "Make";
        //case 1: return "Model";
        //case 2: return "Reg.#";
        //default: return {};
        return _title[section].c_str();
    }

    void append(const s4_history_trade_t& order) {
        beginInsertRows({}, m_data.count(), m_data.count());
        m_data.append(order);
        endInsertRows();
    }
};

}