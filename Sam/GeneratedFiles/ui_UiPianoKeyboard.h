/********************************************************************************
** Form generated from reading UI file 'UiPianoKeyboard.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIPIANOKEYBOARD_H
#define UI_UIPIANOKEYBOARD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiPianoKeyboardClass
{
public:

    void setupUi(QWidget *UiPianoKeyboardClass)
    {
        if (UiPianoKeyboardClass->objectName().isEmpty())
            UiPianoKeyboardClass->setObjectName(QString::fromUtf8("UiPianoKeyboardClass"));
        UiPianoKeyboardClass->resize(400, 300);

        retranslateUi(UiPianoKeyboardClass);

        QMetaObject::connectSlotsByName(UiPianoKeyboardClass);
    } // setupUi

    void retranslateUi(QWidget *UiPianoKeyboardClass)
    {
        UiPianoKeyboardClass->setWindowTitle(QApplication::translate("UiPianoKeyboardClass", "UiPianoKeyboard", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiPianoKeyboardClass: public Ui_UiPianoKeyboardClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIPIANOKEYBOARD_H
