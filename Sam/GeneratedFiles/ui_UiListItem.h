/********************************************************************************
** Form generated from reading UI file 'UiListItem.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UILISTITEM_H
#define UI_UILISTITEM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiListItemClass
{
public:

    void setupUi(QWidget *UiListItemClass)
    {
        if (UiListItemClass->objectName().isEmpty())
            UiListItemClass->setObjectName(QString::fromUtf8("UiListItemClass"));
        UiListItemClass->resize(400, 300);

        retranslateUi(UiListItemClass);

        QMetaObject::connectSlotsByName(UiListItemClass);
    } // setupUi

    void retranslateUi(QWidget *UiListItemClass)
    {
        UiListItemClass->setWindowTitle(QApplication::translate("UiListItemClass", "UiListItem", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiListItemClass: public Ui_UiListItemClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UILISTITEM_H
