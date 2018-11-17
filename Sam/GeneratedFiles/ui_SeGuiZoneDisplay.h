/********************************************************************************
** Form generated from reading UI file 'SeGuiZoneDisplay.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUIZONEDISPLAY_H
#define UI_SEGUIZONEDISPLAY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiZoneDisplayClass
{
public:

    void setupUi(QWidget *SeGuiZoneDisplayClass)
    {
        if (SeGuiZoneDisplayClass->objectName().isEmpty())
            SeGuiZoneDisplayClass->setObjectName(QString::fromUtf8("SeGuiZoneDisplayClass"));
        SeGuiZoneDisplayClass->resize(400, 300);

        retranslateUi(SeGuiZoneDisplayClass);

        QMetaObject::connectSlotsByName(SeGuiZoneDisplayClass);
    } // setupUi

    void retranslateUi(QWidget *SeGuiZoneDisplayClass)
    {
        SeGuiZoneDisplayClass->setWindowTitle(QApplication::translate("SeGuiZoneDisplayClass", "SeGuiZoneDisplay", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiZoneDisplayClass: public Ui_SeGuiZoneDisplayClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUIZONEDISPLAY_H
