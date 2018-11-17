/********************************************************************************
** Form generated from reading UI file 'SamGuiToolEditor.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUITOOLEDITOR_H
#define UI_SAMGUITOOLEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiToolEditorClass
{
public:

    void setupUi(QWidget *SamGuiToolEditorClass)
    {
        if (SamGuiToolEditorClass->objectName().isEmpty())
            SamGuiToolEditorClass->setObjectName(QString::fromUtf8("SamGuiToolEditorClass"));
        SamGuiToolEditorClass->resize(400, 300);

        retranslateUi(SamGuiToolEditorClass);

        QMetaObject::connectSlotsByName(SamGuiToolEditorClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiToolEditorClass)
    {
        SamGuiToolEditorClass->setWindowTitle(QApplication::translate("SamGuiToolEditorClass", "SamGuiToolEditor", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiToolEditorClass: public Ui_SamGuiToolEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUITOOLEDITOR_H
