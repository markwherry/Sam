#include "SamZoneCommand.h"

#include "ALog.h"

SamZoneCommand::SamZoneCommand(SamZone *zone, int propertyId, float value)
: m_zone(zone)
, m_propertyId(propertyId)
, m_value(value)
, m_state(false)
{
	aLog("SamZoneCommand::SamZoneCommand(value)");
}

SamZoneCommand::SamZoneCommand(SamZone *zone, int propertyId, AString text)
: m_zone(zone)
, m_propertyId(propertyId)
, m_value(0.0f)
, m_text(text)
, m_state(false)
{
	aLog("SamZoneCommand::SamZoneCommand(text)");
}

SamZoneCommand::SamZoneCommand(SamZone *zone, int propertyId, bool state)
: m_zone(zone)
, m_propertyId(propertyId)
, m_value(0.0f)
, m_state(state)
{
	aLog("SamZoneCommand::SamZoneCommand(state)");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamZoneCommand::execute(ACommandManager *commandManager, int id)
{
	aLog("SamZoneCommand::execute()");

	ACommand::execute(commandManager, id);

	if(!m_zone) {
		aLog("!m_zone");
		return;
	}

	switch(m_propertyId) {
		case Name:
			aLog("Name: %s", m_text.toChar());
			m_zone->setName(m_text);
			break;
		case Description:
			m_zone->setDescription(m_text);
			break;
		case Group:
			//m_zone->setGroup(m_value);
			break;
		case DynamicRef:
			m_zone->setDynamicRef(m_value);
			break;
		case SampleStart:
			m_zone->setSampleStart(m_value);
			break;

		case OutputGain:
			m_zone->setGain(m_value);
			break;
		case OutputReleaseGain:
			//aLog("OutputReleaseGain");
			m_zone->setReleaseGain(m_value);
			break;
		case OutputPreLoopReleaseGain:
			//aLog("OutputPreLoopReleaseGain");
			m_zone->setPreLoopReleaseGain(m_value);
			break;
		case OutputEnable:
			m_zone->setEnableZoneOutput(m_state);
			break;
		case OutputOutput:
			m_zone->setOutput((int)m_value);
			break;
		case OutputReleaseOutput:
			m_zone->setReleaseOutput((int)m_value);
			break;

		case PitchLo:
			m_zone->setMidiNoteLow(m_value);
			break;
		case PitchHi:
			m_zone->setMidiNoteHigh(m_value);
			break;
		case PitchTrack:
			m_zone->setTrackPitch(m_state);
			break;
		case PitchFineTune:
			m_zone->setFineTune(m_value);
			break;
		case PitchMove:
			m_zone->setMidiNoteRange(m_zone->midiNoteLow()-(int)m_value, m_zone->midiNoteHigh()-(int)m_value);
			break;

		case VelocityLo:
			m_zone->setMidiVelocityLow(m_value);
			break;
		case VelocityHi:
			m_zone->setMidiVelocityHigh(m_value);
			break;
		case VelocityPredictable:
			m_zone->setPredictableVelocity(m_state);
			break;
		case VelocityPredictableCellIndex:
			//aLog("VelocityPredictableCellIndex: %f", m_value);
			m_zone->setPredictableVelocityCellIndex((char)((int)m_value));
			break;
		case VelocitySampleStart:
			m_zone->setVelocitySampleStart(m_value);
			break;
		case VelocitySampleStartThreshold:
			m_zone->setVelocitySampleStartThreshold(m_value);
			break;
		case VelocityMove:
			m_zone->setMidiVelocityRange(m_zone->midiVelocityLow()-(int)m_value, m_zone->midiVelocityHigh()-(int)m_value);
			break;

		case TriggeringOneShot:
			m_zone->setOneShot(m_state);
			break;
		case TriggeringCellTriggerMode:
			m_zone->setTriggerMode(m_value);
			break;
		case TriggeringSingleCellId:
			m_zone->setSingleCellId(m_value);
			break;

		case FilterEnabled:
			m_zone->setFilterEnabled(m_state);
			break;
		case FilterCutOff:
			m_zone->setCutOff(m_value);
			break;
		//case FilterCutOffController:
		//	m_zone->setCutOffController(m_value);
		//	break;
		case FilterMinCutOff:
			m_zone->setMinCutOff(m_value);
			break;
		//case FilterMaxCutOff:
		//	m_zone->setMaxCutOff(m_value);
		//	break;
		case FilterResonance:
			m_zone->setResonance(m_value/100.0f);
			break;
		case FilterVelocityScaling:
			m_zone->setFilterVelocityScaling(m_state);
			break;
		case FilterVelocityDepth:
			//aLog("FilterVelocityDepth: %f", m_value/100.0f);
			m_zone->setFilterVelocityDepth(m_value/100.0f);
			break;
		case FilterVelocityFullRange:
			m_zone->setFilterVelocityFullRange(m_state);
			break;
		case FilterPitchTracking:
			m_zone->setFilterPitchTracking(m_state);
			break;
		case FilterPitchTrackOffset:
			m_zone->setFilterPitchTrackOffset(m_value);
			break;
		case FilterEnvelope:
			m_zone->setFilterEnvelope(m_state);
			break;
		case FilterParameter:
			m_zone->setFilterParameter(m_state);
			break;

		case Env1Attack:
			m_zone->envelope(0)->setAttack(m_value);
			break;
		case Env1AttackVelocity:
			m_zone->envelope(0)->setAttackVelocity(m_value);
			break;
		case Env1AttackVelocityThreshold:
			m_zone->envelope(0)->setAttackVelocityThreshold(m_value);
			break;
		case Env1AttackOffset:
			m_zone->setAttackOffset(m_value);
			break;
		case Env1AttackOffsetMode:
			m_zone->setAttackOffsetMode(m_value);
			break;
		case Env1AttackCurveShape:
			//aLog("Env1AttackCurveShape");
			m_zone->envelope(0)->setAttackShape(m_value);
			break;
		case Env1AttackCurveDepth:
			//aLog("Env1AttackCurveDepth");
			m_zone->envelope(0)->setAttackDepth(m_value);
			break;
		case Env1Hold:
			m_zone->envelope(0)->setHold(m_value);
			break;
		case Env1Decay:
			m_zone->envelope(0)->setDecay(m_value);
			break;
		case Env1Sustain:
			m_zone->envelope(0)->setSustain(m_value / 100.0f);
			break;
		case Env1Release:
			m_zone->envelope(0)->setRelease(m_value);
			break;
		case Env1AutoRelease:
			m_zone->envelope(0)->setAutoRelease(m_state);
			break;
		case Env1ReleaseCurveShape:
			m_zone->envelope(0)->setReleaseShape(m_value);
			break;
		case Env1ReleaseCurveDepth:
			m_zone->envelope(0)->setReleaseDepth(m_value);
			break;

		case Env2Attack:
			m_zone->envelope(1)->setAttack(m_value);
			break;
		case Env2AttackCurveShape:
			m_zone->envelope(1)->setAttackShape(m_value);
			break;
		case Env2AttackCurveDepth:
			m_zone->envelope(1)->setAttackDepth(m_value);
			break;
		case Env2Hold:
			m_zone->envelope(1)->setHold(m_value);
			break;
		case Env2Decay:
			m_zone->envelope(1)->setDecay(m_value);
			break;
		case Env2Sustain:
			m_zone->envelope(1)->setSustain(m_value / 100.0f);
			break;
		case Env2Release:
			m_zone->envelope(1)->setRelease(m_value);
			break;
		case Env2ReleaseCurveShape:
			m_zone->envelope(1)->setReleaseShape(m_value);
			break;
		case Env2ReleaseCurveDepth:
			m_zone->envelope(1)->setReleaseDepth(m_value);
			break;

		case Env3Attack:
			m_zone->envelope(2)->setAttack(m_value);
			break;
		case Env3AttackVelocity:
			m_zone->envelope(2)->setAttackVelocity(m_value);
			break;
		case Env3AttackVelocityThreshold:
			m_zone->envelope(2)->setAttackVelocityThreshold(m_value);
			break;
		case Env3AttackCurveShape:
			m_zone->envelope(2)->setAttackShape(m_value);
			break;
		case Env3AttackCurveDepth:
			m_zone->envelope(2)->setAttackDepth(m_value);
			break;
		case Env3Hold:
			m_zone->envelope(2)->setHold(m_value);
			break;
		case Env3Decay:
			m_zone->envelope(2)->setDecay(m_value);
			break;
		case Env3Sustain:
			m_zone->envelope(2)->setSustain(m_value / 100.0f);
			break;
		case Env3Release:
			m_zone->envelope(2)->setRelease(m_value);
			break;
		case Env3AutoRelease:
			m_zone->envelope(2)->setAutoRelease(m_state);
			break;
		case Env3ReleaseCurveShape:
			m_zone->envelope(2)->setReleaseShape(m_value);
			break;
		case Env3ReleaseCurveDepth:
			m_zone->envelope(2)->setReleaseDepth(m_value);
			break;

		case TempoAdjustsRelease:
			m_zone->setTempoReleaseAdjustment(m_state);
			break;
		case TempoThreshold:
			m_zone->setTempoThreshold(m_value);
			break;
		case TempoReleaseDivisor:
			m_zone->setTempoReleaseDivisor(m_value);
			break;

		case CrossfadeInStart:
			m_zone->setCfInStart(m_value);
			break;
		case CrossfadeInEnd:
			m_zone->setCfInEnd(m_value);
			break;
		case CrossfadeOutStart:
			m_zone->setCfOutStart(m_value);
			break;
		case CrossfadeOutEnd:
			m_zone->setCfOutEnd(m_value);
			break;
		case CrossfadeInCurveDepth:
			m_zone->setCfInCurveDepth(m_value / 100.0f);
			break;
		case CrossfadeOutCurveDepth:
			m_zone->setCfOutCurveDepth(m_value / 100.0f);
			break;
	}
}