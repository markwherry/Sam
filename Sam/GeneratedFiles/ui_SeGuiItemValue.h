/********************************************************************************
** Form generated from reading UI file 'SeGuiItemValue.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUIITEMVALUE_H
#define UI_SEGUIITEMVALUE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiItemValueClass
{
public:

    void setupUi(QWidget *SeGuiItemValueClass)
    {
        if (SeGuiItemValueClass->objectName().isEmpty())
            SeGuiItemValueClass->setObjectName(QString::fromUtf8("SeGuiItemValueClass"));
        SeGuiItemValueClass->resize(400, 300);

        retranslateUi(SeGuiItemValueClass);

        QMetaObject::connectSlotsByName(SeGuiItemValueClass);
    } // setupUi

    void retranslateUi(QWidget *SeGuiItemValueClass)
    {
        SeGuiItemValueClass->setWindowTitle(QApplication::translate("SeGuiItemValueClass", "SeGuiItemValue", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiItemValueClass: public Ui_SeGuiItemValueClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUIITEMVALUE_H
