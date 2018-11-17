/********************************************************************************
** Form generated from reading UI file 'SamGuiSteps.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUISTEPS_H
#define UI_SAMGUISTEPS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiStepsClass
{
public:

    void setupUi(QWidget *SamGuiStepsClass)
    {
        if (SamGuiStepsClass->objectName().isEmpty())
            SamGuiStepsClass->setObjectName(QString::fromUtf8("SamGuiStepsClass"));
        SamGuiStepsClass->resize(400, 300);

        retranslateUi(SamGuiStepsClass);

        QMetaObject::connectSlotsByName(SamGuiStepsClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiStepsClass)
    {
        SamGuiStepsClass->setWindowTitle(QApplication::translate("SamGuiStepsClass", "SamGuiSteps", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiStepsClass: public Ui_SamGuiStepsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUISTEPS_H
