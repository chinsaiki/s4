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
	void action_mode_snapDB();
	void action_mode_snapLive();

signals:

private:
    Ui::s4SnapViewer *ui;


private:
	void mouseMoveEvent(QMouseEvent* )
	{
		//qDebug() << "Kview " << hasMouseTracking() << " " << event->pos().x() << ", " << event->pos().y();
	}

private:



};

}
}