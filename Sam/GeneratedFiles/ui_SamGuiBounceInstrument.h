/********************************************************************************
** Form generated from reading UI file 'SamGuiBounceInstrument.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIBOUNCEINSTRUMENT_H
#define UI_SAMGUIBOUNCEINSTRUMENT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiBounceInstrumentClass
{
public:

    void setupUi(QWidget *SamGuiBounceInstrumentClass)
    {
        if (SamGuiBounceInstrumentClass->objectName().isEmpty())
            SamGuiBounceInstrumentClass->setObjectName(QString::fromUtf8("SamGuiBounceInstrumentClass"));
        SamGuiBounceInstrumentClass->resize(400, 300);

        retranslateUi(SamGuiBounceInstrumentClass);

        QMetaObject::connectSlotsByName(SamGuiBounceInstrumentClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiBounceInstrumentClass)
    {
        SamGuiBounceInstrumentClass->setWindowTitle(QApplication::translate("SamGuiBounceInstrumentClass", "SamGuiBounceInstrument", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiBounceInstrumentClass: public Ui_SamGuiBounceInstrumentClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIBOUNCEINSTRUMENT_H
