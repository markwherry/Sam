/********************************************************************************
** Form generated from reading UI file 'SamGuiChannelInput.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUICHANNELINPUT_H
#define UI_SAMGUICHANNELINPUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiChannelInputClass
{
public:

    void setupUi(QWidget *SamGuiChannelInputClass)
    {
        if (SamGuiChannelInputClass->objectName().isEmpty())
            SamGuiChannelInputClass->setObjectName(QString::fromUtf8("SamGuiChannelInputClass"));
        SamGuiChannelInputClass->resize(400, 300);

        retranslateUi(SamGuiChannelInputClass);

        QMetaObject::connectSlotsByName(SamGuiChannelInputClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiChannelInputClass)
    {
        SamGuiChannelInputClass->setWindowTitle(QApplication::translate("SamGuiChannelInputClass", "SamGuiChannelInput", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiChannelInputClass: public Ui_SamGuiChannelInputClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUICHANNELINPUT_H
