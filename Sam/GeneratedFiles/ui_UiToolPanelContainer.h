/********************************************************************************
** Form generated from reading UI file 'UiToolPanelContainer.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UITOOLPANELCONTAINER_H
#define UI_UITOOLPANELCONTAINER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiToolPanelContainerClass
{
public:

    void setupUi(QWidget *UiToolPanelContainerClass)
    {
        if (UiToolPanelContainerClass->objectName().isEmpty())
            UiToolPanelContainerClass->setObjectName(QString::fromUtf8("UiToolPanelContainerClass"));
        UiToolPanelContainerClass->resize(400, 300);

        retranslateUi(UiToolPanelContainerClass);

        QMetaObject::connectSlotsByName(UiToolPanelContainerClass);
    } // setupUi

    void retranslateUi(QWidget *UiToolPanelContainerClass)
    {
        UiToolPanelContainerClass->setWindowTitle(QApplication::translate("UiToolPanelContainerClass", "UiToolPanelContainer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiToolPanelContainerClass: public Ui_UiToolPanelContainerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UITOOLPANELCONTAINER_H
