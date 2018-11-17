/********************************************************************************
** Form generated from reading UI file 'SamGuiOutputConfigSection.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIOUTPUTCONFIGSECTION_H
#define UI_SAMGUIOUTPUTCONFIGSECTION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiOutputConfigSectionClass
{
public:

    void setupUi(QWidget *SamGuiOutputConfigSectionClass)
    {
        if (SamGuiOutputConfigSectionClass->objectName().isEmpty())
            SamGuiOutputConfigSectionClass->setObjectName(QString::fromUtf8("SamGuiOutputConfigSectionClass"));
        SamGuiOutputConfigSectionClass->resize(400, 300);

        retranslateUi(SamGuiOutputConfigSectionClass);

        QMetaObject::connectSlotsByName(SamGuiOutputConfigSectionClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiOutputConfigSectionClass)
    {
        SamGuiOutputConfigSectionClass->setWindowTitle(QApplication::translate("SamGuiOutputConfigSectionClass", "SamGuiOutputConfigSection", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiOutputConfigSectionClass: public Ui_SamGuiOutputConfigSectionClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIOUTPUTCONFIGSECTION_H
