/********************************************************************************
** Form generated from reading UI file 'SeGuiItemCurve.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUIITEMCURVE_H
#define UI_SEGUIITEMCURVE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiItemCurveClass
{
public:

    void setupUi(QWidget *SeGuiItemCurveClass)
    {
        if (SeGuiItemCurveClass->objectName().isEmpty())
            SeGuiItemCurveClass->setObjectName(QString::fromUtf8("SeGuiItemCurveClass"));
        SeGuiItemCurveClass->resize(400, 300);

        retranslateUi(SeGuiItemCurveClass);

        QMetaObject::connectSlotsByName(SeGuiItemCurveClass);
    } // setupUi

    void retranslateUi(QWidget *SeGuiItemCurveClass)
    {
        SeGuiItemCurveClass->setWindowTitle(QApplication::translate("SeGuiItemCurveClass", "SeGuiItemCurve", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiItemCurveClass: public Ui_SeGuiItemCurveClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUIITEMCURVE_H
