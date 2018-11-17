/********************************************************************************
** Form generated from reading UI file 'SamGuiSplash.ui'
**
** Created: Thu Feb 12 17:05:56 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUISPLASH_H
#define UI_SAMGUISPLASH_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiSplash
{
public:

    void setupUi(QWidget *SamGuiSplash)
    {
        if (SamGuiSplash->objectName().isEmpty())
            SamGuiSplash->setObjectName(QString::fromUtf8("SamGuiSplash"));
        SamGuiSplash->resize(400, 300);

        retranslateUi(SamGuiSplash);

        QMetaObject::connectSlotsByName(SamGuiSplash);
    } // setupUi

    void retranslateUi(QWidget *SamGuiSplash)
    {
        SamGuiSplash->setWindowTitle(QApplication::translate("SamGuiSplash", "SamGuiSplash", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiSplash: public Ui_SamGuiSplash {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUISPLASH_H
