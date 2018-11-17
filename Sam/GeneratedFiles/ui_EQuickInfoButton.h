/********************************************************************************
** Form generated from reading UI file 'EQuickInfoButton.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EQUICKINFOBUTTON_H
#define UI_EQUICKINFOBUTTON_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EQuickInfoButtonClass
{
public:

    void setupUi(QWidget *EQuickInfoButtonClass)
    {
        if (EQuickInfoButtonClass->objectName().isEmpty())
            EQuickInfoButtonClass->setObjectName(QString::fromUtf8("EQuickInfoButtonClass"));
        EQuickInfoButtonClass->resize(400, 300);

        retranslateUi(EQuickInfoButtonClass);

        QMetaObject::connectSlotsByName(EQuickInfoButtonClass);
    } // setupUi

    void retranslateUi(QWidget *EQuickInfoButtonClass)
    {
        EQuickInfoButtonClass->setWindowTitle(QApplication::translate("EQuickInfoButtonClass", "EQuickInfoButton", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EQuickInfoButtonClass: public Ui_EQuickInfoButtonClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EQUICKINFOBUTTON_H
