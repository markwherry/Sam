/********************************************************************************
** Form generated from reading UI file 'SeGuiSampleDisplay.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUISAMPLEDISPLAY_H
#define UI_SEGUISAMPLEDISPLAY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiSampleDisplay
{
public:

    void setupUi(QWidget *SeGuiSampleDisplay)
    {
        if (SeGuiSampleDisplay->objectName().isEmpty())
            SeGuiSampleDisplay->setObjectName(QString::fromUtf8("SeGuiSampleDisplay"));
        SeGuiSampleDisplay->resize(400, 300);

        retranslateUi(SeGuiSampleDisplay);

        QMetaObject::connectSlotsByName(SeGuiSampleDisplay);
    } // setupUi

    void retranslateUi(QWidget *SeGuiSampleDisplay)
    {
        SeGuiSampleDisplay->setWindowTitle(QApplication::translate("SeGuiSampleDisplay", "SeGuiSampleDisplay", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiSampleDisplay: public Ui_SeGuiSampleDisplay {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUISAMPLEDISPLAY_H
