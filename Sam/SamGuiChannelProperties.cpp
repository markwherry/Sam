#include "SamGuiChannelProperties.h"

#include <QPainter>

#include <math.h>

#include "Sam.h"
#include "SamEngine.h"
#include "SamProject.h"
#include "SamRealTimeCommands.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiChannelProperties::SamGuiChannelProperties(QWidget *parent)
	: QWidget(parent)
	, m_channel(0)
{

	int y = 0;
	//ui.setupUi(this);
	labelInput = new UiLabel(this, "MIDI Input");
	labelInput->move(8, y+=8); //8
	labelInput->setDrawUnderline(true);
	labelInput->setTextBold(true);
	labelInput->setTextColor(QColor(240,240,240));
	labelInput->setTextSize(10);

	labelMidiPort = new UiLabel(this, "Port");
	labelMidiPort->setImage(":/Resources/midi16.png");
	labelMidiPort->move(8, y+=24); // 32
	labelMidiPort->setTextColor(QColor(180,180,180));

	selectMidiInput = new SamGuiSelectMidiInputBus(this, 0);
	selectMidiInput->setDrawFill(false);
	selectMidiInput->setDrawTint(false);
	selectMidiInput->setDrawLine(false);
	//selectMidiPort->setTextBold(false);
	//selectMidiPort->setTextColor(QColor(200,200,200));
	selectMidiInput->move(110,y);
	

	labelMidiChannel = new UiLabel(this, "Channel");
	labelMidiChannel->setImage(":/Resources/midichannelsolid16.png");
	labelMidiChannel->move(8, y+=24); //56
	labelMidiChannel->setTextColor(QColor(180,180,180));
	

	selectMidiChannel = new UiSelectMidiChannel(this);
	selectMidiChannel->setDrawFill(false);
	selectMidiChannel->setDrawTint(false);
	selectMidiChannel->setDrawLine(false);
	selectMidiChannel->move(110, y); //56
	selectMidiChannel->setFixedWidth(160);

	buttonMidiMuted = new UiStateButton(this, 1000, 2);
	buttonMidiMuted->move(8, y+=24);
	buttonMidiMuted->setFixedSize(16, 16);
	//buttonMidiMuted->setBackground();
	buttonMidiMuted->setStateImage(0, ":/Resources/panic16.png");
	buttonMidiMuted->setStateImage(1, ":/Resources/panic16.png");
	buttonMidiMuted->setText("MIDI Input Mute / Panic");
	//buttonMidiMuted->setBackgroundColor(0,QColor(255,130,0,30));
	buttonMidiMuted->setStateFillColor(0,QColor(150,100,100));
	buttonMidiMuted->setStateFillColor(1,QColor(250,100,100));
	buttonMidiMuted->setStateLineColor(1,QColor(150,100,100));
	//buttonMidiMuted->setDrawTint();
	//buttonMidiMuted->setDrawOutline();

	connect(buttonMidiMuted, SIGNAL(clickedWithFlags(int, int)), this, SLOT(setMidiMuted(int, int)));

	labelChannelSettings = new UiLabel(this, "Channel Settings");
	labelChannelSettings->move(8, y+=48);
	labelChannelSettings->setDrawUnderline(true);
	labelChannelSettings->setTextBold(true);
	labelChannelSettings->setTextColor(QColor(240,240,240));
	labelChannelSettings->setTextSize(10);

	m_buttonDownbeatOffset = new UiStateButton(this, 1000, 2);
	m_buttonDownbeatOffset->move(8, y+=24);
	m_buttonDownbeatOffset->setFixedSize(16, 16);
	//m_buttonDownbeatOffset->setBackground();
	m_buttonDownbeatOffset->setStateImage(0, ":/Resources/downbeatoffsetb16.png");
	m_buttonDownbeatOffset->setStateImage(1, ":/Resources/downbeatoffsetb16.png");
	m_buttonDownbeatOffset->setText("Downbeat Offset");
	//m_buttonDownbeatOffset->setBackgroundColor(0,QColor(255,130,0,30));
	m_buttonDownbeatOffset->setStateFillColor(0,QColor(100,75,150));
	m_buttonDownbeatOffset->setStateFillColor(1,QColor(170,70,255));
	m_buttonDownbeatOffset->setStateLineColor(1,QColor(100,75,150));
	//m_buttonDownbeatOffset->setDrawTint();
	//m_buttonDownbeatOffset->setDrawOutline();

	connect(m_buttonDownbeatOffset, SIGNAL(clickedWithFlags(int, int)), this, SLOT(setDownbeatOffset(int, int)));

	m_buttonLayerMutesEnabled = new UiStateButton(this, 1000, 2);
	m_buttonLayerMutesEnabled->move(8, y+=24);
	m_buttonLayerMutesEnabled->setFixedSize(16, 16);
	//m_buttonDownbeatOffset->setBackground();
	m_buttonLayerMutesEnabled->setStateImage(0, ":/Resources/layermutes16.png");
	m_buttonLayerMutesEnabled->setStateImage(1, ":/Resources/layermutes16.png");
	m_buttonLayerMutesEnabled->setText("Layer Mutes");
	//m_buttonLayerMutesEnabled->setBackgroundColor(0,QColor(255,130,0,30));
	m_buttonLayerMutesEnabled->setStateFillColor(0,QColor(100,75,150));
	m_buttonLayerMutesEnabled->setStateFillColor(1,QColor(170,70,255));
	m_buttonLayerMutesEnabled->setStateLineColor(1,QColor(100,75,150));
	//m_buttonLayerMutesEnabled->setDrawTint();
	//m_buttonLayerMutesEnabled->setDrawOutline();
	
	
	connect(m_buttonLayerMutesEnabled, SIGNAL(clickedWithFlags(int, int)), this, SLOT(setLayerMutesEnabled(int, int)));


	labelMusicTools = new UiLabel(this, "Music Tools");
	labelMusicTools->move(8, y+=48); //104
	labelMusicTools->setDrawUnderline(true);
	labelMusicTools->setTextBold(true);
	labelMusicTools->setTextColor(QColor(240,240,240));
	labelMusicTools->setTextSize(10);

	//int y = 104 + 24;
	y += 24;
	for(int i=0; i<SamChannel::maxMusicTools; i++) {
		//labelMusicTool[i] = new UiLabel(this, "Tool "+QString::number(i+1));
		////labelMusicTool[i]->setImage(":/Resources/send16.png");
		//labelMusicTool[i]->move(8,y);
		//labelMusicTool[i]->setTextColor(QColor(180,180,180));

		buttonMusicTool[i] = new UiStateButton(this, 2000+i, 2);
		buttonMusicTool[i]->move(8,y+2);
		buttonMusicTool[i]->setFixedSize(16, 16);
		buttonMusicTool[i]->setText("Tool "+QString::number(i+1));
		buttonMusicTool[i]->setStateImage(0, ":/Resources/tool16.png");
		buttonMusicTool[i]->setStateImage(1, ":/Resources/tool16.png");
		buttonMusicTool[i]->setStateFillColor(0,QColor(0, 75, 75));
		buttonMusicTool[i]->setStateFillColor(1,QColor(0, 255, 255));
		buttonMusicTool[i]->setStateLineColor(1,QColor(0, 75, 75));

		connect(buttonMusicTool[i], SIGNAL(clickedWithFlags(int, int)), this, SLOT(setMusicToolActive(int, int)));

		buttonMusicToolEdit[i] = new UiStateButton(this, 2000+i, 2);
		buttonMusicToolEdit[i]->move(254,y+2);
		buttonMusicToolEdit[i]->setFixedSize(16, 16);
		buttonMusicToolEdit[i]->setStateFillColor(0,QColor(0, 75, 75));
		buttonMusicToolEdit[i]->setStateFillColor(1,QColor(0, 255, 255));
		buttonMusicToolEdit[i]->setStateImage(0, ":/Resources/edit16.png");
		buttonMusicToolEdit[i]->setStateImage(1, ":/Resources/edit16.png");

		connect(buttonMusicToolEdit[i], SIGNAL(clickedWithFlags(int, int)), this, SLOT(showMusicToolEditor(int, int)));

		selectMusicTool[i] = new SamGuiSelectMusicTool(this);
		selectMusicTool[i]->setDrawFill(false);
		selectMusicTool[i]->setDrawTint(false);
		selectMusicTool[i]->setDrawLine(false);
		selectMusicTool[i]->move(110, y); //22
		selectMusicTool[i]->setDoubleClick(true);

		connect(selectMusicTool[i], SIGNAL(editorOpened(int, int)), this, SLOT(showMusicToolEditor(int, int)));

		y+= 24;
	}

	labelOutput = new UiLabel(this, "Audio Output");
	labelOutput->move(8,y+=24);
	labelOutput->setDrawUnderline(true);
	labelOutput->setTextBold(true);
	labelOutput->setTextColor(QColor(240,240,240));
	labelOutput->setTextSize(10);

	y += 24;//yAudioOffset+104+24;
	for(int i=0; i<SamChannel::maxOutputs; i++) {
		labelChannelOutput[i] = new UiLabel(this, "Output");
		labelChannelOutput[i]->setImage(":/Resources/audio16.png");
		labelChannelOutput[i]->move(8,y);
		labelChannelOutput[i]->setTextColor(QColor(180,180,180));

		selectAudioBus[i] = new SamGuiSelectAudioOutputBus(this, 0, 0);
		selectAudioBus[i]->setDrawFill(false);
		selectAudioBus[i]->setDrawTint(false);
		selectAudioBus[i]->setDrawLine(false);
		selectAudioBus[i]->move(110, y); //22

		y += 24;
	}

	labelSends = new UiLabel(this, "Audio Sends");
	labelSends->move(8,y+=48);
	labelSends->setDrawUnderline(true);
	labelSends->setTextBold(true);
	labelSends->setTextColor(QColor(240,240,240));
	labelSends->setTextSize(10);

	y += 24;//yAudioOffset+ 0;
	for(int i=0; i<SamAudioBus::maxSends; i++) {
		//labelChannelSend[i] = new UiLabel(this, "Send "+QString::number(i+1));
		//labelChannelSend[i]->setImage(":/Resources/send16.png");
		//labelChannelSend[i]->move(8,y);
		//labelChannelSend[i]->setTextColor(QColor(180,180,180));

		buttonSend[i] = new UiStateButton(this, 3000+i, 2);
		buttonSend[i]->move(8,y+2);
		buttonSend[i]->setFixedSize(16, 16);
		buttonSend[i]->setText("Send "+QString::number(i+1));
		buttonSend[i]->setStateImage(0, ":/Resources/send16_3.png");
		buttonSend[i]->setStateImage(1, ":/Resources/send16_3.png");
		buttonSend[i]->setStateFillColor(0,QColor(109, 30, 126));
		buttonSend[i]->setStateFillColor(1,QColor(220, 62, 255)); //165, 47, 189
		buttonSend[i]->setStateLineColor(1,QColor(109, 30, 126));
		connect(buttonSend[i], SIGNAL(clickedWithFlags(int, int)), this, SLOT(setSendActive(int, int)));

		selectAudioSendBus[i] = new SamGuiSelectAudioOutputBus(this, 0, 0);
		selectAudioSendBus[i]->setDrawFill(false);
		selectAudioSendBus[i]->setDrawTint(false);
		selectAudioSendBus[i]->setDrawLine(false);
		selectAudioSendBus[i]->move(110, y); //22

		faderSends[i] = new UiFader(this, false, 110, 16, 8, i); //160
		faderSends[i]->setLevelColor(QColor(165, 47, 189));
		connect(faderSends[i], SIGNAL(valueChanged(int, float, int)), this, SLOT(sendFaderMoved(int, float, int)));
		


		//y += 24;
	}
	

	//connect(selectMidiPort, SIGNAL(itemSelected(int)), this, SLOT(setMidiPort(int)));
	connect(selectMidiChannel, SIGNAL(itemSelected(int)), this, SLOT(setMidiChannel(int)));

	connect(this, SIGNAL(selectedChannelUpdated()), this, SLOT(updateSelectedChannel()));
	//connect(this, SIGNAL(midiPortUpdated()), this, SLOT(updateMidiPort()));
	connect(this, SIGNAL(midiChannelUpdated()), this, SLOT(updateMidiChannel()));
	connect(this, SIGNAL(midiMutedUpdated()), this, SLOT(updateMidiMuted()));

	connect(this, SIGNAL(sendGainUpdated(void*, void*)), this, SLOT(updateSendGain(void*, void*)));
	connect(this, SIGNAL(sendActiveUpdated()), this, SLOT(updateSendActive()));
	connect(this, SIGNAL(downbeatOffsetUpdated()), this, SLOT(updateDownbeatOffset()));
	connect(this, SIGNAL(layerMutesEnabledUpdated()), this, SLOT(updateLayerMutesEnabled()));

	samEngine->registerListener(this, SamEngine::ContextChannel);
	samEngine->registerListener(this, SamEngine::ContextProject);
	samEngine->registerListener(this, SamEngine::ContextMusicTool);

	connect(this, SIGNAL(musicToolActiveUpdated()), this, SLOT(updateMusicToolActive()));
	

	hideAll();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiChannelProperties::~SamGuiChannelProperties()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::onState(AStateEvent *ev)
{
	//qDebug("SamGuiChannelProperties::onState()");

	if(ev->context() == SamEngine::ContextChannel) {
		switch(ev->item()) {
			case SamChannel::ItemSelected:
				emit selectedChannelUpdated();
				break;
			/*case SamChannel::ItemMidiInputPort:
				if((ev->objectUpdated() == m_channel || samEngine->project()->numSelectedChannels() > 1) && ev->fromObject() != this) {
					emit midiPortUpdated();
				}
				break;*/
			case SamChannel::ItemMidiInputChannel:
				if((ev->updatedObject() == m_channel || samEngine->project()->numSelectedChannels() > 1) && ev->fromObject() != this) {
					emit midiChannelUpdated();
				}
				break;
			case SamChannel::ItemSendAudioBus:
				//qDebug("SamChannel::ItemSendAudioBus");
				emit selectedChannelUpdated();
				break;
			case SamChannel::ItemSendGain:
				//qDebug("SamChannel::ItemSendGain");
				emit sendGainUpdated(ev->updatedObject(), ev->fromObject());
				break;
			case SamChannel::ItemSendActive:
				qDebug("SamChannel::ItemSendActive:");
				emit sendActiveUpdated();
				break;
			case SamChannel::ItemDownbeatOffset:
				emit downbeatOffsetUpdated();
				break;
			case SamChannel::ItemLayerMutesEnabled:
				qDebug("SamChannel::ItemLayerMutesEnabled:");
				emit layerMutesEnabledUpdated();
				break;
			case SamChannel::ItemMidiMuted:
				if(ev->updatedObject() == m_channel) {
					emit midiMutedUpdated();
				}
				break;
		}
	} else if(ev->context() == SamEngine::ContextProject) {
		switch(ev->item()) {
			case SamProject::ItemChannelRemoved:
				emit selectedChannelUpdated();
				break;
		}
	} else if(ev->context() == SamEngine::ContextMusicTool) {
		switch(ev->item()) {
			case SamMusicToolInsert::ItemActive:
				emit musicToolActiveUpdated();
				break;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::showAll(void)
{
	labelInput->show();
	labelMidiPort->show();
	labelMidiChannel->show();
	selectMidiInput->show();
	selectMidiChannel->show();
	buttonMidiMuted->show();

	labelChannelSettings->show();
	m_buttonDownbeatOffset->show();
	m_buttonLayerMutesEnabled->hide();

	labelMusicTools->show();
	for(int i=0; i<SamChannel::maxMusicTools; i++) {
		//labelMusicTool[i]->show();
		buttonMusicTool[i]->show();
		buttonMusicToolEdit[i]->show();
		selectMusicTool[i]->show();
	}

	labelOutput->show();
	//int y = yAudioOffset+104+24;
	int y = labelOutput->y() + 24;
	if(m_channel) {
		for(int i=0; i<SamChannel::maxOutputs; i++) {
			if(i<m_channel->numAudioOutputs()) {
				labelChannelOutput[i]->show();
				selectAudioBus[i]->show();
				y += labelChannelOutput[i]->height();
			} else {
				labelChannelOutput[i]->hide();
				selectAudioBus[i]->hide();
			}
		}
	}

	labelSends->show();
	labelSends->move(8,y+=24);
	y += 24;
	if(m_channel) {
		for(int i=0; i<SamAudioBus::maxSends; i++) {	
			//labelChannelSend[i]->move(8,y);
			//labelChannelSend[i]->show();
			buttonSend[i]->move(8, y+2);
			buttonSend[i]->show();
			selectAudioSendBus[i]->show();
			selectAudioSendBus[i]->move(110, y);
			/*if(m_channel->audioOutputBus(0)->audioSend(i)->audioBusId() > 0) {
				selectAudioSendBus[i]->setTextColor(QColor(165, 47, 189));
			} else {
				selectAudioSendBus[i]->setTextColor(QColor(255,255,255));
			}//*/
			faderSends[i]->move(110, y + 24);
			if(m_channel->audioOutputBus(0)->audioSend(i)->audioBusId() > 0) {
				faderSends[i]->show();
			} else {
				faderSends[i]->hide();
			}
			y += 48;
		}
	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::showAllForMultiple(void)
{
	labelInput->show();
	labelMidiPort->show();
	labelMidiChannel->show();
	selectMidiInput->show();
	selectMidiChannel->show();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::hideAll(void)
{
	labelInput->hide();
	labelMidiPort->hide();
	labelMidiChannel->hide();
	selectMidiInput->hide();
	selectMidiChannel->hide();
	buttonMidiMuted->hide();

	labelChannelSettings->hide();
	m_buttonDownbeatOffset->hide();
	m_buttonLayerMutesEnabled->hide();

	labelMusicTools->hide();
	for(int i=0; i<SamChannel::maxMusicTools; i++) {
		//labelMusicTool[i]->hide();
		buttonMusicTool[i]->hide();
		buttonMusicToolEdit[i]->hide();
		selectMusicTool[i]->hide();
	}

	labelOutput->hide();
	for(int i=0; i<SamChannel::maxOutputs; i++) {
		labelChannelOutput[i]->hide();
		selectAudioBus[i]->hide();
	}

	labelSends->hide();
	for(int i=0; i<SamAudioBus::maxSends; i++) {
		//labelChannelSend[i]->hide();
		buttonSend[i]->hide();
		selectAudioSendBus[i]->hide();
		faderSends[i]->hide();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SamGuiChannelProperties::setMidiPort(int itemIndex)
{
	qDebug("SamGuiChannelProperties::setMidiPort(%d)", itemIndex);

	if(m_channel || samEngine->project()->numSelectedChannels() > 1) {
		for(int i=0; i<samEngine->project()->numChannels(); i++) {
			SamChannel *channel = samEngine->project()->channel(i);
			if(channel->isSelected()) {
				channel->setMidiInputPort(itemIndex, this);
			}
		}
	}
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::setMidiChannel(int itemIndex)
{
	//qDebug("SamGuiChannelProperties::setMidiChannel(%d)", itemIndex);

	if(m_channel || samEngine->project()->numSelectedChannels() > 1) {
		for(int i=0; i<samEngine->project()->numChannels(); i++) {
			SamChannel *channel = samEngine->project()->channel(i);
			if(channel->isSelected()) {
				channel->setMidiInputChannel(itemIndex, this);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::updateSelectedChannel(void)
{
	//qDebug("SamGuiChannelProperties::updateSelectedChannel()");

	m_channel = samEngine->project()->selectedChannel();
	if(!m_channel) {
		hideAll();
		if(samEngine->project()->numSelectedChannels() > 1) {
			showAllForMultiple();
		} else {
			selectMidiInput->setParentChannel(0);
			
			for(int i=0; i<SamChannel::maxMusicTools; i++) {
				selectMusicTool[i]->setChannel(0);
			}
			
			for(int i=0; i<SamChannel::maxOutputs; i++) {
				selectAudioBus[i]->setParentAudioBus(0, 0);
			}

			for(int i=0; i<SamAudioBus::maxSends; i++) {
				selectAudioSendBus[i]->setParentAudioBus(0, 0);
			}

		}
	} else {
		showAll();
		if(m_channel->numAudioOutputs()>1) {
			labelOutput->setText("Audio Outputs");
		} else {
			labelOutput->setText("Audio Output");
		}
		resizeEvent(0);
	}

	


	

	if(m_channel) {

		selectMidiInput->setParentChannel(m_channel);
		//updateMidiPort();
		updateMidiChannel();

		buttonMidiMuted->setState(m_channel->isMidiMuted());

		m_buttonDownbeatOffset->setState(m_channel->isDownbeatOffset());
		m_buttonLayerMutesEnabled->setState(m_channel->isLayerMutesEnabled());

		for(int i=0; i<SamChannel::maxMusicTools; i++) {
			buttonMusicTool[i]->setState(m_channel->musicToolInsert(i)->isActive());
			selectMusicTool[i]->setChannel(m_channel);
			selectMusicTool[i]->setInsertIndex(i);
		}

		for(int i=0; i<SamChannel::maxOutputs; i++) {
			if(i<m_channel->numAudioOutputs()) {
				selectAudioBus[i]->setParentAudioBus(m_channel->audioOutputBus(i), 0);
				//labelChannelOutput[i]->setText(m_channel->version()->output(i)->name().toChar());
				labelChannelOutput[i]->setText(m_channel->audioOutputBus(i)->name().toChar());
			} else {
				selectAudioBus[i]->setParentAudioBus(0, 0);
			}
		}

		if(m_channel->numAudioOutputs() > 0) {
			for(int i=0; i<SamAudioBus::maxSends; i++) {
				qDebug("Setting send %d...", i);
				buttonSend[i]->setState(m_channel->audioOutputBus(0)->audioSend(i)->isActive());
				if(i<m_channel->audioOutputBus(0)->numAudioSends()) {
					selectAudioSendBus[i]->setParentAudioBus(m_channel->audioOutputBus(0), m_channel->audioOutputBus(0)->audioSend(i));
					faderSends[i]->setValueNormalised(m_channel->audioOutputBus(0)->audioSend(i)->gain());
				} else {
					selectAudioSendBus[i]->setParentAudioBus(0, 0);
				}
			}
		}
		
	} else {
	
		

	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SamGuiChannelProperties::updateMidiPort(void)
{
	if(m_channel) {
		selectMidiPort->setSelectedItem(m_channel->midiInputPort());
	} else {
		SamChannel *channel = samEngine->project()->firstSelectedChannel();
		if(channel) {
			selectMidiPort->setSelectedItem(channel->midiInputPort());
		}
	}
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::updateMidiChannel(void)
{
	if(m_channel) {
		selectMidiChannel->setSelectedItem(m_channel->midiInputChannel());
	} else {
		SamChannel *channel = samEngine->project()->firstSelectedChannel();
		if(channel) {
			selectMidiChannel->setSelectedItem(channel->midiInputChannel());
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::sendFaderMoved(int faderId, float value, int flags)
{
	//qDebug("SamGuiChannelProperties::sendFaderMoved(%d, %f)", faderId, value);

	if(m_channel) {
		for(int i=0; i<m_channel->numAudioOutputs(); i++) {
			//qDebug("sendFaderMoved: audioOutputBus %d/%d", i+1, m_channel->numAudioOutputs());
			m_channel->audioOutputBus(i)->audioSend(faderId)->setGain(value, this);
		}
		update();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::updateSendGain(void *updatedObject, void *fromObject)
{
	if(m_channel && fromObject != this) {
		for(int i=0; i<4; i++) {
			if(updatedObject == m_channel->audioOutputBus(0)->audioSend(i)) {
				faderSends[i]->setValueNormalised(m_channel->audioOutputBus(0)->audioSend(i)->gain());
			}
		}
		update();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::setMidiMuted(int id, int flags)
{
	if(m_channel) {
		m_channel->setMidiMuted(!m_channel->isMidiMuted(), this);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::setDownbeatOffset(int id, int flags)
{
	if(m_channel) {
		m_channel->setDownbeatOffset(!m_channel->isDownbeatOffset(), this);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::setLayerMutesEnabled(int id, int flags)
{
	if(m_channel) {
		m_channel->setLayerMutesEnabled(!m_channel->isLayerMutesEnabled(), this);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::setMusicToolActive(int id, int flags)
{
	if(m_channel) {
		id -= 2000;
		if(id >= 0 && id < SamChannel::maxMusicTools) {
			samEngine->executeRealTimeCommand(new RtcSetMusicToolActive(m_channel, id, false, !m_channel->musicToolInsert(id)->isActive()));
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::updateMusicToolActive(void)
{
	if(m_channel) {
		for(int i=0; i<SamChannel::maxMusicTools; i++) {
			buttonMusicTool[i]->setState(m_channel->musicToolInsert(i)->isActive());
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::showMusicToolEditor(int id, int flags)
{
	if(m_channel) {
		id -= 2000;
		if(id >= 0 && id < SamChannel::maxMusicTools) {
			if(m_channel->musicToolInsert(id)->isAssigned()) {
				samGui->toolEditor()->setToolInsert(m_channel->musicToolInsert(id), 0);
				samGui->toolEditor()->show();
				
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::updateMidiMuted(void)
{
	if(m_channel) {
		buttonMidiMuted->setState(m_channel->isMidiMuted());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::updateDownbeatOffset(void)
{
	if(m_channel) {
		m_buttonDownbeatOffset->setState(m_channel->isDownbeatOffset());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::updateLayerMutesEnabled(void)
{
	if(m_channel) {
		m_buttonLayerMutesEnabled->setState(m_channel->isLayerMutesEnabled());
	}
}

void SamGuiChannelProperties::updateSendActive(void)
{
	if(m_channel) {
		for(int i=0; i<SamAudioBus::maxSends; i++) {
			buttonSend[i]->setState(m_channel->audioOutputBus(0)->audioSend(i)->isActive());
		}
	}
}

void SamGuiChannelProperties::setSendActive(int id, int flags)
{
	if(m_channel) {
		id -= 3000;
		if(id >= 0 && id < SamAudioBus::maxSends) {
			bool active = !m_channel->audioOutputBus(0)->audioSend(id)->isActive();
			samEngine->executeRealTimeCommand(new RtcSetSendActive(m_channel->audioOutputBus(0)->audioSend(id), active));
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	int bgc = 60;
	p.setPen(QColor(bgc,bgc,bgc));
	p.setBrush(QColor(bgc,bgc,bgc));
	p.drawRect(0,0,width()-1,height()-1);

	if(buttonSend[0]->isVisible()) {
		if(m_channel) {
			if(m_channel->audioOutputBus(0)->audioSend(0)->audioBusId() > 0) {
				p.fillRect(4, buttonSend[0]->y()-4, width()-8, 44, QColor(165, 47, 189, 50));
			}
		}
		p.fillRect(4, buttonSend[0]->y()-4, width()-8, 44, QColor(255,255,255,20));
		if(faderSends[0]->isVisible()) {
			drawSendGain(p, 110+160-40-1, faderSends[0]->y()-4, m_channel->audioOutputBus(0)->audioSend(0)->gain());
		}
	}

	if(buttonSend[1]->isVisible()) {
		if(m_channel) {
			if(m_channel->audioOutputBus(0)->audioSend(1)->audioBusId() > 0) {
				p.fillRect(4, buttonSend[1]->y()-4, width()-8, 44, QColor(165, 47, 189, 50));
			}
		}
		p.fillRect(4, buttonSend[1]->y()-4, width()-8, 44, QColor(0,0,0,20));
		if(faderSends[1]->isVisible()) {
			drawSendGain(p, 110+160-40-1, faderSends[1]->y()-4, m_channel->audioOutputBus(0)->audioSend(1)->gain());
		}
	}

	if(buttonSend[2]->isVisible()) {
		if(m_channel) {
			if(m_channel->audioOutputBus(0)->audioSend(2)->audioBusId() > 0) {
				p.fillRect(4, buttonSend[2]->y()-4, width()-8, 44, QColor(165, 47, 189, 50));
			}
		}
		p.fillRect(4, buttonSend[2]->y()-4, width()-8, 44, QColor(255,255,255,20));
		if(faderSends[2]->isVisible()) {
			drawSendGain(p, 110+160-40-1, faderSends[2]->y()-4, m_channel->audioOutputBus(0)->audioSend(2)->gain());
		}
	}

	if(buttonSend[3]->isVisible()) {
		if(m_channel) {
			if(m_channel->audioOutputBus(0)->audioSend(3)->audioBusId() > 0) {
				p.fillRect(4, buttonSend[3]->y()-4, width()-8, 44, QColor(165, 47, 189, 50));
			}
		}
		p.fillRect(4, buttonSend[3]->y()-4, width()-8, 44, QColor(0,0,0,20));
		if(faderSends[3]->isVisible()) {
			drawSendGain(p, 110+160-40-1, faderSends[3]->y()-4, m_channel->audioOutputBus(0)->audioSend(3)->gain());
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelProperties::resizeEvent(QResizeEvent *ev)
{
	labelInput->setFixedWidth(width()-16);
	labelOutput->setFixedWidth(width()-16);
	labelSends->setFixedWidth(width()-16);
	labelMusicTools->setFixedWidth(width()-16);
	labelChannelSettings->setFixedWidth(width()-16);
}

void SamGuiChannelProperties::drawSendGain(QPainter &p, int x, int y, float gain)
{
	QColor color(255,255,255);
	QLinearGradient linearGradient(QPointF(0, height()-21), QPointF(0, height()));
	color.setAlpha(40);//90
	linearGradient.setColorAt(0, color);
	color.setAlpha(5);//50
	linearGradient.setColorAt(1, color);

	p.setBrush(QColor(00,00,00,100));
	p.setPen(QColor(00,00,00,100));
	//p.drawRect(x, y, 40, 16);
	p.fillRect(x,y,40,16,QColor(0,0,0,100));

	p.setPen(QColor(255,255,255));
	p.setFont(QFont("", 7));
	QString dB = QString::number(20*log10(gain), 'f', 1);
	if(gain < (float)0.000001f) {
		dB = "-inf";
	}
	p.drawText(x, y, 40+1, 16+1, Qt::AlignCenter|Qt::AlignVCenter, dB);
	linearGradient.setStart(QPoint(x, y));
	linearGradient.setFinalStop(QPoint(x, y+16));
	p.fillRect(x+1, y+1, 40-2, (16/2)-1, linearGradient);
}