#ifndef SAMGUITOOLEDITORTOOLBAR_H
#define SAMGUITOOLEDITORTOOLBAR_H

#include <QWidget>
#include "ui_SamGuiToolEditorToolbar.h"

#include "UiSelectMenu.h"
#include "UiStateButton.h"

class SamGuiToolEditorToolbar : public QWidget
{
	Q_OBJECT

public:
	SamGuiToolEditorToolbar(QWidget *parent = 0);
	~SamGuiToolEditorToolbar();

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiToolEditorToolbarClass ui;
	UiSelectMenu *m_presetMenu;
	UiStateButton *m_buttonAddPreset;
	UiStateButton *m_buttonRemovePreset;
};

#endif // SAMGUITOOLEDITORTOOLBAR_H
