/********************************************************************************
** Form generated from reading UI file 'SeGuiDescriptionProperties.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUIDESCRIPTIONPROPERTIES_H
#define UI_SEGUIDESCRIPTIONPROPERTIES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiDescriptionPropertiesClass
{
public:

    void setupUi(QWidget *SeGuiDescriptionPropertiesClass)
    {
        if (SeGuiDescriptionPropertiesClass->objectName().isEmpty())
            SeGuiDescriptionPropertiesClass->setObjectName(QString::fromUtf8("SeGuiDescriptionPropertiesClass"));
        SeGuiDescriptionPropertiesClass->resize(400, 300);

        retranslateUi(SeGuiDescriptionPropertiesClass);

        QMetaObject::connectSlotsByName(SeGuiDescriptionPropertiesClass);
    } // setupUi

    void retranslateUi(QWidget *SeGuiDescriptionPropertiesClass)
    {
        SeGuiDescriptionPropertiesClass->setWindowTitle(QApplication::translate("SeGuiDescriptionPropertiesClass", "SeGuiDescriptionProperties", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiDescriptionPropertiesClass: public Ui_SeGuiDescriptionPropertiesClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUIDESCRIPTIONPROPERTIES_H
