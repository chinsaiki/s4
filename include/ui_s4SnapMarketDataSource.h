/********************************************************************************
** Form generated from reading UI file 's4SnapMarketDataSource.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_S4SNAPMARKETDATASOURCE_H
#define UI_S4SNAPMARKETDATASOURCE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_snapMarketDataSource
{
public:
    QPushButton *StartButton;
    QPushButton *AddButton;
    QPushButton *ImportButton;
    QPushButton *ExportButton;
    QTabWidget *tabWidget;

    void setupUi(QWidget *snapMarketDataSource)
    {
        if (snapMarketDataSource->objectName().isEmpty())
            snapMarketDataSource->setObjectName(QString::fromUtf8("snapMarketDataSource"));
        snapMarketDataSource->resize(400, 300);
        StartButton = new QPushButton(snapMarketDataSource);
        StartButton->setObjectName(QString::fromUtf8("StartButton"));
        StartButton->setGeometry(QRect(0, 0, 50, 23));
        AddButton = new QPushButton(snapMarketDataSource);
        AddButton->setObjectName(QString::fromUtf8("AddButton"));
        AddButton->setGeometry(QRect(55, 0, 50, 23));
        ImportButton = new QPushButton(snapMarketDataSource);
        ImportButton->setObjectName(QString::fromUtf8("ImportButton"));
        ImportButton->setGeometry(QRect(110, 0, 50, 23));
        ExportButton = new QPushButton(snapMarketDataSource);
        ExportButton->setObjectName(QString::fromUtf8("ExportButton"));
        ExportButton->setGeometry(QRect(165, 0, 50, 23));
        tabWidget = new QTabWidget(snapMarketDataSource);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 30, 401, 261));

        retranslateUi(snapMarketDataSource);

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(snapMarketDataSource);
    } // setupUi

    void retranslateUi(QWidget *snapMarketDataSource)
    {
        snapMarketDataSource->setWindowTitle(QApplication::translate("snapMarketDataSource", "snapMarketDataSource", nullptr));
        StartButton->setText(QApplication::translate("snapMarketDataSource", "Start", nullptr));
        AddButton->setText(QApplication::translate("snapMarketDataSource", "Add", nullptr));
        ImportButton->setText(QApplication::translate("snapMarketDataSource", "Import", nullptr));
        ExportButton->setText(QApplication::translate("snapMarketDataSource", "Export", nullptr));
    } // retranslateUi

};

namespace Ui {
    class snapMarketDataSource: public Ui_snapMarketDataSource {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_S4SNAPMARKETDATASOURCE_H
