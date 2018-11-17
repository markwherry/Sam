/********************************************************************************
** Form generated from reading UI file 'SeGuiVersionProperties.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUIVERSIONPROPERTIES_H
#define UI_SEGUIVERSIONPROPERTIES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiVersionPropertiesClass
{
public:

    void setupUi(QWidget *SeGuiVersionPropertiesClass)
    {
        if (SeGuiVersionPropertiesClass->objectName().isEmpty())
            SeGuiVersionPropertiesClass->setObjectName(QString::fromUtf8("SeGuiVersionPropertiesClass"));
        SeGuiVersionPropertiesClass->resize(400, 300);

        retranslateUi(SeGuiVersionPropertiesClass);

        QMetaObject::connectSlotsByName(SeGuiVersionPropertiesClass);
    } // setupUi

    void retranslateUi(QWidget *SeGuiVersionPropertiesClass)
    {
        SeGuiVersionPropertiesClass->setWindowTitle(QApplication::translate("SeGuiVersionPropertiesClass", "SeGuiVersionProperties", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiVersionPropertiesClass: public Ui_SeGuiVersionPropertiesClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUIVERSIONPROPERTIES_H
