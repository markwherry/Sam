/********************************************************************************
** Form generated from reading UI file 'UiRect.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIRECT_H
#define UI_UIRECT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiRectClass
{
public:

    void setupUi(QWidget *UiRectClass)
    {
        if (UiRectClass->objectName().isEmpty())
            UiRectClass->setObjectName(QString::fromUtf8("UiRectClass"));
        UiRectClass->resize(400, 300);

        retranslateUi(UiRectClass);

        QMetaObject::connectSlotsByName(UiRectClass);
    } // setupUi

    void retranslateUi(QWidget *UiRectClass)
    {
        UiRectClass->setWindowTitle(QApplication::translate("UiRectClass", "UiRect", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiRectClass: public Ui_UiRectClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIRECT_H
