/********************************************************************************
** Form generated from reading UI file 'SamGuiLayerStrip.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUILAYERSTRIP_H
#define UI_SAMGUILAYERSTRIP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiLayerStripClass
{
public:

    void setupUi(QWidget *SamGuiLayerStripClass)
    {
        if (SamGuiLayerStripClass->objectName().isEmpty())
            SamGuiLayerStripClass->setObjectName(QString::fromUtf8("SamGuiLayerStripClass"));
        SamGuiLayerStripClass->resize(400, 300);

        retranslateUi(SamGuiLayerStripClass);

        QMetaObject::connectSlotsByName(SamGuiLayerStripClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiLayerStripClass)
    {
        SamGuiLayerStripClass->setWindowTitle(QApplication::translate("SamGuiLayerStripClass", "SamGuiLayerStrip", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiLayerStripClass: public Ui_SamGuiLayerStripClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUILAYERSTRIP_H
