#include "SamMusicToolInsert.h"

#include "SamEngine.h"
#include "SamChannel.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamMusicToolInsert::SamMusicToolInsert(AModel *parent, int index)
	: SamToolInsert(parent, 1, index)
	, m_index(index)
	, m_musicTool(0)
	//, m_active(false)
	
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamMusicToolInsert::~SamMusicToolInsert()
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamMusicToolInsert::parameterChanged(int id, float value, void *from)
{
	SamChannel *channel = (SamChannel*)parent();
	if(channel) {
		SamEvent ev(SamEvent::Parameter);
		ev.subType = SamParameter::MusicTool + m_index;
		ev.timestamp = 0;
		ev.iValue = id;
		ev.fValue = value;
		channel->addEvent(ev);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamMusicToolInsert::parameterAssigned(int id)
{
	aLog("SamMusicToolInsert::parameterAssigned(%d)", id);

	updateState(SamEngine::ContextMusicTool, 0, m_musicTool, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamMusicToolInsert::isPlaying(void)
{
	return samEngine->isPlaying();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamMusicToolInsert::queueEvent(SamEvent ev)
{
	SamChannel *channel = (SamChannel*)parent();
	if(channel) {
	//	ev.timestamp = samEngine->playStartTimestamp() + (samEngine->calculateNanoPosition(ev.start)-samEngine->playStartPosition());

		//aLog("SamChannel::queueSamEvent(%d) ts=%f (%d) %f", ev.start, ev.timestamp, timeGetTime(), samEngine->currentTimestamp());

		//if(ev->type == SamEvent::seNoteOn) {
		//	m_numNotesInBuffer++;
		//}

		channel->queueEvent(ev);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamMusicToolInsert::numVoices(void)
{
	int nv = 0;

	SamChannel *channel = (SamChannel*)parent();
	if(channel) {
		nv = channel->numVoices();
	}

	return nv;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamVoice* SamMusicToolInsert::voice(int index)
{
	SamVoice *voice = 0;

	SamChannel *channel = (SamChannel*)parent();
	if(channel) {
		voice = channel->voice(index);
	}

	return voice;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamMusicToolInsert::numArticulations(void)
{
	SamChannel *channel = (SamChannel*)parent();

	return channel->version()->numArticulations();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString SamMusicToolInsert::articulationName(int index)
{
	SamChannel *channel = (SamChannel*)parent();

	if(index >= 0 && index < channel->version()->numArticulations()) {
		return channel->version()->articulation(index)->name();
	}

	return "";
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamMusicToolInsert::setTool(SamTool *tool, void* from)
{
	//aLog("SamMusicToolInsert::setMusicTool()");

	SamToolInsert::setTool(tool);

	if(m_musicTool) {
		//m_active = false;
		setActive(false, from);
		delete m_musicTool;
	}

	m_musicTool = (SamMusicTool*)tool;

	if(m_musicTool) {
		setActive(true, from);
		//m_active = true;
	} else aLog("No music tool.");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamMusicToolInsert::setActive(bool active, void* from)
{
	aLog("SamMusicToolInsert::setActive(%d)", (int)active);

	if(active && !m_musicTool) {
		m_active = false; 
	} else {
		m_active = active;
	}

	aLog("->SamMusicToolInsert::setActive(%d)", (int)active);

	updateState(SamEngine::ContextMusicTool, ItemActive, this, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SamMusicToolInsert::processEvent(SamEvent *ev)
{
	//aLog("SamMusicToolInsert::processEvent");

	if(ev->type == SamEvent::Controller) {

		if(m_activeLearn) {
			m_activeController = ev->midiValue1;
			m_activeLearn = false;
		}

		if(m_musicTool->learningParameter() > -1) {

			SamToolParameter *parameter = m_musicTool->parameterFromId(m_musicTool->learningParameter());
			if(parameter) {
				//parameter->setMidiController(ev->midiValue1);
				//m_musicTool->setLearningParameter(-1);
				m_musicTool->setParameterMidiController(m_musicTool->learningParameter(), ev->midiValue1);
				aLog("Parameter assigned %d", parameter->midiController());
			}

		} else {

			if(ev->midiValue1 == m_activeController) {
				//aLog("ev->midiValue2 = %d", ev->midiValue2);
				if(ev->midiValue2 == 0) {
					//m_bypassed = true;
					setActive(true);
				} else {
					//m_bypassed = false;
					setActive(false);
				}
			}

			for(int i=0; i<m_musicTool->numParameters(); i++) {
				//aLog("%d/%d %d == %d", i+1, m_musicTool->numParameters(), m_musicTool->parameter(i)->midiController(), ev->midiValue1 );
				if(m_musicTool->parameter(i)->midiController() == ev->midiValue1) {

					float range = m_musicTool->parameter(i)->midiMax() - m_musicTool->parameter(i)->midiMin();
					float value = m_musicTool->parameter(i)->midiMin() + ((range / 127.0f)* (float)ev->midiValue2);

					m_musicTool->setParameterValueNormalised(m_musicTool->parameter(i)->id(), value); // 0.00787401575f * (float)ev->midiValue2
					updateState(SamEngine::ContextMusicTool, 1, m_musicTool, 0, m_musicTool->parameter(i)->id());
				}
			}

			
		}

	} else if(ev->type == SamEvent::Parameter) {
		m_musicTool->setParameterValueNormalised(ev->iValue, ev->fValue);
	}

	if(m_active) {
		m_musicTool->processEvent(ev);
	}
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamMusicToolInsert::processRange(CsPpqnTime from, CsPpqnTime to)
{
	if(m_active) {
		m_musicTool->processRange(from, to);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamMusicToolInsert::readElement(AXmlDocument *document, AXmlElement *element)
{
	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamMusicToolInsert::writeElement(AXmlDocument *document, AXmlElement *element)
{
	element->setAttribute("id", index());
	if(m_musicTool) {
		element->setAttribute("name", m_musicTool->name());
		element->setAttribute("uuid", m_musicTool->uuid());
		element->setAttribute("active", (int)m_active);
		element->setAttribute("activeController", activeController());
		AXmlElement *elMusicTool = document->createElement("MusicTool");
		m_musicTool->writeState(document, elMusicTool);
		element->addChildElement(elMusicTool);
	}

	return 1;
}