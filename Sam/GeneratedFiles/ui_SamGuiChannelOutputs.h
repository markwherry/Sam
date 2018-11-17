/********************************************************************************
** Form generated from reading UI file 'SamGuiChannelOutputs.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUICHANNELOUTPUTS_H
#define UI_SAMGUICHANNELOUTPUTS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiChannelOutputsClass
{
public:

    void setupUi(QWidget *SamGuiChannelOutputsClass)
    {
        if (SamGuiChannelOutputsClass->objectName().isEmpty())
            SamGuiChannelOutputsClass->setObjectName(QString::fromUtf8("SamGuiChannelOutputsClass"));
        SamGuiChannelOutputsClass->resize(400, 300);

        retranslateUi(SamGuiChannelOutputsClass);

        QMetaObject::connectSlotsByName(SamGuiChannelOutputsClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiChannelOutputsClass)
    {
        SamGuiChannelOutputsClass->setWindowTitle(QApplication::translate("SamGuiChannelOutputsClass", "SamGuiChannelOutputs", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiChannelOutputsClass: public Ui_SamGuiChannelOutputsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUICHANNELOUTPUTS_H
