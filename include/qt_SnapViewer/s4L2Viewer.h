#pragma once

#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>

#include "types/s4type.h"
#include "qt_SnapViewer/s4SnapInstrument.h"



QT_BEGIN_NAMESPACE
namespace Ui {
class L2Viewer;
}
QT_END_NAMESPACE

namespace S4{
namespace QT{



class L2Viewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit L2Viewer(QWidget *parent = nullptr);
    ~L2Viewer();

public slots:
	void action_mode_L2DB();
	void action_mode_L2Live();

signals:

private:
    Ui::L2Viewer *ui;


private:
	void mouseMoveEvent(QMouseEvent* )
	{
		//qDebug() << "Kview " << hasMouseTracking() << " " << event->pos().x() << ", " << event->pos().y();
	}

private:



};

}
}