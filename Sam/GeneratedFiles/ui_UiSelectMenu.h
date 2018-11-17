/********************************************************************************
** Form generated from reading UI file 'UiSelectMenu.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UISELECTMENU_H
#define UI_UISELECTMENU_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiSelectMenuClass
{
public:

    void setupUi(QWidget *UiSelectMenuClass)
    {
        if (UiSelectMenuClass->objectName().isEmpty())
            UiSelectMenuClass->setObjectName(QString::fromUtf8("UiSelectMenuClass"));
        UiSelectMenuClass->resize(400, 300);

        retranslateUi(UiSelectMenuClass);

        QMetaObject::connectSlotsByName(UiSelectMenuClass);
    } // setupUi

    void retranslateUi(QWidget *UiSelectMenuClass)
    {
        UiSelectMenuClass->setWindowTitle(QApplication::translate("UiSelectMenuClass", "UiSelectMenu", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiSelectMenuClass: public Ui_UiSelectMenuClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UISELECTMENU_H
