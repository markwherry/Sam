/********************************************************************************
** Form generated from reading UI file 'SamGuiStepRepeater.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUISTEPREPEATER_H
#define UI_SAMGUISTEPREPEATER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiStepRepeaterClass
{
public:

    void setupUi(QWidget *SamGuiStepRepeaterClass)
    {
        if (SamGuiStepRepeaterClass->objectName().isEmpty())
            SamGuiStepRepeaterClass->setObjectName(QString::fromUtf8("SamGuiStepRepeaterClass"));
        SamGuiStepRepeaterClass->resize(400, 300);

        retranslateUi(SamGuiStepRepeaterClass);

        QMetaObject::connectSlotsByName(SamGuiStepRepeaterClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiStepRepeaterClass)
    {
        SamGuiStepRepeaterClass->setWindowTitle(QApplication::translate("SamGuiStepRepeaterClass", "SamGuiStepRepeater", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiStepRepeaterClass: public Ui_SamGuiStepRepeaterClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUISTEPREPEATER_H
