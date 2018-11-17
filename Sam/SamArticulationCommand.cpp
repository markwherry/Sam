#include "SamArticulationCommand.h"

#include "ALog.h"

#include "SamEngine.h"
#include "SamProject.h"

SamArticulationCommand::SamArticulationCommand(SamArticulation *articulation, int propertyId, float value)
: m_articulation(articulation)
, m_propertyId(propertyId)
, m_value(value)
, m_state(false)
{
	aLog("SamArticulationCommand::SamArticulationCommand(value)");
}

SamArticulationCommand::SamArticulationCommand(SamArticulation *articulation, int propertyId, AString text)
: m_articulation(articulation)
, m_propertyId(propertyId)
, m_value(0.0f)
, m_text(text)
, m_state(false)
{
	aLog("SamArticulationCommand::SamArticulationCommand(text)");
}

SamArticulationCommand::SamArticulationCommand(SamArticulation *articulation, int propertyId, bool state)
: m_articulation(articulation)
, m_propertyId(propertyId)
, m_value(0.0f)
, m_state(state)
{
	aLog("SamArticulationCommand::SamArticulationCommand(state)");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamArticulationCommand::execute(ACommandManager *commandManager, int id)
{
	aLog("SamArticulationCommand::execute()");

	ACommand::execute(commandManager, id);

	if(!m_articulation) {
		aLog("!m_articulation");
		return;
	}

	switch(m_propertyId) {
		case Name:
			aLog("Name: %s", m_text.toChar());
			m_articulation->setName(m_text);
			break;
		case TriggerId:
			m_articulation->setId((int)m_value);
			break;
		case ZoneCrossfadeResponseTime:
			m_articulation->setZoneCrossfadeResponseTime((int)m_value);
			break;
		case ZoneFilterResponseTime:
			m_articulation->setZoneFilterResponseTime((int)m_value);
			break;
		case PolyphonicModulation:
			m_articulation->setPolyphonicModulation((int)m_state);
			break;
		case MaxAftertouch:
			m_articulation->setMaxAftertouch((int)m_value);
			break;
		//case MinimumVelocityController:
		//	m_articulation->setMinimumVelocityController((int)m_value);
		//	break;

		case CurveVelocityShape:
			m_articulation->velocityCurve()->setShape((int)m_value);
			break;
		case CurveVelocityDepth:
			m_articulation->velocityCurve()->setDepth(m_value);
			aLog("CurveVelocityDepth: %f", m_value);
			break;
		case CurveVelocityStart:
			m_articulation->velocityCurve()->setStart(m_value);
			m_articulation->velocityCurve()->print();
			break;
		case CurveVelocityEnd:
			m_articulation->velocityCurve()->setEnd(m_value);
			break;

		case CurveCrossfadeShape:
			m_articulation->curveCrossfadeGain()->setShape((int)m_value);
			break;
		case CurveCrossfadeDepth:
			m_articulation->curveCrossfadeGain()->setDepth(m_value);
			break;
		case CurveCrossfadeStart:
			m_articulation->curveCrossfadeGain()->setStart(m_value);
			break;
		case CurveCrossfadeEnd:
			m_articulation->curveCrossfadeGain()->setEnd(m_value);
			break;

		case CurveReleaseShape:
			m_articulation->curveReleaseAttenuation()->setShape((int)m_value);
			break;
		case CurveReleaseDepth:
			m_articulation->curveReleaseAttenuation()->setDepth(m_value);
			break;
		case CurveReleaseStart:
			//aLog("CurveReleaseStart:");
			m_articulation->curveReleaseAttenuation()->setStart(m_value);
			break;
		case CurveReleaseEnd:
			m_articulation->curveReleaseAttenuation()->setEnd(m_value);
			break;

		case Output:
			m_articulation->setOutput((int)m_value);
			m_articulation->calculateOutputListIds();
			break;
		case OutputRelease:
			m_articulation->setReleaseOutput((int)m_value);
			m_articulation->calculateOutputListIds();
			break;

		case LayerDefault:
			m_articulation->setDefaultLayer((int)m_value);
			break;
		case LayerSelectMode:
			m_articulation->setLayerSelectMode((int)m_value);
			for(int i=0; i<samEngine->project()->numChannels(); i++) {
				SamChannel *channel = samEngine->project()->channel(i);
				if(m_articulation == channel->version()->articulation(channel->currentArticulationIndex())) {
					channel->setLayerSelectMode((int)m_value);
				}
			}
			break;
		case LayerSwitchTime:
			m_articulation->setSwitchLayerTime((int)m_value);
			break;
		case LayerLegatoOverlap:
			m_articulation->setLegatoOverlap((int)m_value);
			break;

		case VelocitySetsCrossfade:
			m_articulation->setVelocitySetsCrossfade(m_state);
			break;
		case VelocityCrossfadeDecay:
			m_articulation->setVelocityCrossfadeDecay(m_state);
			break;
		case VelocityCrossfadeDecayTime:
			m_articulation->setVelocityCrossfadeDecayTime((int)m_value);
			break;
		case VelocityCrossfadeDecayTo:
			m_articulation->setVelocityCrossfadeDecayTo((int)m_value);
			break;

		
	}
}