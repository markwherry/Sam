#include "SamGuiStatusBar.h"

#include "SamEngine.h"
#include "SamProject.h"

#include <QPainter>
#include <QMouseEvent>

#include "AUtility.h"
using namespace Atom;

#include "Sam.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiStatusBar::SamGuiStatusBar(QWidget *parent)
	: QWidget(parent)
	, m_usage(0)
	, m_peak(0)
	, m_memory(0)
	, m_storage(0)
{
	//ui.setupUi(this);

	setFixedHeight(35);

	m_systemLevel = new UiLevel(this, false);
	m_systemLevel->move(220,4);
	m_systemLevel->setFixedSize(100,8);

	m_peakLevel = new UiLevel(this, false);
	m_peakLevel->move(220,(height()/2)+2);
	m_peakLevel->setFixedSize(100,8);
	m_peakLevel->setColor(QColor(180,60,60));

	m_memoryLevel = new UiLevel(this, false);
	m_memoryLevel->move(488,4);
	m_memoryLevel->setFixedSize(100,8);

	m_bpmValue = new UiValue(this, 50, 15, "bpm", 5.0f, 990.0f, 120.0f, 0.5, 2);
	m_bpmValue->move(700+130+130, 1);
	m_bpmValue->setValueTextColor(QColor(255,255,0,200));
	m_bpmValue->setUnitTextColor(QColor(255,255,0,200));

	connect(m_systemLevel, SIGNAL(clicked()), this, SLOT(reset()));
	connect(m_peakLevel, SIGNAL(clicked()), this, SLOT(reset()));
	connect(m_memoryLevel, SIGNAL(clicked()), this, SLOT(reset()));
	connect(m_bpmValue, SIGNAL(valueChanged(float)), this, SLOT(setTempo(float)));

	startTimer(500);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiStatusBar::~SamGuiStatusBar()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStatusBar::reset(void)
{
	// Should this be a realtime command for safety?
	samEngine->resetVoicesPeak();
	samEngine->audioManager()->resetBufferUsagePeak();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStatusBar::setTempo(float bpm)
{
	samEngine->processTempoMessage((double)timeGetTime() * 1000000.0f, bpm);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStatusBar::mouseDoubleClickEvent(QMouseEvent *ev)
{
	if(ev->x() > (130*5) && ev->x() < (130*7)) {
		samGui->audioHardware()->show();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStatusBar::mousePressEvent(QMouseEvent *ev)
{
	reset();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStatusBar::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	//p.fillRect(0, 0, width()-1, height()-1, QColor(0,0,0));

	/*QLinearGradient linearGradient(QPointF(0, 0), QPointF(0, height()));
	linearGradient.setColorAt(0, QColor(0,0,0,00));
	linearGradient.setColorAt(1, QColor(0,0,0,255));

	p.fillRect(0, 0, width()-1, height()-1, linearGradient);//*/

	for(int i=0; i<7; i++) {
		if(i!=1 && i!=3 && i !=5) {
			p.setPen(QColor(255,255,255,10));
			p.drawLine(130*(i+1), 1, 130*(i+1), height()-1);
			p.setPen(QColor(255,255,255,40));
			p.drawLine(130*(i+1)+1, 1, 130*(i+1)+1, height()-1);
		}
	}

	p.setPen(QColor(255,255,255,180));
	//p.setPen(QColor(0,0,0));

	//p.setFont (QFont("",8,QFont::Bold));
	//p.drawText (so+10, 2, 100, 15, Qt::AlignLeft|Qt::AlignVCenter, "Sampler Engine");
	p.setFont (QFont("",8));
	p.drawText (10, 0, 100, 15, Qt::AlignLeft|Qt::AlignVCenter, "Total Voices:");
	p.drawText (10, 15, 100, 15, Qt::AlignLeft|Qt::AlignVCenter, "Voices Peak:");

	p.drawText (140, 0, 100, 15, Qt::AlignLeft|Qt::AlignVCenter, "System Usage:");
	p.drawText (140, 15, 100, 15, Qt::AlignLeft|Qt::AlignVCenter, "System Peak:");

	p.drawText (140+130+130, 0, 100, 15, Qt::AlignLeft|Qt::AlignVCenter, "System Memory:");
	p.drawText (140+130+130, 15, 150, 15, Qt::AlignLeft|Qt::AlignVCenter, "Total Sample Storage:");

	
	p.drawText (140+130+130+130+130, 0, 100, 15, Qt::AlignLeft|Qt::AlignVCenter, "Audio Driver:");
	p.setPen(QColor(255,255,255,240));
	p.drawText (140+130+130+130+130+70, 0, 150, 15, Qt::AlignLeft|Qt::AlignVCenter, samEngine->audioManager()->currentAudioDeviceDriver().toChar());

	p.setPen(QColor(255,255,255,180));
	p.drawText (140+130+130+130+130+130+130, 0, 100, 15, Qt::AlignLeft|Qt::AlignVCenter, "Tempo:");

	p.setPen(QColor(100,200,100,200));
	p.drawText (75, 0, 100, 15, Qt::AlignLeft|Qt::AlignVCenter, QString::number(samEngine->totalVoicesPlaying()));

	p.setPen(QColor(200,100,100,200));
	p.drawText (75, 15, 100, 15, Qt::AlignLeft|Qt::AlignVCenter, QString::number(samEngine->totalVoicesPeak()));
	
	//float bufferTime = (float) samEngine->audioManager()->bufferSize() / samEngine->audioManager()->sampleRate();
	//	int usage = (int)((float)(samEngine->audioManager()->lastBufferDuration() / bufferTime) * 100.0);
	//int usage = (int)(samEngine->audioManager()->lastBufferUsage() * 100.0f);	
	//if(usage>100) {
	//	usage = 100;
	//}

	if(m_usage >= 90) {
		p.setPen(QColor(200,100,100,200));
	} else {
		p.setPen(QColor(100,200,100,200));
	}
	p.drawText (330, 0, 100, 15, Qt::AlignLeft|Qt::AlignVCenter, QString::number(m_usage)+" %");
	p.setPen(QColor(200,100,100,200));
	p.drawText (330, 15, 100, 15, Qt::AlignLeft|Qt::AlignVCenter, QString::number(m_peak)+" %");
	//p.drawText (203, 0, 50, 15, Qt::AlignLeft|Qt::AlignVCenter, "1");
	//p.drawText (203, 0, 28, 15, Qt::AlignRight|Qt::AlignVCenter, QString::number(usage)+"%");

	if(m_memory >= 90) {
		p.setPen(QColor(200,100,100,200));
	} else {
		p.setPen(QColor(100,200,100,200));
	}
	p.drawText(599, 0, 100, 15, Qt::AlignLeft|Qt::AlignVCenter, QString::number(m_memory)+" %");
	p.setPen(QColor(255, 255, 255, 140));
	p.drawText(599, 15, 100, 15, Qt::AlignLeft|Qt::AlignVCenter, QString::number(m_storage)+" MB");

	//p.setPen(QColor(255,255,0,200));
	//p.drawText(700, 0, 100, 15, Qt::AlignLeft|Qt::AlignVCenter, QString::number(120.0f));

	// Version number
	p.setPen(QColor(255,255,255,180));
	p.drawText(width()-150, 15, 140, 15, Qt::AlignRight|Qt::AlignVCenter, VERSION_STRING);

	//p.drawText(width()-150, 0, 140, 15, Qt::AlignRight|Qt::AlignVCenter, QString::number((double)samEngine->timerAverage()));
	//p.drawText(width()-150, 0, 140, 15, Qt::AlignRight|Qt::AlignVCenter, "TEST VERSION");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStatusBar::timerEvent(QTimerEvent *ev)
{
	m_usage = (int)(samEngine->audioManager()->lastBufferUsage() * 100.0f);
	if(m_usage>100) {
		m_usage = 100;
	}

	m_peak = (int)(samEngine->audioManager()->bufferUsagePeak() * 100.0f);
	if(m_peak>100) {
		m_peak = 100;
	}

	m_memory = (int)AUtility::physicalMemoryUsage();
	if(m_memory>100) {
		m_memory = 100;
	}

	m_storage = 0;
	for(int i=0; i<samEngine->numInstruments(); i++) {
		m_storage += samEngine->instrument(i)->elementPool()->size();
	}
	if(m_storage > 0) {
		m_storage /= 1024;
		m_storage /= 1024;
	}

	m_systemLevel->setValue(m_usage);
	if(m_usage >= 90) {
		m_systemLevel->setColor(QColor(180,60,60));
	} else {
		m_systemLevel->setColor(QColor(60,180,60));
	}

	m_peakLevel->setValue(m_peak);
	/*if(m_usage >= 90) {
		m_systemLevel->setColor(QColor(180,60,60));
	} else {
		m_systemLevel->setColor(QColor(60,180,60));
	}*/

	m_memoryLevel->setValue(m_memory);
	if(m_memory >= 90) {
		m_memoryLevel->setColor(QColor(180,60,60));
	} else {
		m_memoryLevel->setColor(QColor(60,180,60));
	}

	m_bpmValue->setValue(samEngine->project()->tempo());

	update();

}