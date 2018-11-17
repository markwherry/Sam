/********************************************************************************
** Form generated from reading UI file 'UiClickMenu.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UICLICKMENU_H
#define UI_UICLICKMENU_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiClickMenuClass
{
public:

    void setupUi(QWidget *UiClickMenuClass)
    {
        if (UiClickMenuClass->objectName().isEmpty())
            UiClickMenuClass->setObjectName(QString::fromUtf8("UiClickMenuClass"));
        UiClickMenuClass->resize(400, 300);

        retranslateUi(UiClickMenuClass);

        QMetaObject::connectSlotsByName(UiClickMenuClass);
    } // setupUi

    void retranslateUi(QWidget *UiClickMenuClass)
    {
        UiClickMenuClass->setWindowTitle(QApplication::translate("UiClickMenuClass", "UiClickMenu", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiClickMenuClass: public Ui_UiClickMenuClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UICLICKMENU_H
