#include "SamGuiMidiMonitorItem.h"

#include "CsMidiName.h"

#include <QPainter>

SamGuiMidiMonitorItem::SamGuiMidiMonitorItem(QWidget *parent, int id, CsMidiMessage midiMessage)
	: QWidget(parent)
	, m_id(id)
	, m_midiMessage(midiMessage)
	, m_filtered(false)
{
	//ui.setupUi(this);

	backgroundColor[0] = QColor(200,200,200);
	backgroundColor[1] = QColor(180,180,180);

	setFixedSize(EMIDIMONITORLIST_ITEMSIZE,20);

	m_lastItemCreated = true;
	m_showNumbers = false;
}

SamGuiMidiMonitorItem::~SamGuiMidiMonitorItem()
{

}

void SamGuiMidiMonitorItem::setLastItemCreated(bool lastItemCreated)
{
	m_lastItemCreated = lastItemCreated;
	//update();
}

void SamGuiMidiMonitorItem::setShowNumbers(bool showNumbers)
{
	m_showNumbers = showNumbers;
}

void SamGuiMidiMonitorItem::setFiltered(bool filtered)
{
	m_filtered = filtered;
}

bool SamGuiMidiMonitorItem::isFiltered(void)
{
	return m_filtered;
}

void SamGuiMidiMonitorItem::paintEvent(QPaintEvent *event)
{
	//qDebug ("EMidiMonitorItem::paintEvent()");
	QPainter p(this);

	int even = 0;
	if (m_id%2==0)
		even = 1;

	if(!m_lastItemCreated) {
		p.setPen(backgroundColor[even]);
		p.setBrush(backgroundColor[even]);
	} else {
		//p.setPen(QColor(225,205,54));
		//p.setBrush(QColor(225,205,54));
		p.setPen(QColor(120,120,200));
		p.setBrush(QColor(120,120,200));
	}
	p.drawRect(0,0,width()-1,height()-1);

	p.setPen(QColor(0,0,0));
	p.drawText(0, 0, 40, 20, Qt::AlignRight|Qt::AlignVCenter, QString::number(m_id+1));

	p.setPen(QColor(160,160,160));
	p.drawLine(45, 0, 45, height());

	QString type = "Unrecognised Event";
	QString data1 = "";
	QString data2 = "";
	int channel = 0;

	if(m_midiMessage.data[0]>=0x80 && m_midiMessage.data[0]<=0x8F){
		type = "Note Off";
		channel = m_midiMessage.data[0] - 0x80;
		data1 = CsMidiName::pitchToName(m_midiMessage.data[1],3).toChar();
		if(m_showNumbers)
			data1 += " ("+QString::number(m_midiMessage.data[1])+")";
		data2 = QString::number(m_midiMessage.data[2]);
	} else if(m_midiMessage.data[0]>=0x90 && m_midiMessage.data[0]<=0x9F){
		type = "Note On";
		if(m_midiMessage.data[2]==0)type+=" (Off)";
		channel = m_midiMessage.data[0] - 0x90;
		data1 = CsMidiName::pitchToName(m_midiMessage.data[1],3).toChar();
		if(m_showNumbers)
			data1 += " ("+QString::number(m_midiMessage.data[1])+")";
		data2 = QString::number(m_midiMessage.data[2]);
	} else if(m_midiMessage.data[0]>=0xA0 && m_midiMessage.data[0]<=0xAF){
		type = "Aftertouch";
	} else if(m_midiMessage.data[0]>=0xB0 && m_midiMessage.data[0]<=0xBF){
		type = "Controller";
		data1 = CsMidiName::controllerToName(m_midiMessage.data[1],1).toChar();
		if(m_showNumbers)
			data1 += " ("+QString::number(m_midiMessage.data[1])+")";
		data2 = QString::number(m_midiMessage.data[2]);
	} else if(m_midiMessage.data[0]>=0xC0 && m_midiMessage.data[0]<=0xCF){
		type = "Program Change";
	} else if(m_midiMessage.data[0]>=0xD0 && m_midiMessage.data[0]<=0xDF){
		type = "Channel Pressure";
	} else if(m_midiMessage.data[0]>=0xE0 && m_midiMessage.data[0]<=0xEF){
		type = "Pitch Wheel";
	} else if(m_midiMessage.data[0]>=0xF0 && m_midiMessage.data[0]<=0xFF){
		type = "System Exclusive";
	}

	//type = "Unrecognised Event";
	//data1 = "Non-registered Parameter MSB";
	//data1 = "Non-reg Par MSB";

	if(m_lastItemCreated) {
		p.setPen(QColor(255,255,255));
	} else {
		p.setPen(QColor(0,0,0));
	}
	p.setFont(QFont("",8,QFont::Bold));
	p.drawText(50, 0, 111, 20, Qt::AlignLeft|Qt::AlignVCenter, type);

	p.setFont(QFont("",8));
	p.drawText(171, 0, 100, 20, Qt::AlignLeft|Qt::AlignVCenter, data1);
	p.drawText(281, 0, 29, 20, Qt::AlignLeft|Qt::AlignVCenter, data2);

	//p.drawText(50, 0, 100, 20, Qt::AlignLeft|Qt::AlignVCenter, "Network MIDI");

	
}