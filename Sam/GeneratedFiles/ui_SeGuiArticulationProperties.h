/********************************************************************************
** Form generated from reading UI file 'SeGuiArticulationProperties.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUIARTICULATIONPROPERTIES_H
#define UI_SEGUIARTICULATIONPROPERTIES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiArticulationPropertiesClass
{
public:

    void setupUi(QWidget *SeGuiArticulationPropertiesClass)
    {
        if (SeGuiArticulationPropertiesClass->objectName().isEmpty())
            SeGuiArticulationPropertiesClass->setObjectName(QString::fromUtf8("SeGuiArticulationPropertiesClass"));
        SeGuiArticulationPropertiesClass->resize(400, 300);

        retranslateUi(SeGuiArticulationPropertiesClass);

        QMetaObject::connectSlotsByName(SeGuiArticulationPropertiesClass);
    } // setupUi

    void retranslateUi(QWidget *SeGuiArticulationPropertiesClass)
    {
        SeGuiArticulationPropertiesClass->setWindowTitle(QApplication::translate("SeGuiArticulationPropertiesClass", "SeGuiArticulationProperties", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiArticulationPropertiesClass: public Ui_SeGuiArticulationPropertiesClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUIARTICULATIONPROPERTIES_H
