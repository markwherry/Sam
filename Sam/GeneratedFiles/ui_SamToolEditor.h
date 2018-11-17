/********************************************************************************
** Form generated from reading UI file 'SamToolEditor.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMTOOLEDITOR_H
#define UI_SAMTOOLEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamToolEditorClass
{
public:

    void setupUi(QWidget *SamToolEditorClass)
    {
        if (SamToolEditorClass->objectName().isEmpty())
            SamToolEditorClass->setObjectName(QString::fromUtf8("SamToolEditorClass"));
        SamToolEditorClass->resize(400, 300);

        retranslateUi(SamToolEditorClass);

        QMetaObject::connectSlotsByName(SamToolEditorClass);
    } // setupUi

    void retranslateUi(QWidget *SamToolEditorClass)
    {
        SamToolEditorClass->setWindowTitle(QApplication::translate("SamToolEditorClass", "SamToolEditor", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamToolEditorClass: public Ui_SamToolEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMTOOLEDITOR_H
