/********************************************************************************
** Form generated from reading UI file 'UiToolbar.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UITOOLBAR_H
#define UI_UITOOLBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiToolbarClass
{
public:

    void setupUi(QWidget *UiToolbarClass)
    {
        if (UiToolbarClass->objectName().isEmpty())
            UiToolbarClass->setObjectName(QString::fromUtf8("UiToolbarClass"));
        UiToolbarClass->resize(400, 300);

        retranslateUi(UiToolbarClass);

        QMetaObject::connectSlotsByName(UiToolbarClass);
    } // setupUi

    void retranslateUi(QWidget *UiToolbarClass)
    {
        UiToolbarClass->setWindowTitle(QApplication::translate("UiToolbarClass", "UiToolbar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiToolbarClass: public Ui_UiToolbarClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UITOOLBAR_H
