/********************************************************************************
** Form generated from reading UI file 'UiScrollBar.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UISCROLLBAR_H
#define UI_UISCROLLBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiScrollBarClass
{
public:

    void setupUi(QWidget *UiScrollBarClass)
    {
        if (UiScrollBarClass->objectName().isEmpty())
            UiScrollBarClass->setObjectName(QString::fromUtf8("UiScrollBarClass"));
        UiScrollBarClass->resize(400, 300);

        retranslateUi(UiScrollBarClass);

        QMetaObject::connectSlotsByName(UiScrollBarClass);
    } // setupUi

    void retranslateUi(QWidget *UiScrollBarClass)
    {
        UiScrollBarClass->setWindowTitle(QApplication::translate("UiScrollBarClass", "UiScrollBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiScrollBarClass: public Ui_UiScrollBarClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UISCROLLBAR_H
