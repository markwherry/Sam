/********************************************************************************
** Form generated from reading UI file 'SamGuiSelectAudioOutputBus.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUISELECTAUDIOOUTPUTBUS_H
#define UI_SAMGUISELECTAUDIOOUTPUTBUS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiSelectAudioOutputBusClass
{
public:

    void setupUi(QWidget *SamGuiSelectAudioOutputBusClass)
    {
        if (SamGuiSelectAudioOutputBusClass->objectName().isEmpty())
            SamGuiSelectAudioOutputBusClass->setObjectName(QString::fromUtf8("SamGuiSelectAudioOutputBusClass"));
        SamGuiSelectAudioOutputBusClass->resize(400, 300);

        retranslateUi(SamGuiSelectAudioOutputBusClass);

        QMetaObject::connectSlotsByName(SamGuiSelectAudioOutputBusClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiSelectAudioOutputBusClass)
    {
        SamGuiSelectAudioOutputBusClass->setWindowTitle(QApplication::translate("SamGuiSelectAudioOutputBusClass", "SamGuiSelectAudioOutputBus", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiSelectAudioOutputBusClass: public Ui_SamGuiSelectAudioOutputBusClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUISELECTAUDIOOUTPUTBUS_H
