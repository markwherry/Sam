/********************************************************************************
** Form generated from reading UI file 'SamGuiBypassButton.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIBYPASSBUTTON_H
#define UI_SAMGUIBYPASSBUTTON_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiBypassButtonClass
{
public:

    void setupUi(QWidget *SamGuiBypassButtonClass)
    {
        if (SamGuiBypassButtonClass->objectName().isEmpty())
            SamGuiBypassButtonClass->setObjectName(QString::fromUtf8("SamGuiBypassButtonClass"));
        SamGuiBypassButtonClass->resize(400, 300);

        retranslateUi(SamGuiBypassButtonClass);

        QMetaObject::connectSlotsByName(SamGuiBypassButtonClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiBypassButtonClass)
    {
        SamGuiBypassButtonClass->setWindowTitle(QApplication::translate("SamGuiBypassButtonClass", "SamGuiBypassButton", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiBypassButtonClass: public Ui_SamGuiBypassButtonClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIBYPASSBUTTON_H
