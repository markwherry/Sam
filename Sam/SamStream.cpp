#include "SamStream.h"

#include "ALog.h"

#include "SamEngine.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamStream::SamStream(AModel *parent)
	: AModel(parent)
	, m_setId(0)
	, m_streamIdInSet(0)
	, m_gain(0.5f)
	, m_midiGainController(-1)
	, m_mute(false)
	, m_solo(0)
	, m_lfeGain(0)
	, m_panX(0)
	, m_panY(0)
	, m_centerAmount(0)
	, m_group(-1)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamStream::SamStream(AModel *parent, const SamStream& stream)
	: AModel(parent)
{
	m_setId = stream.m_setId;
	m_streamIdInSet = stream.m_streamIdInSet;
	m_gain = stream.m_gain;
	m_midiGainController = stream.m_midiGainController;
	m_mute = stream.m_mute;
	m_solo = stream.m_solo;
	m_lfeGain = stream.m_lfeGain;
	m_panX = stream.m_panX;
	m_panY = stream.m_panY;
	m_centerAmount = stream.m_centerAmount;
	m_group = stream.m_group;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamStream::~SamStream()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamStream::setStream(SamStream *stream)
{
	m_setId = stream->m_setId;
	m_streamIdInSet = stream->m_streamIdInSet;
	m_gain = stream->m_gain;
	m_midiGainController = stream->m_midiGainController;
	m_mute = stream->m_mute;
	m_solo = stream->m_solo;
	m_lfeGain = stream->m_lfeGain;
	m_panX = stream->m_panX;
	m_panY = stream->m_panY;
	m_centerAmount = stream->m_centerAmount;
	m_group = stream->m_group;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamStream::setGain(float gain, void* from)
{
	m_gain = gain;

	updateState(SamEngine::ContextStream, ItemGain, this, from);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamStream::setMute(bool mute, void *from)
{ 
	m_mute = mute;

	updateState(SamEngine::ContextStream, ItemMute, this, from);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamStream::setSolo(bool solo, void *from)
{
	m_solo = solo;

	updateState(SamEngine::ContextStream, ItemSolo, this, from);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamStream::setPanX(float panX, void* from)
{
	//aLog("setPanX(%f)", panX);
	
	m_panX = panX;

	updateState(SamEngine::ContextStream, ItemPanX, this, from);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamStream::setPanY(float panY, void* from)
{
	m_panY = panY;

	updateState(SamEngine::ContextStream, ItemPanY, this, from);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamStream::setGroup(int group, void* from)
{
	m_group = group;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamStream::readElement(AXmlDocument *document, AXmlElement *element)
{
	if(element->hasAttribute("setId")) {
		m_setId = element->attribute("setId").toInt();
	} else {
		aLog("<Stream> does not have 'setId' attribute.");
		return false;
	}

	if(element->hasAttribute("streamId")) {
		m_streamIdInSet = element->attribute("streamId").toInt();
	} else {
		aLog("<Stream> does not have 'streamId' attribute.");
		return false;
	}

	m_gain = element->attribute("gain", "0.5").toFloat();
	m_midiGainController = element->attribute("midiGainController", "-1").toInt();
	m_mute = (bool)element->attribute("mute", "0").toInt();
	m_solo = (bool)element->attribute("solo", "0").toInt();
	m_lfeGain = element->attribute("lfeGain", "0.0").toFloat();
	m_panX = element->attribute("panX", "0.5").toFloat();
	m_panY = element->attribute("panY", "0.0").toFloat();
	m_centerAmount = element->attribute("centerAmount", "0.0").toFloat();
	m_group = element->attribute("group", "-1").toInt();

	return true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamStream::writeElement(AXmlDocument *document, AXmlElement *element)
{
	element->setAttribute("setId", m_setId);
	element->setAttribute("streamId", m_streamIdInSet);
	element->setAttribute("gain", m_gain);
	element->setAttribute("midiGainController", m_midiGainController);
	element->setAttribute("mute", (int)m_mute);
	element->setAttribute("solo", (int)m_solo);
	element->setAttribute("lfeGain", m_lfeGain);
	element->setAttribute("panX", m_panX);
	element->setAttribute("panY", m_panY);
	element->setAttribute("centerAmount", m_centerAmount);
	element->setAttribute("group", m_group);

	return 1;
}