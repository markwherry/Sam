/********************************************************************************
** Form generated from reading UI file 'SamGuiPresetDialog.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIPRESETDIALOG_H
#define UI_SAMGUIPRESETDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiPresetDialogClass
{
public:

    void setupUi(QWidget *SamGuiPresetDialogClass)
    {
        if (SamGuiPresetDialogClass->objectName().isEmpty())
            SamGuiPresetDialogClass->setObjectName(QString::fromUtf8("SamGuiPresetDialogClass"));
        SamGuiPresetDialogClass->resize(400, 300);

        retranslateUi(SamGuiPresetDialogClass);

        QMetaObject::connectSlotsByName(SamGuiPresetDialogClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiPresetDialogClass)
    {
        SamGuiPresetDialogClass->setWindowTitle(QApplication::translate("SamGuiPresetDialogClass", "SamGuiPresetDialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiPresetDialogClass: public Ui_SamGuiPresetDialogClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIPRESETDIALOG_H
