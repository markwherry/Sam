/********************************************************************************
** Form generated from reading UI file 'UiValue.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIVALUE_H
#define UI_UIVALUE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiValueClass
{
public:

    void setupUi(QWidget *UiValueClass)
    {
        if (UiValueClass->objectName().isEmpty())
            UiValueClass->setObjectName(QString::fromUtf8("UiValueClass"));
        UiValueClass->resize(400, 300);

        retranslateUi(UiValueClass);

        QMetaObject::connectSlotsByName(UiValueClass);
    } // setupUi

    void retranslateUi(QWidget *UiValueClass)
    {
        UiValueClass->setWindowTitle(QApplication::translate("UiValueClass", "UiValue", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiValueClass: public Ui_UiValueClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIVALUE_H
