#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4189) 
#endif

#include "qt_L2Viewer/s4L2Viewer.h"
#include "qt_L2Viewer/s4L2ViewerWidgetL2Live.h"

#include "ui_s4L2Viewer.h"
#include "common/s4logger.h"

#include <QSplitter>
#include <QScrollArea>
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaType>
#include <QStyleFactory>
#include <QSortFilterProxyModel>
#include <QTableWidget>

using namespace std;

namespace S4 {
namespace QT {

//CREATE_LOCAL_LOGGER("qt_SnapViewer")

#define TITLE_L2_DB QStringLiteral("L2-DB")
#define TITLE_L2_LIVE QStringLiteral("APEX L2-LIVE")

L2Viewer::L2Viewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::L2Viewer)
{   
	ui->setupUi(this);
	this->setWindowTitle("L2Viewer");


	connect(ui->action_mode_tdxDB, &QAction::triggered, this, &L2Viewer::action_mode_L2DB);
	connect(ui->action_mode_L2Live, &QAction::triggered, this, &L2Viewer::action_mode_L2Live);

    action_mode_L2Live();
}

void L2Viewer::action_mode_L2DB()
{
    qDebug() << "not ready!";

	if (this->windowTitle()==TITLE_L2_DB){
		// ((s4SnapViewerWidgetTdxDB*)(this->centralWidget()))->onOpenTdxDB();
		return;
	}
	// s4SnapViewerWidgetTdxDB* pWidget = new s4SnapViewerWidgetTdxDB(this);
	// connect(ui->actionOpen, &QAction::triggered, pWidget, &s4SnapViewerWidgetTdxDB::onOpenTdxDB);
	// connect(ui->actionNextSnap, &QAction::triggered, pWidget, &s4SnapViewerWidgetTdxDB::nextTdxSnap);

	// this->setCentralWidget(pWidget);
	this->setWindowTitle(TITLE_L2_DB);
}

void L2Viewer::action_mode_L2Live()
{
	if (this->windowTitle()==TITLE_L2_LIVE){
		return;
	}
	snapViewerWidgetL2Live* pWidget = new snapViewerWidgetL2Live(this);
	//connect(ui->actionOpen, &QAction::triggered, pWidget, &snapViewerWidgetL2Live::onStartL2LiveReceiver);
	//connect(ui->actionClose, &QAction::triggered, pWidget, &snapViewerWidgetL2Live::onStopL2LiveReceiver);

	this->setCentralWidget(pWidget);
	this->setWindowTitle(TITLE_L2_LIVE);
}

L2Viewer::~L2Viewer()
{
}

}
}
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
