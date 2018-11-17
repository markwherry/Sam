/********************************************************************************
** Form generated from reading UI file 'SeGuiCellProperties.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUICELLPROPERTIES_H
#define UI_SEGUICELLPROPERTIES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiCellPropertiesClass
{
public:

    void setupUi(QWidget *SeGuiCellPropertiesClass)
    {
        if (SeGuiCellPropertiesClass->objectName().isEmpty())
            SeGuiCellPropertiesClass->setObjectName(QString::fromUtf8("SeGuiCellPropertiesClass"));
        SeGuiCellPropertiesClass->resize(400, 300);

        retranslateUi(SeGuiCellPropertiesClass);

        QMetaObject::connectSlotsByName(SeGuiCellPropertiesClass);
    } // setupUi

    void retranslateUi(QWidget *SeGuiCellPropertiesClass)
    {
        SeGuiCellPropertiesClass->setWindowTitle(QApplication::translate("SeGuiCellPropertiesClass", "SeGuiCellProperties", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiCellPropertiesClass: public Ui_SeGuiCellPropertiesClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUICELLPROPERTIES_H
