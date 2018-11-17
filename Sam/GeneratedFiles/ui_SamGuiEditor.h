/********************************************************************************
** Form generated from reading UI file 'SamGuiEditor.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIEDITOR_H
#define UI_SAMGUIEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiEditorClass
{
public:

    void setupUi(QWidget *SamGuiEditorClass)
    {
        if (SamGuiEditorClass->objectName().isEmpty())
            SamGuiEditorClass->setObjectName(QString::fromUtf8("SamGuiEditorClass"));
        SamGuiEditorClass->resize(400, 300);

        retranslateUi(SamGuiEditorClass);

        QMetaObject::connectSlotsByName(SamGuiEditorClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiEditorClass)
    {
        SamGuiEditorClass->setWindowTitle(QApplication::translate("SamGuiEditorClass", "SamGuiEditor", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiEditorClass: public Ui_SamGuiEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIEDITOR_H
