/********************************************************************************
** Form generated from reading UI file 'app.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APP_H
#define UI_APP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *vboxLayout;
    QLabel *label_2;
    QHBoxLayout *hboxLayout;
    QSpinBox *inputA;
    QVBoxLayout *vboxLayout1;
    QPushButton *plus;
    QPushButton *divide;
    QPushButton *multiply;
    QPushButton *minus;
    QSpinBox *inputB;
    QLabel *label;
    QLineEdit *result;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(462, 299);
        vboxLayout = new QVBoxLayout(Form);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout->setObjectName(QStringLiteral("vboxLayout"));
        label_2 = new QLabel(Form);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setTextFormat(Qt::AutoText);
        label_2->setWordWrap(true);

        vboxLayout->addWidget(label_2);

        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        inputA = new QSpinBox(Form);
        inputA->setObjectName(QStringLiteral("inputA"));

        hboxLayout->addWidget(inputA);

        vboxLayout1 = new QVBoxLayout();
#ifndef Q_OS_MAC
        vboxLayout1->setSpacing(6);
#endif
        vboxLayout1->setContentsMargins(0, 0, 0, 0);
        vboxLayout1->setObjectName(QStringLiteral("vboxLayout1"));
        plus = new QPushButton(Form);
        plus->setObjectName(QStringLiteral("plus"));

        vboxLayout1->addWidget(plus);

        divide = new QPushButton(Form);
        divide->setObjectName(QStringLiteral("divide"));

        vboxLayout1->addWidget(divide);

        multiply = new QPushButton(Form);
        multiply->setObjectName(QStringLiteral("multiply"));

        vboxLayout1->addWidget(multiply);

        minus = new QPushButton(Form);
        minus->setObjectName(QStringLiteral("minus"));

        vboxLayout1->addWidget(minus);


        hboxLayout->addLayout(vboxLayout1);

        inputB = new QSpinBox(Form);
        inputB->setObjectName(QStringLiteral("inputB"));

        hboxLayout->addWidget(inputB);

        label = new QLabel(Form);
        label->setObjectName(QStringLiteral("label"));

        hboxLayout->addWidget(label);

        result = new QLineEdit(Form);
        result->setObjectName(QStringLiteral("result"));
        result->setReadOnly(true);

        hboxLayout->addWidget(result);


        vboxLayout->addLayout(hboxLayout);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Scriptable Calculator", nullptr));
        label_2->setText(QApplication::translate("Form", "Right click on a button to script it. The clicked() signal of each button is connected to a dynamic slot which executes the script. Setting the contents in the line edit is done by emitting a dynamic signal to the line edit's setText() slot. (Note that the scripts are evaluated from right to left and do not adhere to mathematical precedence.)", nullptr));
        plus->setText(QApplication::translate("Form", "+", nullptr));
        divide->setText(QApplication::translate("Form", "/", nullptr));
        multiply->setText(QApplication::translate("Form", "*", nullptr));
        minus->setText(QApplication::translate("Form", "-", nullptr));
        label->setText(QApplication::translate("Form", "=", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APP_H
