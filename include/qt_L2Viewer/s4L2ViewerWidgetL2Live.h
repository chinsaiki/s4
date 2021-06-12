﻿#pragma once


#include "types/s4type.h"
#include "qt_SnapViewer/s4SnapViewerWidget.h"
#include "qt_L2Viewer/s4L2TableModel_L2Stats.h"
#include "network/L2_udp_recver_th.h"
#include "qt_common/s4qt_unicTreeView.h"

#include "qt_market/s4MarketDataAgent.h"
#include "qt_market/s4MarketDataSource.h"

#include <QTableView>
#include <QStandardItem>
#include <QStringListModel>

namespace S4{
namespace QT{

//    void updateTableList();


class snapViewerWidgetL2Live : public s4SnapViewerWidget
{
    Q_OBJECT

public:
    explicit snapViewerWidgetL2Live(QWidget *parent = nullptr);
    virtual ~snapViewerWidgetL2Live();

public slots:
	void slot_startMDSource();
	void slot_stopMDSource();
	
	void setCurrentInstrument(const QString& code);
	void openInstrumentTab(const QString& code);
	void closeInstrumentTab(const QString& code);

    // void dbTree_doubleClicked(const QModelIndex &index);
	// void openTdxSnapTab(const std::string& db_name, const std::string& table_name);

    // void nextTdxSnap();
    // void addTdxSnaps();
	virtual void closeSnapTab(int index) override;
signals:
	void signal_status(const QString&);
	void signal_closeSnapTab(const QString&);
protected:

	struct snap_info_t
	{
		mktCodeI_t code;
	};
	std::map<QString, snap_info_t> _instrument_info_cargo;

protected:
	void mouseMoveEvent(QMouseEvent* )
	{
		//qDebug() << "Kview " << hasMouseTracking() << " " << event->pos().x() << ", " << event->pos().y();
	}

protected:
	QTableView* _stats_tv;
	snapTableModel_L2Stats* _stats_model;
	QStandardItem* _aim_security_root;
protected:
	std::shared_ptr<NW::L2DataQ_t> _pL2DataQ;
	std::shared_ptr<NW::L2CmdQ_t> _pL2CmdQ;

	std::shared_ptr<NW::L2_udp_recver_th> _udp_recver_th;
	marketDataAgent* _snapMarketDataLive;

	marketDataSource* _marketDataSource;

	void startMDAgent();
};


}
}