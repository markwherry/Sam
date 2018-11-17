/********************************************************************************
** Form generated from reading UI file 'SeGuiZoneDisplayItem.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUIZONEDISPLAYITEM_H
#define UI_SEGUIZONEDISPLAYITEM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiZoneDisplayItemClass
{
public:

    void setupUi(QWidget *SeGuiZoneDisplayItemClass)
    {
        if (SeGuiZoneDisplayItemClass->objectName().isEmpty())
            SeGuiZoneDisplayItemClass->setObjectName(QString::fromUtf8("SeGuiZoneDisplayItemClass"));
        SeGuiZoneDisplayItemClass->resize(400, 300);

        retranslateUi(SeGuiZoneDisplayItemClass);

        QMetaObject::connectSlotsByName(SeGuiZoneDisplayItemClass);
    } // setupUi

    void retranslateUi(QWidget *SeGuiZoneDisplayItemClass)
    {
        SeGuiZoneDisplayItemClass->setWindowTitle(QApplication::translate("SeGuiZoneDisplayItemClass", "SeGuiZoneDisplayItem", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiZoneDisplayItemClass: public Ui_SeGuiZoneDisplayItemClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUIZONEDISPLAYITEM_H
