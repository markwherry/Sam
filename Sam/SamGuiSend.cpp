#include "SamGuiSend.h"

#include <QPainter>

#include <math.h>

#include "SamEngine.h"
#include "SamProject.h"
#include "SamRealTimeCommands.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiSend::SamGuiSend(QWidget *parent, int index)
	: QWidget(parent)
	, m_channel(0)
	, m_index(m_index)
{
	//ui.setupUi(this);

	int y = 0;

	buttonSend = new UiStateButton(this, 3000, 2);
	buttonSend->move(2,y+2);
	buttonSend->setFixedSize(16, 16);
	buttonSend->setText("Send ");
	buttonSend->setStateImage(0, ":/Resources/send16_3.png");
	buttonSend->setStateImage(1, ":/Resources/send16_3.png");
	buttonSend->setStateFillColor(0,QColor(109, 30, 126));
	buttonSend->setStateFillColor(1,QColor(220, 62, 255)); //165, 47, 189
	buttonSend->setStateLineColor(1,QColor(109, 30, 126));
	connect(buttonSend, SIGNAL(clickedWithFlags(int, int)), this, SLOT(setSendActive(int, int)));

	selectAudioSendBus = new SamGuiSelectAudioOutputBus(this, 0, 0);
	selectAudioSendBus->setFixedWidth(122);
	selectAudioSendBus->setDrawFill(false);
	selectAudioSendBus->setDrawTint(false);
	selectAudioSendBus->setDrawLine(false);
	selectAudioSendBus->setTextBold(false);
	selectAudioSendBus->move(110, y); //110 -> 22

	faderSends = new UiFader(this, false, 140, 16, 8); //100 -> 140
	faderSends->setLevelColor(QColor(70, 70, 70));
	faderSends->move(110, y + 24); // 110 - > 2

	buttonPreFader = new UiStateButton(this, 3000, 2);
	buttonPreFader->move(faderSends->x()+faderSends->width()+4, 2);
	buttonPreFader->setFixedSize(16, 16);
	//buttonPreFader->setText("Send ");
	buttonPreFader->setStateImage(0, ":/Resources/send_prefade16.png");
	buttonPreFader->setStateImage(1, ":/Resources/send_prefade16.png");
	buttonPreFader->setStateFillColor(0,QColor(109, 30, 126));
	buttonPreFader->setStateFillColor(1,QColor(220, 62, 255)); //165, 47, 189
	buttonPreFader->setStateLineColor(1,QColor(109, 30, 126));
	connect(buttonPreFader, SIGNAL(clickedWithFlags(int, int)), this, SLOT(setSendPreFader(int, int)));

	setFixedSize(296-4, 40);//y);

	connect(faderSends, SIGNAL(valueChanged(int, float, int)), this, SLOT(sendFaderMoved(int, float, int)));

	connect(this, SIGNAL(sendGainUpdated(void*, void*)), this, SLOT(updateSendGain(void*, void*)));
	connect(this, SIGNAL(sendActiveUpdated()), this, SLOT(updateSendActive()));
	connect(this, SIGNAL(sendPreFaderUpdated()), this, SLOT(updateSendPreFader()));

	samEngine->registerListener(this, SamEngine::ContextChannel);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiSend::~SamGuiSend()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSend::onState(AStateEvent *ev)
{
	if(ev->context() == SamEngine::ContextChannel) {
		switch(ev->item()) {
			case SamChannel::ItemSendAudioBus:
				//qDebug("SamChannel::ItemSendAudioBus");
				//emit selectedChannelUpdated();
				break;
			case SamChannel::ItemSendGain:
				//qDebug("SamChannel::ItemSendGain");
				emit sendGainUpdated(ev->updatedObject(), ev->fromObject());
				break;
			case SamChannel::ItemSendActive:
				qDebug("SamChannel::ItemSendActive:");
				emit sendActiveUpdated();
				break;
			case SamChannel::ItemSendPreFader:
				qDebug("SamChannel::ItemSendPreFader:");
				emit sendPreFaderUpdated();
				break;
		}
	} 
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSend::setChannel(SamChannel *channel, int index)
{
	//qDebug("***** SamGuiSend::setChannel(%d, %d)", channel, index);

	m_channel = channel;
	m_index   = index;

	if(m_channel) {
		setEnabled(true);
		//buttonMusicTool->setState(m_channel->musicToolInsert(m_index)->isActive());
		//selectMusicTool->setChannel(m_channel);
		//selectMusicTool->setInsertIndex(m_index);
		//buttonMusicTool->setFixedSize(16, 16);
		//buttonMusicTool->setText("Tool "+QString::number(m_index+1));

		buttonSend->setFixedSize(16,16);
		buttonSend->setText("Send "+QString::number(m_index+1));

		if(m_channel->numAudioOutputs() > 0) {
			

				buttonSend->setState(m_channel->audioOutputBus(0)->audioSend(m_index)->isActive());
				updateSendActive();
				buttonPreFader->setState(m_channel->audioOutputBus(0)->audioSend(m_index)->isPreFader());
				if(m_index<m_channel->audioOutputBus(0)->numAudioSends()) {
					selectAudioSendBus->setParentAudioBus(m_channel->audioOutputBus(0), m_channel->audioOutputBus(0)->audioSend(m_index));
					faderSends->setValueNormalised(m_channel->audioOutputBus(0)->audioSend(m_index)->gain());
					
				} else {
					selectAudioSendBus->setParentAudioBus(0, 0);
				}
			
		}


	} else {
		setEnabled(false);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSend::sendFaderMoved(int faderId, float value, int flags)
{
	//qDebug("SamGuiChannelProperties::sendFaderMoved(%d, %f)", faderId, value);

	if(m_channel) {
		for(int i=0; i<m_channel->numAudioOutputs(); i++) {
			//qDebug("sendFaderMoved: audioOutputBus %d/%d", i+1, m_channel->numAudioOutputs());
			m_channel->audioOutputBus(i)->audioSend(m_index)->setGain(value, this);
		}
		update();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSend::updateSendGain(void *updatedObject, void *fromObject)
{
	if(m_channel && fromObject != this) {
		
		if(updatedObject == m_channel->audioOutputBus(0)->audioSend(m_index)) {
			faderSends->setValueNormalised(m_channel->audioOutputBus(0)->audioSend(m_index)->gain());
		}
		
		update();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSend::updateSendActive(void)
{
	if(m_channel) {
		buttonSend->setState(m_channel->audioOutputBus(0)->audioSend(m_index)->isActive());
		if(m_channel->audioOutputBus(0)->audioSend(m_index)->isActive()) {
			faderSends->setLevelColor(QColor(165, 47, 189));
		} else {
			faderSends->setLevelColor(QColor(70,70,70));
		}
	}
	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSend::updateSendPreFader(void)
{
	if(m_channel) {
		buttonPreFader->setState(m_channel->audioOutputBus(0)->audioSend(m_index)->isPreFader());
	}
	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSend::setSendActive(int id, int flags)
{
	if(m_channel) {
		bool active = !m_channel->audioOutputBus(0)->audioSend(m_index)->isActive();
		for(int i=0; i<m_channel->numAudioOutputs(); i++) {
			samEngine->executeRealTimeCommand(new RtcSetSendActive(m_channel->audioOutputBus(i)->audioSend(m_index), active));
		}
	}
	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSend::setSendPreFader(int id, int flags)
{
	if(m_channel) {
		bool preFader = !m_channel->audioOutputBus(0)->audioSend(m_index)->isPreFader();
		for(int i=0; i<m_channel->numAudioOutputs(); i++) {
			samEngine->executeRealTimeCommand(new RtcSetSendPreFader(m_channel->audioOutputBus(i)->audioSend(m_index), preFader));
		}
	}
	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSend::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	if(!m_channel) {
		if(m_index%2 == 0) {
			p.fillRect(0,0,width(),height(),QColor(0,0,0,20));
		} else {
			p.fillRect(0,0,width(),height(),QColor(255,255,255,10));
		}
		return;
	}

	//int bgc = 60;
	//p.setPen(QColor(bgc,bgc,bgc));
	//p.setBrush(QColor(bgc,bgc,bgc));
	//p.drawRect(0,0,width()-1,height()-1);

	/*if(buttonSend->isVisible()) {
		if(m_channel) {
			if(m_channel->audioOutputBus(0)->audioSend(0)->audioBusId() > 0) {
				p.fillRect(4, buttonSend->y()-4, width()-8, 44, QColor(165, 47, 189, 50));
			}
		
			p.fillRect(4, buttonSend->y()-4, width()-8, 44, QColor(255,255,255,20));
			if(faderSends->isVisible()) {
				drawSendGain(p, 110+160-40-1, faderSends->y()-4, m_channel->audioOutputBus(0)->audioSend(0)->gain());
			}
		}
	}*/

	if(m_index%2 == 0) {
		if(m_channel->audioOutputBus(0)->audioSend(m_index)->audioBusId() > 0 && m_channel->audioOutputBus(0)->audioSend(m_index)->isActive()) {
			p.fillRect(0,0,width(),height(),QColor(165, 47, 189, 50));
		} else {
			p.fillRect(0,0,width(),height(),QColor(0,0,0,20));
		}
	} else {
		if(m_channel->audioOutputBus(0)->audioSend(m_index)->audioBusId() > 0 && m_channel->audioOutputBus(0)->audioSend(m_index)->isActive()) {
			p.fillRect(0,0,width(),height(),QColor(165, 47, 189, 25));
		} else {
			p.fillRect(0,0,width(),height(),QColor(255,255,255,10));
		}
	}

	
	//drawSendGain(p, faderSends->x()+faderSends->width()+4, faderSends->y()-4, m_channel->audioOutputBus(0)->audioSend(m_index)->gain()); //110+160-40-1
	drawSendGain(p, faderSends->x()+faderSends->width()+4, faderSends->y()-4, m_channel->audioOutputBus(0)->audioSend(m_index)->gain()); //110+160-40-1
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSend::drawSendGain(QPainter &p, int x, int y, float gain)
{
	int w = 40 - 3;
	QColor color(255,255,255);
	QLinearGradient linearGradient(QPointF(0, height()-21), QPointF(0, height()));
	color.setAlpha(40);//90
	linearGradient.setColorAt(0, color);
	color.setAlpha(5);//50
	linearGradient.setColorAt(1, color);

	p.setBrush(QColor(00,00,00,100));
	p.setPen(QColor(00,00,00,100));
	//p.drawRect(x, y, 40, 16);
	p.fillRect(x,y,w,16,QColor(0,0,0,100));

	p.setPen(QColor(255,255,255));
	p.setFont(QFont("", 7));
	QString dB = QString::number(20*log10(gain), 'f', 1);
	if(gain < (float)0.000001f) {
		dB = "-inf";
	}
	p.drawText(x, y, w+1, 16+1, Qt::AlignCenter|Qt::AlignVCenter, dB);
	linearGradient.setStart(QPoint(x, y));
	linearGradient.setFinalStop(QPoint(x, y+16));
	p.fillRect(x+1, y+1, w-2, (16/2)-1, linearGradient);
}