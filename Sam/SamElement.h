#ifndef __SamElement_h__
#define __SamElement_h__

#include "AModel.h"
#include "ATypes.h"
#include "AString.h"
#include "AList.h"
#include "AFile.h"
using namespace Atom;

class SamOscillator;

struct SamElementBlock
{
	static const int size = 2048;

	char *data;
	char compressionMode;
	enum {
		Uncompressed16Bit = 0,
		Uncompressed24Bit = 1,
		Compressed16Bit   = 2,
		Compressed24Bit   = 3
	};
	int previousSample;
	int previousDiff;
};

class SamElement : public AModel
{
	friend SamOscillator;

public:
    SamElement(AModel *parent, AString path, AString filename);
    ~SamElement();

	unsigned int id(void) { return m_id; }
	void setId(unsigned int id) { m_id = id; }

	//void create(int streams, int bitDepth, int lengthFrames);

	void loadAudioData(void);
	void unloadAudioData(void);
	inline bool isAudioDataLoaded(void) const { return m_audioDataLoaded; }

	bool importAudioFile(AString path, AString filename);
	//void exportAudioFile(Core::String path, Core::String audioFile);
	bool importElementFromChunk(AFile *file);
	bool exportElementFromChunk(AFile *file);

	void decompressBlock(SamElementBlock *block, float *buffer);
	static int calculateBlockDataSize(int bitDepth, char compressionMode);

	inline int lengthFrames(void) const { return m_lengthFrames; }
	inline int numStreams(void) { return m_streams; }
	inline int bitDepth(void) const { return m_bitDepth; }
	inline float sampleRate(void) const { return m_sampleRate; }

	inline int loopStart(void) const { return m_loopStart; }
	inline int loopEnd(void) const { return m_loopEnd; }
	inline int fineTune(void) const { return m_fineTune; }

	AString filename(void) { return m_filename; }
	AString path(void) { return m_path; }

	inline int set(void) { return m_set; }
	inline void setSet(int set) { m_set = set; }

	int numBlocks(void) { return m_numBlocks; }
	//SamElementBlock** blocks(void) { return m_blocks; }
	SamElementBlock* block(int stream, int index);

	unsigned long long size(void) { return m_size; }

	

private:
	AUInt32 m_id;
	AString m_path;
	AString m_filename;
	int m_streams;
	int m_set;
	int m_lengthFrames;
	int m_bitDepth;
	float m_sampleRate;
	int m_loopStart;
	int m_loopEnd;
	int m_fineTune;
	unsigned long long m_size;

	int m_numBlocks;
	SamElementBlock **m_blocks;

	bool m_audioDataLoaded;
};

#endif // __SamElement_h__
