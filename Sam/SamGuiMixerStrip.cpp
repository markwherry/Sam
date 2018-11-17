#include "SamGuiMixerStrip.h"

#include <QPainter>

#include "SamEngine.h"
#include "SamProject.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiMixerStrip::SamGuiMixerStrip(QWidget *parent, SamChannel *channel, int audioBusIndex)
	: QWidget(parent)
	, m_channel(channel)
	, m_audioBusIndex(audioBusIndex)
	//, m_selected(false)
{
	//ui.setupUi(this);

	setFixedSize(100,381+20); //setFixedSize(50,300);

	int y = 4 + 24;

	for(int i=0; i<4; i++) {
		m_inserts[i] = new SamGuiMixerInsert(this, channel, audioBusIndex, i);
		m_inserts[i]->setFixedSize(width()-8, 20);
		m_inserts[i]->move(4, y);
		y += (m_inserts[i]->height()+4);
	}

	/*m_fader = new UiFader(this, true, 150, 28, 21);
	m_fader->move((width()/2)-(m_fader->width()/2)-1, height()-54-m_fader->height()-20);
	m_fader->setTrackSize(7,7);
	//m_fader->setLevelColor(QColor(220,220,220));
	//connect(m_fader, SIGNAL(valueChanged(float, int)), this, SLOT(faderValueChange(float, int)));*/

	connect(this, SIGNAL(needToUpdate()), this, SLOT(update()));

	samEngine->registerListener(this, SamEngine::ContextAudioTool);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiMixerStrip::~SamGuiMixerStrip()
{
	samEngine->unregisterListener(this, SamEngine::ContextAudioTool);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMixerStrip::onState(AStateEvent *ev)
{
	if(ev->context() == SamEngine::ContextAudioTool) {
		switch(ev->item()) {
			case SamAudioToolInsert::ItemActive:
				emit needToUpdate();
				break;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMixerStrip::setSelected(bool selected)
{
	//m_selected = selected;
	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMixerStrip::mousePressEvent(QMouseEvent *ev)
{
	//qDebug("SamGuiMixerStrip::mousePressEvent()");
	//emit selected();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMixerStrip::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.fillRect(0,0,width(),height(),QColor(78,78,78));
	p.drawRect(0, 0, width()-1, height()-1);

	//p.setPen(QColor(0,0,0));
	//p.drawLine(width()-1,0,width()-1,height()-1);

	p.setPen (QColor(100,100,100));
	p.drawRect (0,0,width()-1, height()-1);

	p.setPen(QColor(0,0,0));
	p.drawLine(width()-1,0,width()-1,height()-1);

	p.setFont(QFont("Calibri", 8));
	p.setPen(QColor(255,255,255,100));
	p.drawText(0, 4, width(), 20, Qt::AlignCenter, "INSERTS");

	/*if(m_selected) {
		p.setPen(QColor(120,120,200));
		p.setBrush(QColor(255,255,255,10));
		p.drawRect (0,0,width()-1, height()-1);
		QLinearGradient lgBack(QPointF(0, 0), QPointF(0, height()));
		QColor col(QColor(255,255,255));
		col.setAlpha(20);//240//90 // 230
		lgBack.setColorAt(0, col);
		col.setAlpha(0);//255//50 // 255
		lgBack.setColorAt(1, col);
		//p.setBrush(lgBack);
		p.fillRect(0, 0, width()-1, (height()/5)*3, lgBack);
	}*/

	// Stream Number
	p.setFont(QFont("Calibri", 18, QFont::Bold));
	p.setPen(QColor(255,255,255,40));
	p.drawText(2, height()-55-20, width()-3, 34, Qt::AlignTop|Qt::AlignCenter, QString::number(m_audioBusIndex+1));

	

	QFont f("",0,QFont::Bold);
	f.setPixelSize(9); //9
	f.setStyleStrategy(QFont::PreferAntialias);
	p.setFont(f);
	p.setPen(QColor(255,255,255,200));
	if(m_channel) {
		p.fillRect(0,height()-21,width()-1,21,QColor(50,150,50)); //0,0,0,100
		p.drawText(2, height()-20, width()-3, 20, Qt::AlignCenter|Qt::AlignVCenter, m_channel->audioOutputBus(m_audioBusIndex)->name().toChar());
	} else {
		p.fillRect(0,height()-21,width()-1,21,QColor(50,50,150));
		SamProject* project = samEngine->project();
		p.drawText(2, height()-20, width()-3, 20, Qt::AlignCenter|Qt::AlignVCenter, project->audioBus(m_audioBusIndex)->name().toChar());
	}
}