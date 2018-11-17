/********************************************************************************
** Form generated from reading UI file 'UiCheckBox.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UICHECKBOX_H
#define UI_UICHECKBOX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiCheckBoxClass
{
public:

    void setupUi(QWidget *UiCheckBoxClass)
    {
        if (UiCheckBoxClass->objectName().isEmpty())
            UiCheckBoxClass->setObjectName(QString::fromUtf8("UiCheckBoxClass"));
        UiCheckBoxClass->resize(400, 300);

        retranslateUi(UiCheckBoxClass);

        QMetaObject::connectSlotsByName(UiCheckBoxClass);
    } // setupUi

    void retranslateUi(QWidget *UiCheckBoxClass)
    {
        UiCheckBoxClass->setWindowTitle(QApplication::translate("UiCheckBoxClass", "UiCheckBox", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiCheckBoxClass: public Ui_UiCheckBoxClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UICHECKBOX_H
