/********************************************************************************
** Form generated from reading UI file 'UiLevel.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UILEVEL_H
#define UI_UILEVEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiLevelClass
{
public:

    void setupUi(QWidget *UiLevelClass)
    {
        if (UiLevelClass->objectName().isEmpty())
            UiLevelClass->setObjectName(QString::fromUtf8("UiLevelClass"));
        UiLevelClass->resize(400, 300);

        retranslateUi(UiLevelClass);

        QMetaObject::connectSlotsByName(UiLevelClass);
    } // setupUi

    void retranslateUi(QWidget *UiLevelClass)
    {
        UiLevelClass->setWindowTitle(QApplication::translate("UiLevelClass", "UiLevel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiLevelClass: public Ui_UiLevelClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UILEVEL_H
