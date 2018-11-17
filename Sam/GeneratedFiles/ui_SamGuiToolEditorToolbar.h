/********************************************************************************
** Form generated from reading UI file 'SamGuiToolEditorToolbar.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUITOOLEDITORTOOLBAR_H
#define UI_SAMGUITOOLEDITORTOOLBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiToolEditorToolbarClass
{
public:

    void setupUi(QWidget *SamGuiToolEditorToolbarClass)
    {
        if (SamGuiToolEditorToolbarClass->objectName().isEmpty())
            SamGuiToolEditorToolbarClass->setObjectName(QString::fromUtf8("SamGuiToolEditorToolbarClass"));
        SamGuiToolEditorToolbarClass->resize(400, 300);

        retranslateUi(SamGuiToolEditorToolbarClass);

        QMetaObject::connectSlotsByName(SamGuiToolEditorToolbarClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiToolEditorToolbarClass)
    {
        SamGuiToolEditorToolbarClass->setWindowTitle(QApplication::translate("SamGuiToolEditorToolbarClass", "SamGuiToolEditorToolbar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiToolEditorToolbarClass: public Ui_SamGuiToolEditorToolbarClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUITOOLEDITORTOOLBAR_H
