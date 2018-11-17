/********************************************************************************
** Form generated from reading UI file 'SamGuiToolContainer.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUITOOLCONTAINER_H
#define UI_SAMGUITOOLCONTAINER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiToolContainerClass
{
public:

    void setupUi(QWidget *SamGuiToolContainerClass)
    {
        if (SamGuiToolContainerClass->objectName().isEmpty())
            SamGuiToolContainerClass->setObjectName(QString::fromUtf8("SamGuiToolContainerClass"));
        SamGuiToolContainerClass->resize(400, 300);

        retranslateUi(SamGuiToolContainerClass);

        QMetaObject::connectSlotsByName(SamGuiToolContainerClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiToolContainerClass)
    {
        SamGuiToolContainerClass->setWindowTitle(QApplication::translate("SamGuiToolContainerClass", "SamGuiToolContainer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiToolContainerClass: public Ui_SamGuiToolContainerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUITOOLCONTAINER_H
