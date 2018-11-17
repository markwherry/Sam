/********************************************************************************
** Form generated from reading UI file 'SamGuiMidiMonitorItem.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIMIDIMONITORITEM_H
#define UI_SAMGUIMIDIMONITORITEM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiMidiMonitorItemClass
{
public:

    void setupUi(QWidget *SamGuiMidiMonitorItemClass)
    {
        if (SamGuiMidiMonitorItemClass->objectName().isEmpty())
            SamGuiMidiMonitorItemClass->setObjectName(QString::fromUtf8("SamGuiMidiMonitorItemClass"));
        SamGuiMidiMonitorItemClass->resize(400, 300);

        retranslateUi(SamGuiMidiMonitorItemClass);

        QMetaObject::connectSlotsByName(SamGuiMidiMonitorItemClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiMidiMonitorItemClass)
    {
        SamGuiMidiMonitorItemClass->setWindowTitle(QApplication::translate("SamGuiMidiMonitorItemClass", "SamGuiMidiMonitorItem", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiMidiMonitorItemClass: public Ui_SamGuiMidiMonitorItemClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIMIDIMONITORITEM_H
