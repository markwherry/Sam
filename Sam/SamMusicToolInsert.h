#ifndef __SamMusicToolInsert_h__
#define __SamMusicToolInsert_h__

#include "SamToolInsert.h"

#include "SamMusicTool.h"

class SamMusicToolInsert : public SamToolInsert, public SamToolMaster
{
public:
	SamMusicToolInsert(AModel *parent, int index);
	~SamMusicToolInsert();

	void parameterChanged(int id, float value, void *from);
	void parameterAssigned(int id);
	bool isPlaying(void);
	void queueEvent(SamEvent ev);

	int numVoices(void);
	SamVoice* voice(int index);

	int numArticulations(void);
	AString articulationName(int index);

	SamMusicTool* musicTool(void) { return m_musicTool; }
	void setTool(SamTool *tool, void* from = 0);

	void setActive(bool active, void* from = 0);

	//void processEvent(SamEvent *ev);
	void processRange(CsPpqnTime from, CsPpqnTime to);

	enum SamZoneStateItem {
		ItemActive = 0
	};

	int readElement(AXmlDocument *document, AXmlElement *element);
	int writeElement(AXmlDocument *document, AXmlElement *element);

protected:
private:
	int m_index;
	SamMusicTool* m_musicTool;
	//bool m_active;
	
};

#endif