#ifndef SEGUICELLITEM_H
#define SEGUICELLITEM_H

#include "UiListItem.h"
//#include "ui_SeGuiCellItem.h"

#include "SamCell.h"

#include "SeGuiSampleDisplay.h"

class SeGuiCellItem : public UiListItem
{
	Q_OBJECT

public:
	SeGuiCellItem(QWidget *parent, SamCell *cell);
	~SeGuiCellItem();

protected:
	void paintEvent(QPaintEvent *ev);
	void resizeEvent(QResizeEvent *ev);

	void setSelected(bool selected);
	bool isSelected(void) { return m_cell->isSelected(); }

private:
	//Ui::SeGuiCellItemClass ui;
	SamCell *m_cell;

	SeGuiSampleDisplay *m_sampleDisplay;
};

#endif // SEGUICELLITEM_H
