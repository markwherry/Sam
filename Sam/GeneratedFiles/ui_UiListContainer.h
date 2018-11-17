/********************************************************************************
** Form generated from reading UI file 'UiListContainer.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UILISTCONTAINER_H
#define UI_UILISTCONTAINER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiListContainerClass
{
public:

    void setupUi(QWidget *UiListContainerClass)
    {
        if (UiListContainerClass->objectName().isEmpty())
            UiListContainerClass->setObjectName(QString::fromUtf8("UiListContainerClass"));
        UiListContainerClass->resize(400, 300);

        retranslateUi(UiListContainerClass);

        QMetaObject::connectSlotsByName(UiListContainerClass);
    } // setupUi

    void retranslateUi(QWidget *UiListContainerClass)
    {
        UiListContainerClass->setWindowTitle(QApplication::translate("UiListContainerClass", "UiListContainer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiListContainerClass: public Ui_UiListContainerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UILISTCONTAINER_H
