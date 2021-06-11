/********************************************************************************
** Form generated from reading UI file 'ddwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DDWIDGET_H
#define UI_DDWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DDwidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QLabel *label;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *DDwidget)
    {
        if (DDwidget->objectName().isEmpty())
            DDwidget->setObjectName(QString::fromUtf8("DDwidget"));
        DDwidget->resize(481, 334);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DDwidget->sizePolicy().hasHeightForWidth());
        DDwidget->setSizePolicy(sizePolicy);
        DDwidget->setAcceptDrops(true);
        verticalLayout_2 = new QVBoxLayout(DDwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(29, 37, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer_2 = new QSpacerItem(13, 91, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        label = new QLabel(DDwidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(24);
        label->setFont(font);
        label->setScaledContents(true);
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        verticalSpacer = new QSpacerItem(13, 91, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);

        horizontalSpacer = new QSpacerItem(29, 37, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(DDwidget);

        QMetaObject::connectSlotsByName(DDwidget);
    } // setupUi

    void retranslateUi(QWidget *DDwidget)
    {
        DDwidget->setWindowTitle(QApplication::translate("DDwidget", "Form", nullptr));
        label->setText(QApplication::translate("DDwidget", "Drag And Drop here!", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DDwidget: public Ui_DDwidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DDWIDGET_H
