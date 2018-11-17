#include "SeGuiZoneProperties.h"

#include "SamGuiEditor.h"
#include "SamZoneCommand.h"

SeGuiZoneProperties::SeGuiZoneProperties(QWidget *parent, int category)
	: QWidget(parent)
	, m_category(category)
	, m_zone(0)
{
	//ui.setupUi(this);

	m_list = new UiList(this);
	m_list->setSelectable(false);
	m_list->setMovable(false);

	switch(m_category) {
		case 0: // General
			initGeneral();
			break;
		case 1: // Levels and Outputs
			initLevelsAndOutputs();
			break;
		case 2: // Pitch
			initPitch();
			break;
		case 3: //Velocity
			initVelocity();
			break;
		case 4: // Triggering
			initTriggering();
			break;
		case 5: // Filter
			initFilter();
			break;
		case 6: // Env 1
			initEnvelope1();
			break;
		case 7: // Env 2
			initEnvelope2();
			break;
		case 8: // Env 3
			initEnvelope3();
			break;
		case 9: // Tempo Release Modulation
			initTempoReleaseModulation();
			break;
		case 10: // Crossfade
			initCrossfade();
			break;
	}

	setDisabled(true);

	setFixedSize(296, m_list->height());
}

SeGuiZoneProperties::~SeGuiZoneProperties()
{

}

void SeGuiZoneProperties::setZone(SamZone *zone, int numZonesSelected)
{
	//qDebug("SeGuiZoneProperties::setZone()");

	m_zone = zone;
	if(!m_zone || numZonesSelected < 1) {
		qDebug("!m_zone [%d] numZonesSelected = %d", m_zone, numZonesSelected);
		setDisabled(true);
		return;
	}

	setDisabled(false);

	switch(m_category) {
		case 0: // General
			m_name->setText(m_zone->name().toChar());
			m_description->setText(m_zone->description().toChar());
			//m_group->setValue(m_zone->groupId());
			m_dynamicRef->setValue(m_zone->dynamicRef());
			m_sampleStart->setValue(m_zone->sampleStart());
			break;
		case 1:
			m_gain->setValue(m_zone->gain());
			m_releaseGain->setValue(m_zone->releaseGain());
			m_preLoopReleaseGain->setValue(m_zone->preLoopReleaseGain());
			m_enableZoneOutput->setChecked(m_zone->isEnableZoneOutput());
			m_output->setVersion((SamVersion*)m_zone->parent()->parent()->parent());
			m_releaseOutput->setVersion((SamVersion*)m_zone->parent()->parent()->parent());
			m_output->setOutput(m_zone->outputListId());
			m_releaseOutput->setOutput(m_zone->releaseOutputListId());
			if(m_zone->isEnableZoneOutput()) {
				m_output->setEnabled(true);
				m_releaseOutput->setEnabled(true);
			} else {
				m_output->setEnabled(false);
				m_releaseOutput->setEnabled(false);
			}
			break;
		case 2:
			m_pitchLo->setValue(m_zone->midiNoteLow());
			m_pitchHi->setValue(m_zone->midiNoteHigh());
			m_trackPitch->setChecked(m_zone->isTrackPitch());
			m_fineTune->setValue(m_zone->fineTune());
			break;
		case 3:
			m_velocityLo->setValue(m_zone->midiVelocityLow());
			m_velocityHi->setValue(m_zone->midiVelocityHigh());
			m_predictableVelocity->setChecked(m_zone->isPredictableVelocity());
			m_predictableVelocityCellIndex->setValue(m_zone->predictableVelocityCellIndex());
			m_velocitySampleStart->setValue(m_zone->velocitySampleStart());
			m_velocitySampleStartThreshold->setValue(m_zone->velocitySampleStartThreshold());
			break;
		case 4:
			m_oneShot->setChecked(m_zone->isOneShot());
			m_triggerMode->setValue(m_zone->triggerMode());
			m_singleCellId->setValue(m_zone->singleCellId());
			break;
		case 5:
			m_filterEnabled->setChecked(m_zone->isFilterEnabled());
			m_filterCutOff->setValue(m_zone->cutOff());
			m_resonance->setValue(m_zone->resonance() * 100);
			m_minCutOff->setValue(m_zone->minCutOff());
			//m_maxCutOff->setValue(m_zone->maxCutOff());
			//m_cutOffController->setValue(m_zone->cutOffController());
			m_filterVelocityScaling->setChecked(m_zone->isFilterVelocityScaling());
			//aLog("m_zone->filterVelocityDepth() = %f", m_zone->filterVelocityDepth());
			m_filterVelocityDepth->setValue(m_zone->filterVelocityDepth() * 100);
			m_filterVelocityFullRange->setChecked(m_zone->isFilterVelocityFullRange());
			m_filterPitchTracking->setChecked(m_zone->isFilterPitchTracking());
			m_filterPitchTrackOffset->setValue(m_zone->filterPitchTrackOffset());
			m_filterEnvelope->setChecked(m_zone->isFilterEnvelope());
			m_filterParameter->setChecked(m_zone->isFilterParameter());
			break;
		case 6:
			m_env1Attack->setValue(m_zone->envelope(0)->attack());
			m_env1AttackVelocity->setValue(m_zone->envelope(0)->attackVelocity());
			m_env1AttackVelocityThreshold->setValue(m_zone->envelope(0)->attackVelocityThreshold());
			m_env1AttackOffset->setValue(m_zone->attackOffset());
			m_env1AttackOffsetMode->setValue(m_zone->attackOffsetMode());
			m_env1AttackCurve->setShape(m_zone->envelope(0)->attackShape());
			m_env1AttackCurve->setDepth(m_zone->envelope(0)->attackDepth());
			m_env1Hold->setValue(m_zone->envelope(0)->hold());
			m_env1Decay->setValue(m_zone->envelope(0)->decay());
			m_env1Sustain->setValue(m_zone->envelope(0)->sustain() * 100);
			m_env1Release->setValue(m_zone->envelope(0)->release());
			m_env1AutoRelease->setChecked(m_zone->envelope(0)->isAutoRelease());
			m_env1ReleaseCurve->setShape(m_zone->envelope(0)->releaseShape());
			m_env1ReleaseCurve->setDepth(m_zone->envelope(0)->releaseDepth());
			break;
		case 7:
			m_env2Attack->setValue(m_zone->envelope(1)->attack());
			m_env2AttackCurve->setShape(m_zone->envelope(1)->attackShape());
			m_env2AttackCurve->setDepth(m_zone->envelope(1)->attackDepth());
			m_env2Hold->setValue(m_zone->envelope(1)->hold());
			m_env2Decay->setValue(m_zone->envelope(1)->decay());
			m_env2Sustain->setValue(m_zone->envelope(1)->sustain() * 100);
			m_env2Release->setValue(m_zone->envelope(1)->release());
			m_env2ReleaseCurve->setShape(m_zone->envelope(1)->releaseShape());
			m_env2ReleaseCurve->setDepth(m_zone->envelope(1)->releaseDepth());
			break;
		case 8:
			m_env3Attack->setValue(m_zone->envelope(2)->attack());
			m_env3AttackCurve->setShape(m_zone->envelope(2)->attackShape());
			m_env3AttackCurve->setDepth(m_zone->envelope(2)->attackDepth());
			m_env3Hold->setValue(m_zone->envelope(2)->hold());
			m_env3Decay->setValue(m_zone->envelope(2)->decay());
			m_env3Sustain->setValue(m_zone->envelope(2)->sustain() * 100);
			m_env3Release->setValue(m_zone->envelope(2)->release());
			m_env3ReleaseCurve->setShape(m_zone->envelope(2)->releaseShape());
			m_env3ReleaseCurve->setDepth(m_zone->envelope(2)->releaseDepth());
			break;
		case 9:
			m_tempoReleaseAdjustment->setChecked(m_zone->isTempoReleaseAdjustment());
			m_tempoThreshold->setValue(m_zone->tempoThreshold());
			m_tempoReleaseDivisor->setValue(m_zone->tempoReleaseDivisor());
			break;
		case 10:
			m_inStart->setValue(m_zone->cfInStart());
			m_inEnd->setValue(m_zone->cfInEnd());
			m_outStart->setValue(m_zone->cfOutStart());
			m_outEnd->setValue(m_zone->cfOutEnd());
			m_inCurveDepth->setValue(m_zone->cfInCurveDepth() * 100.0f);
			m_outCurveDepth->setValue(m_zone->cfOutCurveDepth() * 100.0f);
			break;
	}


}

void SeGuiZoneProperties::setValue(int propertyId, float value)
{
	SamLayer *layer = (SamLayer*)m_zone->parent();
	for(int i=0; i<layer->numZones(); i++) {
		if(layer->zone(i)->isSelected()) {
			samEditor->commandManager()->execute(new SamZoneCommand(layer->zone(i), propertyId, value));
		}
	}
}

void SeGuiZoneProperties::setState(int propertyId, bool state)
{
	SamLayer *layer = (SamLayer*)m_zone->parent();
	for(int i=0; i<layer->numZones(); i++) {
		if(layer->zone(i)->isSelected()) {
			samEditor->commandManager()->execute(new SamZoneCommand(layer->zone(i), propertyId, state));
		}
	}

	if(propertyId == SamZoneCommand::OutputEnable) {
		if(state) {
			m_output->setEnabled(true);
			m_releaseOutput->setEnabled(true);
		} else {
			m_output->setEnabled(false);
			m_releaseOutput->setEnabled(false);
		}
	}
}

void SeGuiZoneProperties::setText(int propertyId, QString text)
{
	qDebug("SeGuiZoneProperties::setText(%d, %s)", propertyId, qPrintable(text));
	QByteArray ba = text.toAscii();
	SamLayer *layer = (SamLayer*)m_zone->parent();
	for(int i=0; i<layer->numZones(); i++) {
		if(layer->zone(i)->isSelected()) {
			samEditor->commandManager()->execute(new SamZoneCommand(layer->zone(i), propertyId, AString(ba.data())));
		}
	}
}

void SeGuiZoneProperties::initGeneral(void)
{
	m_name = new SeGuiItemText(this, SamZoneCommand::Name, "Name");
	//m_name->setTint(true, QColor(120,120,200));
	connect(m_name, SIGNAL(textChanged(int, QString)), this, SLOT(setText(int, QString)));
	m_list->addItem(m_name);

	m_description = new SeGuiItemText(this, SamZoneCommand::Description, "Description");
	//description->setTint(true, QColor(120,120,200));
	connect(m_description, SIGNAL(textChanged(int, QString)), this, SLOT(setText(int, QString)));
	m_list->addItem(m_description);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	//m_group = new SeGuiItemValue(this, SamZoneCommand::Group, "Group", "", 0, 127, 1, 1, 2);
	//group->setTint(true, QColor(120,120,200));
	//pitchLo->setExtraInfo(1);
	//connect(m_group, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	//m_list->addItem(m_group);

	m_dynamicRef = new SeGuiItemValue(this, SamZoneCommand::DynamicRef, "Dynamic Ref", "", 0, 10, 0, 1, 8);
	//dynamic->setTint(true, QColor(120,120,200));
	m_dynamicRef->setExtraInfo(2);
	connect(m_dynamicRef, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_dynamicRef);

	m_sampleStart = new SeGuiItemValue(this, SamZoneCommand::SampleStart, "Sample Start", "samples", 0, 44100, 0, 1, 2);
	//sampleStart->setTint(true, QColor(120,120,200));
	connect(m_sampleStart, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_sampleStart);
}

void SeGuiZoneProperties::initLevelsAndOutputs(void)
{
	m_gain = new SeGuiItemValue(this, SamZoneCommand::OutputGain, "Gain", "dB", -12, 12, 0.0, 0.25, 2);
	//m_gain->setTint(true, QColor(255,0,0));
	connect(m_gain, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_gain);

	m_releaseGain = new SeGuiItemValue(this, SamZoneCommand::OutputReleaseGain, "Release Gain", "dB", -24, 24, 0.0, 0.25, 2);
	//m_releaseGain->setTint(true, QColor(255,0,0));
	connect(m_releaseGain, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_releaseGain);

	m_preLoopReleaseGain = new SeGuiItemValue(this, SamZoneCommand::OutputPreLoopReleaseGain, "Pre-Loop Rel Gain", "dB", -12, 12, 0.0, 0.25, 2);
	//m_preLoopReleaseGain->setTint(true, QColor(255,0,0));
	connect(m_preLoopReleaseGain, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_preLoopReleaseGain);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_enableZoneOutput = new SeGuiItemBool(this, SamZoneCommand::OutputEnable, "Enable Zone Output");
	//m_enableZoneOutput->setTint(true, QColor(255,0,0));
	connect(m_enableZoneOutput, SIGNAL(changed(int, bool)), this, SLOT(setState(int, bool)));
	m_list->addItem(m_enableZoneOutput);

	m_output = new SeGuiItemSelectOutput(this, SamZoneCommand::OutputOutput, "Output");
	//m_output->setTint(true, QColor(255,0,0));
	connect(m_output, SIGNAL(outputChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_output);

	m_releaseOutput = new SeGuiItemSelectOutput(this, SamZoneCommand::OutputReleaseOutput, "Release Output");
	//m_releaseOutput->setTint(true, QColor(255,0,0));
	connect(m_releaseOutput, SIGNAL(outputChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_releaseOutput);
}

void SeGuiZoneProperties::initPitch(void)
{
	m_pitchLo = new SeGuiItemValue(this, SamZoneCommand::PitchLo, "Pitch Low", "", 0, 127, 1, 1, 2);
	//pitchLo->setTint(true, QColor(192,0,255));
	m_pitchLo->setExtraInfo(1);
	m_pitchLo->setLiveUpdate();
	connect(m_pitchLo, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_pitchLo);

	m_pitchHi = new SeGuiItemValue(this, SamZoneCommand::PitchHi, "Pitch High", "", 0, 127, 1, 1, 2);
	//pitchHi->setTint(true, QColor(192,0,255));
	m_pitchHi->setExtraInfo(1);
	m_pitchHi->setLiveUpdate();
	connect(m_pitchHi, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_pitchHi);

	m_trackPitch = new SeGuiItemBool(this, SamZoneCommand::PitchTrack, "Track Pitch");
	//m_trackPitch->setTint(true, QColor(192,0,255));
	connect(m_trackPitch, SIGNAL(changed(int, bool)), this, SLOT(setState(int, bool)));
	m_list->addItem(m_trackPitch);

	m_fineTune = new SeGuiItemValue(this, SamZoneCommand::PitchFineTune, "Fine Tune", "cents", -99, 99, 0, 1, 2);
	//fineTune->setTint(true, QColor(192,0,255));
	connect(m_fineTune, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_fineTune);
}

void SeGuiZoneProperties::initVelocity(void)
{
	m_velocityLo = new SeGuiItemValue(this, SamZoneCommand::VelocityLo, "Velocity Low", "", 1, 127, 1, 1, 2);
	//m_velocityLo->setTint(true, QColor(0,0,255));
	m_velocityLo->setLiveUpdate();
	connect(m_velocityLo, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_velocityLo);

	m_velocityHi = new SeGuiItemValue(this, SamZoneCommand::VelocityHi, "Velocity High", "", 1, 127, 1, 1, 2);
	//m_velocityHi->setTint(true, QColor(0,0,255));
	m_velocityHi->setLiveUpdate();
	connect(m_velocityHi, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_velocityHi);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_predictableVelocity = new SeGuiItemBool(this, SamZoneCommand::VelocityPredictable, "Predictable Velocity");
	//m_predictableVelocity->setTint(true, QColor(0,0,255));
	connect(m_predictableVelocity, SIGNAL(changed(int, bool)), this, SLOT(setState(int, bool)));
	m_list->addItem(m_predictableVelocity);

	m_predictableVelocityCellIndex = new SeGuiItemValue(this, SamZoneCommand::VelocityPredictableCellIndex, "Predictable Cell", "", 0, 127, 0, 1, 2);
	connect(m_predictableVelocityCellIndex, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_predictableVelocityCellIndex);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_velocitySampleStart = new SeGuiItemValue(this, SamZoneCommand::VelocitySampleStart, "Velocity Sample Start", "", 0, 20000, 1, 1, 2);
	//m_velocitySampleStart->setTint(true, QColor(0,0,255));
	connect(m_velocitySampleStart, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_velocitySampleStart);

	m_velocitySampleStartThreshold = new SeGuiItemValue(this, SamZoneCommand::VelocitySampleStartThreshold, "Threshold", "", 0, 127, 1, 1, 2); //Vel Sample Start 
	//m_velocitySampleStartThreshold->setTint(true, QColor(0,0,255));
	connect(m_velocitySampleStartThreshold, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_velocitySampleStartThreshold);
}

void SeGuiZoneProperties::initTriggering(void)
{
	m_oneShot = new SeGuiItemBool(this, SamZoneCommand::TriggeringOneShot, "One Shot");
	//m_oneShot->setTint(true, QColor(255,0,191));
	connect(m_oneShot, SIGNAL(changed(int, bool)), this, SLOT(setState(int, bool)));
	m_list->addItem(m_oneShot);

	m_triggerMode = new SeGuiItemValue(this, SamZoneCommand::TriggeringCellTriggerMode, "Cell Trigger Mode", "", 0, 2, 0, 1, 8);
	//triggerMode->setTint(true, QColor(255,0,191));
	m_triggerMode->setExtraInfo(3);
	connect(m_triggerMode, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_triggerMode);

	m_singleCellId = new SeGuiItemValue(this, SamZoneCommand::TriggeringSingleCellId, "Single Cell Id", "", 0, 127, 0, 1, 4);
	//singleCellId->setTint(true, QColor(255,0,191));
	connect(m_singleCellId, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_singleCellId);
}

void SeGuiZoneProperties::initFilter(void)
{
	m_filterEnabled = new SeGuiItemBool(this, SamZoneCommand::FilterEnabled, "Enable Filter");
	m_filterEnabled->setToolTip("<b>Toggles whether the filter is enabled.</b>");
	connect(m_filterEnabled, SIGNAL(changed(int, bool)), this, SLOT(setState(int, bool)));
	m_list->addItem(m_filterEnabled);

	m_filterCutOff = new SeGuiItemValue(this, SamZoneCommand::FilterCutOff, "Cut-off Frequency", "Hz", 10, 22050, 2000, 1, 2);
	m_filterCutOff->setToolTip("<b>Sets the cut-off frequency of the filter.</b><br><br>Note: This value will be ignored if Pitch Tracking is enabled.");
	connect(m_filterCutOff, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_filterCutOff);

	m_resonance = new SeGuiItemValue(this, SamZoneCommand::FilterResonance, "Resonance", "%", 0, 100, 0, 1, 2);
	m_resonance->setToolTip("<b>Sets the amount of resonance in the filter.</b>");
	connect(m_resonance, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_resonance);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_filterPitchTracking = new SeGuiItemBool(this, SamZoneCommand::FilterPitchTracking, "Pitch Tracking");
	m_filterPitchTracking->setToolTip("<b>Toggles whether the cut-off frequency is determined by the pitch of played notes.</b>");
	connect(m_filterPitchTracking, SIGNAL(changed(int, bool)), this, SLOT(setState(int, bool)));
	m_list->addItem(m_filterPitchTracking);

	m_filterPitchTrackOffset = new SeGuiItemValue(this, SamZoneCommand::FilterPitchTrackOffset, "Pitch Track Offset", "semitones", -64, 64, 0, 1, 2);
	m_filterPitchTrackOffset->setToolTip("<b>The offset in semitones of the pitch used to track the filter.</b><br><br>For example: If this value is 0, when a pitch is played, the frequency of that pitch will be used as the cut-off value. However, if the offset is -12, the frequency of the note an octave lower than played will be used.");
	connect(m_filterPitchTrackOffset, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_filterPitchTrackOffset);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_filterVelocityScaling = new SeGuiItemBool(this, SamZoneCommand::FilterVelocityScaling, "Velocity Scaling");
	m_filterVelocityScaling->setToolTip("<b>Toggles whether the cut-off frequency is modulated by velocity.</b>");
	connect(m_filterVelocityScaling, SIGNAL(changed(int, bool)), this, SLOT(setState(int, bool)));
	m_list->addItem(m_filterVelocityScaling);

	m_filterVelocityDepth = new SeGuiItemValue(this, SamZoneCommand::FilterVelocityDepth, "Velocity Depth", "%", 0, 100, 1.0f, 1, 2);
	m_filterVelocityDepth->setToolTip("<b>Sets how much the velocity of a note influences the modulation of the cut-off frequency.</b>");
	connect(m_filterVelocityDepth, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_filterVelocityDepth);

	m_filterVelocityFullRange = new SeGuiItemBool(this, SamZoneCommand::FilterVelocityFullRange, "Full Velocity Range");
	m_filterVelocityFullRange->setToolTip("<b>If enabled, the cut-off frequency is scaled according to the full velocity range, rather than the velocity range of the given Zone.</b>");
	connect(m_filterVelocityFullRange, SIGNAL(changed(int, bool)), this, SLOT(setState(int, bool)));
	m_list->addItem(m_filterVelocityFullRange);

	//m_cutOffController = new SeGuiItemValue(this, SamZoneCommand::FilterCutOffController, "Cut-off Controller", "", 0, 1, 0, 1, 2);
	//m_cutOffController->setToolTip("<b>Specifies whether a controller is used to set the cut-off frequency of the filter.</b><br><br>This property has two values:<br>0 - No Controller<br>1 - Modulation Wheel.");
	//connect(m_cutOffController, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	//m_list->addItem(m_cutOffController);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_minCutOff = new SeGuiItemValue(this, SamZoneCommand::FilterMinCutOff, "Minimum Cut-off", "Hz", 10, 22050, 0, 1, 2);
	m_minCutOff->setToolTip("<b>Sets the minimum value the cut-off frequency can be.</b><br><br>This is useful if the filter is being modulated via velocity or a controller.");
	connect(m_minCutOff, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_minCutOff);

	/*m_maxCutOff = new SeGuiItemValue(this, SamZoneCommand::FilterMaxCutOff, "Maximum Cut-off", "Hz", 0, 22050, 22050, 1, 2);
	m_maxCutOff->setToolTip("<b>Sets the maximum value the cut-off frequency can be.</b><br><br>This is the maximum value the cut-off frequency will be when modulated by a controller.");
	connect(m_maxCutOff, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_maxCutOff);*/

	//SeGuiItemValue *m_resonance;

	m_filterEnvelope = new SeGuiItemBool(this, SamZoneCommand::FilterEnvelope, "Use Envelope");
	m_filterEnvelope->setToolTip("<b>Toggles whether the filter is modulated by the Filter Envelope (3).</b>");
	connect(m_filterEnvelope, SIGNAL(changed(int, bool)), this, SLOT(setState(int, bool)));
	m_list->addItem(m_filterEnvelope);

	m_filterParameter = new SeGuiItemBool(this, SamZoneCommand::FilterParameter, "Use Parameter Mod");
	m_filterParameter->setToolTip("<b>Toggles whether the filter is modulated by the Filter Parameter (if it's added to the Articulation).</b>");
	connect(m_filterParameter, SIGNAL(changed(int, bool)), this, SLOT(setState(int, bool)));
	m_list->addItem(m_filterParameter);

	

	//SeGuiItemCurve *m_filterVelocityCurve;
}

void SeGuiZoneProperties::initEnvelope1(void)
{
	m_env1Attack = new SeGuiItemValue(this, SamZoneCommand::Env1Attack, "Attack", "ms", 0, 10000, 0.0, 1.0, 2);
	//env1Attack->setTint(true, QColor(0,255,0));
	connect(m_env1Attack, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env1Attack);

	m_env1AttackVelocity = new SeGuiItemValue(this, SamZoneCommand::Env1AttackVelocity, "Attack Velocity", "ms", 0, 10000, 0.0, 1.0, 2);
	m_env1AttackVelocity->setToolTip("If the velocity of the note being triggered is greater than or equal to the Attack Velocity Threshold, the Attack Velocity value will be used for the Attack stage of the envelope (rather than the normal Attack value).<br><br>However, if the velocity of the note is less than the Attack Velocity Threshold--and the Attack Velocity value is less than the Attack value--the attack stage of the envelope will be scaled between the Attack and Attack Velocity values. For example, if Attack is 100, Attack Velocity is 50, and Attack Velocity Threshold is 64, triggering a note with a velocity of 32 will result in an Attack time of 75");
	//env1AttackVelocity->setTint(true, QColor(0,255,0));
	connect(m_env1AttackVelocity, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env1AttackVelocity);

	m_env1AttackVelocityThreshold = new SeGuiItemValue(this, SamZoneCommand::Env1AttackVelocityThreshold, "Attack Vel Threshold", "", 0, 127, 0.0, 1.0, 2);
	m_env1AttackVelocityThreshold->setToolTip("If the velocity of the note being triggered is greater than or equal to the Attack Velocity Threshold, the Attack Velocity value will be used for the Attack stage of the envelope (rather than the normal Attack value).<br><br>However, if the velocity of the note is less than the Attack Velocity Threshold--and the Attack Velocity value is less than the Attack value--the attack stage of the envelope will be scaled between the Attack and Attack Velocity values. For example, if Attack is 100, Attack Velocity is 50, and Attack Velocity Threshold is 64, triggering a note with a velocity of 32 will result in an Attack time of 75");
	//env1AttackVelocityThreshold->setTint(true, QColor(0,255,0));
	connect(m_env1AttackVelocityThreshold, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env1AttackVelocityThreshold);

	m_env1AttackOffset = new SeGuiItemValue(this, SamZoneCommand::Env1AttackOffset, "Attack Offset", "ms", 0, 1000, 0, 1, 2);
	m_env1AttackOffset->setToolTip("<b>Attack Offset is a way of extending the attack time of the envelope based on a Parameter</b><br><br>Note: Parameters are typically be controlled by MIDI Controllers.");
	//env1AttackOffset->setTint(true, QColor(0,255,0));
	connect(m_env1AttackOffset, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env1AttackOffset);

	m_env1AttackOffsetMode = new SeGuiItemValue(this, SamZoneCommand::Env1AttackOffsetMode, "Attack Offset Mode", "", 0, 1, 0, 1, 4);
	m_env1AttackOffsetMode->setToolTip("If Attack Offset Mode is set to 0 (Switch), the Attack Offset value will be added to the attack time of the envelope if the Parameter is at 100% (ie. the MIDI Controller is all the way up at 127).<br><br>If Attack Offset Mode is set to 1 (Scale), the proportion of the Attack Offset value that will be added to the Attack time is based on the value of the Parameter. For example, if the Parameter is set to 50% (the MIDI Controller is half way), half of the Attack Offset value will be added to the Attack.");
	//env1AttackOffsetMode->setTint(true, QColor(0,255,0));
	m_env1AttackOffsetMode->setExtraInfo(7);
	connect(m_env1AttackOffsetMode, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env1AttackOffsetMode);

	m_env1AttackCurve = new SeGuiItemCurve(this, "Attack Curve", false);
	//env1AttackCurve->setTint(true, QColor(0,255,0));
	m_env1AttackCurve->setPropertyIdShape(SamZoneCommand::Env1AttackCurveShape);
	m_env1AttackCurve->setPropertyIdDepth(SamZoneCommand::Env1AttackCurveDepth);
	connect(m_env1AttackCurve, SIGNAL(shapeChanged(int, float)), this, SLOT(setValue(int, float)));
	////connect(env1AttackCurve, SIGNAL(startChanged(float)), this, SLOT(setCrossfadeCurveStart(float)));
	////connect(env1AttackCurve, SIGNAL(endChanged(float)), this, SLOT(setCrossfadeCurveEnd(float)));
	connect(m_env1AttackCurve, SIGNAL(depthChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env1AttackCurve);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_env1Hold = new SeGuiItemValue(this, SamZoneCommand::Env1Hold, "Hold", "ms", 0, 10000, 0.0, 1.0, 2);
	//env1Hold->setTint(true, QColor(0,255,0));
	connect(m_env1Hold, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env1Hold);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_env1Decay = new SeGuiItemValue(this, SamZoneCommand::Env1Decay, "Decay", "ms", 0, 10000, 0.0, 1.0, 2);
	//env1Decay->setTint(true, QColor(0,255,0));
	connect(m_env1Decay, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env1Decay);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_env1Sustain = new SeGuiItemValue(this, SamZoneCommand::Env1Sustain, "Sustain", "%", 0, 200, 0.0, 1.0, 2);
	//env1Sustain->setTint(true, QColor(0,255,0));
	connect(m_env1Sustain, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env1Sustain);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_env1Release = new SeGuiItemValue(this, SamZoneCommand::Env1Release, "Release", "ms", 0, 10000, 0.0, 1.0, 2);
	//env1Release->setTint(true, QColor(0,255,0));
	connect(m_env1Release, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env1Release);

	m_env1AutoRelease = new SeGuiItemBool(this, SamZoneCommand::Env1AutoRelease, "Auto Release");
	//env1AutoRelease->setTint(true, QColor(0,255,0));
	connect(m_env1AutoRelease, SIGNAL(changed(int, bool)), this, SLOT(setState(int, bool)));
	m_list->addItem(m_env1AutoRelease);

	m_env1ReleaseCurve = new SeGuiItemCurve(this, "Release Curve", false);
	//env1ReleaseCurve->setTint(true, QColor(0,255,0));
	m_env1ReleaseCurve->setPropertyIdShape(SamZoneCommand::Env1ReleaseCurveShape);
	m_env1ReleaseCurve->setPropertyIdDepth(SamZoneCommand::Env1ReleaseCurveDepth);
	connect(m_env1ReleaseCurve, SIGNAL(shapeChanged(int, float)), this, SLOT(setValue(int, float)));
	////connect(env1ReleaseCurve, SIGNAL(startChanged(float)), this, SLOT(setCrossfadeCurveStart(float)));
	////connect(env1ReleaseCurve, SIGNAL(endChanged(float)), this, SLOT(setCrossfadeCurveEnd(float)));
	connect(m_env1ReleaseCurve, SIGNAL(depthChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env1ReleaseCurve);
}

void SeGuiZoneProperties::initEnvelope2(void)
{
	m_env2Attack = new SeGuiItemValue(this, SamZoneCommand::Env2Attack, "Attack", "ms", 0, 10000, 0.0, 1.0, 2);
	//env2Attack->setTint(true, QColor(255,255,0));
	connect(m_env2Attack, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env2Attack);

	m_env2AttackCurve = new SeGuiItemCurve(this, "Attack Curve", false);
	//env2AttackCurve->setTint(true, QColor(255,255,0));
	m_env2AttackCurve->setPropertyIdShape(SamZoneCommand::Env2AttackCurveShape);
	m_env2AttackCurve->setPropertyIdDepth(SamZoneCommand::Env2AttackCurveDepth);
	connect(m_env2AttackCurve, SIGNAL(shapeChanged(int, float)), this, SLOT(setValue(int, float)));
	//connect(env2AttackCurve, SIGNAL(startChanged(float)), this, SLOT(setCrossfadeCurveStart(float)));
	//connect(env2AttackCurve, SIGNAL(endChanged(float)), this, SLOT(setCrossfadeCurveEnd(float)));
	connect(m_env2AttackCurve, SIGNAL(depthChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env2AttackCurve);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_env2Hold = new SeGuiItemValue(this, SamZoneCommand::Env2Hold, "Hold", "ms", 0, 10000, 0.0, 1.0, 2);
	//env2Hold->setTint(true, QColor(255,255,0));
	connect(m_env2Hold, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env2Hold);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_env2Decay = new SeGuiItemValue(this, SamZoneCommand::Env2Decay, "Decay", "ms", 0, 10000, 0.0, 1.0, 2);
	//env2Decay->setTint(true, QColor(255,255,0));
	connect(m_env2Decay, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env2Decay);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_env2Sustain = new SeGuiItemValue(this, SamZoneCommand::Env2Sustain, "Sustain", "%", 0, 200, 0.0, 1.0, 2);
	//env2Sustain->setTint(true, QColor(255,255,0));
	connect(m_env2Sustain, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env2Sustain);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_env2Release = new SeGuiItemValue(this, SamZoneCommand::Env2Release, "Release", "ms", 0, 10000, 0.0, 1.0, 2);
	//env2Release->setTint(true, QColor(255,255,0));
	connect(m_env2Release, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env2Release);

	m_env2ReleaseCurve = new SeGuiItemCurve(this, "Release Curve", false);
	//env2ReleaseCurve->setTint(true, QColor(255,255,0));
	m_env2ReleaseCurve->setPropertyIdShape(SamZoneCommand::Env2ReleaseCurveShape);
	m_env2ReleaseCurve->setPropertyIdDepth(SamZoneCommand::Env2ReleaseCurveDepth);
	connect(m_env2ReleaseCurve, SIGNAL(shapeChanged(int, float)), this, SLOT(setValue(int, float)));
	//connect(env1ReleaseCurve, SIGNAL(startChanged(float)), this, SLOT(setCrossfadeCurveStart(float)));
	//connect(env1ReleaseCurve, SIGNAL(endChanged(float)), this, SLOT(setCrossfadeCurveEnd(float)));
	connect(m_env2ReleaseCurve, SIGNAL(depthChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env2ReleaseCurve);
}

void SeGuiZoneProperties::initEnvelope3(void)
{
	m_env3Attack = new SeGuiItemValue(this, SamZoneCommand::Env3Attack, "Attack", "ms", 0, 10000, 0.0, 1.0, 2);
	//env3Attack->setTint(true, QColor(255,255,0));
	connect(m_env3Attack, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env3Attack);

	m_env3AttackCurve = new SeGuiItemCurve(this, "Attack Curve", false);
	//env3AttackCurve->setTint(true, QColor(255,255,0));
	m_env3AttackCurve->setPropertyIdShape(SamZoneCommand::Env3AttackCurveShape);
	m_env3AttackCurve->setPropertyIdDepth(SamZoneCommand::Env3AttackCurveDepth);
	connect(m_env3AttackCurve, SIGNAL(shapeChanged(int, float)), this, SLOT(setValue(int, float)));
	//connect(env3AttackCurve, SIGNAL(startChanged(float)), this, SLOT(setCrossfadeCurveStart(float)));
	//connect(env3AttackCurve, SIGNAL(endChanged(float)), this, SLOT(setCrossfadeCurveEnd(float)));
	connect(m_env3AttackCurve, SIGNAL(depthChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env3AttackCurve);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_env3Hold = new SeGuiItemValue(this, SamZoneCommand::Env3Hold, "Hold", "ms", 0, 10000, 0.0, 1.0, 2);
	//env3Hold->setTint(true, QColor(255,255,0));
	connect(m_env3Hold, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env3Hold);
	
	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_env3Decay = new SeGuiItemValue(this, SamZoneCommand::Env3Decay, "Decay", "ms", 0, 10000, 0.0, 1.0, 2);
	//env3Decay->setTint(true, QColor(255,255,0));
	connect(m_env3Decay, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env3Decay);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_env3Sustain = new SeGuiItemValue(this, SamZoneCommand::Env3Sustain, "Sustain", "%", 0, 200, 0.0, 1.0, 2);
	//env3Sustain->setTint(true, QColor(255,255,0));
	connect(m_env3Sustain, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env3Sustain);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_env3Release = new SeGuiItemValue(this, SamZoneCommand::Env3Release, "Release", "ms", 0, 10000, 0.0, 1.0, 2);
	//env3Release->setTint(true, QColor(255,255,0));
	connect(m_env3Release, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env3Release);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_env3ReleaseCurve = new SeGuiItemCurve(this, "Release Curve", false);
	//env3ReleaseCurve->setTint(true, QColor(255,255,0));
	m_env3ReleaseCurve->setPropertyIdShape(SamZoneCommand::Env3ReleaseCurveShape);
	m_env3ReleaseCurve->setPropertyIdDepth(SamZoneCommand::Env3ReleaseCurveDepth);
	connect(m_env3ReleaseCurve, SIGNAL(shapeChanged(int, float)), this, SLOT(setValue(int, float)));
	//connect(env3ReleaseCurve, SIGNAL(startChanged(float)), this, SLOT(setCrossfadeCurveStart(float)));
	//connect(env3ReleaseCurve, SIGNAL(endChanged(float)), this, SLOT(setCrossfadeCurveEnd(float)));
	connect(m_env3ReleaseCurve, SIGNAL(depthChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_env3ReleaseCurve);
}

void SeGuiZoneProperties::initTempoReleaseModulation(void)
{
	m_tempoReleaseAdjustment = new SeGuiItemBool(this, SamZoneCommand::TempoAdjustsRelease, "Tempo Adjusts Rls");
	//m_tempoReleaseAdjustment->setTint(true, QColor(208,186,0));
	connect(m_tempoReleaseAdjustment, SIGNAL(changed(int, bool)), this, SLOT(setState(int, bool)));
	m_list->addItem(m_tempoReleaseAdjustment);

	m_tempoThreshold = new SeGuiItemValue(this, SamZoneCommand::TempoThreshold, "Tempo Threshold", "bpm", 1, 240, 120, 1, 2);
	//tempoThreshold->setTint(true, QColor(208,186,0));
	connect(m_tempoThreshold, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_tempoThreshold);

	m_tempoReleaseDivisor = new SeGuiItemValue(this, SamZoneCommand::TempoReleaseDivisor, "Tempo Release Divisor", "", 1, 8, 2, 2, 2);
	//tempoReleaseDivisor->setTint(true, QColor(208,186,0));
	connect(m_tempoReleaseDivisor, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_tempoReleaseDivisor);
}

void SeGuiZoneProperties::initCrossfade(void)
{
	m_inStart = new SeGuiItemValue(this, SamZoneCommand::CrossfadeInStart, "In Start", "", 0, 127, 0, 1, 2);
	//inStart->setTint(true, QColor(255,128,0));
	connect(m_inStart, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_inStart);

	m_inEnd = new SeGuiItemValue(this, SamZoneCommand::CrossfadeInEnd, "In End", "", 0, 127, 0, 1, 2);
	//inEnd->setTint(true, QColor(255,128,0));
	connect(m_inEnd, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_inEnd);

	m_outStart = new SeGuiItemValue(this, SamZoneCommand::CrossfadeOutStart, "Out Start", "", 0, 127, 0, 1, 2);
	//outStart->setTint(true, QColor(255,128,0));
	connect(m_outStart, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_outStart);

	m_outEnd = new SeGuiItemValue(this, SamZoneCommand::CrossfadeOutEnd, "Out End", "", 0, 127, 0, 1, 2);
	//outEnd->setTint(true, QColor(255,128,0));
	connect(m_outEnd, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_outEnd);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));


	m_inCurveDepth = new SeGuiItemValue(this, SamZoneCommand::CrossfadeInCurveDepth, "In Curve Depth", "%", 0, 100, 0, 1, 2);
	//inCurveDepth->setTint(true, QColor(255,128,0));
	connect(m_inCurveDepth, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_inCurveDepth);

	m_outCurveDepth = new SeGuiItemValue(this, SamZoneCommand::CrossfadeOutCurveDepth, "Out Curve Depth", "%", 0, 100, 0, 1, 2);
	//outCurveDepth->setTint(true, QColor(255,128,0));
	connect(m_outCurveDepth, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_outCurveDepth);
}

void SeGuiZoneProperties::resizeEvent(QResizeEvent *ev)
{
	m_list->setFixedSize(width(),height());
}