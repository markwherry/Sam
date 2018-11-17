/********************************************************************************
** Form generated from reading UI file 'UiFader.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIFADER_H
#define UI_UIFADER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiFaderClass
{
public:

    void setupUi(QWidget *UiFaderClass)
    {
        if (UiFaderClass->objectName().isEmpty())
            UiFaderClass->setObjectName(QString::fromUtf8("UiFaderClass"));
        UiFaderClass->resize(400, 300);

        retranslateUi(UiFaderClass);

        QMetaObject::connectSlotsByName(UiFaderClass);
    } // setupUi

    void retranslateUi(QWidget *UiFaderClass)
    {
        UiFaderClass->setWindowTitle(QApplication::translate("UiFaderClass", "UiFader", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiFaderClass: public Ui_UiFaderClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIFADER_H
