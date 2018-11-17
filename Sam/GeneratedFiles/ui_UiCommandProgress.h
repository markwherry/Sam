/********************************************************************************
** Form generated from reading UI file 'UiCommandProgress.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UICOMMANDPROGRESS_H
#define UI_UICOMMANDPROGRESS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiCommandProgressClass
{
public:

    void setupUi(QWidget *UiCommandProgressClass)
    {
        if (UiCommandProgressClass->objectName().isEmpty())
            UiCommandProgressClass->setObjectName(QString::fromUtf8("UiCommandProgressClass"));
        UiCommandProgressClass->resize(400, 300);

        retranslateUi(UiCommandProgressClass);

        QMetaObject::connectSlotsByName(UiCommandProgressClass);
    } // setupUi

    void retranslateUi(QWidget *UiCommandProgressClass)
    {
        UiCommandProgressClass->setWindowTitle(QApplication::translate("UiCommandProgressClass", "UiCommandProgress", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiCommandProgressClass: public Ui_UiCommandProgressClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UICOMMANDPROGRESS_H
