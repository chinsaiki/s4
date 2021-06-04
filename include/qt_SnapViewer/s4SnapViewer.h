#pragma once

#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>

#include "types/s4type.h"
#include "qt_SnapViewer/s4SnapInstrument.h"



QT_BEGIN_NAMESPACE
namespace Ui {
class s4SnapViewer;
}
QT_END_NAMESPACE

namespace S4{
namespace QT{



class s4SnapViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit s4SnapViewer(QWidget *parent = nullptr);
    ~s4SnapViewer();

public slots:
//    void updateTableList();
    void dbTree_doubleClicked(const QModelIndex &index);
	void openTdxSnapTab(const std::string& db_name, const std::string& table_name);

    void snap_level_doubleClicked();

signals:

private:
    Ui::s4SnapViewer *ui;
	QStandardItemModel* _dbTree_model = nullptr;

	std::map<std::string, std::string> _db_list;

	struct snap_info_t
	{
		std::vector<tdx_snap_t> snaps;
		int curse;
		snapInstrument* view;
	};
	std::map<QString, snap_info_t> _instrument_cargo;

private:
	void mouseMoveEvent(QMouseEvent* )
	{
		//qDebug() << "Kview " << hasMouseTracking() << " " << event->pos().x() << ", " << event->pos().y();
	}
	void snapTab_closeRequest(int index);

private:


	void onOpenTdxDB();

	bool tabAlreadyExists(const QString& tabName) const;

};

}
}