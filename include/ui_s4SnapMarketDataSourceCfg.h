/********************************************************************************
** Form generated from reading UI file 's4SnapMarketDataSourceCfg.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_S4SNAPMARKETDATASOURCECFG_H
#define UI_S4SNAPMARKETDATASOURCECFG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_snapMarketDataSourceCfg
{
public:
    QLabel *label_ip;
    QLineEdit *lineEdit_ip0;
    QLabel *label_port;
    QLineEdit *lineEdit_port;
    QLabel *label_type;
    QComboBox *comboBox_type;
    QLabel *label_name;
    QLineEdit *lineEdit_name;
    QLineEdit *lineEdit_ip1;
    QLineEdit *lineEdit_ip2;
    QLineEdit *lineEdit_ip3;
    QLabel *label_ip0;
    QLabel *label_ip1;
    QLabel *label_ip2;
    QPushButton *pushButton_edit;

    void setupUi(QWidget *snapMarketDataSourceCfg)
    {
        if (snapMarketDataSourceCfg->objectName().isEmpty())
            snapMarketDataSourceCfg->setObjectName(QStringLiteral("snapMarketDataSourceCfg"));
        snapMarketDataSourceCfg->resize(251, 158);
        label_ip = new QLabel(snapMarketDataSourceCfg);
        label_ip->setObjectName(QStringLiteral("label_ip"));
        label_ip->setGeometry(QRect(10, 40, 31, 20));
        lineEdit_ip0 = new QLineEdit(snapMarketDataSourceCfg);
        lineEdit_ip0->setObjectName(QStringLiteral("lineEdit_ip0"));
        lineEdit_ip0->setGeometry(QRect(50, 40, 41, 20));
        label_port = new QLabel(snapMarketDataSourceCfg);
        label_port->setObjectName(QStringLiteral("label_port"));
        label_port->setGeometry(QRect(10, 70, 54, 20));
        lineEdit_port = new QLineEdit(snapMarketDataSourceCfg);
        lineEdit_port->setObjectName(QStringLiteral("lineEdit_port"));
        lineEdit_port->setGeometry(QRect(50, 70, 91, 20));
        label_type = new QLabel(snapMarketDataSourceCfg);
        label_type->setObjectName(QStringLiteral("label_type"));
        label_type->setGeometry(QRect(10, 100, 54, 12));
        comboBox_type = new QComboBox(snapMarketDataSourceCfg);
        comboBox_type->setObjectName(QStringLiteral("comboBox_type"));
        comboBox_type->setGeometry(QRect(50, 100, 191, 22));
        label_name = new QLabel(snapMarketDataSourceCfg);
        label_name->setObjectName(QStringLiteral("label_name"));
        label_name->setGeometry(QRect(10, 10, 54, 20));
        lineEdit_name = new QLineEdit(snapMarketDataSourceCfg);
        lineEdit_name->setObjectName(QStringLiteral("lineEdit_name"));
        lineEdit_name->setGeometry(QRect(50, 10, 191, 20));
        lineEdit_ip1 = new QLineEdit(snapMarketDataSourceCfg);
        lineEdit_ip1->setObjectName(QStringLiteral("lineEdit_ip1"));
        lineEdit_ip1->setGeometry(QRect(100, 40, 41, 20));
        lineEdit_ip2 = new QLineEdit(snapMarketDataSourceCfg);
        lineEdit_ip2->setObjectName(QStringLiteral("lineEdit_ip2"));
        lineEdit_ip2->setGeometry(QRect(150, 40, 41, 20));
        lineEdit_ip3 = new QLineEdit(snapMarketDataSourceCfg);
        lineEdit_ip3->setObjectName(QStringLiteral("lineEdit_ip3"));
        lineEdit_ip3->setGeometry(QRect(200, 40, 41, 20));
        label_ip0 = new QLabel(snapMarketDataSourceCfg);
        label_ip0->setObjectName(QStringLiteral("label_ip0"));
        label_ip0->setGeometry(QRect(93, 40, 16, 20));
        label_ip1 = new QLabel(snapMarketDataSourceCfg);
        label_ip1->setObjectName(QStringLiteral("label_ip1"));
        label_ip1->setGeometry(QRect(143, 40, 16, 20));
        label_ip2 = new QLabel(snapMarketDataSourceCfg);
        label_ip2->setObjectName(QStringLiteral("label_ip2"));
        label_ip2->setGeometry(QRect(193, 40, 16, 20));
        pushButton_edit = new QPushButton(snapMarketDataSourceCfg);
        pushButton_edit->setObjectName(QStringLiteral("pushButton_edit"));
        pushButton_edit->setGeometry(QRect(170, 130, 75, 23));

        retranslateUi(snapMarketDataSourceCfg);

        QMetaObject::connectSlotsByName(snapMarketDataSourceCfg);
    } // setupUi

    void retranslateUi(QWidget *snapMarketDataSourceCfg)
    {
        snapMarketDataSourceCfg->setWindowTitle(QApplication::translate("snapMarketDataSourceCfg", "snapMarketDataSourceCfg", nullptr));
        label_ip->setText(QApplication::translate("snapMarketDataSourceCfg", "IP:", nullptr));
        label_port->setText(QApplication::translate("snapMarketDataSourceCfg", "Port", nullptr));
        label_type->setText(QApplication::translate("snapMarketDataSourceCfg", "Type", nullptr));
        label_name->setText(QApplication::translate("snapMarketDataSourceCfg", "Name", nullptr));
        label_ip0->setText(QApplication::translate("snapMarketDataSourceCfg", ".", nullptr));
        label_ip1->setText(QApplication::translate("snapMarketDataSourceCfg", ".", nullptr));
        label_ip2->setText(QApplication::translate("snapMarketDataSourceCfg", ".", nullptr));
        pushButton_edit->setText(QApplication::translate("snapMarketDataSourceCfg", "\345\256\214\346\210\220", nullptr));
    } // retranslateUi

};

namespace Ui {
    class snapMarketDataSourceCfg: public Ui_snapMarketDataSourceCfg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_S4SNAPMARKETDATASOURCECFG_H
