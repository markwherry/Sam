/********************************************************************************
** Form generated from reading UI file 'SeGuiItemHeading.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUIITEMHEADING_H
#define UI_SEGUIITEMHEADING_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiItemHeadingClass
{
public:

    void setupUi(QWidget *SeGuiItemHeadingClass)
    {
        if (SeGuiItemHeadingClass->objectName().isEmpty())
            SeGuiItemHeadingClass->setObjectName(QString::fromUtf8("SeGuiItemHeadingClass"));
        SeGuiItemHeadingClass->resize(400, 300);

        retranslateUi(SeGuiItemHeadingClass);

        QMetaObject::connectSlotsByName(SeGuiItemHeadingClass);
    } // setupUi

    void retranslateUi(QWidget *SeGuiItemHeadingClass)
    {
        SeGuiItemHeadingClass->setWindowTitle(QApplication::translate("SeGuiItemHeadingClass", "SeGuiItemHeading", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiItemHeadingClass: public Ui_SeGuiItemHeadingClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUIITEMHEADING_H
