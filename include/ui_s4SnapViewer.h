/********************************************************************************
** Form generated from reading UI file 's4SnapViewer.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_S4SNAPVIEWER_H
#define UI_S4SNAPVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_s4SnapViewer
{
public:
    QAction *actionExit;
    QAction *actionNextSnap;
    QAction *action_mode_L2Live;
    QAction *action_mode_tdxDB;
    QAction *actionOpen;
    QAction *actionClose;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuMode_2;
    QMenu *menuExport;
    QMenu *menuExport_2;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *s4SnapViewer)
    {
        if (s4SnapViewer->objectName().isEmpty())
            s4SnapViewer->setObjectName(QStringLiteral("s4SnapViewer"));
        s4SnapViewer->resize(800, 604);
        s4SnapViewer->setMinimumSize(QSize(0, 604));
        s4SnapViewer->setBaseSize(QSize(0, 20));
        actionExit = new QAction(s4SnapViewer);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionNextSnap = new QAction(s4SnapViewer);
        actionNextSnap->setObjectName(QStringLiteral("actionNextSnap"));
        action_mode_L2Live = new QAction(s4SnapViewer);
        action_mode_L2Live->setObjectName(QStringLiteral("action_mode_L2Live"));
        action_mode_tdxDB = new QAction(s4SnapViewer);
        action_mode_tdxDB->setObjectName(QStringLiteral("action_mode_tdxDB"));
        actionOpen = new QAction(s4SnapViewer);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionClose = new QAction(s4SnapViewer);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        centralwidget = new QWidget(s4SnapViewer);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        s4SnapViewer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(s4SnapViewer);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuMode_2 = new QMenu(menuFile);
        menuMode_2->setObjectName(QStringLiteral("menuMode_2"));
        menuExport = new QMenu(menubar);
        menuExport->setObjectName(QStringLiteral("menuExport"));
        menuExport_2 = new QMenu(menuExport);
        menuExport_2->setObjectName(QStringLiteral("menuExport_2"));
        s4SnapViewer->setMenuBar(menubar);
        statusbar = new QStatusBar(s4SnapViewer);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        s4SnapViewer->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuExport->menuAction());
        menuFile->addAction(menuMode_2->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionClose);
        menuFile->addAction(actionExit);
        menuMode_2->addAction(action_mode_L2Live);
        menuMode_2->addAction(action_mode_tdxDB);
        menuExport->addAction(menuExport_2->menuAction());
        menuExport->addAction(actionNextSnap);
        menuExport_2->addSeparator();

        retranslateUi(s4SnapViewer);
        QObject::connect(actionExit, SIGNAL(triggered()), s4SnapViewer, SLOT(close()));

        QMetaObject::connectSlotsByName(s4SnapViewer);
    } // setupUi

    void retranslateUi(QMainWindow *s4SnapViewer)
    {
        s4SnapViewer->setWindowTitle(QApplication::translate("s4SnapViewer", "s4SnapViewer", nullptr));
        actionExit->setText(QApplication::translate("s4SnapViewer", "Exit", nullptr));
        actionNextSnap->setText(QApplication::translate("s4SnapViewer", "Next Snap", nullptr));
#ifndef QT_NO_TOOLTIP
        actionNextSnap->setToolTip(QApplication::translate("s4SnapViewer", "Next Snap", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionNextSnap->setShortcut(QApplication::translate("s4SnapViewer", "Alt+X", nullptr));
#endif // QT_NO_SHORTCUT
        action_mode_L2Live->setText(QApplication::translate("s4SnapViewer", "L2 Live", nullptr));
#ifndef QT_NO_SHORTCUT
        action_mode_L2Live->setShortcut(QApplication::translate("s4SnapViewer", "Ctrl+L", nullptr));
#endif // QT_NO_SHORTCUT
        action_mode_tdxDB->setText(QApplication::translate("s4SnapViewer", "TDX DB", nullptr));
#ifndef QT_NO_SHORTCUT
        action_mode_tdxDB->setShortcut(QApplication::translate("s4SnapViewer", "Ctrl+T", nullptr));
#endif // QT_NO_SHORTCUT
        actionOpen->setText(QApplication::translate("s4SnapViewer", "Open", nullptr));
#ifndef QT_NO_SHORTCUT
        actionOpen->setShortcut(QApplication::translate("s4SnapViewer", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionClose->setText(QApplication::translate("s4SnapViewer", "Close", nullptr));
#ifndef QT_NO_SHORTCUT
        actionClose->setShortcut(QApplication::translate("s4SnapViewer", "Ctrl+Q", nullptr));
#endif // QT_NO_SHORTCUT
        menuFile->setTitle(QApplication::translate("s4SnapViewer", "File", nullptr));
        menuMode_2->setTitle(QApplication::translate("s4SnapViewer", "Mode", nullptr));
        menuExport->setTitle(QApplication::translate("s4SnapViewer", "Tools", nullptr));
        menuExport_2->setTitle(QApplication::translate("s4SnapViewer", "Export...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class s4SnapViewer: public Ui_s4SnapViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_S4SNAPVIEWER_H
