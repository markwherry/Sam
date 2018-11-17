#include "SamGuiMixerInsert.h"

#include <qpainter.h>

#include "SamEngine.h"
#include "SamProject.h"
#include "SamRealTimeCommands.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiMixerInsert::SamGuiMixerInsert(QWidget *parent, SamChannel *channel, int outputIndex, int insertIndex)
	: QWidget(parent)
	, m_channel(channel)
	, m_outputIndex(outputIndex)
	, m_insertIndex(insertIndex)
	, m_mouseOver(false)
{
	//ui.setupUi(this);

	m_selectAudioTool = new SamGuiSelectAudioTool(this, true);
	m_selectAudioTool->setChannel(channel, outputIndex, insertIndex);
	
	m_pixmap = new QPixmap(":/Resources/tool16.png");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiMixerInsert::~SamGuiMixerInsert()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMixerInsert::enterEvent(QEvent *event)
{
	if(!isEnabled()) return;

	m_mouseOver = true;
	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMixerInsert::leaveEvent(QEvent *event)
{
	if(!isEnabled()) return;

	m_mouseOver = false;
	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMixerInsert::mousePressEvent(QMouseEvent *ev)
{
	SamAudioToolInsert *ati = 0;
	if(m_channel) {
		ati = m_channel->audioOutputBus(m_outputIndex)->audioInsert(m_insertIndex);
	} else {
		ati = samEngine->project()->audioBus(m_outputIndex)->audioInsert(m_insertIndex);
	}

	if(ati->isAssigned()) {
		if(m_channel) {
			samEngine->executeRealTimeCommand(new RtcSetAudioToolActive(m_channel->audioOutputBus(m_outputIndex), m_insertIndex, !ati->isActive()));
		} else {
			samEngine->executeRealTimeCommand(new RtcSetAudioToolActive(samEngine->project()->audioBus(m_outputIndex), m_insertIndex, !ati->isActive()));
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMixerInsert::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	SamAudioToolInsert *ati = 0;
	if(m_channel) {
		ati = m_channel->audioOutputBus(m_outputIndex)->audioInsert(m_insertIndex);
	} else {
		ati = samEngine->project()->audioBus(m_outputIndex)->audioInsert(m_insertIndex);
	}

	p.setPen(QColor(30,30,30)); 
	
	if(ati->isActive()) {
		p.setBrush(QColor(80,80,255));
	} else {
		p.setBrush(QColor(60,60,100));  // 1 = 255, 127, 0, // 0 = 127, 64, 0 //88,13,143   /// Orange on color 224,163,28; off: 144,83,0
	}

	p.drawRect(0,0,width()-1,height()-1);

	p.drawLine(0+height(), 0, 0+height(), height());

	if(ati->isActive()) {
		p.setPen(QColor(100,100,255));
		p.drawLine(1,1,19,1);
		p.drawLine(19,1,19,18);
		p.drawLine(19,18,1, 18);
		p.drawLine(1,18,1,1);
	}
	
	//p.setPen(QColor(255,255,255,25));
	//p.drawLine(0+height()+1, 1, 0+height()+1, height()-1);

	p.fillRect(height(), 1, width()-height(), height()-1, QColor(0,0,0,80));

	if(ati->isAssigned()) {
		QFont f("",0,QFont::Bold);
		f.setPixelSize(9); //9
		f.setStyleStrategy(QFont::PreferAntialias);
		p.setFont(f);
		p.setPen(QColor(255,255,255));
		int offset = height()+(height()/4);
		p.drawText(offset, 1, width()-offset-(height()/4), height(), Qt::AlignLeft|Qt::AlignVCenter, ati->name().toChar());
	}

	p.drawPixmap((height()/2)-8, (height()/2)-8, *m_pixmap);

	if(m_mouseOver) {
		p.fillRect(1, 1, width()-2, height()-2, QColor(255,255,255,25));
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMixerInsert::resizeEvent(QResizeEvent *ev)
{
	m_selectAudioTool->move(height(), 0);
	m_selectAudioTool->setFixedSize(width()-height(), height());
}