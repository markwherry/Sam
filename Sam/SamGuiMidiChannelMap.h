#ifndef SAMGUIMIDICHANNELMAP_H
#define SAMGUIMIDICHANNELMAP_H

#include <QWidget>
//#include "ui_SamGuiMidiChannelMap.h"

#include "AStateManager.h"
using namespace Atom;

#include <QScrollArea>
#include <QScrollBar>

#include "SamGuiMidiChannelMapHeader.h"
#include "SamGuiMidiChannelMapList.h"
#include "SamGuiMidiChannelMapDisplay.h"

class SamGuiMidiChannelMap : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiMidiChannelMap(QWidget *parent = 0);
	~SamGuiMidiChannelMap();

	void onState(AStateEvent *ev);

public slots:
	void updateMap(void);
	void updateHighlight(int port, int channel);
	void updateVScrollBarRange(void);
	void updateVScrollBarValue(int value);
	void doVerticalScroll(int value);

signals:
	void mapUpdated(void);

protected:
	void paintEvent(QPaintEvent *ev);
	void resizeEvent(QResizeEvent *ev);
	void showEvent(QShowEvent *ev);

private:
	//Ui::SamGuiMidiChannelMap ui;
	SamGuiMidiChannelMapHeader  *m_header;
	SamGuiMidiChannelMapList    *m_list;
	SamGuiMidiChannelMapDisplay *m_display;

	QScrollArea *m_areaHeader;
	QScrollArea *m_areaList;
	QScrollArea *m_areaDisplay;

	//QScrollBar *m_scrollH;
	QScrollBar *m_scrollV;
};

#endif // SAMGUIMIDICHANNELMAP_H
