/********************************************************************************
** Form generated from reading UI file 'SeGuiCellItem.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUICELLITEM_H
#define UI_SEGUICELLITEM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiCellItemClass
{
public:

    void setupUi(QWidget *SeGuiCellItemClass)
    {
        if (SeGuiCellItemClass->objectName().isEmpty())
            SeGuiCellItemClass->setObjectName(QString::fromUtf8("SeGuiCellItemClass"));
        SeGuiCellItemClass->resize(400, 300);

        retranslateUi(SeGuiCellItemClass);

        QMetaObject::connectSlotsByName(SeGuiCellItemClass);
    } // setupUi

    void retranslateUi(QWidget *SeGuiCellItemClass)
    {
        SeGuiCellItemClass->setWindowTitle(QApplication::translate("SeGuiCellItemClass", "SeGuiCellItem", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiCellItemClass: public Ui_SeGuiCellItemClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUICELLITEM_H
