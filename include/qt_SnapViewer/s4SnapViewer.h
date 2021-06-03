#pragma once

#include <QMainWindow>
#include <QPushButton>
//#include "qt\console\cliparser.h"
#include "qt_console/s4console.h"
//#include "qt\s4qt_view_k.h"
#include "qt_common/s4qt_data_if.h"
//#include "network/tcp_client.h"
#include "qt_common/s4qt_tcp.h"
#include "qt_Kviewer/s4Kviewer_instrumentTab.h"
#include <QDebug>
#include <QMouseEvent>

namespace Ui {
class s4SnapViewer;
}

namespace S4{
namespace QT{



class s4SnapViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit s4SnapViewer(QWidget *parent = 0);
    ~s4SnapViewer();

public slots:
	void load(const std::string& mktCode, const std::string& stgName, const std::string& orderTblName);
	void onTcpRecvJson(const std::shared_ptr<nlohmann::json>& pJ);

	void onButton_next_trade(void);
	void onButton_last_trade(void);

signals:
	void signal_getInfo(const std::string & stkName, const struct S4::stkInfoReq_t& infoReq, class S4::stkInfo_t*& info);
	void signal_loadOrdres(const std::string & stkName, const std::string & stgName, const std::string & orderTblName, std::vector<S4::s4_history_trade_t>& history_trade_data);
private:
    Ui::s4SnapViewer *ui;

	//s3qt::myKwin* pmyKwin;
	Kviewer_instrumentTab* _instrument_tab;
	// QTabWidget* _instrument_tab2;
	// QTabWidget* _instrument_tab3;

	//cliparser* pCLI;
	data_panel_t _data_panel;
	std::shared_ptr<S4::QT::s4qt_data_if> _data_if;

	std::shared_ptr<qt_tcp_json_client> _pTcp_json_client;

	QPushButton* button_last_trade;
	QPushButton* button_next_trade;

	s4console* _console;
private:
	void mouseMoveEvent(QMouseEvent* )
	{
		//qDebug() << "Kview " << hasMouseTracking() << " " << event->pos().x() << ", " << event->pos().y();
	}

private:
	void showData();

	void onOpenDB();
	void onCallConsole();
	void onTcpSetup();


};

}
}