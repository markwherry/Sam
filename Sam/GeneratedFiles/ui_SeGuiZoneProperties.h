/********************************************************************************
** Form generated from reading UI file 'SeGuiZoneProperties.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUIZONEPROPERTIES_H
#define UI_SEGUIZONEPROPERTIES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiZonePropertiesClass
{
public:

    void setupUi(QWidget *SeGuiZonePropertiesClass)
    {
        if (SeGuiZonePropertiesClass->objectName().isEmpty())
            SeGuiZonePropertiesClass->setObjectName(QString::fromUtf8("SeGuiZonePropertiesClass"));
        SeGuiZonePropertiesClass->resize(400, 300);

        retranslateUi(SeGuiZonePropertiesClass);

        QMetaObject::connectSlotsByName(SeGuiZonePropertiesClass);
    } // setupUi

    void retranslateUi(QWidget *SeGuiZonePropertiesClass)
    {
        SeGuiZonePropertiesClass->setWindowTitle(QApplication::translate("SeGuiZonePropertiesClass", "SeGuiZoneProperties", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiZonePropertiesClass: public Ui_SeGuiZonePropertiesClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUIZONEPROPERTIES_H
