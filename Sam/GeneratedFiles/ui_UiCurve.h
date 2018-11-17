/********************************************************************************
** Form generated from reading UI file 'UiCurve.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UICURVE_H
#define UI_UICURVE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiCurveClass
{
public:

    void setupUi(QWidget *UiCurveClass)
    {
        if (UiCurveClass->objectName().isEmpty())
            UiCurveClass->setObjectName(QString::fromUtf8("UiCurveClass"));
        UiCurveClass->resize(400, 300);

        retranslateUi(UiCurveClass);

        QMetaObject::connectSlotsByName(UiCurveClass);
    } // setupUi

    void retranslateUi(QWidget *UiCurveClass)
    {
        UiCurveClass->setWindowTitle(QApplication::translate("UiCurveClass", "UiCurve", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiCurveClass: public Ui_UiCurveClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UICURVE_H
