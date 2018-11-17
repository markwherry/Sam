/********************************************************************************
** Form generated from reading UI file 'SamGuiChannel.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUICHANNEL_H
#define UI_SAMGUICHANNEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiChannelClass
{
public:

    void setupUi(QWidget *SamGuiChannelClass)
    {
        if (SamGuiChannelClass->objectName().isEmpty())
            SamGuiChannelClass->setObjectName(QString::fromUtf8("SamGuiChannelClass"));
        SamGuiChannelClass->resize(400, 300);

        retranslateUi(SamGuiChannelClass);

        QMetaObject::connectSlotsByName(SamGuiChannelClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiChannelClass)
    {
        SamGuiChannelClass->setWindowTitle(QApplication::translate("SamGuiChannelClass", "SamGuiChannel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiChannelClass: public Ui_SamGuiChannelClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUICHANNEL_H
