#include "SamAudioSend.h"

#include "SamAudioBus.h"
#include "SamEngine.h"
#include "SamProject.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamAudioSend::SamAudioSend(AModel *parent, SamAudioBus *parentAudioBus, int id)
: AModel(parent)
, m_parentAudioBus(parentAudioBus)
, m_id(id)
, m_active(false)
, m_audioBusId(0)
//, m_audioBus(0)
, m_gain(1.0f)
, m_preFader(false)
{
	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamAudioSend::~SamAudioSend()
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioSend::setAudioBusId(int audioBusId, void *from)
{
	aLog(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CsAudioSend::setAudioBusId(%d)", audioBusId);

	m_audioBusId = audioBusId;

	/*m_audioBus = samEngine->project()->audioHardwareBusFromId(m_audioBusId);

	if(!m_audioBus) {
		m_active = false;
	}*/

	if(m_audioBusId == 0) {
		m_active = false;
		updateState(SamEngine::ContextChannel, SamChannel::ItemSendActive, this, from);
	}

	updateState(SamEngine::ContextChannel, SamChannel::ItemSendAudioBus, this, from);
}

void SamAudioSend::setActive(bool active, void *from)
{
	aLog(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> SamAudioSend::setActive(%d)", (int)m_active);

	m_active = active;
	updateState(SamEngine::ContextChannel, SamChannel::ItemSendActive, this, from);

	/*if(!m_audioBus) {
		m_active = false;
	}*/

	if(m_audioBusId == 0) {
		m_active = false;
		updateState(SamEngine::ContextChannel, SamChannel::ItemSendActive, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioSend::setGain(float gain, void *from)
{
	m_gain = gain;

	updateState(SamEngine::ContextChannel, SamChannel::ItemSendGain, this, from);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioSend::setPreFader(bool preFader, void *from)
{
	m_preFader = preFader;

	updateState(SamEngine::ContextChannel, SamChannel::ItemSendPreFader, this, from);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamAudioSend::readElement(AXmlDocument *document, AXmlElement *element)
{
	aLog("+++++ SamAudioSend::readElement() +++++");
	// ignore id -- should be in order
	m_active = (bool)element->attribute("active").toInt();
	aLog("m_active = %d", (int)m_active);

	m_audioBusId = element->attribute("audioBusId").toInt();
	/*m_audioBus = samEngine->project()->audioHardwareBusFromId(m_audioBusId);
	aLog("m_audioBus = %d (m_audioBusId = %d)", m_audioBus, m_audioBusId);
	if(!m_audioBus) { aLog("FALSE");
		m_active = false;
	}*/
	if(m_audioBusId == 0) {
		m_active = false;
	}
	m_gain = element->attribute("gain").toFloat();
	m_preFader = (bool)element->attribute("preFader").toInt();

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamAudioSend::writeElement(AXmlDocument *document, AXmlElement *element)
{
	element->setAttribute("id", m_id);
	element->setAttribute("active", (int)m_active);
	element->setAttribute("audioBusId", m_audioBusId);
	element->setAttribute("gain", m_gain);
	element->setAttribute("preFader", (int)m_preFader);

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioSend::debug(void)
{
	aLog("id         = %d", m_id);
	aLog("active     = %d", (int)m_active);
	aLog("audioBusId = %d", m_audioBusId);
	aLog("gain       = %f", m_gain);
	aLog("preFader   = %d", (int)m_preFader);
}