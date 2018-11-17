#ifndef __CsAudioFile_h__
#define __CsAudioFile_h__

#include "AFile.h"
using namespace Atom;

#include "CsAudioBuffer.h"

namespace Cs {

class CsAudioFile : public AFile
{
public:
	CsAudioFile(AString audioFilename);

	int read(bool loadSampleData);
	int readWaveHeader(void);
	int readAiffHeader(void);

	int writeHeader(void);

	void writeAudioBuffer(CsAudioBuffer *audioBuffer);

	bool isSampleDataLoaded(void) { return m_sampleDataLoaded; }
	int dataPosition(void) { return m_dataPosition; }
	int dataSize(void) { return m_dataSize; }
	char* data(void) { return m_data; }

	inline int format(void) const { return m_format; }
	void setFormat(int format) { m_format = format; }

	inline int bitDepth(void) const { return m_bitDepth; }
	void setBitDepth(int bitDepth) { m_bitDepth = bitDepth; }

	inline int channels(void) const { return m_channels; }
	void setChannels(int channels) { m_channels = channels; }

	inline double sampleRate(void) const { return m_sampleRate; }
	void setSampleRate(double sampleRate) { m_sampleRate = sampleRate; }

	inline int lengthFrames(void) const { return m_lengthFrames; }
	void setLengthFrames(int frames);

	inline unsigned int loopStart(void) const { return m_loopStart; }
	void setLoopStart(unsigned int pos) { m_loopStart = pos; }

	inline unsigned int loopEnd(void) const { return m_loopEnd; }
	void setLoopEnd(unsigned int pos) { m_loopEnd = pos; }

	inline int fineTune(void) const { return m_fineTune; }
	void setFineTune(int fineTune) { m_fineTune = fineTune; }

	void setMidiUnityNote(int note) { m_midiUnityNote = note; }

	void setMidiPitchFraction(int fraction) { m_midiPitchFraction = fraction; }

	int bytePositionFromFrames(int frames);

	int readData(int fromFrame, int numFrames, char *buffer);
	int readSamplesAsFloat(int fromFrame, int numFrames, float **buffer);

	static void convertDataToFloat(char *data, int bitDepth, int numFrames, int numChannels, float **buffer);

	int loadSampleData(void);

	void debugFormat(void);

	enum Formats {
		Unknown = 0,
		Wave    = 1,
		Aiff    = 2,
		Caf     = 3
	};

protected:
private:
	int writeWaveHeader(void);

	int m_format;
	int m_bitDepth;
	double m_sampleRate;
	int m_channels;
	bool m_loadSampleData;
	bool m_sampleDataLoaded;
	int m_dataPosition;
	int m_dataSize;
	char *m_data;
	bool m_dataIsLittleEndian;
	int m_lengthFrames;
	unsigned int m_loopStart;
	unsigned int m_loopEnd;
	int m_fineTune;
	int m_midiUnityNote;
	int m_midiPitchFraction;
};

}

#endif // __CsAudioFile_h__