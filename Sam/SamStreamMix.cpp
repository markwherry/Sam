#include "SamStreamMix.h"

#include "ALog.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamStreamMix::SamStreamMix(AModel *parent)
	: AModel(parent)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamStreamMix::SamStreamMix(AModel *parent, const SamStreamMix& streamMix)
	: AModel(parent)
{
	m_name = streamMix.m_name;
	m_description = streamMix.m_description;
	m_mixer = streamMix.m_mixer;
	m_speakerConfig = streamMix.m_speakerConfig;

	for(int i=0; i<streamMix.m_streams.size(); i++) {
		SamStream *stream = new SamStream(this, *streamMix.m_streams[i]);
		m_streams.append(stream);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamStreamMix::~SamStreamMix()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamStreamMix::createMonoStreamMix(int streams) 
{
	m_name = "Multi-mono";
	m_description = "Simple Multi-mono Stream Mix";
	m_mixer = "Sam";
	if(streams==2)
		m_speakerConfig = CsSpeakerConfig(CsSpeakerConfig::Stereo);
	else
		m_speakerConfig  = CsSpeakerConfig(CsSpeakerConfig::Surround40);

	for(int i=0; i<streams; i++) {
		SamStream *stream = new SamStream(this);
		
		stream->setGain(0.5);
		stream->setSetId(i);
		stream->setStreamIdInSet(0);

		stream->setPanX(0.0);
		if((i+1)%2==0) 
			stream->setPanX(1.0);

		stream->setPanY(0.0);
		if(i>1)stream->setPanY(1.0);

		m_streams.append(stream);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamStreamMix::createStereoStreamMix(void)
{
	m_name = "Stereo";
	m_description = "Simple Stereo Stream Mix";
	m_mixer = "Sam";
	m_speakerConfig  = CsSpeakerConfig(CsSpeakerConfig::Stereo);

	for(int i=0; i<CsSpeakerConfig::numSpeakersWithinConfig(m_speakerConfig.config()); i++) {
		SamStream *stream = new SamStream(this);

		stream->setGain(0.5);
		stream->setSetId(0);
		stream->setStreamIdInSet(i);

		if(m_speakerConfig.config()==CsSpeakerConfig::Stereo) {
			stream->setPanX((float)i);
		}

		m_streams.append(stream);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamStreamMix::createQuadStreamMix(int stereoPairs)
{
	m_name = "Quad";
	m_description = "Quad Stream Mix";
	m_mixer = "Sam";
	m_speakerConfig  = CsSpeakerConfig(CsSpeakerConfig::Surround40);

	int controller[3] = { 4, 82, 81 };

	float xsix[12] = {
		0.0, 1.0,
		0.0, 1.0,
		0.0, 1.0,
		0.0, 1.0,
		0.5, 0.5,
		0.5, 0.5 };

	float ysix[12] = {
		0.0, 0.0,
		0.0, 0.0,
		0.0, 0.0,
		1.0, 1.0,
		0.0, 0.0,
		0.0, 0.0
	};

	int c = 0;

	for(int i=0; i<stereoPairs; i++) {
		for(int j=0; j<2; j++) {
			SamStream *stream = new SamStream(this);
			stream->setGain(0.5);
			stream->setSetId(i);
			stream->setStreamIdInSet(j);
			stream->setPanX((float)j);
			m_streams.append(stream);

			if(stereoPairs==2 && i==1) {
				stream->setPanY(1.0f);
			}

			if(stereoPairs==3 && i==2) {
				stream->setPanY(1.0f);
			}

			if(stereoPairs==3) {
				stream->setMidiGainController(controller[i]);
			}

			if(stereoPairs==6) {
				stream->setPanX(xsix[c]);
				stream->setPanY(ysix[c]);
				c++;
			}

		}
	}
	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamStreamMix::createFromBounceGroups(AList<SamBounceGroup*> *bounceGroups)
{
	m_name = "Mix";
	m_description = "Stream Mix from Bounce";
	m_mixer = "Sam";
	m_speakerConfig = CsSpeakerConfig(CsSpeakerConfig::Surround40);

	int panx[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	int pany[4] = { 0.0f, 0.0f, 1.0f, 1.0f };

	for(int i=0; i<bounceGroups->size(); i++) {
		SamBounceGroup *bg = bounceGroups->at(i);
		for(int j=0; j<bg->config().numSpeakers(); j++) {
			SamStream *stream = new SamStream(this);
			stream->setGain(0.5);
			stream->setSetId(i);
			stream->setStreamIdInSet(j);

			if(bg->config().numSpeakers() == 1) {
				stream->setPanX(0.5);
			} else {
				stream->setPanX(panx[j]);
				stream->setPanY(pany[j]);
			}

			m_streams.append(stream);
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamStream* SamStreamMix::getStream(int setId, int streamId)
{
	//aLog("SamStreamMix::getStream(%d, %d)", setId, streamId);
	for(int i=0; i<m_streams.size(); i++) {
		//aLog("Stream %d/%d", i+1, m_streams.size());
		if(m_streams[i]->setId()==setId && m_streams[i]->streamIdInSet()==streamId) {
			return m_streams[i];
		}
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamStreamMix::indexFromStream(SamStream *stream)
{
	for(int i=0; i<m_streams.size(); i++) {
		//aLog("Stream %d/%d", i+1, m_streams.size());
		if(m_streams[i] == stream) {
			return i;
		}
	}

	return -1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamStreamMix::isSolo(void)
{
	int soloCount = 0;

	for(int i=0; i<m_streams.size(); i++) {
		if(m_streams[i]->solo()) {
			soloCount++;
		}
	}

	if(soloCount>0) {
		return true;
	}

	return false;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamStreamMix::setStreamMix(SamStreamMix *streamMix)
{
	m_name = streamMix->m_name;
	m_description = streamMix->m_description;
	m_mixer = streamMix->m_mixer;
	m_speakerConfig = streamMix->m_speakerConfig;

	for(int i=0; i<m_streams.size(); i++) {
		if(i >= streamMix->numStreams()) {
			break;
		}
		m_streams[i]->setStream(streamMix->stream(i));
		
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamStreamMix::readElement(AXmlDocument *document, AXmlElement *element)
{
	// Read <StreamMix> attributes
	// ---------------------------
	if(element->hasAttribute("name")) {
		m_name = element->attribute("name");
	} else {
		aLog("<StreamMix> does not have 'name' attribute.");
		return false;
	}

	m_description = element->attribute("description", "");
	m_mixer = element->attribute("mixer", "");

	if(element->hasAttribute("outputConfiguration")) {
		AString outputConfig = element->attribute("outputConfiguration");
		if(outputConfig == "mono" || outputConfig == "Mono") {
			m_speakerConfig.setConfig(CsSpeakerConfig::Mono);
		} else if(outputConfig == "stereo" || outputConfig == "Stereo") {
			m_speakerConfig.setConfig(CsSpeakerConfig::Stereo);
		} else if(outputConfig == "quad" || outputConfig == "Quad") {
			m_speakerConfig.setConfig(CsSpeakerConfig::Surround40);
		} else if(outputConfig == "5.0") {
			m_speakerConfig.setConfig(CsSpeakerConfig::Surround50);
		} else {
			aLog("outputConfiguation attribute does not have a valid value [%s].", outputConfig.toChar());
			return false;
		}
	} else {
		aLog("<StreamMix> does not have 'outputConfiguration' attribute.");
		return 0;
	}


	// Read <Stream>s
	// --------------
	for(int i=0; i<element->numChildElements(); i++) {
		AXmlElement *elementStream = element->childElement(i);
		aLog("Reading <Stream> %d", i);
		SamStream *stream = new SamStream(this);
		if(!stream->readElement(document, elementStream)) {
			aLog("Error reading <Stream> %d", i);
			delete stream;
			return 0;
		}
		m_streams.append(stream);
	}

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamStreamMix::writeElement(AXmlDocument *document, AXmlElement *element)
{
	element->setAttribute("name", m_name);
	element->setAttribute("description", m_description);

	if(m_mixer.length()>0) {
		element->setAttribute("mixer", m_mixer);
	}

	element->setAttribute("outputConfiguration", CsSpeakerConfig::speakerConfigName(m_speakerConfig.config()));

	for(int i=0; i<m_streams.size(); i++) {
		AXmlElement *elementStream = document->createElement("Stream");
		m_streams[i]->writeElement(document, elementStream);
		element->addChildElement(elementStream);
	}
	
	return 1;
}