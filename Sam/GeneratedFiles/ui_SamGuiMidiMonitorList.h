/********************************************************************************
** Form generated from reading UI file 'SamGuiMidiMonitorList.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIMIDIMONITORLIST_H
#define UI_SAMGUIMIDIMONITORLIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiMidiMonitorListClass
{
public:

    void setupUi(QWidget *SamGuiMidiMonitorListClass)
    {
        if (SamGuiMidiMonitorListClass->objectName().isEmpty())
            SamGuiMidiMonitorListClass->setObjectName(QString::fromUtf8("SamGuiMidiMonitorListClass"));
        SamGuiMidiMonitorListClass->resize(400, 300);

        retranslateUi(SamGuiMidiMonitorListClass);

        QMetaObject::connectSlotsByName(SamGuiMidiMonitorListClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiMidiMonitorListClass)
    {
        SamGuiMidiMonitorListClass->setWindowTitle(QApplication::translate("SamGuiMidiMonitorListClass", "SamGuiMidiMonitorList", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiMidiMonitorListClass: public Ui_SamGuiMidiMonitorListClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIMIDIMONITORLIST_H
