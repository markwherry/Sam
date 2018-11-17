/********************************************************************************
** Form generated from reading UI file 'SamGuiBounceGroup.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIBOUNCEGROUP_H
#define UI_SAMGUIBOUNCEGROUP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiBounceGroupClass
{
public:

    void setupUi(QWidget *SamGuiBounceGroupClass)
    {
        if (SamGuiBounceGroupClass->objectName().isEmpty())
            SamGuiBounceGroupClass->setObjectName(QString::fromUtf8("SamGuiBounceGroupClass"));
        SamGuiBounceGroupClass->resize(400, 300);

        retranslateUi(SamGuiBounceGroupClass);

        QMetaObject::connectSlotsByName(SamGuiBounceGroupClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiBounceGroupClass)
    {
        SamGuiBounceGroupClass->setWindowTitle(QApplication::translate("SamGuiBounceGroupClass", "SamGuiBounceGroup", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiBounceGroupClass: public Ui_SamGuiBounceGroupClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIBOUNCEGROUP_H
