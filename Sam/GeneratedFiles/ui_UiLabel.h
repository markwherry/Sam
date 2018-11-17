/********************************************************************************
** Form generated from reading UI file 'UiLabel.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UILABEL_H
#define UI_UILABEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiLabelClass
{
public:

    void setupUi(QWidget *UiLabelClass)
    {
        if (UiLabelClass->objectName().isEmpty())
            UiLabelClass->setObjectName(QString::fromUtf8("UiLabelClass"));
        UiLabelClass->resize(400, 300);

        retranslateUi(UiLabelClass);

        QMetaObject::connectSlotsByName(UiLabelClass);
    } // setupUi

    void retranslateUi(QWidget *UiLabelClass)
    {
        UiLabelClass->setWindowTitle(QApplication::translate("UiLabelClass", "UiLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiLabelClass: public Ui_UiLabelClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UILABEL_H
