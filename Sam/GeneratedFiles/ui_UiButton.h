/********************************************************************************
** Form generated from reading UI file 'UiButton.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIBUTTON_H
#define UI_UIBUTTON_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <uiobject.h>

QT_BEGIN_NAMESPACE

class Ui_UiButtonClass
{
public:

    void setupUi(UiObject *UiButtonClass)
    {
        if (UiButtonClass->objectName().isEmpty())
            UiButtonClass->setObjectName(QString::fromUtf8("UiButtonClass"));
        UiButtonClass->resize(400, 300);

        retranslateUi(UiButtonClass);

        QMetaObject::connectSlotsByName(UiButtonClass);
    } // setupUi

    void retranslateUi(UiObject *UiButtonClass)
    {
        UiButtonClass->setWindowTitle(QApplication::translate("UiButtonClass", "UiButton", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiButtonClass: public Ui_UiButtonClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIBUTTON_H
