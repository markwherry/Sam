#ifndef SAMGUIMIDIMONITORITEM_H
#define SAMGUIMIDIMONITORITEM_H

#include <QWidget>
//#include "ui_SamGuiMidiMonitorItem.h"

#include "CsMidiMessage.h"
using namespace Cs;

#define EMIDIMONITORLIST_ITEMSIZE 320

class SamGuiMidiMonitorItem : public QWidget
{
	Q_OBJECT

public:
	SamGuiMidiMonitorItem(QWidget *parent, int id, CsMidiMessage midiMessage);
	~SamGuiMidiMonitorItem();

	void setLastItemCreated(bool lastItemCreated);
	void setShowNumbers(bool showNumbers);
	void setFiltered(bool filtered);
	bool isFiltered(void);
	void setId(int id) { m_id = id; }
	int id(void) { return m_id; }
	unsigned char getMidiEventType(void) { return m_midiMessage.data[0]; } 
	

protected:
	void paintEvent(QPaintEvent *event);

private:
	//Ui::SamGuiMidiMonitorItemClass ui;
	int m_id;
	CsMidiMessage m_midiMessage;
	QColor backgroundColor[2];
	bool m_lastItemCreated;
	bool m_showNumbers;
	bool m_filtered;
};

#endif // SAMGUIMIDIMONITORITEM_H
