/********************************************************************************
** Form generated from reading UI file 'SamGuiToolEditorItem.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUITOOLEDITORITEM_H
#define UI_SAMGUITOOLEDITORITEM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiToolEditorItemClass
{
public:

    void setupUi(QWidget *SamGuiToolEditorItemClass)
    {
        if (SamGuiToolEditorItemClass->objectName().isEmpty())
            SamGuiToolEditorItemClass->setObjectName(QString::fromUtf8("SamGuiToolEditorItemClass"));
        SamGuiToolEditorItemClass->resize(400, 300);

        retranslateUi(SamGuiToolEditorItemClass);

        QMetaObject::connectSlotsByName(SamGuiToolEditorItemClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiToolEditorItemClass)
    {
        SamGuiToolEditorItemClass->setWindowTitle(QApplication::translate("SamGuiToolEditorItemClass", "SamGuiToolEditorItem", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiToolEditorItemClass: public Ui_SamGuiToolEditorItemClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUITOOLEDITORITEM_H
