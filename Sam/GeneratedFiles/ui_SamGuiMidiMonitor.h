/********************************************************************************
** Form generated from reading UI file 'SamGuiMidiMonitor.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIMIDIMONITOR_H
#define UI_SAMGUIMIDIMONITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiMidiMonitorClass
{
public:

    void setupUi(QWidget *SamGuiMidiMonitorClass)
    {
        if (SamGuiMidiMonitorClass->objectName().isEmpty())
            SamGuiMidiMonitorClass->setObjectName(QString::fromUtf8("SamGuiMidiMonitorClass"));
        SamGuiMidiMonitorClass->resize(400, 300);

        retranslateUi(SamGuiMidiMonitorClass);

        QMetaObject::connectSlotsByName(SamGuiMidiMonitorClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiMidiMonitorClass)
    {
        SamGuiMidiMonitorClass->setWindowTitle(QApplication::translate("SamGuiMidiMonitorClass", "SamGuiMidiMonitor", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiMidiMonitorClass: public Ui_SamGuiMidiMonitorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIMIDIMONITOR_H
