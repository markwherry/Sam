#include "SamGuiMidiMonitorList.h"

#include <QPainter>

SamGuiMidiMonitorList::SamGuiMidiMonitorList(QWidget *parent)
    : QWidget(parent)
{
	setFixedWidth(EMIDIMONITORLIST_ITEMSIZE);

}

SamGuiMidiMonitorList::~SamGuiMidiMonitorList()
{
	qDebug("EMidiMonitorList::~EMidiMonitorList()");
	for (int i=0; i<itemList.size(); i++)
		delete itemList[i];
	qDebug("After delete");
	itemList.~QList();
	qDebug("After ~");
	//itemList.clear();*/
}

void SamGuiMidiMonitorList::addItem(CsMidiMessage midiMessage, bool filtered, bool showNumbers)
{
	//qDebug("EMidiMonitorList::addItem()");

	int newYPosition = 0;
	int lastId = -1;

	if(!itemList.isEmpty()) {
		for(int i=itemList.size()-1; i>=0; i--) {
			if(!itemList[i]->isFiltered()) {
				newYPosition = itemList[i]->y() + itemList[i]->height();
				if(!filtered)itemList[i]->setLastItemCreated(false);
				lastId = itemList[i]->id();
				break;
			}
		}
		//newYPosition = itemList.last()->y() + itemList.last()->height();
		//itemList.last()->setLastItemCreated(false);
	}

	SamGuiMidiMonitorItem *item = new SamGuiMidiMonitorItem(this, lastId+1, midiMessage);
	item->setShowNumbers(showNumbers);
	item->move(0,newYPosition);
	if(filtered) {
		item->setFiltered(true);
		item->hide();
		item->setLastItemCreated(false);
	} else {
		item->show();
		setFixedHeight(newYPosition+20);
	}

	itemList.append(item);

	

	update();

	//qDebug ("w=%d, h=%d", width(), height());
}

//void EMidiMonitorList::resetHeight(void)
//{
//	if(!itemList.isEmpty()){
//		setFixedHeight(itemList.last()->y()+itemList.last()->height());
//	}
//}

void SamGuiMidiMonitorList::updateFilteredList(bool noteOn, bool noteOff, bool aftertouch, bool controller, bool programChange, bool channelPressure, bool pitchWheel, bool sysEx)
{
	int runningId = 0;
	int runningY = 0;
	for(int i=0; i<itemList.size(); i++) {
		itemList[i]->setLastItemCreated(false);

		bool filtered = false;

		if (itemList[i]->getMidiEventType()>=0x80 && itemList[i]->getMidiEventType()<=0x8F && noteOff) {
			filtered = true;
		} else if (itemList[i]->getMidiEventType()>=0x90 && itemList[i]->getMidiEventType()<=0x9F && noteOn) {
			filtered = true;
		} else if (itemList[i]->getMidiEventType()>=0xA0 && itemList[i]->getMidiEventType()<=0xAF && aftertouch) {
			filtered = true;
		} else if (itemList[i]->getMidiEventType()>=0xB0 && itemList[i]->getMidiEventType()<=0xBF && controller) {
			filtered = true;
		} else if (itemList[i]->getMidiEventType()>=0xC0 && itemList[i]->getMidiEventType()<=0xCF && programChange) {
			filtered = true;
		} else if (itemList[i]->getMidiEventType()>=0xD0 && itemList[i]->getMidiEventType()<=0xDF && channelPressure) {
			filtered = true;
		} else if (itemList[i]->getMidiEventType()>=0xE0 && itemList[i]->getMidiEventType()<=0xEF && pitchWheel) {
			filtered = true;
		} else if (itemList[i]->getMidiEventType()>=0xF0 && itemList[i]->getMidiEventType()<=0xFF && sysEx) {
			filtered = true;
		}

		itemList[i]->setFiltered(filtered);

		if(filtered) {
			itemList[i]->hide();
			itemList[i]->move(0,0);
		} else {
			itemList[i]->setId(runningId);
			runningId++;
			itemList[i]->move(0,runningY);
			runningY+=20;
			itemList[i]->show();
		}
	}

	for(int i=itemList.size()-1; i>=0; i--) {
		if(!itemList[i]->isFiltered()) {
			itemList[i]->setLastItemCreated(true);
			break;
		}
	}

	setFixedHeight(runningY);

}

void SamGuiMidiMonitorList::clearList(void)
{
	for (int i=0; i<itemList.size(); i++)
		delete itemList[i];

	itemList.clear();

	setFixedHeight(0);

	//update();
}

void SamGuiMidiMonitorList::showNumbers(bool checked)
{
	for(int i=0; i<itemList.size(); i++) {
		itemList[i]->setShowNumbers(checked);
		itemList[i]->update();
	}
}

void SamGuiMidiMonitorList::deleteFilteredEvents(void)
{
	for(int i=itemList.size()-1; i>=0; i--) {
		if(itemList[i]->isFiltered()) {
			delete itemList[i];
			itemList.removeAt(i);
		}
	}
}

void SamGuiMidiMonitorList::paintEvent(QPaintEvent *event)
{
	QPainter p(this);

	p.setPen(QColor(255,0,0));
	p.setBrush(QColor(0,255,255));
	p.drawRect(0,0,width()-1,height()-1);
}