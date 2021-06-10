/********************************************************************************
** Form generated from reading UI file 's4Kviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_S4KVIEWER_H
#define UI_S4KVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_s4Kviewer
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QAction *actionCallConsole;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuconsole;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *s4Kviewer)
    {
        if (s4Kviewer->objectName().isEmpty())
            s4Kviewer->setObjectName(QStringLiteral("s4Kviewer"));
        s4Kviewer->resize(800, 600);
        actionOpen = new QAction(s4Kviewer);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionExit = new QAction(s4Kviewer);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionCallConsole = new QAction(s4Kviewer);
        actionCallConsole->setObjectName(QStringLiteral("actionCallConsole"));
        centralwidget = new QWidget(s4Kviewer);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        s4Kviewer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(s4Kviewer);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuconsole = new QMenu(menubar);
        menuconsole->setObjectName(QStringLiteral("menuconsole"));
        s4Kviewer->setMenuBar(menubar);
        statusbar = new QStatusBar(s4Kviewer);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        s4Kviewer->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuconsole->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionExit);
        menuconsole->addAction(actionCallConsole);

        retranslateUi(s4Kviewer);
        QObject::connect(actionExit, SIGNAL(triggered()), s4Kviewer, SLOT(close()));

        QMetaObject::connectSlotsByName(s4Kviewer);
    } // setupUi

    void retranslateUi(QMainWindow *s4Kviewer)
    {
        s4Kviewer->setWindowTitle(QApplication::translate("s4Kviewer", "MainWindow", nullptr));
        actionOpen->setText(QApplication::translate("s4Kviewer", "Open", nullptr));
        actionExit->setText(QApplication::translate("s4Kviewer", "Exit", nullptr));
        actionCallConsole->setText(QApplication::translate("s4Kviewer", "Call console", nullptr));
        menuFile->setTitle(QApplication::translate("s4Kviewer", "File", nullptr));
        menuconsole->setTitle(QApplication::translate("s4Kviewer", "console", nullptr));
    } // retranslateUi

};

namespace Ui {
    class s4Kviewer: public Ui_s4Kviewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_S4KVIEWER_H
