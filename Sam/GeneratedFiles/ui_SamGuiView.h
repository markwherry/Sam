/********************************************************************************
** Form generated from reading UI file 'SamGuiView.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIVIEW_H
#define UI_SAMGUIVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiViewClass
{
public:

    void setupUi(QWidget *SamGuiViewClass)
    {
        if (SamGuiViewClass->objectName().isEmpty())
            SamGuiViewClass->setObjectName(QString::fromUtf8("SamGuiViewClass"));
        SamGuiViewClass->resize(400, 300);

        retranslateUi(SamGuiViewClass);

        QMetaObject::connectSlotsByName(SamGuiViewClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiViewClass)
    {
        SamGuiViewClass->setWindowTitle(QApplication::translate("SamGuiViewClass", "SamGuiView", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiViewClass: public Ui_SamGuiViewClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIVIEW_H
