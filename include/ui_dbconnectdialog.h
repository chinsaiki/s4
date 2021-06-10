/********************************************************************************
** Form generated from reading UI file 'dbconnectdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DBCONNECTDIALOG_H
#define UI_DBCONNECTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DbConnectDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox;
    QSpacerItem *horizontalSpacer;
    QSplitter *splitter;
    QLabel *label_2;
    QVBoxLayout *verticalLayout;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DbConnectDialog)
    {
        if (DbConnectDialog->objectName().isEmpty())
            DbConnectDialog->setObjectName(QStringLiteral("DbConnectDialog"));
        DbConnectDialog->resize(400, 431);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DbConnectDialog->sizePolicy().hasHeightForWidth());
        DbConnectDialog->setSizePolicy(sizePolicy);
        DbConnectDialog->setModal(true);
        verticalLayout_2 = new QVBoxLayout(DbConnectDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(DbConnectDialog);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(label);

        comboBox = new QComboBox(DbConnectDialog);
        comboBox->addItem(QString());
        comboBox->setObjectName(QStringLiteral("comboBox"));
        sizePolicy1.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(comboBox);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalSpacer = new QSpacerItem(379, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(horizontalSpacer);

        splitter = new QSplitter(DbConnectDialog);
        splitter->setObjectName(QStringLiteral("splitter"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy2);
        splitter->setOrientation(Qt::Horizontal);
        label_2 = new QLabel(splitter);
        label_2->setObjectName(QStringLiteral("label_2"));
        splitter->addWidget(label_2);

        verticalLayout_2->addWidget(splitter);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        buttonBox = new QDialogButtonBox(DbConnectDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(buttonBox->sizePolicy().hasHeightForWidth());
        buttonBox->setSizePolicy(sizePolicy3);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(DbConnectDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), DbConnectDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DbConnectDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(DbConnectDialog);
    } // setupUi

    void retranslateUi(QDialog *DbConnectDialog)
    {
        DbConnectDialog->setWindowTitle(QApplication::translate("DbConnectDialog", "Open DB...", nullptr));
        label->setText(QApplication::translate("DbConnectDialog", "Choose DB type", nullptr));
        comboBox->setItemText(0, QApplication::translate("DbConnectDialog", "sqlite3", nullptr));

        label_2->setText(QApplication::translate("DbConnectDialog", "Database file:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DbConnectDialog: public Ui_DbConnectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DBCONNECTDIALOG_H
