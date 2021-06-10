/********************************************************************************
** Form generated from reading UI file 'sqlviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SQLVIEWER_H
#define UI_SQLVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SqlViewer
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QAction *actionDatabase;
    QAction *actionCurrent_table_as_CSV;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QSplitter *splitter_3;
    QSplitter *splitter_2;
    QTreeView *dbTree;
    QTabWidget *tabWidget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuExport;
    QMenu *menuExport_2;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *SqlViewer)
    {
        if (SqlViewer->objectName().isEmpty())
            SqlViewer->setObjectName(QStringLiteral("SqlViewer"));
        SqlViewer->resize(809, 604);
        SqlViewer->setMinimumSize(QSize(0, 604));
        SqlViewer->setBaseSize(QSize(0, 20));
        actionOpen = new QAction(SqlViewer);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionExit = new QAction(SqlViewer);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionDatabase = new QAction(SqlViewer);
        actionDatabase->setObjectName(QStringLiteral("actionDatabase"));
        actionCurrent_table_as_CSV = new QAction(SqlViewer);
        actionCurrent_table_as_CSV->setObjectName(QStringLiteral("actionCurrent_table_as_CSV"));
        centralwidget = new QWidget(SqlViewer);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        splitter_3 = new QSplitter(centralwidget);
        splitter_3->setObjectName(QStringLiteral("splitter_3"));
        splitter_3->setOrientation(Qt::Vertical);
        splitter_2 = new QSplitter(splitter_3);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter_2->sizePolicy().hasHeightForWidth());
        splitter_2->setSizePolicy(sizePolicy);
        splitter_2->setLayoutDirection(Qt::LeftToRight);
        splitter_2->setOrientation(Qt::Horizontal);
        dbTree = new QTreeView(splitter_2);
        dbTree->setObjectName(QStringLiteral("dbTree"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(dbTree->sizePolicy().hasHeightForWidth());
        dbTree->setSizePolicy(sizePolicy1);
        dbTree->setMinimumSize(QSize(60, 0));
        dbTree->setBaseSize(QSize(0, 0));
        dbTree->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        dbTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
        splitter_2->addWidget(dbTree);
        tabWidget = new QTabWidget(splitter_2);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy2);
        tabWidget->setMinimumSize(QSize(0, 0));
        tabWidget->setTabsClosable(true);
        splitter_2->addWidget(tabWidget);
        splitter_3->addWidget(splitter_2);

        verticalLayout_2->addWidget(splitter_3);

        SqlViewer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(SqlViewer);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 809, 23));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuExport = new QMenu(menubar);
        menuExport->setObjectName(QStringLiteral("menuExport"));
        menuExport_2 = new QMenu(menuExport);
        menuExport_2->setObjectName(QStringLiteral("menuExport_2"));
        SqlViewer->setMenuBar(menubar);
        statusbar = new QStatusBar(SqlViewer);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        SqlViewer->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuExport->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionExit);
        menuExport->addAction(menuExport_2->menuAction());
        menuExport_2->addAction(actionDatabase);
        menuExport_2->addSeparator();
        menuExport_2->addAction(actionCurrent_table_as_CSV);

        retranslateUi(SqlViewer);
        QObject::connect(actionExit, SIGNAL(triggered()), SqlViewer, SLOT(close()));

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(SqlViewer);
    } // setupUi

    void retranslateUi(QMainWindow *SqlViewer)
    {
        SqlViewer->setWindowTitle(QApplication::translate("SqlViewer", "SqlViewer", nullptr));
        actionOpen->setText(QApplication::translate("SqlViewer", "Open S4 configure", nullptr));
#ifndef QT_NO_SHORTCUT
        actionOpen->setShortcut(QApplication::translate("SqlViewer", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionExit->setText(QApplication::translate("SqlViewer", "Exit", nullptr));
        actionDatabase->setText(QApplication::translate("SqlViewer", "Database", nullptr));
        actionCurrent_table_as_CSV->setText(QApplication::translate("SqlViewer", "Current table as CSV", nullptr));
        menuFile->setTitle(QApplication::translate("SqlViewer", "File", nullptr));
        menuExport->setTitle(QApplication::translate("SqlViewer", "Tools", nullptr));
        menuExport_2->setTitle(QApplication::translate("SqlViewer", "Export...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SqlViewer: public Ui_SqlViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SQLVIEWER_H
