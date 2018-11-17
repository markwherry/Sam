/********************************************************************************
** Form generated from reading UI file 'UiPanner.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIPANNER_H
#define UI_UIPANNER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiPannerClass
{
public:

    void setupUi(QWidget *UiPannerClass)
    {
        if (UiPannerClass->objectName().isEmpty())
            UiPannerClass->setObjectName(QString::fromUtf8("UiPannerClass"));
        UiPannerClass->resize(400, 300);

        retranslateUi(UiPannerClass);

        QMetaObject::connectSlotsByName(UiPannerClass);
    } // setupUi

    void retranslateUi(QWidget *UiPannerClass)
    {
        UiPannerClass->setWindowTitle(QApplication::translate("UiPannerClass", "UiPanner", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiPannerClass: public Ui_UiPannerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIPANNER_H
