#ifndef __SamCell_h__
#define __SamCell_h__

#include "AModel.h"
#include "AList.h"
#include "AXmlDocument.h"
using namespace Atom;

#include "SamElement.h"
#include "SamElementPool.h"
#include "SamSlice.h"

class SamCell : public AModel
{
public:
    SamCell(AModel *parent);
	SamCell(AModel *parent, const SamCell& cell, int elementIdOffset = 0, bool parametersOnly = false);
    ~SamCell();

	void setSelected(bool selected, void *from, bool shouldUpdate);

	bool addElement(int sourceId, int releaseId);
	inline int numElements(void) { return m_elements.size(); }
	inline SamElement* element(int index) { return m_elements[index]; }
	inline SamElement* elementRelease(int index) { return m_releases[index]; }

	bool hasRelease(void);
	bool isSourceRelease(void);

	void assignSourceToRelease(void);
	void clearReleaseElements(void);

	// Acessors
	inline int lengthFrames(void) const { return m_lengthFrames; }
	inline int releaseLengthFrames(void) const { return m_releaseLengthFrames; }

	inline int noteBase(void) { return m_noteBase; }
	inline void setNoteBase(int noteBase) { m_noteBase = noteBase; }

	inline float fineTune(void) const { return m_fineTune; }
	inline void setFineTune(float fineTune) { m_fineTune = fineTune; }

	inline bool isLoop(void) { return m_loop; }
	inline void setLoop(bool loop) { m_loop = loop; }
	
	inline void setLoopStart(unsigned int loopStart) { m_loopStart = loopStart; }
	inline unsigned int loopStart(void) const { return m_loopStart; }

	inline void setLoopEnd(unsigned int loopEnd) { m_loopEnd = loopEnd; }
	inline unsigned int loopEnd(void) const { return m_loopEnd; }

	inline int loopCrossfadeLength(void) const { return m_loopCrossfadeLength; }
	void setLoopCrossfadeLength(long loopCrossfadeLength);

	inline unsigned long sourceStart(void) const { return m_sourceStart; }
	void setSourceStart(unsigned long sourceStart) { m_sourceStart = sourceStart; }

	inline unsigned long releaseStart(void) const { return m_releaseStart; }
	void setReleaseStart(unsigned long releaseStart) { m_releaseStart = releaseStart; }

	inline float sampleRate(void) const { return m_sampleRate; }

	// Slices
	int numSlices(void) { return m_slices.size(); }
	SamSlice* slice(int index) { return m_slices[index]; }

	inline float tempo(void) const { return m_tempo; }
	void setTempo(float tempo) { m_tempo = tempo; }


	// Load and save
	int readElement(AXmlDocument *document, AXmlElement *element);
	int writeElement(AXmlDocument *document, AXmlElement *element);

	enum SamZoneStateItem {
		ItemSelected = 0,
		ItemProperty = 1
	};

private:
	SamElementPool *m_elementPool;

	// Basic attributes
	int m_noteBase;
	int m_fineTune;
	bool m_loop;
	unsigned int m_loopStart;
	unsigned int m_loopEnd;
	int m_loopCrossfadeLength;
	unsigned int m_sourceStart;
	unsigned int m_releaseStart;
	float m_releaseGain;
	int m_lengthFrames;
	int m_releaseLengthFrames;

	AList<SamElement*> m_elements;
	AList<SamElement*> m_releases;
	AList<int> m_elementIds;
	AList<int> m_releaseIds;

	AList<SamSlice*> m_slices;
	float m_tempo;

	float m_sampleRate;
};

#endif // SAMCELL_H
