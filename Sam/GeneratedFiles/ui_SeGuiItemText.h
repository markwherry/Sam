/********************************************************************************
** Form generated from reading UI file 'SeGuiItemText.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUIITEMTEXT_H
#define UI_SEGUIITEMTEXT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiItemTextClass
{
public:

    void setupUi(QWidget *SeGuiItemTextClass)
    {
        if (SeGuiItemTextClass->objectName().isEmpty())
            SeGuiItemTextClass->setObjectName(QString::fromUtf8("SeGuiItemTextClass"));
        SeGuiItemTextClass->resize(400, 300);

        retranslateUi(SeGuiItemTextClass);

        QMetaObject::connectSlotsByName(SeGuiItemTextClass);
    } // setupUi

    void retranslateUi(QWidget *SeGuiItemTextClass)
    {
        SeGuiItemTextClass->setWindowTitle(QApplication::translate("SeGuiItemTextClass", "SeGuiItemText", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiItemTextClass: public Ui_SeGuiItemTextClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUIITEMTEXT_H
