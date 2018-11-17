/********************************************************************************
** Form generated from reading UI file 'SeGuiItemBool.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUIITEMBOOL_H
#define UI_SEGUIITEMBOOL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiItemBoolClass
{
public:

    void setupUi(QWidget *SeGuiItemBoolClass)
    {
        if (SeGuiItemBoolClass->objectName().isEmpty())
            SeGuiItemBoolClass->setObjectName(QString::fromUtf8("SeGuiItemBoolClass"));
        SeGuiItemBoolClass->resize(400, 300);

        retranslateUi(SeGuiItemBoolClass);

        QMetaObject::connectSlotsByName(SeGuiItemBoolClass);
    } // setupUi

    void retranslateUi(QWidget *SeGuiItemBoolClass)
    {
        SeGuiItemBoolClass->setWindowTitle(QApplication::translate("SeGuiItemBoolClass", "SeGuiItemBool", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiItemBoolClass: public Ui_SeGuiItemBoolClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUIITEMBOOL_H
