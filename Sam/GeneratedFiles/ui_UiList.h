/********************************************************************************
** Form generated from reading UI file 'UiList.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UILIST_H
#define UI_UILIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiListClass
{
public:

    void setupUi(QWidget *UiListClass)
    {
        if (UiListClass->objectName().isEmpty())
            UiListClass->setObjectName(QString::fromUtf8("UiListClass"));
        UiListClass->resize(400, 300);

        retranslateUi(UiListClass);

        QMetaObject::connectSlotsByName(UiListClass);
    } // setupUi

    void retranslateUi(QWidget *UiListClass)
    {
        UiListClass->setWindowTitle(QApplication::translate("UiListClass", "UiList", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiListClass: public Ui_UiListClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UILIST_H
