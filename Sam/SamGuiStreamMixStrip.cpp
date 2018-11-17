#include "SamGuiStreamMixStrip.h"

#include <QPainter>
#include <QMouseEvent>
#include <math.h>

#include "SamStreamMix.h"
#include "Sam.h"
#include "SamCommands.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiStreamMixStrip::SamGuiStreamMixStrip(QWidget *parent, int id)
	: QWidget(parent)
	, m_id(id)
	, m_channel(0)
	, m_stream(0)
{
	//ui.setupUi(this);

	setFixedSize(50,381+20);

	int y = 5;
	m_selectGroup = new UiSelectMenu(this, width()-8);
	m_selectGroup->move(4,y);
	QString grItems[13] = { "-", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L" };
	for(int i=0; i<13; i++) {
		m_selectGroup->addItem(grItems[i]);
	}
	connect(m_selectGroup, SIGNAL(itemSelected(int)), this, SLOT(setGroup(int)));
	y+= m_selectGroup->height();
	y+=2;


	m_labelMidiPort = new UiLabel(this, "");
	m_labelMidiPort->setImage(":/Resources/midi16.png");
	m_labelMidiPort->move(6,y);

	m_valueController = new UiValue(this, 20, 20, "", -1, 127, -1, 1, 2);
	m_valueController->move(26,y);
	m_valueController->setValueTextColor(QColor(220,220,220));
	connect(m_valueController, SIGNAL(valueChanged(float)), this, SLOT(setController(float)));

	y+=20;
	y+=5;

	y += 32; // Panner read out
	y += 2;

	m_panner = new UiPanner(this, 40);
	m_panner->move((width()/2)-(m_panner->width()/2)+1, y);
	connect(m_panner, SIGNAL(valueChanged(float, float)), this, SLOT(pannerValueChange(float, float)));
	y += 40;
	y += 5+1;

	m_buttonMute = new UiStateButton(this, 1000, 2);
	m_buttonMute->move(5, y);
	m_buttonMute->setFixedSize(16, 16);
	//m_buttonMute->setBackground();
	m_buttonMute->setStateImage(0, ":/Resources/mute16.png");
	m_buttonMute->setStateImage(1, ":/Resources/mute16.png");
	//buttonMute->setBackgroundColor(0,QColor(255,130,0,30));
	m_buttonMute->setStateFillColor(0,QColor(118,103,88));
	m_buttonMute->setStateFillColor(1,QColor(255,130,0));
	m_buttonMute->setStateLineColor(1,QColor(118,103,88));
	//m_buttonMute->setDrawTint();
	//m_buttonMute->setDrawOutline();
	
	connect(m_buttonMute, SIGNAL(clickedWithFlags(int, int)), this, SLOT(setMute(int, int)));

	m_buttonSolo = new UiStateButton(this, 1001, 2);
	m_buttonSolo->setFixedSize(16, 16);
	m_buttonSolo->move(29, y);
	//m_buttonSolo->setBackground();
	m_buttonSolo->setStateImage(0, ":/Resources/solo16.png");
	m_buttonSolo->setStateImage(1, ":/Resources/solo16.png");
	//buttonSolo->setBackgroundColor(0,QColor(255,230,0,30));
	m_buttonSolo->setStateFillColor(0,QColor(118,115,88));
	m_buttonSolo->setStateFillColor(1,QColor(255,230,0));
	m_buttonSolo->setStateLineColor(1,QColor(118,115,88));
	//m_buttonSolo->setDrawTint();
	//m_buttonSolo->setDrawOutline();
	
	connect(m_buttonSolo, SIGNAL(clickedWithFlags(int, int)), this, SLOT(setSolo(int, int)));
	

	m_fader = new UiFader(this, true, 150, 28, 21, id);
	m_fader->move((width()/2)-(m_fader->width()/2)-1, height()-54-m_fader->height()-20);
	m_fader->setTrackSize(7,7);
	//m_fader->setLevelColor(QColor(220,220,220));
	connect(m_fader, SIGNAL(valueChanged(float, int)), this, SLOT(faderValueChange(float, int)));

	m_buttonActive = new UiStateButton(this, 1002, 2);
	m_buttonActive->setFixedSize(40, 16);
	m_buttonActive->move(5, height() - 41);
	//m_buttonActive->setText("Active");
	m_buttonActive->setStateImage(0, ":/Resources/memory16.png");
	m_buttonActive->setStateImage(1, ":/Resources/memory16.png");
	
	m_buttonActive->setStateFillColor(0,QColor(23,59,25));
	m_buttonActive->setStateFillColor(1,QColor(70,175,70)); 
	m_buttonActive->setStateLineColor(1,QColor(38,107,42));
	
	//m_buttonActive->setStateFillColor(0,QColor(175,70,70)); 
	//m_buttonActive->setStateLineColor(0,QColor(108,40,40));
	// 255,170,0
	connect(m_buttonActive, SIGNAL(clickedWithFlags(int, int)), this, SLOT(setActive(int, int)));
	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiStreamMixStrip::~SamGuiStreamMixStrip()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixStrip::setStream(SamChannel *channel, SamStream *stream)
{
	m_channel = channel;
	m_stream = stream;
	if(m_stream) {
		int groupIndex = m_stream->group()+1; //aLog("***** groupIndex = %d", groupIndex);
		if(groupIndex < 0 || groupIndex > 12) groupIndex = 0; //aLog("     ***** groupIndex = %d", groupIndex);
		m_selectGroup->setSelectedItem(groupIndex);
		m_valueController->setValue(m_stream->midiGainController());
		m_fader->setValueNormalised(stream->gain());
		m_panner->setXNormalised(stream->panX());
		m_panner->setYNormalised(stream->panY());

		/*QColor groupColors[13] = { QColor(175,175,175),
			                       QColor(175, 70, 70), QColor( 70,175, 70), QColor( 70, 70,175),
								   QColor(175, 70,175), QColor(175,175, 70), QColor( 70,175,175),
								   QColor(175, 70, 70), QColor( 70,175, 70), QColor( 70, 70,175),
								   QColor(175, 70,175), QColor(175,175, 70), QColor( 70,175,175) };*/
		/*QColor groupColors[17] = { QColor(175,175,175),
									QColor(163,46,46),   QColor(163,46,104), QColor(104,46,163),
		                            QColor(46,46,163), QColor(26,84,143), QColor(26,143,143), QColor(26,143,84),
									QColor(74,143,26), QColor(143,143,26), QColor(143,113,26),
									QColor(143,84,26),QColor(143,55,26), QColor(215,24,0), QColor(215,98,0) };*/
	    //m_fader->setLevelColor(groupColors[groupIndex]);
		//m_selectGroup->setTextColor(groupColors[groupIndex]);
		m_buttonMute->setState((int)m_stream->mute());
		updateMute();
		m_buttonSolo->setState((int)m_stream->solo());

		int set = m_stream->setId();
		if(set >= 0 && set < m_channel->instrument()->elementPool()->numSets()) {
			m_buttonActive->setState((bool)m_channel->instrument()->elementPool()->set(set)->isActive());
		}
	}
	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixStrip::updateFader(void)
{
	if(m_stream) {
		m_fader->setValueNormalised(m_stream->gain());
	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixStrip::updatePan(void)
{
	if(m_stream) {
		m_panner->setXNormalised(m_stream->panX());
		m_panner->setYNormalised(m_stream->panY());
	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixStrip::updateMute(void)
{
	if(m_stream) {
		m_buttonMute->setState((int)m_stream->mute());
		if(m_stream->mute()) {
			m_fader->setLevelColor(QColor(70,70,70));
		} else {
			m_fader->setLevelColor(QColor(70,175,70));
		}
	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixStrip::updateSolo(void)
{
	if(m_stream) {
		m_buttonSolo->setState((int)m_stream->solo());
	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixStrip::updateActive(void)
{
	if(m_stream && m_channel) {
		int set = m_stream->setId();
		if(set >= 0 && set < m_channel->instrument()->elementPool()->numSets()) {
			m_buttonActive->setState((bool)m_channel->instrument()->elementPool()->set(set)->isActive());
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixStrip::faderValueChange(float value, int flags)
{
	//aLog("SamGuiStreamMixStrip::faderValueChange(%f, %d)", value, flags);
	if(m_channel) {
		SamEvent ev(SamEvent::Parameter);
		ev.subType = SamParameter::StreamGain;
		ev.timestamp = 0;
		ev.iValue = m_id;
		ev.fValue = value;
		ev.from = this;
		if(flags == 0) {
			m_channel->addEvent(ev);
		} else if(flags == 1) {
			for(int i=0; i<m_channel->streamMix()->numStreams(); i++) {
				ev.iValue = i;
				m_channel->addEvent(ev);
			}
		}
	}
	//update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixStrip::pannerValueChange(float x, float y)
{
	//qDebug("SamGuiStreamMixStrip::pannerValueChange(%f, %f)", x, y);
	if(m_channel) {
		SamEvent ev(SamEvent::Parameter);
		ev.subType = SamParameter::StreamPanX;
		ev.timestamp = 0;
		ev.iValue = m_id;
		ev.fValue = x;
		m_channel->addEvent(ev);

		ev.subType = SamParameter::StreamPanY;
		ev.timestamp = 0;
		ev.fValue = y;
		m_channel->addEvent(ev);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixStrip::setController(float value)
{
	aLog("SamGuiStreamMixStrip::setController(%d)", (int)value);

	m_stream->setMidiGainController((int)value);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixStrip::setGroup(int index)
{
	aLog("SamGuiStreamMixStrip::setGroup(%d)", index);

	m_stream->setGroup(index-1);

	/*QColor groupColors[17] = { QColor(175,175,175),
									QColor(163,46,46),   QColor(163,46,104), QColor(104,46,163),
		                            QColor(46,46,163), QColor(26,84,143), QColor(26,143,143), QColor(26,143,84),
									QColor(74,143,26), QColor(143,143,26), QColor(143,113,26),
									QColor(143,84,26),QColor(143,55,26), QColor(215,24,0), QColor(215,98,0) };*/
	    //m_fader->setLevelColor(groupColors[index]);
	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixStrip::setMute(int id, int flags)
{
	//aLog("SamGuiStreamMixStrip::setMute(%d)", id);

	if(m_channel) {
		SamEvent ev(SamEvent::Parameter);
		ev.subType = SamParameter::StreamMute;
		ev.timestamp = 0;
		ev.iValue = m_id;
		ev.fValue = !m_stream->mute();
		ev.from = this;
		if(flags == 0) {
			m_channel->addEvent(ev);
		} else if(flags == 1) {
			for(int i=0; i<m_channel->streamMix()->numStreams(); i++) {
				ev.iValue = i;
				m_channel->addEvent(ev);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixStrip::setSolo(int id, int flags)
{
	if(m_channel) {
		SamEvent ev(SamEvent::Parameter);
		ev.subType = SamParameter::StreamSolo;
		ev.timestamp = 0;
		ev.iValue = m_id;
		ev.fValue = !m_stream->solo();
		ev.from = this;
		if(flags == 0) {
			m_channel->addEvent(ev);
		} else if(flags == 1) {
			for(int i=0; i<m_channel->streamMix()->numStreams(); i++) {
				ev.iValue = i;
				m_channel->addEvent(ev);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixStrip::setSetName(QString name)
{
	qDebug("SamGuiStreamMixStrip::setSetName(%s)", qPrintable(name));

	disconnect(samGui->textFieldDialog(), SIGNAL(editConfirmed(QString)), this, SLOT(setSetName(QString)));

	if(name.length() > 0) {
		SamInstrument *instrument = m_channel->instrument();
		int set = m_stream->setId();
		if(set >= 0 && set < instrument->elementPool()->numSets()) {
			QByteArray ba = name.toAscii();
			//instrument->elementPool()->set(set)->setName(ba.data());
			samGui->commandManager()->executeInBackground(new SetSetName(instrument->elementPool()->set(set), ba.data()));
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixStrip::setActive(int id, int flags)
{
	qDebug("SamGuiStreamMixStrip::setActive()");

	SamInstrument *instrument = m_channel->instrument();
	int set = m_stream->setId();
	if(set >= 0 && set < instrument->elementPool()->numSets()) {
		instrument->elementPool()->set(set)->setActive(!instrument->elementPool()->set(set)->isActive());
		samGui->commandManager()->executeInBackground(new UpdateSetElementStates(instrument, set, instrument->elementPool()->set(set)->isActive()));
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixStrip::mouseDoubleClickEvent(QMouseEvent *ev)
{
	if(ev->y() > height()-21) {
		SamInstrument *instrument = m_channel->instrument();
		int set = m_stream->setId();
		if(set >= 0 && set < instrument->elementPool()->numSets()) {
			samGui->textFieldDialog()->setText(instrument->elementPool()->set(set)->name().toChar());
			samGui->textFieldDialog()->move(mapToGlobal(QPoint(0, height()-21-samGui->textFieldDialog()->height())));
			connect(samGui->textFieldDialog(), SIGNAL(editConfirmed(QString)), this, SLOT(setSetName(QString)));
			samGui->textFieldDialog()->show();
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixStrip::paintEvent(QPaintEvent *ev)
{
	//aLog("SamGuiStreamMixStrip::paintEvent()");

	if(!m_stream) {
		aLog("Should not be here.");
		return;
	}

	QPainter p(this);

	QColor backgroundColor[2] = {
		QColor(70, 70, 70),
		QColor(90, 90, 90)
	};

	// Background
	int cIndex = 0;
	if (m_id%2==0)
		cIndex = 1;
	/*p.setBrush (backgroundColor[cIndex]);
	p.setPen (backgroundColor[cIndex]);
	p.drawRect (0,0,width()-1, height()-1);*/
	
	p.setBrush (QColor(0,0,0));
	QColor groupColors[17] = { QColor(100,100,100),
									QColor(163,46,46),   QColor(163,46,104), QColor(104,46,163),
		                            QColor(46,46,163), QColor(26,84,143), QColor(26,143,143), QColor(26,143,84),
									QColor(74,143,26), QColor(143,143,26), QColor(143,113,26),
									QColor(143,84,26),QColor(143,55,26), QColor(215,24,0), QColor(215,98,0) };
	int groupIndex = m_stream->group()+1; //aLog("***** groupIndex = %d", groupIndex);
		if(groupIndex < 0 || groupIndex > 12) groupIndex = 0;
	QColor bc = groupColors[groupIndex];

	SamStreamMix *ssm = (SamStreamMix*)m_stream->parent();
	if(m_stream->mute() || (ssm->isSolo() && !m_stream->solo())) {
		bc = QColor(0,0,0);
	}
	bc.setAlpha(150);
		p.setBrush(bc);
	p.setPen (QColor(100,100,100));
	p.drawRect (0,0,width()-1, height()-1);

	QColor col = backgroundColor[cIndex];
	col = QColor(80,80,80);
	QLinearGradient lgBack(QPointF(0, 0), QPointF(width(), 0));
	col.setAlpha(180);//240//90 // 230
	lgBack.setColorAt(0, col);
	col.setAlpha(200);//255//50 // 255
	lgBack.setColorAt(1, col);

	//p.setBrush(lgBack);
	//p.setPen(QColor(0,0,0,0));
	//p.drawRect (0,0,width()-1, height()-1);
	p.fillRect(1,1,width()-2,height()-2,lgBack);
	p.setPen(QColor(0,0,0));
	p.drawLine(width()-1,0,width()-1,height()-1);

	// Stream Number
	p.setFont(QFont("Calibri", 18, QFont::Bold));
	p.setPen(QColor(255,255,255,40));
	p.drawText(2, height()-55-20, width()-3, 34, Qt::AlignTop|Qt::AlignCenter, QString::number(m_id+1));

	p.fillRect(0,height()-21,width()-1,21,QColor(0,0,0,100));
	SamInstrument *instrument = m_channel->instrument();
	int set = m_stream->setId();
	if(set >= 0 && set < instrument->elementPool()->numSets()) {
		//p.setFont(QFont("", 8));//, QFont::Bold));
		//p.setPen(QColor(255,255,255,200));
		//p.drawText(2, height()-21, width()-3, 20, Qt::AlignCenter|Qt::AlignVCenter, instrument->elementPool()->set(set)->name().toChar());

		QFont f("",0,QFont::Bold);
		f.setPixelSize(9); //9
		f.setStyleStrategy(QFont::PreferAntialias);
		p.setFont(f);
		p.setPen(QColor(255,255,255,200));
		p.drawText(2, height()-20, width()-3, 20, Qt::AlignCenter|Qt::AlignVCenter, instrument->elementPool()->set(set)->name().toChar());
	}

	QColor color(255,255,255);
	QLinearGradient linearGradient(QPointF(0, height()-21), QPointF(0, height()));
	color.setAlpha(40);//90
	linearGradient.setColorAt(0, color);
	color.setAlpha(5);//50
	linearGradient.setColorAt(1, color);

	//p.setBrush(linearGradient);
	//p.setPen(QColor(0,0,0,0));
	//p.drawRect(0,height()-21,width()-1,10);

	p.setBrush(QColor(00,00,00));
	p.setPen(QColor(00,00,00));
	p.drawRect(5, m_fader->y()-16-10, width()-10-1, 16);


	p.setPen(QColor(255,255,255));
	p.setFont(QFont("", 7));
	QString dB = QString::number(20*log10(m_stream->gain()), 'f', 1);
	if(m_stream->gain() < (float)0.000001f) {
		dB = "-inf";
	}
	p.drawText(5, m_fader->y()-16-10, width()-10+0, 16+1, Qt::AlignCenter|Qt::AlignVCenter, dB);
	linearGradient.setStart(QPoint(0, m_fader->y()-16-10));
	linearGradient.setFinalStop(QPoint(0, m_fader->y()-10));
	p.fillRect(6, 1+m_fader->y()-16-10, width()-10-1-1, (16/2)-1, linearGradient);



	p.setPen(QColor(0,0,0,100));
	p.drawLine(4,m_fader->y()+50-1, width()-1-4, m_fader->y()+50-1);
	p.setPen(QColor(255,255,255,75));
	p.drawLine(4,m_fader->y()+50, width()-1-4, m_fader->y()+50);
	

	//p.drawText(width()-21, m_fader->y()+50-11, 17, 10, Qt::AlignRight|Qt::AlignTop, "-6");


	int piy = 5+20+2+20+5;
	p.fillRect(5, piy, width()-10, 32, QColor(0,0,0));
	p.setPen(QColor(120,120,120));
	p.setFont(QFont("", 7));
	p.drawText(9, piy, width()-10+1, 16+1, Qt::AlignLeft|Qt::AlignVCenter, "X");
	p.drawText(9, piy+16, width()-10+1, 16+1, Qt::AlignLeft|Qt::AlignVCenter, "Y");
	p.setPen(QColor(255,255,255));

	QString xt, yt;
	if(m_stream->panX() == 0.0f) xt = "Left";
	else if(m_stream->panX() == 0.5f) xt = "Cent";
	else if(m_stream->panX() == 1.0f) xt = "Rght";
	else {
		if(m_stream->panX() < 0.5) {
			xt = "L";
			float n = m_stream->panX() * 2;
			n = 1 - n;
			n*=100;
			if((int)n==0) { xt = "Cent"; }
			else { xt += QString::number((int)n); }
		} else if(m_stream->panX() > 0.5) {
			xt = "R";
			float n = m_stream->panX() - 0.5;
			n *= 2;
			n *= 100;
			xt += QString::number((int)n);
		}
	}

	if(m_stream->panY() == 0.0f) yt = "Frnt";
	else if(m_stream->panY() == 0.5f) yt = "Cent";
	else if(m_stream->panY() == 1.0f) yt = "Back";
	else {
		if(m_stream->panY() < 0.5) {
			yt = "F";
			float n = m_stream->panY() * 2;
			n = 1 - n;
			n*=100;
			if((int)n==0) { yt = "Cent"; }
			else { yt += QString::number((int)n); }
		} else if(m_stream->panY() > 0.5) {
			yt = "B";
			float n = m_stream->panY() - 0.5;
			n *= 2;
			n *= 100;
			yt += QString::number((int)n);
		}
	}
	p.drawText(9, piy, width()-18+1, 16+1, Qt::AlignRight|Qt::AlignVCenter, xt);
	p.drawText(9, piy+16, width()-18+1, 16+1, Qt::AlignRight|Qt::AlignVCenter, yt);

	//linearGradient.setStart(QPoint(0, piy));
	//linearGradient.setFinalStop(QPoint(0, piy+32));
	//p.fillRect(6, piy+1, width()-10-2, (32/2)-1, linearGradient);

	p.setPen(QColor(0,0,0,60));
	p.drawLine(1, piy-4, width()-2, piy-4);

	p.drawLine(1, piy+32+2+40+2, width()-2, piy+32+2+40+2); 


	//aLog("EXIT: SamGuiStreamMixStrip::paintEvent()");
}