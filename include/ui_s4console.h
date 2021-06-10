/********************************************************************************
** Form generated from reading UI file 's4console.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_S4CONSOLE_H
#define UI_S4CONSOLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_s4console
{
public:
    QVBoxLayout *verticalLayout;

    void setupUi(QDialog *s4console)
    {
        if (s4console->objectName().isEmpty())
            s4console->setObjectName(QStringLiteral("s4console"));
        s4console->resize(640, 480);
        verticalLayout = new QVBoxLayout(s4console);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

        retranslateUi(s4console);

        QMetaObject::connectSlotsByName(s4console);
    } // setupUi

    void retranslateUi(QDialog *s4console)
    {
        s4console->setWindowTitle(QApplication::translate("s4console", "s4console", nullptr));
    } // retranslateUi

};

namespace Ui {
    class s4console: public Ui_s4console {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_S4CONSOLE_H
