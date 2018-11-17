#include "SamGuiArticulationSection.h"

#include "SamEngine.h"
#include "SamProject.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiArticulationSection::SamGuiArticulationSection(QWidget *parent)
	: UiSection(parent, "Articulation Crossfade")
	, m_channel(0)
{
	//ui.setupUi(this);

	setFixedHeight(74);

	int y = 4;

	m_buttonCrossfade = new UiStateButton(this, 1000, 2);
	m_buttonCrossfade->move(4, y+=24);
	m_buttonCrossfade->setFixedSize(16, 16);
	//buttonMidiMuted->setBackground();
	m_buttonCrossfade->setStateImage(0, ":/Resources/cross16.png");
	m_buttonCrossfade->setStateImage(1, ":/Resources/cross16.png");
	m_buttonCrossfade->setText("Enable / Disable");
	//buttonMidiMuted->setBackgroundColor(0,QColor(255,130,0,30));
	m_buttonCrossfade->setStateFillColor(0,QColor(102,147,25));
	m_buttonCrossfade->setStateFillColor(1,QColor(149,255,25));
	m_buttonCrossfade->setStateLineColor(1,QColor(102,147,25));
	//buttonMidiMuted->setDrawTint();
	//buttonMidiMuted->setDrawOutline();

	connect(m_buttonCrossfade, SIGNAL(clickedWithFlags(int, int)), this, SLOT(setCrossfadeMode(int, int)));

	m_buttonStore = new UiStateButton(this, 110, 1);
	m_buttonStore->setDrawFill(false);
	m_buttonStore->setStateImage(0, ":/Resources/plus16.png");
	//m_buttonStore->setStateImage(1, ":/Resources/savemixtoinstrument16.png");
	m_buttonStore->setFixedSize(16,16);
	m_buttonStore->move(4, y+=24);
	m_buttonStore->setText("Set as Default");
	m_buttonStore->setStateLineColor(0,QColor(200,200,200));

	connect(m_buttonStore, SIGNAL(clicked(int)), this, SLOT(setAsDefault(int)));

	

	connect(this, SIGNAL(selectedChannelUpdated()), this, SLOT(updateSelectedChannel()));
	connect(this, SIGNAL(crossfadeUpdated()), this, SLOT(updateCrossfadeMode()));

	samEngine->registerListener(this, SamEngine::ContextChannel);
	samEngine->registerListener(this, SamEngine::ContextProject);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiArticulationSection::~SamGuiArticulationSection()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiArticulationSection::onState(AStateEvent *ev)
{
	if(ev->context() == SamEngine::ContextChannel) {
		switch(ev->item()) {
			case SamChannel::ItemSelected:
				emit selectedChannelUpdated();
				break;
			case SamChannel::ItemArticulationMode:
				emit crossfadeUpdated();
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
void SamGuiArticulationSection::updateSelectedChannel(void)
{
	//qDebug("SamGuiChannelProperties::updateSelectedChannel()");

	//if(m_channel

	m_channel = samEngine->project()->selectedChannel();

	if(!m_channel) {
		if(samEngine->project()->numSelectedChannels() > 1) {
			// Show for multiple
		} else {
			// ??? Should handle this elsewhere

		}
		//m_curve->setCurveColor(QColor(90,90,90));
	} else {
		updateCrossfadeMode();	
		//m_curve->setShape(m_channel->curveArticulationCrossfade()->shape());
		//m_curve->setDepth(m_channel->curveArticulationCrossfade()->depth());
		//m_curve->setStart(m_channel->curveArticulationCrossfade()->start());
		//m_curve->setEnd(m_channel->curveArticulationCrossfade()->end());
	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiArticulationSection::setCrossfadeMode(int id, int flags)
{
	if(m_channel) {
		m_channel->setArticulationMode(!m_channel->isArticulationMode());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiArticulationSection::updateCrossfadeMode(void)
{
	if(m_channel) {
		m_buttonCrossfade->setState((int)m_channel->isArticulationMode());
		/*if(m_channel->isArticulationMode()) {
			m_curve->setCurveColor(QColor(149,255,25));
		} else {
			m_curve->setCurveColor(QColor(90,90,90));
		}
		m_curve->update();*/
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiArticulationSection::setAsDefault(int id)
{
	qDebug("SamGuiArticulationSection::setAsDefault()");

	if(m_channel) {
		m_channel->setDefaultCfArticulations(m_channel->currentArticulationIndex(), m_channel->currentDefaultArticulationIndex());
	}
}