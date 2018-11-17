/********************************************************************************
** Form generated from reading UI file 'SamGuiSelectPreset.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUISELECTPRESET_H
#define UI_SAMGUISELECTPRESET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiSelectPresetClass
{
public:

    void setupUi(QWidget *SamGuiSelectPresetClass)
    {
        if (SamGuiSelectPresetClass->objectName().isEmpty())
            SamGuiSelectPresetClass->setObjectName(QString::fromUtf8("SamGuiSelectPresetClass"));
        SamGuiSelectPresetClass->resize(400, 300);

        retranslateUi(SamGuiSelectPresetClass);

        QMetaObject::connectSlotsByName(SamGuiSelectPresetClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiSelectPresetClass)
    {
        SamGuiSelectPresetClass->setWindowTitle(QApplication::translate("SamGuiSelectPresetClass", "SamGuiSelectPreset", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiSelectPresetClass: public Ui_SamGuiSelectPresetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUISELECTPRESET_H
