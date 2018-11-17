/********************************************************************************
** Form generated from reading UI file 'UiTabBar.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UITABBAR_H
#define UI_UITABBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiTabBarClass
{
public:

    void setupUi(QWidget *UiTabBarClass)
    {
        if (UiTabBarClass->objectName().isEmpty())
            UiTabBarClass->setObjectName(QString::fromUtf8("UiTabBarClass"));
        UiTabBarClass->resize(400, 300);

        retranslateUi(UiTabBarClass);

        QMetaObject::connectSlotsByName(UiTabBarClass);
    } // setupUi

    void retranslateUi(QWidget *UiTabBarClass)
    {
        UiTabBarClass->setWindowTitle(QApplication::translate("UiTabBarClass", "UiTabBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiTabBarClass: public Ui_UiTabBarClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UITABBAR_H
