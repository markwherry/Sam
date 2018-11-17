#include "SamGuiOutputConfigSection.h"

#include "SamEngine.h"
#include "SamRealTimeCommands.h" 

SamGuiOutputConfigSection::SamGuiOutputConfigSection(QWidget *parent)
	: UiSection(parent, "Output Configuration")
	, m_channel(0)
{
	//ui.setupUi(this);

	setFixedHeight(123);

	int y = 4;
	m_button50 = new UiStateButton(this, 100, 2);
	m_button50->setDrawFill(false);
	m_button50->setStateImage(0, ":/Resources/sc_50_16.png");
	m_button50->setStateImage(1, ":/Resources/sc_50_16_on.png");
	m_button50->setFixedSize(16,16);
	m_button50->move(4, y+=24);
	m_button50->setText("5.0");
	//y += 24;
	
	m_buttonQuad = new UiStateButton(this, 101, 2);
	m_buttonQuad->setDrawFill(false);
	m_buttonQuad->setStateImage(0, ":/Resources/sc_quad_16.png");
	m_buttonQuad->setStateImage(1, ":/Resources/sc_quad_16_on.png");
	m_buttonQuad->setFixedSize(16,16);
	m_buttonQuad->move(4, y+=24);
	m_buttonQuad->setText("Quad");

	m_buttonStereo = new UiStateButton(this, 102, 2);
	m_buttonStereo->setDrawFill(false);
	m_buttonStereo->setStateImage(0, ":/Resources/sc_stereo_16.png");
	m_buttonStereo->setStateImage(1, ":/Resources/sc_stereo_16_on.png");
	m_buttonStereo->setFixedSize(16,16);
	m_buttonStereo->move(4, y+=24);
	m_buttonStereo->setText("Stereo");

	m_buttonMono = new UiStateButton(this, 103, 2);
	m_buttonMono->setDrawFill(false);
	m_buttonMono->setStateImage(0, ":/Resources/sc_mono_16.png");
	m_buttonMono->setStateImage(1, ":/Resources/sc_mono_16_on.png");
	m_buttonMono->setFixedSize(16,16);
	m_buttonMono->move(4, y+=24);
	m_buttonMono->setText("Mono");

//  Disable the following four connections until this works again
//	connect(m_button50, SIGNAL(clicked(int)), this, SLOT(outputClicked(int)));
//	connect(m_buttonQuad, SIGNAL(clicked(int)), this, SLOT(outputClicked(int)));
//	connect(m_buttonStereo, SIGNAL(clicked(int)), this, SLOT(outputClicked(int)));
//	connect(m_buttonMono, SIGNAL(clicked(int)), this, SLOT(outputClicked(int)));

	//connect(this, SIGNAL(channelConfigUpdated()), this, SLOT(refresh()));

	//samEngine->registerListener(this, SamEngine::ContextChannel);
}

SamGuiOutputConfigSection::~SamGuiOutputConfigSection()
{

}

/*void SamGuiOutputConfigSection::onState(AStateEvent *ev)
{
	qDebug("SamGuiOutputConfigSection::onState()");

	switch(ev->item()) {
		case SamChannel::ItemConfig:
			qDebug("SamChannel::ItemConfig:");
			emit channelConfigUpdated();
			break;
	}
}*/

void SamGuiOutputConfigSection::setSpeakerConfig(SamChannel *channel, CsSpeakerConfig config)
{
	m_channel = channel;
	m_config = config;

	refresh();
}

void SamGuiOutputConfigSection::outputClicked(int id)
{
	if(!m_channel) {
		return;
	}

	switch(id) {
		case 100:
			//m_channel->setSpeakerConfig(CsSpeakerConfig(CsSpeakerConfig::Surround50));
			samEngine->executeRealTimeCommand(new RtcSetChannelConfig(m_channel, CsSpeakerConfig(CsSpeakerConfig::Surround50)));
			break;
		case 101:
			//m_channel->setSpeakerConfig(CsSpeakerConfig(CsSpeakerConfig::Surround40));
			samEngine->executeRealTimeCommand(new RtcSetChannelConfig(m_channel, CsSpeakerConfig(CsSpeakerConfig::Surround40)));
			break;
		case 102:
			//m_channel->setSpeakerConfig(CsSpeakerConfig(CsSpeakerConfig::Stereo));
			samEngine->executeRealTimeCommand(new RtcSetChannelConfig(m_channel, CsSpeakerConfig(CsSpeakerConfig::Stereo)));
			break;
		case 103:
			//m_channel->setSpeakerConfig(CsSpeakerConfig(CsSpeakerConfig::Mono));
			samEngine->executeRealTimeCommand(new RtcSetChannelConfig(m_channel, CsSpeakerConfig(CsSpeakerConfig::Mono)));
			break;
	}
}

void SamGuiOutputConfigSection::refresh(void)
{
	m_button50->setState(0);
	m_buttonQuad->setState(0);
	m_buttonStereo->setState(0);
	m_buttonMono->setState(0);

	switch(m_config.config()) {
		case CsSpeakerConfig::Mono:
			m_buttonMono->setState(1);
			break;
		case CsSpeakerConfig::Stereo:
			m_buttonStereo->setState(1);
			break;
		case CsSpeakerConfig::Surround40:
			m_buttonQuad->setState(1);
			break;
		case CsSpeakerConfig::Surround50:
			m_button50->setState(1);
			break;
	}
}