/********************************************************************************
** Form generated from reading UI file 'SamGuiCsToolEditor.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUICSTOOLEDITOR_H
#define UI_SAMGUICSTOOLEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiCsToolEditorClass
{
public:

    void setupUi(QWidget *SamGuiCsToolEditorClass)
    {
        if (SamGuiCsToolEditorClass->objectName().isEmpty())
            SamGuiCsToolEditorClass->setObjectName(QString::fromUtf8("SamGuiCsToolEditorClass"));
        SamGuiCsToolEditorClass->resize(400, 300);

        retranslateUi(SamGuiCsToolEditorClass);

        QMetaObject::connectSlotsByName(SamGuiCsToolEditorClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiCsToolEditorClass)
    {
        SamGuiCsToolEditorClass->setWindowTitle(QApplication::translate("SamGuiCsToolEditorClass", "SamGuiCsToolEditor", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiCsToolEditorClass: public Ui_SamGuiCsToolEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUICSTOOLEDITOR_H
