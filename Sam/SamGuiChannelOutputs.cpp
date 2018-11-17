#include "SamGuiChannelOutputs.h"

#include <QPainter>

#include "SamEngine.h"
#include "SamProject.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiChannelOutputs::SamGuiChannelOutputs(QWidget *parent)
	: QWidget(parent)
	, m_channel(0)
{
	//ui.setupUi(this);

	QPalette plt = palette();
	plt.setColor(QPalette::Background, QColor(60,60,60));
	setPalette(plt);
	setAutoFillBackground(true);
	setBackgroundRole(QPalette::Background);

	int y = 4;

	for(int i=0; i<SamChannel::maxOutputs; i++) {
		labelChannelOutput[i] = new UiLabel(this, "Output");
		labelChannelOutput[i]->setImage(":/Resources/audio16.png");
		labelChannelOutput[i]->move(2,y);
		labelChannelOutput[i]->setTextColor(QColor(180,180,180));
		labelChannelOutput[i]->hide();

		selectAudioBus[i] = new SamGuiSelectAudioOutputBus(this, 0, 0);
		selectAudioBus[i]->setDrawFill(false);
		selectAudioBus[i]->setDrawTint(false);
		selectAudioBus[i]->setDrawLine(false);
		selectAudioBus[i]->move(110, y); //22
		selectAudioBus[i]->hide();

		y += 24;
	}

	setFixedSize(296, y);

	connect(this, SIGNAL(selectedChannelUpdated()), this, SLOT(updateSelectedChannel()));

	samEngine->registerListener(this, SamEngine::ContextChannel);
	samEngine->registerListener(this, SamEngine::ContextProject);

	m_pixBackground = new QPixmap(":/Resources/black_linen_v2.png");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiChannelOutputs::~SamGuiChannelOutputs()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelOutputs::onState(AStateEvent *ev)
{
	//qDebug("SamGuiChannelProperties::onState()");

	if(ev->context() == SamEngine::ContextChannel) {
		switch(ev->item()) {
			case SamChannel::ItemSelected:
				emit selectedChannelUpdated();
				break;
		}
	} else if(ev->context() == SamEngine::ContextProject) {
		switch(ev->item()) {
			case SamProject::ItemChannelRemoved:
				emit selectedChannelUpdated();
				break;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelOutputs::updateSelectedChannel(void)
{
	//qDebug("SamGuiChannelProperties::updateSelectedChannel()");

	m_channel = samEngine->project()->selectedChannel();
	if(!m_channel) {
		//hideAll();
		for(int i=0; i<SamChannel::maxOutputs; i++) {
			labelChannelOutput[i]->hide();
			selectAudioBus[i]->hide();
		}
		if(samEngine->project()->numSelectedChannels() > 1) {
			//showAllForMultiple();
		} else {
			
			
			
			
			for(int i=0; i<SamChannel::maxOutputs; i++) {
				selectAudioBus[i]->setParentAudioBus(0, 0);
			}

			

		}
	} else {
		//showAll();
		for(int i=0; i<SamChannel::maxOutputs; i++) {
			if(i<m_channel->numAudioOutputs()) {
				labelChannelOutput[i]->show();
				selectAudioBus[i]->show();
				//y += labelChannelOutput[i]->height();
			} else {
				labelChannelOutput[i]->hide();
				selectAudioBus[i]->hide();
			}
		}
		
		//resizeEvent(0);
	}

	


	

	if(m_channel) {

		

		for(int i=0; i<SamChannel::maxOutputs; i++) {
			if(i<m_channel->numAudioOutputs()) {
				selectAudioBus[i]->setParentAudioBus(m_channel->audioOutputBus(i), 0);
				//labelChannelOutput[i]->setText(m_channel->version()->output(i)->name().toChar());
				labelChannelOutput[i]->setText(m_channel->audioOutputBus(i)->name().toChar());
			} else {
				selectAudioBus[i]->setParentAudioBus(0, 0);
			}
		}

		
		
	} else {
	
		

	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelOutputs::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	//p.drawPixmap(0,0,*m_pixBackground);
	p.fillRect(0,0,width(),height(),QColor(50,50,50));
}