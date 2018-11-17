/********************************************************************************
** Form generated from reading UI file 'UiStateButton.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UISTATEBUTTON_H
#define UI_UISTATEBUTTON_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiStateButtonClass
{
public:

    void setupUi(QWidget *UiStateButtonClass)
    {
        if (UiStateButtonClass->objectName().isEmpty())
            UiStateButtonClass->setObjectName(QString::fromUtf8("UiStateButtonClass"));
        UiStateButtonClass->resize(400, 300);

        retranslateUi(UiStateButtonClass);

        QMetaObject::connectSlotsByName(UiStateButtonClass);
    } // setupUi

    void retranslateUi(QWidget *UiStateButtonClass)
    {
        UiStateButtonClass->setWindowTitle(QApplication::translate("UiStateButtonClass", "UiStateButton", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiStateButtonClass: public Ui_UiStateButtonClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UISTATEBUTTON_H
