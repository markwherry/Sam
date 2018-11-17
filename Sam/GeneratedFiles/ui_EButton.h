/********************************************************************************
** Form generated from reading UI file 'EButton.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EBUTTON_H
#define UI_EBUTTON_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EButtonClass
{
public:

    void setupUi(QWidget *EButtonClass)
    {
        if (EButtonClass->objectName().isEmpty())
            EButtonClass->setObjectName(QString::fromUtf8("EButtonClass"));
        EButtonClass->resize(400, 300);

        retranslateUi(EButtonClass);

        QMetaObject::connectSlotsByName(EButtonClass);
    } // setupUi

    void retranslateUi(QWidget *EButtonClass)
    {
        EButtonClass->setWindowTitle(QApplication::translate("EButtonClass", "EButton", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EButtonClass: public Ui_EButtonClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EBUTTON_H
