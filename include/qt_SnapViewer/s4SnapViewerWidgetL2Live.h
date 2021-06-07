#pragma once


#include "types/s4type.h"
#include "qt_SnapViewer/s4SnapViewerWidget.h"
#include "network/L2_udp_recver_th.h"

#include <QTableView>

namespace S4{
namespace QT{

//    void updateTableList();


class s4SnapViewerWidgetL2Live : public s4SnapViewerWidget
{
    Q_OBJECT

public:
    explicit s4SnapViewerWidgetL2Live(QWidget *parent = nullptr);
    virtual ~s4SnapViewerWidgetL2Live();

public slots:
	void onStartL2LiveReceiver();
	void onStopL2LiveReceiver();
	
	void openInstrumentTab(mktCodeI_t);
	void closeSnapTab(int index);

    void dbTree_doubleClicked(const QModelIndex &index);
	// void openTdxSnapTab(const std::string& db_name, const std::string& table_name);

    // void nextTdxSnap();
    // void addTdxSnaps();

signals:
	void signal_status(const QString&);
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

	std::shared_ptr<NW::L2DataQ_t> _pL2DataQ;
	std::shared_ptr<NW::L2CmdQ_t> _pCmdQ;

	QTableView* _stats_tv;

};

}
}