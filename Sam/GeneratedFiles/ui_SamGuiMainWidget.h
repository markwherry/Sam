/********************************************************************************
** Form generated from reading UI file 'SamGuiMainWidget.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIMAINWIDGET_H
#define UI_SAMGUIMAINWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiMainWidgetClass
{
public:

    void setupUi(QWidget *SamGuiMainWidgetClass)
    {
        if (SamGuiMainWidgetClass->objectName().isEmpty())
            SamGuiMainWidgetClass->setObjectName(QString::fromUtf8("SamGuiMainWidgetClass"));
        SamGuiMainWidgetClass->resize(400, 300);

        retranslateUi(SamGuiMainWidgetClass);

        QMetaObject::connectSlotsByName(SamGuiMainWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiMainWidgetClass)
    {
        SamGuiMainWidgetClass->setWindowTitle(QApplication::translate("SamGuiMainWidgetClass", "SamGuiMainWidget", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiMainWidgetClass: public Ui_SamGuiMainWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIMAINWIDGET_H
