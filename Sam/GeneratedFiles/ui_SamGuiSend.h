/********************************************************************************
** Form generated from reading UI file 'SamGuiSend.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUISEND_H
#define UI_SAMGUISEND_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiSendClass
{
public:

    void setupUi(QWidget *SamGuiSendClass)
    {
        if (SamGuiSendClass->objectName().isEmpty())
            SamGuiSendClass->setObjectName(QString::fromUtf8("SamGuiSendClass"));
        SamGuiSendClass->resize(400, 300);

        retranslateUi(SamGuiSendClass);

        QMetaObject::connectSlotsByName(SamGuiSendClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiSendClass)
    {
        SamGuiSendClass->setWindowTitle(QApplication::translate("SamGuiSendClass", "SamGuiSend", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiSendClass: public Ui_SamGuiSendClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUISEND_H
