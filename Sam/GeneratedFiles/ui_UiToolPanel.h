/********************************************************************************
** Form generated from reading UI file 'UiToolPanel.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UITOOLPANEL_H
#define UI_UITOOLPANEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiToolPanelClass
{
public:

    void setupUi(QWidget *UiToolPanelClass)
    {
        if (UiToolPanelClass->objectName().isEmpty())
            UiToolPanelClass->setObjectName(QString::fromUtf8("UiToolPanelClass"));
        UiToolPanelClass->resize(400, 300);

        retranslateUi(UiToolPanelClass);

        QMetaObject::connectSlotsByName(UiToolPanelClass);
    } // setupUi

    void retranslateUi(QWidget *UiToolPanelClass)
    {
        UiToolPanelClass->setWindowTitle(QApplication::translate("UiToolPanelClass", "UiToolPanel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiToolPanelClass: public Ui_UiToolPanelClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UITOOLPANEL_H
