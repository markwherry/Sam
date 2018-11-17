/********************************************************************************
** Form generated from reading UI file 'SamGuiSelectMidiInputBus.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUISELECTMIDIINPUTBUS_H
#define UI_SAMGUISELECTMIDIINPUTBUS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiSelectMidiInputBusClass
{
public:

    void setupUi(QWidget *SamGuiSelectMidiInputBusClass)
    {
        if (SamGuiSelectMidiInputBusClass->objectName().isEmpty())
            SamGuiSelectMidiInputBusClass->setObjectName(QString::fromUtf8("SamGuiSelectMidiInputBusClass"));
        SamGuiSelectMidiInputBusClass->resize(400, 300);

        retranslateUi(SamGuiSelectMidiInputBusClass);

        QMetaObject::connectSlotsByName(SamGuiSelectMidiInputBusClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiSelectMidiInputBusClass)
    {
        SamGuiSelectMidiInputBusClass->setWindowTitle(QApplication::translate("SamGuiSelectMidiInputBusClass", "SamGuiSelectMidiInputBus", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiSelectMidiInputBusClass: public Ui_SamGuiSelectMidiInputBusClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUISELECTMIDIINPUTBUS_H
