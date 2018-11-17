#include "SeGuiArticulationProperties.h"

#include "SamGuiEditor.h"
#include "SamArticulationCommand.h"

SeGuiArticulationProperties::SeGuiArticulationProperties(QWidget *parent)
	: QWidget(parent)
	, m_articulation(0)
{
	//ui.setupUi(this);

	m_list = new UiList(this);
	m_list->setSelectable(false);
	m_list->setMovable(false);

	m_name = new SeGuiItemText(this, SamArticulationCommand::Name, "Name");
	connect(m_name, SIGNAL(textChanged(int, QString)), this, SLOT(setText(int, QString)));
	m_list->addItem(m_name);

	m_triggerId = new SeGuiItemValue(this, SamArticulationCommand::TriggerId, "Trigger Id", "", -1, 127, -1, 1, 2);
	m_triggerId->setExtraInfo(5);
	connect(m_triggerId, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_triggerId);

	//m_minimumVelocityController = new SeGuiItemValue(this, SamArticulationCommand::MinimumVelocityController, "Min Vel Controller", "", -1, 127, -1, 1, 2);
	//connect(m_minimumVelocityController, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	//m_list->addItem(m_minimumVelocityController);

	m_zoneCrossfadeResponseTime = new SeGuiItemValue(this, SamArticulationCommand::ZoneCrossfadeResponseTime, "Zone Cf Response", "ms", 1, 1000, 1, 1, 2);
	connect(m_zoneCrossfadeResponseTime, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_zoneCrossfadeResponseTime);

	m_zoneFilterResponseTime = new SeGuiItemValue(this, SamArticulationCommand::ZoneFilterResponseTime, "Zone Filter Response", "ms", 1, 1000, 1, 1, 2);
	connect(m_zoneFilterResponseTime, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_zoneFilterResponseTime);

	m_polyphonicModulation = new SeGuiItemBool(this, SamArticulationCommand::PolyphonicModulation, "Poly Modulation");
	connect(m_polyphonicModulation, SIGNAL(changed(int, bool)), this, SLOT(setState(int, bool)));
	m_list->addItem(m_polyphonicModulation);

	//m_maxAftertouch = new SeGuiItemValue(this, SamArticulationCommand::MaxAftertouch, "Maximum Aftertouch", "", 0, 127, 127, 1, 2);
	//connect(m_maxAftertouch, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	//m_list->addItem(m_maxAftertouch);

	m_velocityCrossfade = new SeGuiItemHeading(this, "Velocity Crossfade");
	m_list->addItem(m_velocityCrossfade);
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_velocitySetsCrossfade = new SeGuiItemBool(this, SamArticulationCommand::VelocitySetsCrossfade, "Velocity Sets Cf");
	connect(m_velocitySetsCrossfade, SIGNAL(changed(int, bool)), this, SLOT(setState(int, bool)));
	m_list->addItem(m_velocitySetsCrossfade);

	/*m_velocityCrossfadeDecay = new SeGuiItemBool(this, SamArticulationCommand::VelocityCrossfadeDecay, "Vel Cf Decay");
	connect(m_velocityCrossfadeDecay, SIGNAL(changed(int, bool)), this, SLOT(setState(int, bool)));
	//m_list->addItem(m_velocityCrossfadeDecay);

	m_velocityCrossfadeDecayTime = new SeGuiItemValue(this, SamArticulationCommand::VelocityCrossfadeDecayTime, "Vel Cf Decay Time", "ms", 0, 4000, 0, 1, 2);
	connect(m_velocityCrossfadeDecayTime, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	//m_list->addItem(m_velocityCrossfadeDecayTime);

	m_velocityCrossfadeDecayTo = new SeGuiItemValue(this, SamArticulationCommand::VelocityCrossfadeDecayTo, "Vel Cf Decay To", "", 0, 127, 0, 1, 2);
	connect(m_velocityCrossfadeDecayTo, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	//m_list->addItem(m_velocityCrossfadeDecayTo);*/

	m_curvesAndReleases = new SeGuiItemHeading(this, "Curves");
	m_list->addItem(m_curvesAndReleases);
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_velocityCurve = new SeGuiItemCurve(this, "Velocity Curve");
	m_velocityCurve->setPropertyIdShape(SamArticulationCommand::CurveVelocityShape);
	m_velocityCurve->setPropertyIdDepth(SamArticulationCommand::CurveVelocityDepth);
	m_velocityCurve->setPropertyIdStart(SamArticulationCommand::CurveVelocityStart);
	m_velocityCurve->setPropertyIdEnd(SamArticulationCommand::CurveVelocityEnd);
	connect(m_velocityCurve, SIGNAL(shapeChanged(int, float)), this, SLOT(setValue(int, float)));
	connect(m_velocityCurve, SIGNAL(startChanged(int, float)), this, SLOT(setValue(int, float)));
	connect(m_velocityCurve, SIGNAL(endChanged(int, float)), this, SLOT(setValue(int, float)));
	connect(m_velocityCurve, SIGNAL(depthChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_velocityCurve);

	m_crossfadeCurve = new SeGuiItemCurve(this, "Crossfade Controller Attenuation Curve");
	m_crossfadeCurve->setPropertyIdShape(SamArticulationCommand::CurveCrossfadeShape);
	m_crossfadeCurve->setPropertyIdDepth(SamArticulationCommand::CurveCrossfadeDepth);
	m_crossfadeCurve->setPropertyIdStart(SamArticulationCommand::CurveCrossfadeStart);
	m_crossfadeCurve->setPropertyIdEnd(SamArticulationCommand::CurveCrossfadeEnd);
	connect(m_crossfadeCurve, SIGNAL(shapeChanged(int, float)), this, SLOT(setValue(int, float)));
	connect(m_crossfadeCurve, SIGNAL(startChanged(int, float)), this, SLOT(setValue(int, float)));
	connect(m_crossfadeCurve, SIGNAL(endChanged(int, float)), this, SLOT(setValue(int, float)));
	connect(m_crossfadeCurve, SIGNAL(depthChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_crossfadeCurve);

	m_releaseCurve = new SeGuiItemCurve(this, "Release Amplitude Curve");
	m_releaseCurve->setPropertyIdShape(SamArticulationCommand::CurveReleaseShape);
	m_releaseCurve->setPropertyIdDepth(SamArticulationCommand::CurveReleaseDepth);
	m_releaseCurve->setPropertyIdStart(SamArticulationCommand::CurveReleaseStart);
	m_releaseCurve->setPropertyIdEnd(SamArticulationCommand::CurveReleaseEnd);
	connect(m_releaseCurve, SIGNAL(shapeChanged(int, float)), this, SLOT(setValue(int, float)));
	connect(m_releaseCurve, SIGNAL(startChanged(int, float)), this, SLOT(setValue(int, float)));
	connect(m_releaseCurve, SIGNAL(endChanged(int, float)), this, SLOT(setValue(int, float)));
	connect(m_releaseCurve, SIGNAL(depthChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_releaseCurve);

	m_outputs = new SeGuiItemHeading(this, "Outputs");
	m_list->addItem(m_outputs);
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_output = new SeGuiItemSelectOutput(this, SamArticulationCommand::Output, "Output");
	connect(m_output, SIGNAL(outputChanged(int)), this, SLOT(setItem(int, float)));
	m_list->addItem(m_output);
	m_releaseOutput = new SeGuiItemSelectOutput(this, SamArticulationCommand::OutputRelease, "Release Output");
	connect(m_releaseOutput, SIGNAL(outputChanged(int)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_releaseOutput);

	m_layerControls = new SeGuiItemHeading(this, "Layer Controls");
	m_list->addItem(m_layerControls);
	m_list->addItem(new SeGuiItemHeading(this, ""));
	
	m_defaultLayer = new SeGuiItemValue(this, SamArticulationCommand::LayerDefault, "Default Layer", "", 0, 0, 0, 1, 2);
	connect(m_defaultLayer, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_defaultLayer);

	m_layerSelectMode = new SeGuiItemValue(this, SamArticulationCommand::LayerSelectMode, "Layer Select Mode", "", 0, 3, 0, 1, 2);
	m_layerSelectMode->setExtraInfo(6);
	connect(m_layerSelectMode, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_layerSelectMode);

	m_switchLayerTime = new SeGuiItemValue(this, SamArticulationCommand::LayerSwitchTime, "Switch Layer Time", "ms", 0, 1000, 0, 1, 2);
	connect(m_switchLayerTime, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_switchLayerTime);

	m_legatoOverlap = new SeGuiItemValue(this, SamArticulationCommand::LayerLegatoOverlap, "Legato Overlap Time", "ms", 0, 1000, 0, 1, 2);
	connect(m_legatoOverlap, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_legatoOverlap);

	setFixedSize(296, m_list->height());
}

SeGuiArticulationProperties::~SeGuiArticulationProperties()
{

}

void SeGuiArticulationProperties::setVersion(SamVersion *version, SamArticulation *articulation)
{
	m_version = version;
	m_articulation = articulation;

	if(m_articulation) {
		m_name->setText(m_articulation->name().toChar());
		m_triggerId->setValue(m_articulation->id());
		m_zoneCrossfadeResponseTime->setValue(m_articulation->zoneCrossfadeResponseTime());
		m_zoneFilterResponseTime->setValue(m_articulation->zoneFilterResponseTime());
		//m_minimumVelocityController->setValue(m_articulation->minimumVelocityController());
		m_polyphonicModulation->setChecked((bool)m_articulation->polyphonicModulation());
		//m_maxAftertouch->setValue(m_articulation->maxAftertouch());
		m_velocitySetsCrossfade->setChecked(m_articulation->isVelocitySetsCrossfade());
		//m_velocityCrossfadeDecay->setChecked(m_articulation->isVelocityCrossfadeDecay());
		//m_velocityCrossfadeDecayTime->setValue(m_articulation->velocityCrossfadeDecayTime());
		//m_velocityCrossfadeDecayTo->setValue(m_articulation->velocityCrossfadeDecayTo());
		m_velocityCurve->setShape(m_articulation->velocityCurve()->shape());
		m_velocityCurve->setDepth(m_articulation->velocityCurve()->depth());
		m_velocityCurve->setStart(m_articulation->velocityCurve()->start());
		m_velocityCurve->setEnd(m_articulation->velocityCurve()->end());
		m_crossfadeCurve->setShape(m_articulation->curveCrossfadeGain()->shape());
		m_crossfadeCurve->setDepth(m_articulation->curveCrossfadeGain()->depth());
		m_crossfadeCurve->setStart(m_articulation->curveCrossfadeGain()->start());
		m_crossfadeCurve->setEnd(m_articulation->curveCrossfadeGain()->end());
		m_releaseCurve->setShape(m_articulation->curveReleaseAttenuation()->shape());
		m_releaseCurve->setDepth(m_articulation->curveReleaseAttenuation()->depth());
		m_releaseCurve->setStart(m_articulation->curveReleaseAttenuation()->start());
		m_releaseCurve->setEnd(m_articulation->curveReleaseAttenuation()->end());
		m_defaultLayer->setValue(m_articulation->defaultLayer());
		m_layerSelectMode->setValue(m_articulation->layerSelectMode());
		m_switchLayerTime->setValue(m_articulation->switchLayerTime());
		m_legatoOverlap->setValue(m_articulation->legatoOverlap());
		m_output->setVersion(m_version);
		m_output->setOutput(m_articulation->outputListId());
		m_releaseOutput->setVersion(m_version);
		m_releaseOutput->setOutput(m_articulation->releaseOutputListId());
	}
}

void SeGuiArticulationProperties::setValue(int propertyId, float value)
{
	if(m_articulation) {
		samEditor->commandManager()->execute(new SamArticulationCommand(m_articulation, propertyId, value));
	}
}

void SeGuiArticulationProperties::setState(int propertyId, bool state)
{
	if(m_articulation) {
		samEditor->commandManager()->execute(new SamArticulationCommand(m_articulation, propertyId, state));
	}
}

void SeGuiArticulationProperties::setText(int propertyId, QString text)
{
	//qDebug("SeGuiArticulationProperties::setText(%d, %s)", propertyId, qPrintable(text));
	if(m_articulation) {
		QByteArray ba = text.toAscii();
		samEditor->commandManager()->execute(new SamArticulationCommand(m_articulation, propertyId, AString(ba.data())));
	}
}

void SeGuiArticulationProperties::resizeEvent(QResizeEvent *ev)
{
	m_list->setFixedSize(width(),height());
}