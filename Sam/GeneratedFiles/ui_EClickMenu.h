/********************************************************************************
** Form generated from reading UI file 'EClickMenu.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ECLICKMENU_H
#define UI_ECLICKMENU_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EClickMenuClass
{
public:

    void setupUi(QWidget *EClickMenuClass)
    {
        if (EClickMenuClass->objectName().isEmpty())
            EClickMenuClass->setObjectName(QString::fromUtf8("EClickMenuClass"));
        EClickMenuClass->resize(400, 300);

        retranslateUi(EClickMenuClass);

        QMetaObject::connectSlotsByName(EClickMenuClass);
    } // setupUi

    void retranslateUi(QWidget *EClickMenuClass)
    {
        EClickMenuClass->setWindowTitle(QApplication::translate("EClickMenuClass", "EClickMenu", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EClickMenuClass: public Ui_EClickMenuClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ECLICKMENU_H
