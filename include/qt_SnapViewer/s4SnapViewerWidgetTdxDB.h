#pragma once


#include "types/s4type.h"
#include "qt_SnapViewer/s4SnapViewerWidget.h"


namespace S4{
namespace QT{

//    void updateTableList();


class s4SnapViewerWidgetTdxDB : public s4SnapViewerWidget
{
    Q_OBJECT

public:
    explicit s4SnapViewerWidgetTdxDB(QWidget *parent = nullptr);
    virtual ~s4SnapViewerWidgetTdxDB();

public slots:
	void onOpenTdxDB();

	void closeSnapTab(int index);

    void dbTree_doubleClicked(const QModelIndex &index);
	void openTdxSnapTab(const std::string& db_name, const std::string& table_name);

    void nextTdxSnap();
    // void addTdxSnaps();

signals:
	void signal_status(const QString&);

protected:

	std::map<std::string, std::string> _db_list;

	struct snap_info_t
	{
		std::vector<tdx_snap_t> snaps;
		int curse;
	};
	std::map<QString, snap_info_t> _instrument_info_cargo;

protected:
	void mouseMoveEvent(QMouseEvent* )
	{
		//qDebug() << "Kview " << hasMouseTracking() << " " << event->pos().x() << ", " << event->pos().y();
	}

protected:



};

}
}