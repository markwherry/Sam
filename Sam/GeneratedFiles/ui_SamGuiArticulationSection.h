/********************************************************************************
** Form generated from reading UI file 'SamGuiArticulationSection.ui'
**
** Created: Thu Feb 12 17:05:56 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIARTICULATIONSECTION_H
#define UI_SAMGUIARTICULATIONSECTION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiArticulationSection
{
public:

    void setupUi(QWidget *SamGuiArticulationSection)
    {
        if (SamGuiArticulationSection->objectName().isEmpty())
            SamGuiArticulationSection->setObjectName(QString::fromUtf8("SamGuiArticulationSection"));
        SamGuiArticulationSection->resize(400, 300);

        retranslateUi(SamGuiArticulationSection);

        QMetaObject::connectSlotsByName(SamGuiArticulationSection);
    } // setupUi

    void retranslateUi(QWidget *SamGuiArticulationSection)
    {
        SamGuiArticulationSection->setWindowTitle(QApplication::translate("SamGuiArticulationSection", "SamGuiArticulationSection", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiArticulationSection: public Ui_SamGuiArticulationSection {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIARTICULATIONSECTION_H
