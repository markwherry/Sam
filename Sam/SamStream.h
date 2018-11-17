#ifndef __SamStream_h__
#define __SamStream_h__

#include "AModel.h"
#include "AXmlDocument.h"
using namespace Atom;

class SamStream : public AModel
{
public:
    SamStream(AModel *parent);
	SamStream(AModel *parent, const SamStream& stream);
    ~SamStream();

	void setStream(SamStream *stream);

	// Accessors
	int setId(void) { return m_setId; }
	void setSetId(int setId) { m_setId = setId; }

	int streamIdInSet(void) { return m_streamIdInSet; }
	void setStreamIdInSet(int streamIdInSet) { m_streamIdInSet = streamIdInSet; }

	float gain(void) { return m_gain; }
	void setGain(float gain, void* from = 0);

	int midiGainController(void) { return m_midiGainController; }
	void setMidiGainController(int midiGainController) { m_midiGainController = midiGainController; }

	bool mute(void) { return m_mute; }
	void setMute(bool mute, void *from = 0);

	int solo(void) { return m_solo; }
	void setSolo(bool solo, void *from = 0);

	float lfeGain(void) { return m_lfeGain; }
	void setLfeGain(float lfeGain) { m_lfeGain = lfeGain; }

	float panX(void) { return m_panX; }
	void setPanX(float panX, void* from = 0);

	float panY(void) { return m_panY; }
	void setPanY(float panY, void* from = 0);

	float centerAmount(void) { return m_centerAmount; }
	void setCenterAmount(float centerAmount) { m_centerAmount = centerAmount; }

	inline int group(void) { return m_group; }
	void setGroup(int group, void* from = 0);

	// Load and save
	int readElement(AXmlDocument *document, AXmlElement *element);
	int writeElement(AXmlDocument *document, AXmlElement *element);

	enum SamStreamItem {
		ItemGain = 0,
		ItemPanX = 1,
		ItemPanY = 2,
		ItemMute = 3,
		ItemSolo = 4
	};

private:
	int m_setId;
	int m_streamIdInSet;
	float m_gain;
	int m_midiGainController;
	float m_gainLinear;
	float m_lfeGain;
	float m_panX;
	float m_panY;
	float m_centerAmount;
	bool m_mute;
	int m_solo;
    int m_group;
};

#endif // __SamStream_h__
