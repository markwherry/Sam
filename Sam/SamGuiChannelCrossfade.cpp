#include "SamGuiChannelCrossfade.h"

#include "SamEngine.h"
#include "SamProject.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiChannelCrossfade::SamGuiChannelCrossfade(QWidget *parent)
	: QWidget(parent)
	, m_channel(0)
{
	//ui.setupUi(this);

	int y = 0;

	m_buttonCrossfade = new UiStateButton(this, 1000, 2);
	m_buttonCrossfade->move(4, y+=4);
	m_buttonCrossfade->setFixedSize(16, 16);
	//buttonMidiMuted->setBackground();
	m_buttonCrossfade->setStateImage(0, ":/Resources/cross16.png");
	m_buttonCrossfade->setStateImage(1, ":/Resources/cross16.png");
	m_buttonCrossfade->setText("Articulation Crossfade Mode");
	//buttonMidiMuted->setBackgroundColor(0,QColor(255,130,0,30));
	m_buttonCrossfade->setStateFillColor(0,QColor(102,147,25));
	m_buttonCrossfade->setStateFillColor(1,QColor(149,255,25));
	m_buttonCrossfade->setStateLineColor(1,QColor(102,147,25));
	//buttonMidiMuted->setDrawTint();
	//buttonMidiMuted->setDrawOutline();

	connect(m_buttonCrossfade, SIGNAL(clickedWithFlags(int, int)), this, SLOT(setCrossfadeMode(int, int)));

	m_curve = new SeGuiItemCurve(this, "Articulation Curve");
	m_curve->move(0, y+= 20);
	m_curve->setFixedWidth(296);
	m_curve->setPropertyIdShape(0);
	m_curve->setPropertyIdDepth(1);
	m_curve->setPropertyIdStart(2);
	m_curve->setPropertyIdEnd(3);
	connect(m_curve, SIGNAL(shapeChanged(int, float)), this, SLOT(setValue(int, float)));
	connect(m_curve, SIGNAL(startChanged(int, float)), this, SLOT(setValue(int, float)));
	connect(m_curve, SIGNAL(endChanged(int, float)), this, SLOT(setValue(int, float)));
	connect(m_curve, SIGNAL(depthChanged(int, float)), this, SLOT(setValue(int, float)));

	setFixedSize(296, y+m_curve->height());

	connect(this, SIGNAL(selectedChannelUpdated()), this, SLOT(updateSelectedChannel()));
	connect(this, SIGNAL(crossfadeUpdated()), this, SLOT(updateCrossfadeMode()));

	samEngine->registerListener(this, SamEngine::ContextChannel);
	samEngine->registerListener(this, SamEngine::ContextProject);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiChannelCrossfade::~SamGuiChannelCrossfade()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelCrossfade::onState(AStateEvent *ev)
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
void SamGuiChannelCrossfade::updateSelectedChannel(void)
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
		m_curve->setCurveColor(QColor(90,90,90));
	} else {
		updateCrossfadeMode();	
		m_curve->setShape(m_channel->curveArticulationCrossfade()->shape());
		m_curve->setDepth(m_channel->curveArticulationCrossfade()->depth());
		m_curve->setStart(m_channel->curveArticulationCrossfade()->start());
		m_curve->setEnd(m_channel->curveArticulationCrossfade()->end());
	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelCrossfade::setCrossfadeMode(int id, int flags)
{
	if(m_channel) {
		m_channel->setArticulationMode(!m_channel->isArticulationMode());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelCrossfade::updateCrossfadeMode(void)
{
	if(m_channel) {
		m_buttonCrossfade->setState((int)m_channel->isArticulationMode());
		if(m_channel->isArticulationMode()) {
			m_curve->setCurveColor(QColor(149,255,25));
		} else {
			m_curve->setCurveColor(QColor(90,90,90));
		}
		m_curve->update();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelCrossfade::setValue(int propertyId, float value)
{
	if(!m_channel) return;

	switch(propertyId) {
		case 0: // Shape
			m_channel->curveArticulationCrossfade()->setShape((int)value);
			break;
		case 1: // Depth
			m_channel->curveArticulationCrossfade()->setDepth(value);
			break;
		case 2: // Start
			m_channel->curveArticulationCrossfade()->setStart(value);
			break;
		case 3: // End
			m_channel->curveArticulationCrossfade()->setEnd(value);
			break;
	}
}