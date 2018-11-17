#include "SamGuiPianoController.h"

#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>

#include "Sam.h"
#include "SamEngine.h"

SamGuiPianoController::SamGuiPianoController(QWidget *parent)
	: UiPianoKeyboard(parent, 8, 60, 128, 0)
	, m_channel(0)
	, m_attenuationMode(false)
	, m_attenuationStart(firstMidiNote())
	, m_attenuationEnd(firstMidiNote()+numPitches())
	, m_attenuationFadeIn(0)
	, m_attenuationFadeOut(0)
	, amAdjust(-1)
	, clickedOnNote(0)
	, leftButtonPressed(false)
{
	//ui.setupUi(this);
	//m_pianoKeyboard = new UiPianoKeyboard(this, 9, 60);

	pixFadeIn = new QPixmap(":/Resources/fadein10.png");
	pixFadeOut = new QPixmap(":/Resources/fadeout10.png");
	pixArrowLeft = new QPixmap(":/Resources/arrowleft10.png");
	pixArrowRight = new QPixmap(":/Resources/arrowright10.png");
	pixArrowLeftRight = new QPixmap(":/Resources/arrowleftright10.png");

	guiCurveIn = new UiCurve(this);
	guiCurveIn->setDepth(0.5f);
	guiCurveIn->setShape(3);
	guiCurveIn->hide();
	connect(guiCurveIn, SIGNAL(depthAdjusted(float)), this, SLOT(makeInQAdjustment(float)));

	guiCurveOut = new UiCurve(this);
	guiCurveOut->setStart(1.0f);
	guiCurveOut->setEnd(0.0f);
	guiCurveOut->setDepth(0.5f);
	guiCurveOut->setShape(3);
	guiCurveOut->hide();
	connect(guiCurveOut, SIGNAL(depthAdjusted(float)), this, SLOT(makeOutQAdjustment(float)));

	//connect(samGui->mainWidget(), SIGNAL(timerTick()), this, SLOT(updateActivePitches()));

	//samEngine->registerListener(this, SamEngine::ContextMidiInput);
	samEngine->registerListener(this, SamEngine::ContextChannel);
	connect(this, SIGNAL(midiReceived(int, int)), this, SLOT(midiEvent(int, int)));

	connect(this, SIGNAL(pitchPressed(int, int, int)), this, SLOT(createNoteEvent(int, int, int)));
	connect(this, SIGNAL(pitchReleased(int, int, int)), this, SLOT(createNoteEvent(int, int, int)));
	connect(this, SIGNAL(pitchAftertouch(int, int)), this, SLOT(createPolyAftEvent(int, int)));

	for(int i=0; i<128; i++) {
		m_pitchActive[i] = false;
	}
}

SamGuiPianoController::~SamGuiPianoController()
{

}

void SamGuiPianoController::onState(AStateEvent *ev)
{
	//qDebug("SamGuiChannel::onState()");

	//if(ev->context() == SamEngine::ContextMidiInput) {
	//	emit midiReceived(ev->item(), (int)ev->value());
	//}

	if(ev->context() == SamEngine::ContextChannel && ev->updatedObject() == m_channel) {
		emit midiReceived((int)ev->value(), m_channel->midiInputPort());
	}
}

void SamGuiPianoController::setAttenuationMode(bool enabled)
{
	m_attenuationMode = enabled;
	setChannel(m_channel);
}

void SamGuiPianoController::updateActivePitches(void)
{
	//qDebug("SamGuiPianoController::updateActivePitches()");
	/*if(m_channel) {
		bool somethingChanged = false;
		for(int i=0; i<128; i++) {
			if(m_pitchDown[i] != m_channel->velocity(i)) {
				m_pitchDown[i] = m_channel->velocity(i);
				somethingChanged = true;
			}
		}
		if(somethingChanged) {
			update();
		}
	}*/
}

void SamGuiPianoController::setChannel(SamChannel *channel)
{
	qDebug("SamGuiKeyboard::setChannel()");

	m_channel = channel;

	if(m_channel) {
		SamVersion *version = m_channel->version();

		if(version) {

			for (int i=0; i<128; i++) {
				m_pitchActive[i] = false;
				//m_pitchUsed[i] = 0;

				if (version->isPitchValid(m_channel->currentArticulationIndex(), -1, i)) {
					m_pitchActive[i] = true; 
				//} else if (channel->isPitchATriggerKey (i)) { 
				//	m_pitchUsed[i] = 2;
				}
			}
		}

		QColor midiChannelColor[17] = { QColor(160,160,160),
								QColor(143,26,26),   QColor(143,26,84), QColor(143,26,143), QColor(84,26,143),
	                            QColor(26,26,143), QColor(26,84,143), QColor(26,143,143), QColor(26,143,84),
								QColor(89,153,0),QColor(74,143,26), QColor(143,143,26), QColor(143,113,26),
								QColor(143,84,26),QColor(143,55,26), QColor(215,24,0), QColor(215,98,0) };

		int channelColorIndex = m_channel->midiInputChannel();
		setNoteDownColor(midiChannelColor[channelColorIndex]);
		
		m_attenuationStart = m_channel->attenuationStart();
		m_attenuationEnd = m_channel->attenuationEnd();
		m_attenuationFadeIn = m_channel->attenuationFadeIn();
		m_attenuationFadeOut = m_channel->attenuationFadeOut();
		guiCurveIn->setDepth(m_channel->attenuationFadeInQ());
		guiCurveOut->setDepth(m_channel->attenuationFadeOutQ());
		if(m_attenuationMode) {
			guiCurveIn->show();
			guiCurveOut->show();
		}

	} else {
		for (int i=0; i<128; i++) {
			//m_pitchUsed[i] = 0;
			m_pitchActive[i] = false;
		}
		guiCurveIn->hide();
		guiCurveOut->hide();
	}

	update();
	if(m_attenuationMode) {
		updateCurves();
	}
}

void SamGuiPianoController::makeInQAdjustment(float q)
{
	qDebug("SamGuiKeyboard::makeQAdjustment()");

	if(m_channel) {
		//m_channel->setAttenuationForMidiNotes(m_attenuationStart, m_attenuationEnd, m_attenuationFadeIn, m_attenuationFadeOut, guiCurveIn->q(), guiCurveOut->q());
		m_channel->setAttenuationFadeInQ(q);
		m_channel->updateAttenuationsForMidiNotes();
	}
}

void SamGuiPianoController::makeOutQAdjustment(float q)
{
	qDebug("SamGuiKeyboard::makeQAdjustment()");

	if(m_channel) {
		//m_channel->setAttenuationForMidiNotes(m_attenuationStart, m_attenuationEnd, m_attenuationFadeIn, m_attenuationFadeOut, guiCurveIn->q(), guiCurveOut->q());
		m_channel->setAttenuationFadeOutQ(q);
		m_channel->updateAttenuationsForMidiNotes();
	}
}

void SamGuiPianoController::midiEvent(int ev, int port)
{
	//qDebug("SamGuiChannel::midiEvent(%d, %d)", ev, port);

	if(!m_channel) {
		return;
	}

	if(port != m_channel->midiInputPort()) {
		//qDebug("No port (%d != %d)", port, m_channel->midiInputPort());
		return;
	}

	unsigned char status = ev & 0x000000FF;

	//qDebug("%d = %d", status & 0x0F, m_channel->midiInputChannel());
	if((status & 0x0F)+1 == m_channel->midiInputChannel() || m_channel->midiInputChannel() ==0 ) {
		unsigned char data1  = (ev>>8) & 0x000000FF;
		unsigned char data2  = (ev>>16) & 0x000000FF;

		unsigned char type = status & 0xF0;

		if(type == 0x90 || type == 0x80) {
			//updateActivePitches();
			if(data1 >= 0 && data1 < 128) {
				m_pitchDown[data1] = data2;
				if(type == 0x80) m_pitchDown[data1] = 0;
				m_aftertouch[data1] = 0;
				update();
			}
		} else if(type == 0xA0) {
			if(data1 >= 0 && data1 < 128) {
				m_aftertouch[data1] = data2;
				update();
			}
		}
	}
}

void SamGuiPianoController::createNoteEvent(int pitch, int velocity, int modifiers)
{
	//qDebug("SamGuiPianoController::createMidiEvent(%d, %d, %d)", pitch, velocity, modifiers);

	if(m_channel) {
		CsMidiMessage msg(CsMidiMessage::NoteOn);
		if(velocity == 0) {
			msg.type = CsMidiMessage::NoteOff;
		}
		msg.device = 1;
		msg.port = m_channel->midiInputPort();
		msg.timestamp = timeGetTime() * 1000000.0f;
		msg.data[0] = 0x90+m_channel->midiInputChannel()-1;
		msg.data[1] = pitch;
		msg.data[2] = velocity;
		msg.channel = m_channel->midiInputChannel();
		msg.length = 3;
		m_channel->processMidiMessage(msg);
		m_pitchDown[pitch] = velocity;
		update();
	}
}

void SamGuiPianoController::createPolyAftEvent(int pitch, int aftertouch)
{
	//qDebug("SamGuiPianoController::createPolyAftEvent(%d, %d)", pitch, aftertouch);

	if(m_channel) {
		CsMidiMessage msg(CsMidiMessage::Aftertouch);
		msg.device = 1;
		msg.port = m_channel->midiInputPort();
		msg.timestamp = timeGetTime() * 1000000.0f;
		msg.data[0] = 0xA0+m_channel->midiInputChannel()-1;
		msg.data[1] = pitch;
		msg.data[2] = aftertouch;
		msg.channel = m_channel->midiInputChannel();
		msg.length = 3;
		m_channel->processMidiMessage(msg);
		m_aftertouch[pitch] = aftertouch;
		update();
	}
}

void SamGuiPianoController::mouseMoveEvent(QMouseEvent *ev)
{
	if(m_attenuationMode && m_channel) {
		switch(amAdjust) {
			case 0:
				m_attenuationStart = firstMidiNote()+(ev->x()/pitchWidth());
				if(m_attenuationStart<firstMidiNote()) {
					m_attenuationStart = firstMidiNote();
				}
				if(m_attenuationStart+m_attenuationFadeIn>m_attenuationEnd-m_attenuationFadeOut-1) {
					m_attenuationStart = m_attenuationEnd - m_attenuationFadeIn - m_attenuationFadeOut - 1;
				}
				if(m_attenuationStart>m_attenuationEnd-3) {
					m_attenuationStart = m_attenuationEnd-3;
				}
				break;
			case 1:
				m_attenuationEnd = firstMidiNote()+(ev->x()/pitchWidth());
				if(m_attenuationEnd>firstMidiNote()+numPitches()) {
					m_attenuationEnd = firstMidiNote()+numPitches();
				}
				if(m_attenuationEnd-m_attenuationFadeOut<m_attenuationStart+m_attenuationFadeIn + 1) {
					m_attenuationEnd = m_attenuationFadeOut+m_attenuationStart+m_attenuationFadeIn+1;
				}
				//if(m_attenuationEnd) {
				//}
				if(m_attenuationEnd<m_attenuationStart+3) {
					m_attenuationEnd = m_attenuationStart+3;
				}
				break;
			case 2:
				m_attenuationFadeIn = (ev->x()/pitchWidth())-(m_attenuationStart-firstMidiNote());
				if(m_attenuationFadeIn<0)
					m_attenuationFadeIn = 0;
				if(m_attenuationStart+m_attenuationFadeIn>m_attenuationEnd-m_attenuationFadeOut-1) {
					m_attenuationFadeIn = m_attenuationEnd - m_attenuationFadeOut - m_attenuationStart - 1;
				}
				break;
			case 3:
				m_attenuationFadeOut = (m_attenuationEnd-firstMidiNote())-(ev->x()/pitchWidth());
				if(m_attenuationFadeOut<0) {
					m_attenuationFadeOut = 0;
				}
				if(m_attenuationEnd-m_attenuationFadeOut-1<m_attenuationStart+m_attenuationFadeIn) {
					m_attenuationFadeOut = (m_attenuationEnd-m_attenuationStart)-m_attenuationFadeIn-1;
				}
				break;

		}
		update();
		updateCurves();
		
	} else {
		UiPianoKeyboard::mouseMoveEvent(ev);
	}
}

void SamGuiPianoController::mousePressEvent(QMouseEvent *ev)
{
	clickedOnNote = pitchFromPoint(ev->x(), ev->y());
	leftButtonPressed = false;

	if(m_attenuationMode  && m_channel && ev->buttons()==Qt::LeftButton) {

		if(ev->y()>height()/2) { // Adjust start and end points
			
			if(ev->x()<((m_attenuationStart-firstMidiNote())*pitchWidth())+pitchWidth()) {
				amAdjust = 0;
			} else if(ev->x()>((m_attenuationEnd-firstMidiNote())*pitchWidth())-pitchWidth()) {
				amAdjust = 1;
			}

		} else { // Adjust fade in and out points

			if(ev->x()<((m_attenuationFadeIn+(m_attenuationStart-firstMidiNote()))*pitchWidth())+pitchWidth()) {
				amAdjust = 2;
			} else if(ev->x()>(((m_attenuationEnd-m_attenuationFadeOut)-firstMidiNote())*pitchWidth())-pitchWidth()) {
				qDebug("amAdjust=3");
				amAdjust = 3;
			}

		}

	} else {

		UiPianoKeyboard::mousePressEvent(ev);
		/* else if (event->buttons()==Qt::RightButton && m_currentChannelId>-1) {
			m_leftButtonPressed = false;
			SamChannel *channel = SamEngine::instance()->getChannel (m_currentChannelId);
			QString text = "";
			if (channel->isPitchValid (pitch)) {
				QString name = channel->getZoneName(pitch,127);
				QString description = channel->getZoneDescription(pitch,127);
				if (name=="")
					text = "<b>Instrument Key</b><br>";
				else
					text = "<b>Instrument Key (" + name + ")</b><br>";
					
				if (description=="")
					text += "No description available.";
				else
					text += description;
				text += "<br><br>(An Instrument Key triggers a note within the Instrument.)";
				
			} else if (channel->isPitchATriggerKey (pitch)) { 
				text = "<b>Trigger Key</b><br>";
				text += "No description available.";
				text += "<br><br>(A Trigger Key sets one or more parameters within an Instrument when pressed.)";
			} else {
				text = "<b>Unassigned Key</b><br>";
				text += "This key has no function assigned, so nothing will happen when it's pressed.";
			}
			QToolTip::showText (mapToGlobal(QPoint(event->x(),event->y())),text, this);
		} *///else {
		//	leftButtonPressed = false;
		//}

	}
}

void SamGuiPianoController::mouseReleaseEvent(QMouseEvent *ev)
{
	if(m_attenuationMode && m_channel) {
		if(m_channel) {
			m_channel->setAttenuationForMidiNotes(m_attenuationStart, m_attenuationEnd, m_attenuationFadeIn, m_attenuationFadeOut);
		}
	} else {
		UiPianoKeyboard::mouseReleaseEvent(ev);
	}

	amAdjust = -1;
}

void SamGuiPianoController::paintEvent(QPaintEvent *ev)
{
	UiPianoKeyboard::paintEvent(ev);

	if(m_attenuationMode && m_channel) {
		
		QPainter p(this);

		p.setRenderHint(QPainter::Antialiasing);
		//p.setPen(QColor(255,255,255,50));
		//p.setBrush(QColor(255,255,255,50));
		//p.drawRect(0,0,width()-1,height()-1);

		int attenuationRange = (m_attenuationEnd-m_attenuationStart);

		p.setPen(QColor(0,0,0));
		p.setBrush(QColor(50,100,50,100));
		p.drawRect ((m_attenuationStart-firstMidiNote())*pitchWidth(),0,attenuationRange*pitchWidth(), height()-1);

		QPen pen(QColor(255,255,255,200));
		pen.setWidth(2);
		p.setPen(pen);
		if(m_attenuationFadeIn > 0) {
			//p.drawLine(1+((m_attenuationStart-firstMidiNote())*pitchWidth()), height()-2, (m_attenuationFadeIn+(m_attenuationStart-firstMidiNote()))*pitchWidth(), 1);
			//p.drawLine((m_attenuationFadeIn+(m_attenuationStart-firstMidiNote()))*pitchWidth(), height()-2,(m_attenuationFadeIn+(m_attenuationStart-firstMidiNote()))*pitchWidth(), 1);
		}

		if(m_attenuationFadeOut > 0) {
			//p.drawLine((m_attenuationEnd-firstMidiNote())*pitchWidth(), height()-2, ((m_attenuationEnd-m_attenuationFadeOut)-firstMidiNote())*pitchWidth(), 1);
			//p.drawLine(((m_attenuationEnd-m_attenuationFadeOut)-firstMidiNote())*pitchWidth(), height()-2,((m_attenuationEnd-m_attenuationFadeOut)-firstMidiNote())*pitchWidth(), 1);
		}

		//if(m_attenuationFadeIn > 0 || m_attenuationFadeOut > 0) {
			p.drawLine((m_attenuationFadeIn+(m_attenuationStart-firstMidiNote()))*pitchWidth(), 1, ((m_attenuationEnd-m_attenuationFadeOut)-firstMidiNote())*pitchWidth(), 1);
		//}

		//if(m_attenuationStart == firstMidiNote()) {
		//	p.drawPixmap(2+(m_attenuationStart-firstMidiNote())*pitchWidth(), height()-12, *pixArrowRight);
		//} else {
			p.drawPixmap(2+(m_attenuationStart-firstMidiNote())*pitchWidth(), height()-12, *pixArrowLeftRight);
		//}

		p.drawPixmap(((m_attenuationEnd-firstMidiNote())*pitchWidth())-12, height()-12, *pixArrowLeftRight);

		p.drawPixmap((m_attenuationFadeIn+(m_attenuationStart-firstMidiNote()))*pitchWidth(), 2, *pixFadeIn);
		p.drawPixmap((((m_attenuationEnd-m_attenuationFadeOut)-firstMidiNote())*pitchWidth())-12,2,*pixFadeOut);

		
	}//*/
}

void SamGuiPianoController::resizeEvent(QResizeEvent *ev)
{
	float newWidth = (float)ev->size().width();
	float npw = newWidth / (float)numPitches();
	setPitchWidth(npw);
	setFixedSize(pitchWidth() * numPitches(), 60);
	updateCurves();
}

void SamGuiPianoController::updateCurves(void)
{
	guiCurveIn->move((m_attenuationStart-firstMidiNote())*pitchWidth(),0);
	guiCurveIn->setFixedSize(m_attenuationFadeIn*pitchWidth(), height());
	guiCurveIn->update();

	guiCurveOut->move(((m_attenuationEnd-m_attenuationFadeOut)-firstMidiNote())*pitchWidth(),0);
	guiCurveOut->setFixedSize(m_attenuationFadeOut*pitchWidth(), height());
	guiCurveOut->update();
}