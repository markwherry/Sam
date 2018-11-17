/********************************************************************************
** Form generated from reading UI file 'UiSection.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UISECTION_H
#define UI_UISECTION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiSectionClass
{
public:

    void setupUi(QWidget *UiSectionClass)
    {
        if (UiSectionClass->objectName().isEmpty())
            UiSectionClass->setObjectName(QString::fromUtf8("UiSectionClass"));
        UiSectionClass->resize(400, 300);

        retranslateUi(UiSectionClass);

        QMetaObject::connectSlotsByName(UiSectionClass);
    } // setupUi

    void retranslateUi(QWidget *UiSectionClass)
    {
        UiSectionClass->setWindowTitle(QApplication::translate("UiSectionClass", "UiSection", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiSectionClass: public Ui_UiSectionClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UISECTION_H
