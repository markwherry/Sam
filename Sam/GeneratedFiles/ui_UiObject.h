/********************************************************************************
** Form generated from reading UI file 'UiObject.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIOBJECT_H
#define UI_UIOBJECT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiObjectClass
{
public:

    void setupUi(QWidget *UiObjectClass)
    {
        if (UiObjectClass->objectName().isEmpty())
            UiObjectClass->setObjectName(QString::fromUtf8("UiObjectClass"));
        UiObjectClass->resize(400, 300);

        retranslateUi(UiObjectClass);

        QMetaObject::connectSlotsByName(UiObjectClass);
    } // setupUi

    void retranslateUi(QWidget *UiObjectClass)
    {
        UiObjectClass->setWindowTitle(QApplication::translate("UiObjectClass", "UiObject", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiObjectClass: public Ui_UiObjectClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIOBJECT_H
