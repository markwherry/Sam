#ifndef __SamAudioSend_h__
#define __SamAudioSend_h__

#include "AModel.h"
#include "AXmlDocument.h"
using namespace Atom;

class SamAudioBus;

class SamAudioSend : public AModel
{
public:
	SamAudioSend(AModel *parent, SamAudioBus *parentAudioBus, int id);
	~SamAudioSend();

	int id(void) { return m_id; }

	int audioBusId(void) { return m_audioBusId; }
	void setAudioBusId(int audioBusId, void *from = 0);

	bool isActive(void) { return m_active; }
	void setActive(bool active = true, void *from = 0);

	float gain(void) { return m_gain; }
	void setGain(float gain, void *from = 0);

	bool isPreFader(void) { return m_preFader; }
	void setPreFader(bool preFader, void *from = 0);

	int readElement(AXmlDocument *document, AXmlElement *element);
	int writeElement(AXmlDocument *document, AXmlElement *element);

	void debug(void);
	
protected:
private:
	SamAudioBus *m_parentAudioBus;
	int   m_id;
	//SamAudioBus *m_audioBus;
	int   m_audioBusId;
	bool  m_active;
	float m_gain;
	bool  m_preFader;
};

#endif