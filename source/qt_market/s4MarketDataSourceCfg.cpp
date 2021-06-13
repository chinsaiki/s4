#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4189) 
#endif

#include "qt_market/s4MarketDataSource.h"
#include "qt_market/s4MarketDataSourceCfg.h"
#include "qt_common/s4qt_itemDelegateNumberOnly.h"

#include "ui_s4MarketDataSourceCfg.h"

#include <QSplitter>
#include <QScrollArea>
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaType>
#include <QStyleFactory>
#include <QSortFilterProxyModel>
#include <QTableWidget>
#include <QIntValidator>

using namespace std;

namespace S4 {
namespace QT {


#define BOTTUM_TEXT_EDIT QStringLiteral("修改")
#define BOTTUM_TEXT_OK QStringLiteral("确定")
marketDataSourceCfg::marketDataSourceCfg(const QString& Name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::marketDataSourceCfg)
{   
	ui->setupUi(this);


    ui->lineEdit_name->setText(Name);
    ui->lineEdit_ip0->setText("0");
    ui->lineEdit_ip1->setText("0");
    ui->lineEdit_ip2->setText("0");
    ui->lineEdit_ip3->setText("0");
    ui->lineEdit_port->setText("8888");
    
    QIntValidator *validator_ip = new numberValidator(0, 255, this);
    ui->lineEdit_ip0->setValidator(validator_ip);
    ui->lineEdit_ip1->setValidator(validator_ip);
    ui->lineEdit_ip2->setValidator(validator_ip);
    ui->lineEdit_ip3->setValidator(validator_ip);

    QIntValidator *validator_port = new numberValidator(1, 65535, this);
    ui->lineEdit_port->setValidator(validator_port);

    QStringList strList;
    strList<<"UDP"<<"UDP-lite";
    ui->comboBox_type->addItems(strList);

    std::vector<SockUtil::nic_description_t> NICs;
    SockUtil::listNIC(NICs);
    QStringList NICs_str;
    NICs_str << QStringLiteral("自动");
    _NICs_info.clear();
    for (auto& NIC : NICs){
        if (NIC.isEth && NIC.isUp){
            QString show_nic;
#ifdef WIN32
            show_nic += QString::fromStdWString(NIC.nic_name) + "(";
#else
            show_nic += QString::fromStdString(NIC.nic_name);
#endif
            show_nic += NIC.local_ip.c_str();
            show_nic += ")";
            NICs_str << show_nic;
            _NICs_info << NIC;
        }
    }
    ui->comboBox_nic->addItems(NICs_str);

    connect(ui->pushButton_edit, &QPushButton::clicked, this, &marketDataSourceCfg::slot_onButtom);

    _editable = true;
    ui->pushButton_edit->setText(BOTTUM_TEXT_OK);
}

void marketDataSourceCfg::slot_disableEdit()
{

	if (_editable) {
		slot_onButtom();
	}
	ui->pushButton_edit->setDisabled(true);
}

void marketDataSourceCfg::slot_enableEdit() {
	ui->pushButton_edit->setEnabled(true);
}

void marketDataSourceCfg::slot_onButtom()
{
    if (_editable){
        _editable = false;
        
        ui->lineEdit_name->setEnabled(false);
        ui->lineEdit_ip0->setEnabled(false);
        ui->lineEdit_ip1->setEnabled(false);
        ui->lineEdit_ip2->setEnabled(false);
        ui->lineEdit_ip3->setEnabled(false);
        ui->lineEdit_port->setEnabled(false);
        ui->comboBox_type->setEnabled(false);
        ui->comboBox_nic->setEnabled(false);

        ui->pushButton_edit->setText(BOTTUM_TEXT_EDIT);

        emit signal_fixed(this);
    }else{
        _editable = true;
        
        ui->lineEdit_name->setEnabled(true);
        ui->lineEdit_ip0->setEnabled(true);
        ui->lineEdit_ip1->setEnabled(true);
        ui->lineEdit_ip2->setEnabled(true);
        ui->lineEdit_ip3->setEnabled(true);
        ui->lineEdit_port->setEnabled(true);
        ui->comboBox_type->setEnabled(true);
        ui->comboBox_nic->setEnabled(true);

        ui->pushButton_edit->setText(BOTTUM_TEXT_OK);
    }
}


struct marketDataSourceCfg::cfg_t marketDataSourceCfg::getCfg() const
{
    cfg_t ret;

    ret.Name = ui->lineEdit_name->text().toStdString();
    ret.listen_IP = ui->lineEdit_ip0->text().toStdString() + 
        "." +ui->lineEdit_ip1->text().toStdString() + 
        "." +ui->lineEdit_ip2->text().toStdString() + 
        "." +ui->lineEdit_ip3->text().toStdString();
    ret.Port = ui->lineEdit_port->text().toUInt();
    ret.SourceType = ui->comboBox_type->currentText().toStdString();
    int i = ui->comboBox_nic->currentIndex() - 1;
    if (i < 0){
        ret.local_IP = "0.0.0.0";
    }else{
        ret.local_IP = _NICs_info[i].local_ip;
    }

    return ret;
}

QString marketDataSourceCfg::label() const 
{
	return ui->lineEdit_name->text(); 
}


marketDataSourceCfg::~marketDataSourceCfg()
{
}

}
}
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
