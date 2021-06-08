#pragma once

#include <QTableView>
#include <QTreeView>
#include <QStandardItemModel>

#include "types/s4type.h"
#include "qt_SnapViewer/s4SnapInstrument.h"

#include "network/L2_udp_recver_th.h"
#include "s4SnapMarketDataLive.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class s4SnapViewerWidget;
}
QT_END_NAMESPACE

namespace S4{
namespace QT{


//快照或流行情 可视化应用主窗口
class s4SnapViewerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit s4SnapViewerWidget(QWidget *parent = nullptr);
    virtual ~s4SnapViewerWidget();

public slots:

	virtual void openSnapTab(const QString& tab_name, snapInstrument* pInstrument);
	virtual void closeSnapTab(int index);

	void newTree(const QString& root_name, const std::vector<QString>& leaf_names);
	void delTree(const QString& root_name);

	void appendTreeItems(const QString& root_name, const std::vector<QString>& leaf_names);
	void removeTreeItems(const QString& root_name, const std::vector<QString>& leaf_names);

signals:

protected:
    QTreeView *_treeView;
    QTabWidget *_tabWidget;
protected:
    Ui::s4SnapViewerWidget *ui;
	QStandardItemModel* _tree_model = nullptr;

	std::map<QString, snapInstrument*> _instrument_view_cargo;

protected:
	void mouseMoveEvent(QMouseEvent* )
	{
		//qDebug() << "s4SnapViewerWidget " << hasMouseTracking() << " " << event->pos().x() << ", " << event->pos().y();
	}

protected:
	bool tabAlreadyExists(const QString& tabName) const;

};

}
}