#include "SamGuiChannel.h"

#include <QPainter>
#include <QMouseEvent>

#include "SamEngine.h"
#include "SamProject.h"

#include "Sam.h"

SamGuiChannel::SamGuiChannel(QWidget *parent, SamChannel *channel)
	: UiListItem(parent, 44)
	, m_channel(channel)
	, m_mouseOver(false)
{
	//ui.setupUi(this);

	if(m_channel->label().length() > 0) {
		m_label = new UiLabel(this, m_channel->label().toChar(), height()/2, 260-16);
	} else {
		m_label = new UiLabel(this, m_channel->version()->name().toChar(), height()/2, 260-16);
	}
	
	m_label->move(60,2);
	m_label->setEditable();
	m_label->setTextSize(16);
	connect(m_label, SIGNAL(textChanged(QString)), this, SLOT(updateLabel(QString)));

	int x = 2;
	m_indicatorNote = new UiIndicator(this, 8, height()-4);
	m_indicatorNote->move(341, 2); //(x,2); 
	m_indicatorNote->setColor(QColor(255,255,255));

	x += ((350+8)-2);

	labelMidiPort = new UiLabel(this, "");
	labelMidiPort->setImage(":/Resources/midi16.png");
	labelMidiPort->move(x,3);

	labelMidiChannel = new UiLabel(this, "");
	labelMidiChannel->setImage(":/Resources/midichannelsolid16.png");
	labelMidiChannel->move(x,(height()/2)+2);

	x += 20; //(350+28) -2

	selectMidiInput = new SamGuiSelectMidiInputBus(this, channel);
	selectMidiInput->setDrawFill(false);
	selectMidiInput->setDrawTint(false);
	selectMidiInput->setDrawLine(false);
	selectMidiInput->setTextBold(false);
	selectMidiInput->setTextColor(QColor(200,200,200));
	selectMidiInput->move(x, 2);
	//selectMidiInput->setSelectedItem(m_channel->midiInputPort());
	selectMidiInput->setParentChannel(m_channel);

	selectMidiChannel = new UiSelectMidiChannel(this);
	selectMidiChannel->setDrawFill(false);
	selectMidiChannel->setDrawTint(false);
	selectMidiChannel->setDrawLine(false);
	selectMidiChannel->move(x, 22); //22
	selectMidiChannel->setSelectedItem(m_channel->midiInputChannel());
	selectMidiChannel->setFixedWidth(160);

	//connect(selectMidiPort, SIGNAL(itemSelected(int)), this, SLOT(setMidiPort(int)));
	connect(selectMidiChannel, SIGNAL(itemSelected(int)), this, SLOT(setMidiChannel(int)));
	//connect(this, SIGNAL(midiPortUpdated()), this, SLOT(updateMidiPort()));
	connect(this, SIGNAL(midiChannelUpdated()), this, SLOT(updateMidiChannel()));
	//connect(this, SIGNAL(updated()), this, SLOT(update()));

	x += 164;

	buttonMidiMuted = new UiStateButton(this, 1000, 2);
	buttonMidiMuted->move(x, 4);
	buttonMidiMuted->setFixedSize(16, 16);
	//buttonMidiMuted->setBackground();
	buttonMidiMuted->setStateImage(0, ":/Resources/panic16.png");
	buttonMidiMuted->setStateImage(1, ":/Resources/panic16.png");
	//buttonMidiMuted->setText("MIDI Input Mute");
	//buttonMidiMuted->setBackgroundColor(0,QColor(255,130,0,30));
	buttonMidiMuted->setStateFillColor(0,QColor(150,100,100));
	buttonMidiMuted->setStateFillColor(1,QColor(250,100,100));
	buttonMidiMuted->setStateLineColor(1,QColor(150,100,100));
	//buttonMidiMuted->setDrawTint();
	//buttonMidiMuted->setDrawOutline();
	connect(buttonMidiMuted, SIGNAL(clickedWithFlags(int, int)), this, SLOT(setMidiMuted(int, int)));
	if(m_channel->isMidiMuted()) {
		buttonMidiMuted->setState(1);
	}

	m_buttonDownbeatOffset = new UiStateButton(this, 1000, 2);
	m_buttonDownbeatOffset->move(x, 24);
	m_buttonDownbeatOffset->setFixedSize(16, 16);
	//m_buttonDownbeatOffset->setBackground();
	m_buttonDownbeatOffset->setStateImage(0, ":/Resources/downbeatoffsetb16.png");
	m_buttonDownbeatOffset->setStateImage(1, ":/Resources/downbeatoffsetb16.png");
	//m_buttonDownbeatOffset->setText("Downbeat Offset");
	//m_buttonDownbeatOffset->setBackgroundColor(0,QColor(255,130,0,30));
	m_buttonDownbeatOffset->setStateFillColor(0,QColor(100,75,150));
	m_buttonDownbeatOffset->setStateFillColor(1,QColor(170,70,255));
	m_buttonDownbeatOffset->setStateLineColor(1,QColor(100,75,150));
	//m_buttonDownbeatOffset->setDrawTint();
	//m_buttonDownbeatOffset->setDrawOutline();
	connect(m_buttonDownbeatOffset, SIGNAL(clickedWithFlags(int, int)), this, SLOT(setDownbeatOffset(int, int)));
	if(m_channel->isDownbeatOffset()) {
		m_buttonDownbeatOffset->setState(1);
	}

	m_buttonMute = new UiStateButton(this, 2000, 2);
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

	m_buttonSolo = new UiStateButton(this, 2001, 2);
	m_buttonSolo->setFixedSize(16, 16);
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

	x += 32;

	labelVoices = new UiLabel(this, "");
	labelVoices->setImage(":/Resources/notes16.png");
	labelVoices->move(x,3);

	labelMaxVoices = new UiLabel(this, "");
	labelMaxVoices->setImage(":/Resources/voicespeak16.png");
	labelMaxVoices->move(x,(height()/2)+2);

	x += 21; // (350+8+200+24)-2

	valueVoices = new UiValue(this, 20, 20, "", 1, 256, m_channel->numVoices(), 1, 2);
	valueVoices->move(x, (height()/2)+2);
	valueVoices->setValueTextColor(QColor(220,220,220));
	connect(valueVoices, SIGNAL(valueChanged(float)), this, SLOT(setNumVoices(float)));

	
	x += (32+3) ;
	

	/*buttonEdit = new UiStateButton(this, -1, 1);
	buttonEdit->move(23,4);
	buttonEdit->setFixedSize(32,20);
	//buttonEdit->setDrawFill(false);
	buttonEdit->setFillColor(QColor(90,90,90,150));
	buttonEdit->setDrawTint(false);
	buttonEdit->hide();
	connect(buttonEdit, SIGNAL(clickedWithFlags(int, int)), this, SLOT(edit(int, int)));*/

	buttonLock = new UiStateButton(this, 0, 2);
	buttonLock->setDrawFill(false);
	buttonLock->setDrawLine(false);
	buttonLock->setDrawTint(false);
	buttonLock->setFixedSize(11, 13);
	buttonLock->setStateImage(0, ":/Resources/lock_11_13_off.png");
	buttonLock->setStateImage(1, ":/Resources/lock_11_13_on.png");
	buttonLock->move(25, (height()/2)+3);
	connect(buttonLock, SIGNAL(clicked(int)), this, SLOT(toggleArticulationLock(int)));
	if(m_channel->isArticulationLock()) {
		buttonLock->setState(1);
	}

	buttonArt = new UiStateButton(this, 0, 2);
	buttonArt->setDrawFill(false);
	buttonArt->setDrawLine(false);
	buttonArt->setDrawTint(false);
	buttonArt->setFixedSize(11, 13);
	buttonArt->setStateImage(0, ":/Resources/cross_11_13_off.png");
	buttonArt->setStateImage(1, ":/Resources/cross_11_13_on_green.png");
	buttonArt->move(40, (height()/2)+3);
	connect(buttonArt, SIGNAL(clicked(int)), this, SLOT(toggleArticulationMode(int)));
	if(m_channel->isArticulationMode()) {
		buttonArt->setState(1);
	}

	selectArticulation = new UiSelectMenu(this, 150, 20);
	selectArticulation->move(55, 22);
	selectArticulation->setDrawFill(false);
	selectArticulation->setDrawTint(false);
	selectArticulation->setDrawLine(false);
	selectArticulation->setTextBold(false);
	selectArticulation->setTextColor(QColor(200,200,200));
	updateArticulationList();
	updateArticulationColor();
	connect(selectArticulation, SIGNAL(itemSelected(int)), this, SLOT(setArticulation(int)));

	connect(this, SIGNAL(articulationLockUpdated()), this, SLOT(updateArticulationLock()));
	connect(this, SIGNAL(articulationModeUpdated()), this, SLOT(updateArticulationMode()));
	connect(this, SIGNAL(articulationUpdated()), this, SLOT(updateArticulation()));
	connect(this, SIGNAL(articulationColorUpdated()), this, SLOT(updateArticulationColor()));
	connect(this, SIGNAL(layerUpdated()), this, SLOT(update()));
	connect(this, SIGNAL(midiMutedUpdated()), this, SLOT(updateMidiMuted()));
	connect(this, SIGNAL(downbeatOffsetUpdated()), this, SLOT(updateDownbeatOffset()));
	connect(this, SIGNAL(muteUpdated()), this, SLOT(updateMute()));
	connect(this, SIGNAL(soloUpdated()), this, SLOT(updateSolo()));

	connect(this, SIGNAL(editInstrumentFromChannel(SamChannel*)), samGui, SLOT(openEditor(SamChannel*)));

	samEngine->registerListener(this, SamEngine::ContextChannel);
	//samEngine->registerListener(this, SamEngine::ContextMidiInput);

	connect(this, SIGNAL(midiReceived(int, int)), this, SLOT(midiEvent(int, int)));

	/*if(m_channel->version()->numArticulations() < 2) {
		buttonArt->hide();
		selectArticulation->hide();

		if(m_channel->version()->numArticulations() == 1) {
			if(m_channel->version()->articulation(0)->id() == -1) {
				buttonLock->hide();
			}
		}
	}//*/

	updateArticulation();

	connect(samGui->mainWidget(), SIGNAL(timerTick()), m_indicatorNote, SLOT(updateTimer()));

	QString ils[6] = { "Volume", "Expression", "Modulation", "Sustain", "Brightness", "Pitchbend" };
	//QColor ics[6] = { QColor(255,255,255), QColor(225,225,225), QColor(195,195,195), QColor(165,165,165), QColor(135,135,135) };
	QColor ics[6] = { QColor(200,100,100), QColor(100,100,200), QColor(200,100,200), QColor(200,200,100), QColor(255,128,100), QColor(100,200,200) };
	int ictrls[6] = { 7, 11, 1, 64, 74, -1 };
	//int ix = 610;
	for(int i=0; i<6; i++) {
		m_indicators[i] = new SamGuiIndicatorBlock(this, ils[i], ics[i], ictrls[i]);
		m_indicators[i]->move(x, 2);
		x += m_indicators[i]->width()+5;
	}

	m_layerStrip = new SamGuiLayerStrip(this, channel);
	m_layerStrip->hide();
}

SamGuiChannel::~SamGuiChannel()
{
	aLog("SamGuiChannel::~SamGuiChannel()");

	samEngine->unregisterListener(this, SamEngine::ContextChannel);
	//samEngine->unregisterListener(this, SamEngine::ContextMidiInput);

	aLog("EXIT: SamGuiChannel::~SamGuiChannel()");
}

void SamGuiChannel::onState(AStateEvent *ev)
{
	//qDebug("SamGuiChannel::onState()");

	if(ev->context() == SamEngine::ContextChannel) {
		switch(ev->item()) {
			/*case SamChannel::ItemMidiInputPort:
				if(ev->objectUpdated() == m_channel && ev->fromObject() != this) {
					emit midiPortUpdated();
				}
				update();
				break;*/
			case SamChannel::ItemMidiInputChannel:
				if(ev->updatedObject()==m_channel && ev->fromObject() != this) {
					emit midiChannelUpdated();
					//emit updated();
				}
				break;
			case SamChannel::ItemArticulation:
				if(ev->fromObject() != this) {
					emit articulationUpdated();
				}
				emit articulationColorUpdated();
				break;
			case SamChannel::ItemArticulationLock:
				emit articulationLockUpdated();
				break;
			case SamChannel::ItemArticulationMode:
				emit articulationModeUpdated();
				break;
			case SamChannel::ItemLayer:
				emit layerUpdated();
				break;
			case SamChannel::ItemMidiInput:
				if(ev->updatedObject() == m_channel) {
					emit midiReceived((int)ev->value(), 0);
				}
				break;
			case SamChannel::ItemMidiMuted:
				if(ev->updatedObject() == m_channel) {
					emit midiMutedUpdated();
				}
				break;
			case SamChannel::ItemDownbeatOffset:
				if(ev->updatedObject() == m_channel) {
					emit downbeatOffsetUpdated();
				}
				break;
			case SamChannel::ItemMute:
				emit muteUpdated();
				break;
			case SamChannel::ItemSolo:
				emit soloUpdated();
				break;
		}
	} //else if(ev->context() == SamEngine::ContextMidiInput) {
		//emit midiReceived(ev->item(), (int)ev->value());
	//}
}

void SamGuiChannel::updateNextFrame(void)
{
	labelVoices->setText(QString::number(m_channel->numVoicesPlaying()));

	update();
}

/*void SamGuiChannel::setMidiPort(int itemIndex)
{
	qDebug("SamGuiChannel::setMidiPort(%d)", itemIndex);

	if(samEngine->project()->numSelectedChannels() > 1) {
		for(int i=0; i<samEngine->project()->numChannels(); i++) {
			SamChannel *channel = samEngine->project()->channel(i);
			if(channel->isSelected()) {
				channel->setMidiInputPort(itemIndex, this);
			}
		}
	} else {
		m_channel->setMidiInputPort(itemIndex, this);
	}
}*/

void SamGuiChannel::setMidiChannel(int itemIndex)
{
	//qDebug("SamGuiChannel::setMidiChannel(%d)", itemIndex);

	if(samEngine->project()->numSelectedChannels() > 1) {
		for(int i=0; i<samEngine->project()->numChannels(); i++) {
			SamChannel *channel = samEngine->project()->channel(i);
			if(channel->isSelected()) {
				channel->setMidiInputChannel(itemIndex, this);
			}
		}
	} else {
		m_channel->setMidiInputChannel(itemIndex, this);
	}
}

/*void SamGuiChannel::updateMidiPort(void)
{
	//selectMidiPort->setSelectedItem(m_channel->midiInputPort());
}*/

void SamGuiChannel::updateMidiChannel(void)
{
	selectMidiChannel->setSelectedItem(m_channel->midiInputChannel());
	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannel::setMidiMuted(int id, int flags)
{
	m_channel->setMidiMuted(!m_channel->isMidiMuted(), this);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannel::updateMidiMuted(void)
{
	buttonMidiMuted->setState(m_channel->isMidiMuted());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannel::setDownbeatOffset(int id, int flags)
{
	if(m_channel) {
		m_channel->setDownbeatOffset(!m_channel->isDownbeatOffset(), this);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannel::updateMute(void)
{
	m_buttonMute->setState(m_channel->isMute());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannel::updateSolo(void)
{
	m_buttonSolo->setState(m_channel->isSolo());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannel::updateDownbeatOffset(void)
{
	m_buttonDownbeatOffset->setState(m_channel->isDownbeatOffset());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannel::updateLabel(QString text)
{
	qDebug("SamGuiChannel::updateLabel()");

	QByteArray ba = text.toAscii();
	m_channel->setLabel(ba.data());

	if(!samGui->isShowLabels() || text.length() < 1) {
		m_label->setText(m_channel->version()->name().toChar());
	}
}


// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannel::refreshLabel(void)
{
	qDebug("SamGuiChannel::updateLabel()");

	if(m_channel->label().length() > 0 && samGui->isShowLabels()) {
		m_label->setText(m_channel->label().toChar());
	} else {
		m_label->setText(m_channel->version()->name().toChar());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannel::toggleArticulationLock(int)
{
	qDebug("SamGuiChannel::toggleArticulationLock()");

	SamEvent ev(SamEvent::Parameter);
	ev.subType = SamParameter::ArticulationLock;
	ev.iValue = (int)!m_channel->isArticulationLock();
	m_channel->addEvent(ev);
}

void SamGuiChannel::toggleArticulationMode(int)
{
	SamEvent ev(SamEvent::Parameter);
	ev.subType = SamParameter::ArticulationMode;
	ev.iValue = (int)!m_channel->isArticulationMode();
	m_channel->addEvent(ev);
}

void SamGuiChannel::setArticulation(int index)
{
	qDebug("SamGuiChannel::setArticulation(%d)", index);

	SamEvent ev(SamEvent::Parameter);
	ev.subType = SamParameter::Articulation;
	ev.iValue = index;
	ev.from = this;
	m_channel->addEvent(ev);

	if(m_channel->isArticulationLock()) {
		selectArticulation->setSelectedItem(m_channel->currentArticulationIndex());
	}
}

void SamGuiChannel::setNumVoices(float value)
{
	qDebug("SamGuiChannel::setNumVoices(%d)", (int)value);

	m_channel->setNumVoices((int)value);
}

void SamGuiChannel::setMute(int id, int flags)
{
	SamEvent ev(SamEvent::Parameter);
	ev.subType = SamParameter::ChannelMute;
	ev.iValue = (int)!m_channel->isMute();
	ev.from = this;
	m_channel->addEvent(ev);
}

void SamGuiChannel::setSolo(int id, int flags)
{
	SamEvent ev(SamEvent::Parameter);
	ev.subType = SamParameter::ChannelSolo;
	ev.iValue = (int)!m_channel->isSolo();
	ev.from = this;
	m_channel->addEvent(ev);
}

void SamGuiChannel::updateArticulationLock(void)
{
	buttonLock->setState((int)m_channel->isArticulationLock());

	updateArticulationColor();
}

void SamGuiChannel::updateArticulationMode(void)
{
	buttonArt->setState((int)m_channel->isArticulationMode());

	updateArticulationColor();
}

void SamGuiChannel::updateArticulation(void)
{
	qDebug("SamGuiChannel::updateArticulation()");

	/*for(int i=0; i<m_parameterControls.size(); i++) {
		delete m_parameterControls[i];
	}
	m_parameterControls.clear();//*/

	if(m_channel->currentArticulationIndex() > -1) {
		selectArticulation->setSelectedItem(m_channel->currentArticulationIndex());

		/*int x = 10;
		int y = 46;
		for(int i=0; i<m_channel->version()->articulation(m_channel->currentArticulationIndex())->numParameters(); i++) {
			SamGuiParameterControl *pc = new SamGuiParameterControl(this, m_channel, m_channel->version()->articulation(m_channel->currentArticulationIndex())->parameter(i));
			pc->move(x, y);
			x += (pc->width() + 8);
			m_parameterControls.append(pc);
		}//*/

	}
	updateArticulationColor();

	

	update();
}

void SamGuiChannel::updateArticulationList(void)
{
	selectArticulation->clear();

	for(int i=0; i<m_channel->version()->numArticulations(); i++) {
		AString entry = m_channel->version()->articulation(i)->name();
		if(m_channel->version()->articulation(i)->id() > -1) {
			int id = m_channel->version()->articulation(i)->id();
			if(samGui->isChannelProgramDisplay()) {
				id++;
			}
			entry.append(AString(" (") + AString::number(id) + AString(")"));
		}
		selectArticulation->addItem(entry.toChar());
	}

	updateArticulation();
}

void SamGuiChannel::updateArticulationColor(void)
{
	if(m_channel->currentArticulationIndex() < 0) {
		selectArticulation->setTextColor(QColor(255,100,100));
	} else {
		if(m_channel->isArticulationMode()) {
			selectArticulation->setTextColor(QColor(149,255,25)); //   0,200,255
		} else {
			if(m_channel->isArticulationLock()) {
				selectArticulation->setTextColor(QColor(255,255,150));
			} else {
				selectArticulation->setTextColor(QColor(200,200,200));
			}
		}
	}
}

/*void SamGuiChannel::edit(int id, int flags)
{
	emit editInstrumentFromChannel(m_channel);
}*/

void SamGuiChannel::midiEvent(int ev, int port)
{
	//qDebug("SamGuiChannel::midiEvent(%d, %d)", ev, port);

	//if(port != m_channel->midiInputPort()) {
	//	//qDebug("No port (%d != %d)", port, m_channel->midiInputPort());
	//	return;
	//}

	unsigned char status = ev & 0x000000FF;
	unsigned char data1  = (ev>>8) & 0x000000FF;
	unsigned char data2  = (ev>>16) & 0x000000FF;

	//qDebug("%d = %d", status & 0x0F, m_channel->midiInputChannel());
	if((status & 0x0F)+1 == m_channel->midiInputChannel() || m_channel->midiInputChannel() == 0 ) {
		

		unsigned char type = status & 0xF0; //aLog("type = %02x", type);

		if(type == 0x90) {
			if(data2 > 0) {
				m_indicatorNote->setValue(data2, true);
			}
		} else if(type == 0xB0) {
			for(int i=0; i<5; i++) {
				if(m_indicators[i]->controller() == data1) {
					m_indicators[i]->setValue(data2);
				}
			}
		} else if(type == 0xE0) {
			unsigned short a = data1;
			unsigned short b = data2;
			unsigned short bend = b;
			bend <<= 7;
			bend |= a;
			//qDebug("bend=%d", bend/128);
			m_indicators[5]->setValue(bend/128);
		}
	}
}

void SamGuiChannel::mousePressEvent(QMouseEvent *ev)
{
	if(QRect(315,5,27,22).contains(ev->pos())) {
		select();
		emit editInstrumentFromChannel(m_channel);
	//} else if(ev->modifiers() == Qt::AltModifier) {
	} else if(QRect(0,0,8,height()).contains(ev->pos())) {
		/*if(height()==44) {
			changeHeight(88);
			//m_layerStrip->show();
			//m_layerStrip->setFixedSize(width()-24-8, 22);
			//m_layerStrip->move(8+12, 44+11);
		} else {
			changeHeight(44);
			//m_layerStrip->hide();
		}//*/
	} else if(ev->button() == Qt::RightButton) {
		//samGui->mainWidget()->popupChannelIndicators()->exec(ev->globalPos());
	} else {
		UiListItem::mousePressEvent(ev);
	}
}

/*void SamGuiChannel::mouseDoubleClickEvent(QMouseEvent *ev)
{
	//if(ev->modifiers() != Qt::AltModifier) {
	//	emit editInstrumentFromChannel(m_channel);
	//}

	if(QRect(60,0,260-8,height()/2).contains(ev->pos())) {

	}
}*/

void SamGuiChannel::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	QColor midiChannelColor[17] = { QColor(160,160,160),
									QColor(143,26,26),   QColor(143,26,84), QColor(143,26,143), QColor(84,26,143),
		                            QColor(26,26,143), QColor(26,84,143), QColor(26,143,143), QColor(26,143,84),
									QColor(89,153,0),QColor(74,143,26), QColor(143,143,26), QColor(143,113,26),
									QColor(143,84,26),QColor(143,55,26), QColor(215,24,0), QColor(215,98,0) };

	int channelColorIndex = m_channel->midiInputChannel();

	if(m_channel->isMute()) {
		p.fillRect(0, 0, width(), height()-1, QColor(30,30,30));
	} else {
		p.fillRect(0, 0, width(), height()-1, QColor(75,75,75));
	}
	
	QColor backgroundColor = midiChannelColor[channelColorIndex];
	backgroundColor.setAlpha(60);
	p.fillRect(0, 0, 350, 43, backgroundColor); // Dark color background
	
	if(1) {
		if(positionInList()%2) {
			backgroundColor = QColor(80,80,80);
		} else {
			backgroundColor = QColor(70,70,70);
		}
	}

	if(isSelected()) {
		backgroundColor = QColor(60,60,100);
	}

	if(m_channel->isMute()) {
		backgroundColor = QColor(30,30,30);
	}

	p.fillRect(350, 0, width(), 43, backgroundColor); // Top layer background color
	if(height()>44) {
		p.fillRect(0, 44, width()-1, 43, backgroundColor);
	}

	p.fillRect(0,0,351, 43, QColor(0,0,0,90));
	//if(height()>44) {
	//	p.fillRect(351, 44, width()-1, 43, QColor(0,0,0,90));
	//}

	//p.fillRect(342, 0, 8, 43, midiChannelColor[channelColorIndex]);
	p.fillRect(0,0,8,height()-1, midiChannelColor[channelColorIndex]);

	p.setPen(QColor(255,255,255,75));
	p.setFont(QFont("Calibri",16,QFont::Bold));
	//p.drawRect(23,3,32,20);
	//if(!buttonEdit->isVisible()) {
		p.drawText(0,0,52,height(),Qt::AlignRight|Qt::AlignTop, QString::number(positionInList()+1));
	//}
	
		p.setPen(Qt::black);
		p.drawLine(8,0,8,height());

//	p.setPen(Qt::white);
//	p.setFont(QFont("Calibri",16));//,QFont::Bold));
//	p.drawText(60,0,260-8,height(),Qt::AlignLeft|Qt::AlignTop, m_channel->version()->name().toChar());

	if(m_mouseOver) {
		p.setPen(QColor(255,255,255));
		p.setFont(QFont("Calibri",12));
		p.drawText(315-8,5,27,height(),Qt::AlignRight|Qt::AlignTop, "edit");

	} else if(m_channel->version()->reference() > 0) {
		p.setPen(QColor(255,255,255,75));
		p.setFont(QFont("Calibri",12));
		p.drawText(320-8,5,22,height(),Qt::AlignRight|Qt::AlignTop, QString("v")+QString::number(m_channel->version()->reference()));
	}

	//p.setPen(Qt::white);

	if(m_channel->version()->articulation(m_channel->currentArticulationIndex())->numLayers() > 1) {
		QString layerText;
		p.setPen(QColor(200,200,200));
		switch(m_channel->layerSelectMode(m_channel->currentArticulationIndex())) {
			case SamArticulation::SingleLayer:
				if(m_channel->layer()->name().length() > 0) {
					layerText = m_channel->layer()->name().toChar();
				}
				break;
			case SamArticulation::CycleLayers:
				layerText = "Alternating";
				break;
			case SamArticulation::AllLayers:
				layerText = "All Layers";
				break;
			case SamArticulation::Legato:
				layerText = "Legato";
				p.setPen(QColor(255,255,155));
				break;
		}
		
		int layerBaseY = (44/2)+6;
		for(int i=0; i<3; i++) {
			p.drawLine(210, layerBaseY+(i*4), 210+12, layerBaseY+(i*4));
		}
		p.setFont (QFont ("Calibri",10));
		p.drawText(230,(44/2)-1,90,44/2,Qt::AlignLeft|Qt::AlignVCenter, layerText);
	}

#ifdef USE_VOICE_TIMER
	p.setPen(QColor(255,220,0));
	p.setFont (QFont ("Calibri",10));
	p.drawText(width()-50,0,50,height(), Qt::AlignLeft|Qt::AlignTop, QString::number(m_channel->averageVoiceTime()));
#endif

	// Draw divider lines
	// ------------------
	p.setPen(QColor(0,0,0,45));
	p.drawLine(350,0,350,43);
	p.drawLine(buttonMidiMuted->x()+buttonMidiMuted->width()+8,0,buttonMidiMuted->x()+buttonMidiMuted->width()+8,43);
	if(height() > 44) {
		/*p.drawLine(8,43,width(),43);
		for(int i=0; i<m_parameterControls.size(); i++) {
			int x = m_parameterControls[i]->x()+m_parameterControls[i]->width();
			
			p.drawLine(x,44,x,height()-1);
		}*/
	}

	for(int i=0; i<6; i++) {
		p.drawLine(m_indicators[i]->x(), 0, m_indicators[i]->x(), 43);
		if(!(i%2)) {
			p.fillRect(m_indicators[i]->x()+1, 0, m_indicators[i]->width()+4, 43, QColor(0,0,0,30));
		} else {
			p.fillRect(m_indicators[i]->x()+1, 0, m_indicators[i]->width()+4, 43, QColor(255,255,255,15));
		}
	}

	// Selected tint
	// -------------
	if (isSelected()) {
		p.setPen(QColor(120,120,200));
		p.setBrush(QColor(255,255,255,10));
		p.drawRect (0,0,width()-1, height()-1);
		p.drawLine(1,1,width()-2,1);
		p.drawLine(width()-2,1,width()-2,height()-2);
		p.drawLine(width()-2,height()-2,1,height()-2);
		p.drawLine(1, height()-2, 1, 1); 
		/*QLinearGradient lgBack(QPointF(0, 0), QPointF(0, height()));
		QColor col(QColor(255,255,255));
		col.setAlpha(20);//240//90 // 230
		lgBack.setColorAt(0, col);
		col.setAlpha(0);//255//50 // 255
		lgBack.setColorAt(1, col);
		//p.setBrush(lgBack);
		//p.fillRect(0, 0, width()-1, (height()/5)*3, lgBack); */
	}
	

	/*QColor backgroundColor[2] = {
		QColor(70, 70, 70),
		QColor(90, 90, 90)
	};

	// Background
	int cIndex = 0;
	if (positionInList()%2==0)
		cIndex = 1;
	//p.setBrush (backgroundColor[cIndex]);
	//p.setPen (backgroundColor[cIndex]);
	//p.drawRect (0,0,width()-1, height()-1);
	// ------------------------------


	p.setBrush (QColor(0,0,0));
	p.setPen (QColor(0,0,0));
	p.drawRect (0,0,width()-1, height()-1);
	QColor col = backgroundColor[cIndex];
	QLinearGradient lgBack(QPointF(0, 0), QPointF(0, height()));
	col.setAlpha(180);//240//90 // 230
	lgBack.setColorAt(0, col);
	col.setAlpha(200);//255//50 // 255
	lgBack.setColorAt(1, col);

	p.setBrush(lgBack);
	p.setPen(QColor(0,0,0,0));
	p.drawRect (0,0,width()-1, height()-1);

	p.fillRect(0,0,width()-1,height()/2, QColor(0,0,0,12));
	//p.fillRect(0,height()/2,width()-1,height()/2, QColor(0,0,0,10));
	p.setPen(QColor(0,0,0,20));
	p.drawLine(0, height()/2, width()-1, height()/2);


	p.setPen (QColor(70,70,70));
	//p.drawLine (width()-1, 0, width()-1, height()-1);
	p.drawLine (width()-1,height()-1, 0, height()-1);

	//p.drawLine(400,0,400,height()-2);

	p.setPen (QColor(80,80,80));
	p.drawLine(350+1,0,350+1,height()-2);
	p.drawLine(0,0,width()-1,0);


	p.setPen(QColor(0,0,0));
	p.setBrush(QColor(0,0,0));
	p.drawRect(0, 1, 350, height()-3);
	p.drawRect(width()-1-8-1,1,9,height()-3);

	

	// MIDI Channel Color Bar
	// ----------------------
	QColor midiChannelColor[17] = { QColor(160,160,160),
									QColor(143,26,26),   QColor(143,26,84), QColor(143,26,143), QColor(84,26,143),
		                            QColor(26,26,143), QColor(26,84,143), QColor(26,143,143), QColor(26,143,84),
									QColor(89,153,0),QColor(74,143,26), QColor(143,143,26), QColor(143,113,26),
									QColor(143,84,26),QColor(143,55,26), QColor(215,24,0), QColor(215,98,0) };

	int channelColorId = m_channel->midiInputChannel();
	p.setBrush(midiChannelColor[channelColorId]);
	p.setPen(midiChannelColor[channelColorId]);
	p.drawRect(0,1,16,height()-3);//, 6.0, 6.0);
	p.drawRect(width()-1-8, 1, 8, height()-3);
	

	// Background strip text
	


	p.setPen(QColor(240,240,240));
	p.setFont(QFont("Calibri",8,QFont::Bold));
	p.drawText(0,0,16,height(),Qt::AlignCenter|Qt::AlignVCenter, QString::number(positionInList()+1));

	p.setPen(Qt::white);
	p.setFont(QFont ("Calibri",12,QFont::Bold));
	p.drawText(24,0,170,height()/2,Qt::AlignLeft|Qt::AlignVCenter, m_channel->version()->name().toChar());

	QString layerText;
	if(m_channel->layerSelectMode()==0) {
		if(m_channel->layer()->name().length() > 0) {
			layerText = m_channel->layer()->name().toChar();
		}
	} else if(m_channel->layerSelectMode()==1) {
		layerText = "Alternating";
	} else if(m_channel->layerSelectMode()==2) {
		layerText = "All Layers";
	}
	p.setPen(QColor(200,200,200));
	int layerBaseY = (height()/2)+6;
	for(int i=0; i<3; i++) {
		p.drawLine(210, layerBaseY+(i*4), 210+12, layerBaseY+(i*4));
	}
	p.setFont (QFont ("Calibri",10));
	p.drawText(230,(height()/2)-1,90,height()/2,Qt::AlignLeft|Qt::AlignVCenter, layerText);

#ifdef USE_VOICE_TIMER
	p.setPen(QColor(255,220,0));
	p.drawText(200,0,50,height()/2, Qt::AlignLeft|Qt::AlignVCenter, QString::number(m_channel->averageVoiceTime()));
#endif

	QColor color(255,255,255);
	QLinearGradient linearGradient(QPointF(0, 0), QPointF(0, height()));
	color.setAlpha(40);//90
	linearGradient.setColorAt(0, color);
	color.setAlpha(5);//50
	linearGradient.setColorAt(1, color);

	p.setBrush(linearGradient);
	p.setPen(QColor(0,0,0,0));
	p.drawRect(0,0,350,height()/2);
	p.drawRect(width()-9,0,8,(height()/2)+1);

	


	// Selected tint
	// -------------
	if (isSelected()) {
		p.setPen(QColor(255,255,255));
		p.setBrush(QColor(255,255,255,60));
		p.drawRect (0,0,width()-1, height()-1);
	}*/
}

void SamGuiChannel::enterEvent(QEvent *ev)
{
	//buttonEdit->show();
	m_mouseOver = true;
	update();
}

void SamGuiChannel::leaveEvent(QEvent *ev)
{
	//buttonEdit->hide();
	m_mouseOver = false;
	update();
}

void SamGuiChannel::timerEvent(QTimerEvent *ev)
{
	//m_indicatorNote->timerEvent(0);
}

void SamGuiChannel::resizeEvent(QResizeEvent *ev)
{
	qDebug("*>*>*>*>****** SamGuiChannel::resizeEvent() %d, %d", width(), height());

	m_layerStrip->setFixedSize(width()-24-8, 22);


	m_indicatorNote->setFixedHeight(height()-4);
	
	
	if(height() > 22) {
		selectMidiInput->show();
		selectMidiChannel->show();
		labelMidiPort->show();
		labelMidiChannel->show();
	} else {
		selectMidiInput->hide();
		selectMidiChannel->hide();
		labelMidiPort->hide();
		labelMidiChannel->hide();

	}

	m_buttonMute->raise();
	m_buttonMute->move(width()-20, 4);

	m_buttonSolo->raise();
	m_buttonSolo->move(width()-20, 24);
}