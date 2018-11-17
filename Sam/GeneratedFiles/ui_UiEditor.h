/********************************************************************************
** Form generated from reading UI file 'UiEditor.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIEDITOR_H
#define UI_UIEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiEditorClass
{
public:

    void setupUi(QWidget *UiEditorClass)
    {
        if (UiEditorClass->objectName().isEmpty())
            UiEditorClass->setObjectName(QString::fromUtf8("UiEditorClass"));
        UiEditorClass->resize(400, 300);

        retranslateUi(UiEditorClass);

        QMetaObject::connectSlotsByName(UiEditorClass);
    } // setupUi

    void retranslateUi(QWidget *UiEditorClass)
    {
        UiEditorClass->setWindowTitle(QApplication::translate("UiEditorClass", "UiEditor", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiEditorClass: public Ui_UiEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIEDITOR_H
