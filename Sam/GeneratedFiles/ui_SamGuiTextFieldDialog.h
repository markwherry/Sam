/********************************************************************************
** Form generated from reading UI file 'SamGuiTextFieldDialog.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUITEXTFIELDDIALOG_H
#define UI_SAMGUITEXTFIELDDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiTextFieldDialogClass
{
public:

    void setupUi(QWidget *SamGuiTextFieldDialogClass)
    {
        if (SamGuiTextFieldDialogClass->objectName().isEmpty())
            SamGuiTextFieldDialogClass->setObjectName(QString::fromUtf8("SamGuiTextFieldDialogClass"));
        SamGuiTextFieldDialogClass->resize(400, 300);

        retranslateUi(SamGuiTextFieldDialogClass);

        QMetaObject::connectSlotsByName(SamGuiTextFieldDialogClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiTextFieldDialogClass)
    {
        SamGuiTextFieldDialogClass->setWindowTitle(QApplication::translate("SamGuiTextFieldDialogClass", "SamGuiTextFieldDialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiTextFieldDialogClass: public Ui_SamGuiTextFieldDialogClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUITEXTFIELDDIALOG_H
