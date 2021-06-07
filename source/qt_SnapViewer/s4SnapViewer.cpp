#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4189) 
#endif

#include "qt_SnapViewer/s4SnapViewer.h"
#include "qt_SnapViewer/s4SnapViewerWidgetTdxDB.h"
#include "qt_SnapViewer/s4SnapViewerWidgetL2Live.h"

#include "ui_s4SnapViewer.h"
#include "common/s4logger.h"

#include <QSplitter>
#include <QScrollArea>
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaType>
#include <QStyleFactory>
#include <QSortFilterProxyModel>
#include <QTableWidget>

#ifdef max
#undef max
#endif

using namespace std;

namespace S4 {
namespace QT {

//CREATE_LOCAL_LOGGER("qt_SnapViewer")

#define TITLE_TDX_DB QStringLiteral("TDX-DB")
#define TITLE_TDX_LIVE QStringLiteral("TDX-LIVE")
#define TITLE_L2_DB QStringLiteral("L2-DB")
#define TITLE_L2_LIVE QStringLiteral("L2-LIVE")

s4SnapViewer::s4SnapViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::s4SnapViewer)
{   
	ui->setupUi(this);
	this->setWindowTitle("SnapViewer");

	ui->centralwidget->setMouseTracking(true);

	connect(ui->action_mode_tdxDB, &QAction::triggered, this, &s4SnapViewer::action_mode_tdxDB);
	connect(ui->action_mode_L2Live, &QAction::triggered, this, &s4SnapViewer::action_mode_L2Live);

}

void s4SnapViewer::action_mode_tdxDB()
{
	if (this->windowTitle()==TITLE_TDX_DB){
		((s4SnapViewerWidgetTdxDB*)(this->centralWidget()))->onOpenTdxDB();
		return;
	}
	s4SnapViewerWidgetTdxDB* pWidget = new s4SnapViewerWidgetTdxDB(this);
	connect(ui->actionOpen, &QAction::triggered, pWidget, &s4SnapViewerWidgetTdxDB::onOpenTdxDB);
	connect(ui->actionNextSnap, &QAction::triggered, pWidget, &s4SnapViewerWidgetTdxDB::nextTdxSnap);

	this->setCentralWidget(pWidget);
	this->setWindowTitle(TITLE_TDX_DB);
}

void s4SnapViewer::action_mode_L2Live()
{
	if (this->windowTitle()==TITLE_L2_LIVE){
		return;
	}
	s4SnapViewerWidgetL2Live* pWidget = new s4SnapViewerWidgetL2Live(this);
	connect(ui->actionOpen, &QAction::triggered, pWidget, &s4SnapViewerWidgetL2Live::onStartL2LiveReceiver);
	connect(ui->actionClose, &QAction::triggered, pWidget, &s4SnapViewerWidgetL2Live::onStopL2LiveReceiver);

	this->setCentralWidget(pWidget);
	this->setWindowTitle(TITLE_L2_LIVE);
}

s4SnapViewer::~s4SnapViewer()
{
}

}
}
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
