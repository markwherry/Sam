#ifndef SAMGUIMIDIMONITORLIST_H
#define SAMGUIMIDIMONITORLIST_H

#include <QWidget>
//#include "ui_SamGuiMidiMonitorList.h"

#include <QList>

#include "CsMidiMessage.h"

#include "SamGuiMidiMonitorItem.h"

class SamGuiMidiMonitorList : public QWidget
{
	Q_OBJECT

public:
	SamGuiMidiMonitorList(QWidget *parent = 0);
	~SamGuiMidiMonitorList();

	void addItem(CsMidiMessage midiMessage, bool filtered, bool showNumbers);
	//void resetHeight(void);
	void updateFilteredList(bool noteOn, bool noteOff, bool aftertouch, bool controller, bool programChange, bool channelPressure, bool pitchWheel, bool sysEx);

public slots:
	void clearList(void);
	void showNumbers(bool checked);
	void deleteFilteredEvents(void);

protected:
	void paintEvent(QPaintEvent *event);

private:
	//Ui::SamGuiMidiMonitorListClass ui;
	QList<SamGuiMidiMonitorItem*> itemList;
	
};

#endif // SAMGUIMIDIMONITORLIST_H
