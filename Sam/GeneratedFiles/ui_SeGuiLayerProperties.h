/********************************************************************************
** Form generated from reading UI file 'SeGuiLayerProperties.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUILAYERPROPERTIES_H
#define UI_SEGUILAYERPROPERTIES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiLayerPropertiesClass
{
public:

    void setupUi(QWidget *SeGuiLayerPropertiesClass)
    {
        if (SeGuiLayerPropertiesClass->objectName().isEmpty())
            SeGuiLayerPropertiesClass->setObjectName(QString::fromUtf8("SeGuiLayerPropertiesClass"));
        SeGuiLayerPropertiesClass->resize(400, 300);

        retranslateUi(SeGuiLayerPropertiesClass);

        QMetaObject::connectSlotsByName(SeGuiLayerPropertiesClass);
    } // setupUi

    void retranslateUi(QWidget *SeGuiLayerPropertiesClass)
    {
        SeGuiLayerPropertiesClass->setWindowTitle(QApplication::translate("SeGuiLayerPropertiesClass", "SeGuiLayerProperties", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiLayerPropertiesClass: public Ui_SeGuiLayerPropertiesClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUILAYERPROPERTIES_H
