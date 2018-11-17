#ifndef SEGUIZONEDISPLAYITEM_H
#define SEGUIZONEDISPLAYITEM_H

#include <QWidget>
//#include "ui_SeGuiZoneDisplayItem.h"

#include "SamZone.h"

class SeGuiZoneDisplayItem : public QWidget
{
	Q_OBJECT

public:
	SeGuiZoneDisplayItem(QWidget *parent, SamZone *zone);
	~SeGuiZoneDisplayItem();

	void paint(QPainter &p);

	SamZone* zone(void) { return m_zone; }

	int hoverMode(void) { return m_hoverMode; }

protected:
	void enterEvent(QEvent *ev);
	void leaveEvent(QEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);

private:
	//Ui::SeGuiZoneDisplayItemClass ui;
	SamZone *m_zone;
	bool m_showNumCells;
	int m_hoverMode;
};

#endif // SEGUIZONEDISPLAYITEM_H
