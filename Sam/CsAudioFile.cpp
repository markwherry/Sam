#include "CsAudioFile.h"

#include "ALog.h"

#include "IeeeExtended.h"

namespace Cs {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsAudioFile::CsAudioFile(AString audioFilename)
	: AFile(audioFilename)
	, m_loadSampleData(false)
	, m_sampleDataLoaded(false)
	, m_dataPosition(0)
	, m_dataSize(0)
	, m_data(0)
	, m_format(Unknown)
	, m_bitDepth(0)
	, m_sampleRate(0.0)
	, m_channels(0)
	, m_dataIsLittleEndian(true)
	, m_lengthFrames(0)
	, m_loopStart(0)
	, m_loopEnd(0)
	, m_midiPitchFraction(0)
	, m_midiUnityNote(0)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsAudioFile::read(bool loadSampleData)
{
	m_loadSampleData = loadSampleData;

	char chunkName[5] = { 0, 0, 0, 0, 0};
	unsigned int chunkSize = 0;

	readChunkName(chunkName);

	// RIFF
	// ----
	if(compareChunkName(chunkName, "RIFF")) {
		//aLog("RIFF");
		readU32Lsb(&chunkSize);
		//aLog("chunkSize = %d", chunkSize);

		readChunkName(chunkName);
		if(compareChunkName(chunkName, "WAVE")) {
			//aLog("WAVE");
			if(!readWaveHeader()) {
				return 0;
			}
		}

	// IFF
	// ---
	} else if(compareChunkName(chunkName, "FORM")) {
		aLog("FORM");
		readU32Msb(&chunkSize);
		aLog("chunkSize = %d", chunkSize);

		readChunkName(chunkName);
		if(compareChunkName(chunkName, "AIFF") || compareChunkName(chunkName, "AIFC")) {
			aLog("AIFF(C)");
			if(!readAiffHeader()) {
				return 0;
			}
		}

	// Unrecognised
	// ------------
	} else {
		return 0;
	}

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsAudioFile::readWaveHeader(void)
{
	char chunkName[5] = { 0, 0, 0, 0, 0};
	unsigned int chunkSize = 0;
	unsigned short usValue = 0;
	unsigned int ulValue = 0;

	while(readChunkName(chunkName)) {
		readU32Lsb(&chunkSize);
		//aLog("[%s] %d", chunkName, chunkSize);

		// Format chunk
		// ------------
		if(compareChunkName(chunkName, "fmt ")) {

			unsigned short formatTag;
			readU16Lsb(&formatTag);                               // Format Tag (2)
			readU16Lsb(&usValue); m_channels = usValue;           // Channels (2)
			readU32Lsb(&ulValue); m_sampleRate = (double)ulValue; // Sample Rate (4)
			readU32Lsb(&ulValue);                                 // Average Bytes Per Second (4)
			readU16Lsb(&usValue);                                 // Block Align (2)
			readU16Lsb(&usValue); m_bitDepth = usValue;           // Bit Depth (2)

			//debugFormat();


			long extendedChunkSize = chunkSize-16;
			if (extendedChunkSize > 0) {
				// Need to handle an extended format properly, but for now we'll just skip the extra bytes
				setPosition(fromCurrent, extendedChunkSize);
			}

		// Data chunk
		// ----------
		} else if(compareChunkName(chunkName, "data")) {
			m_dataPosition = position();
			m_dataSize = chunkSize;

			m_lengthFrames = m_dataSize/((m_bitDepth/8)*m_channels);
			
			if (m_loadSampleData) {
				loadSampleData();
			} else {
				setPosition(fromCurrent, chunkSize);
			}

		// Sample Chunk
		// ------------
		} else if(compareChunkName(chunkName, "smpl")) {
			//setPosition(fromCurrent, chunkSize);
			int l;
			int numLoops;
			read32Lsb(&l); // Manufacturer
			read32Lsb(&l); // Product
			read32Lsb(&l); // Sample Period
			read32Lsb(&m_midiUnityNote); // MIDI Unity Note
			//aLog("MIDI Unity Note = %ld", m_midiUnityNote);
			read32Lsb(&l); // MIDI Pitch Fraction
			//aLog("MIDI Pitch Fraction = %ld", l);
			read32Lsb(&l); // SMPTE Format
			read32Lsb(&l); // SMPTE Offset
			read32Lsb(&numLoops); // Number of sample loops
			//aLog("number of sample loops %ld", numLoops);
			read32Lsb(&l); // Sampler Data
			//aLog("sampler data = %ld", l);

			for(long i=0; i<numLoops; i++) {
				read32Lsb(&l); // Cue Point ID
				read32Lsb(&l); // Type
				readU32Lsb(&m_loopStart); // Start
				//aLog("Start = %ld", m_loopStart);
				readU32Lsb(&m_loopEnd); // End
				//aLog("End = %ld", m_loopEnd);
				read32Lsb(&l); // Fraction
				read32Lsb(&l); // Play count
			}

			//setPosition(fromCurrent, l);

		// Instrument Chunk
		// ----------------
		} else if(compareChunkName(chunkName, "inst")) {

			char instBytes[8];
			readBytes(8, instBytes);

		// iXML Chunk
		// ------------
		} else if(compareChunkName(chunkName, "iXML")) {
			//setFilePosition(fromCurrent, chunkSize);
			char *iXmlData = (char*)malloc(chunkSize+1);
			readBytes(chunkSize, iXmlData);
			//for(int i=0; i<chunkSize; i++) {
			//	aLog("[%d] %c", i, iXmlData[i]);
			//}
			iXmlData[chunkSize] = 0;
			aLog("[%s]", iXmlData);
			free(iXmlData);
		
		// Unrecognised chunk
		// ------------------
		} else {

			//aLog("*************** Unrecognised Chunk!");
			if(chunkSize%2 != 0) { //aLog("Adding pad byte");
				chunkSize++;
			}
			
			setPosition(fromCurrent, chunkSize);
		}
	}
	
	m_format = Wave;

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsAudioFile::readAiffHeader(void)
{
	char chunkName[5] = { 0, 0, 0, 0, 0};
	unsigned int chunkSize = 0;
	unsigned short usValue = 0;
	unsigned int ulValue = 0;

	while(readChunkName(chunkName)) {
		readU32Msb(&chunkSize);
		aLog("[%s] %d", chunkName, chunkSize);
		
		// COMM
		// ----
		if(compareChunkName(chunkName, "COMM")) {
			readU16Msb(&usValue); m_channels = usValue;     // Channels (2)
			readU32Msb(&ulValue); m_lengthFrames = ulValue; // Frames (4)
			readU16Msb(&usValue); m_bitDepth = usValue;     // Bit Depth (2)
			unsigned char bytes[10];
			readBytes(10, bytes);
			m_sampleRate = ConvertFromIeeeExtended(bytes);

			//debugFormat();

			// The chunkSize will be > 16 if the format is AIFC (rather than AIFF)
			if(chunkSize > 18) {
				char compressionType[5] = { 0, 0, 0, 0, 0 };
				readChunkName(compressionType);
				aLog("compressionType = %s", compressionType);

				if(compareChunkName(compressionType, "sowt")) {
					// Data is uncompressed, but Little Endian
				} else if(compareChunkName(compressionType, "NONE")) {
					m_dataIsLittleEndian = false;
				} else {
					// A compression type is used, which is currently unsupported.
					return 0;
				}

				// Skip the pstring describing the compression type
				setPosition(fromCurrent, chunkSize - 22);
			}

		// SSND
		// ----
		} else if(compareChunkName(chunkName, "SSND")) {
			m_dataSize = m_lengthFrames * (m_bitDepth/8) * m_channels;
			int commentSize = (chunkSize - m_dataSize) - 8;
			//aLog("m_dataSize = %d", m_dataSize);
			//aLog("chunkSize = %d (diff = %d)", chunkSize, chunkSize - m_dataSize);

			readU32Msb(&ulValue); // Offset
			readU32Msb(&ulValue); // Block size

			setPosition(fromCurrent, commentSize); // Skip the comment

			m_dataPosition = position();

			if (m_loadSampleData) {
				loadSampleData();
			} else {
				setPosition(fromCurrent, chunkSize);
			}

		// Unrecognised chunk
		// ------------------
		} else {
			setPosition(fromCurrent, chunkSize);
		}

	}

	m_format = Aiff;

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsAudioFile::writeHeader(void)
{
	//CSDEBUG("CsAudioFile::writeHeader\n");
	writeChunkName("RIFF");
	
	switch(m_format) {
		case Wave:
			return writeWaveHeader();
			break;
		default:
			return 0; // No file format set (or file format not implemented!)
			break;
	}
	
	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsAudioFile::writeWaveHeader(void)
{
	// Write the total size of the RIFF file-4
	writeU32Lsb(m_dataSize + 4 + 24 + 8 + 64); // 64 = smpl, 8 = data chunk name and size, 24 = format chunk, 4 = WAVE
	
	writeChunkName("WAVE");
	
	// Write the format chunk
	// ----------------------
	writeChunkName("fmt ");
	writeU32Lsb(16);
	writeU16Lsb(1); // Format Tag (2)
	writeU16Lsb(m_channels); // Channels (2)
	long tempSampleRate = (long)m_sampleRate;
	writeU32Lsb(tempSampleRate); // Sample Rate (4)
	writeU32Lsb(tempSampleRate * (m_bitDepth/8) * m_channels); // Average Bytes Per Second (4)
	writeU16Lsb((m_bitDepth/8) * m_channels); // Block Align (2)
	writeU16Lsb(m_bitDepth); // Bit Depth (2)

	// Write the sampler chunk
	// -----------------------
	writeChunkName("smpl"); // (4)
	writeU32Lsb(36 + 24); // (4)

	write32Lsb(0); // Manufacturer (4)
	write32Lsb(0); // Product (4)
	write32Lsb(22675); // Sample Period (4)
	write32Lsb(m_midiUnityNote); // MIDI Unity Note (4)
	write32Lsb(m_midiPitchFraction); // MIDI Pitch Fraction (4)
	write32Lsb(0); // SMPTE Format (4)
	write32Lsb(0); // SMPTE Offset (4)
	write32Lsb(1); // Number of sample loops (4)
	write32Lsb(0); // Sampler Data (4)

	// Loop (24)
	write32Lsb(0); // Cue Point ID (4)
	write32Lsb(0); // Type (4)
	write32Lsb((int)m_loopStart); // Start (4)
	write32Lsb((int)m_loopEnd); // End (4)
	write32Lsb(0); // Fraction (4)
	write32Lsb(0); // Play count (4)
	
	
	// Write the start of the data chunk
	// ---------------------------------
	writeChunkName("data");
	writeU32Lsb(m_dataSize);
	
	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsAudioFile::writeAudioBuffer(CsAudioBuffer *audioBuffer)
{
	//const double fScaler16 = (double)0x7fffL;
	const double fScaler24 = (double)0x7fffffL;
	//const double fScaler32 = (double)0x7fffffffL;

	//double scaler = (double)((double)0x7fffL) + .49999; // (long)((double)(*buffer++) * sc)
	double scaler = fScaler24 + 0.49999;

	for(int i=0; i<audioBuffer->numFrames(); i++) {
		for(int j=0; j<audioBuffer->numStreams(); j++) {
			float *data = audioBuffer->stream(j);
			//write16Lsb((short)(data[i]*scaler));
			if(data[i] > 1.0f) data[i] = 1.0f;
			if(data[i] < -1.0f) data[i] = -1.0f;
			write24Lsb((int)((double)data[i]*scaler));
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*int AudioFile::setFilePositionBySample (unsigned long samplePosition)
{
	unsigned long offset = samplePosition*(m_resolution/8);
	//aLog("offset=%ld\n", offset);
	setFilePositionAbsolute (offset+startOfData);
	sampleBytePointer = samplePosition;
	
	return 1;
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsAudioFile::setLengthFrames(int frames)
{
	m_lengthFrames = frames;

	m_dataSize = m_lengthFrames * (m_bitDepth/8)* m_channels;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsAudioFile::bytePositionFromFrames(int frames)
{
	int frameSize = (m_bitDepth/8) * m_channels;
	return frameSize * frames;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsAudioFile::readData(int fromFrame, int numFrames, char *buffer)
{
	//aLog("CsAudioFile::readData(%d, %d)", fromFrame, numFrames);
	// Figure out the byte start position
	int byteIndex = bytePositionFromFrames(fromFrame);
	if(byteIndex > m_dataSize) {
		//aLog("byteIndex > m_dataSize");
		return 0;
	}

	// Figure out the length in bytes to read
	int bytesToRead = bytePositionFromFrames(numFrames);
	//aLog("byteIndex = %d, bytesToRead = %d, m_dataSize = %d", byteIndex, bytesToRead, m_dataSize);
	if(byteIndex+bytesToRead > m_dataSize) {
		//bytesToRead = (byteIndex+bytesToRead) - m_dataSize; //Idiot, this is inverted!
		bytesToRead = m_dataSize - byteIndex;
		//aLog(" bytesToRead = %d", bytesToRead);
		if(bytesToRead < 1) {
			return 0;
		}
	}

	if(m_sampleDataLoaded) {
		memcpy(buffer, m_data+byteIndex, (size_t)bytesToRead);
	} else {
		setPosition(fromBeginning, m_dataPosition+byteIndex);
		//aLog("readBytes = %d (from = %d)", readBytes(bytesToRead, buffer), bytesToRead);
		readBytes(bytesToRead, buffer);
	}

	return bytesToRead;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsAudioFile::readSamplesAsFloat(int fromFrame, int numFrames, float **buffer)
{
	float scaler = 1.0f;
	if(m_bitDepth == 16) {
		scaler = 1.0f/32768.0f;
	} else if(m_bitDepth == 24) {
		scaler = 1.0f/0x7fffff;
	}

	if(m_sampleDataLoaded) {
		int byteIndex = bytePositionFromFrames(fromFrame);
		if(byteIndex > m_dataSize) {
			return 0;
		}
		for(int i=0; i<numFrames; i++) {
			for(int j=0; j<m_channels; j++) {
				char bytes[8];
				int bytesPerSample = m_bitDepth/8;
				for(int k=0; k<bytesPerSample; k++) {
					bytes[k] = (unsigned char)m_data[byteIndex];
					byteIndex++;
				}

				int value = ((int)bytes[bytesPerSample-1])<<((bytesPerSample-1)*8);
				for (int k=bytesPerSample-2; k>-1; k--) {
					value |= ((unsigned int)(unsigned char)bytes[k])<<(k*8);
				}
				buffer[j][i] = (float)value * scaler;
			}
		}
	}

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsAudioFile::convertDataToFloat(char *data, int bitDepth, int numFrames, int numChannels, float **buffer)
{
	float scaler = 1.0f;
	if(bitDepth == 16) {
		scaler = 1.0f/32768.0f;
	} else if(bitDepth == 24) {
		scaler = 1.0f/0x7fffff;
	}

	int byteIndex = 0;

	for(int i=0; i<numFrames; i++) {
		for(int j=0; j<numChannels; j++) {
			char bytes[8];
			int bytesPerSample = bitDepth/8;
			for(int k=0; k<bytesPerSample; k++) {
				bytes[k] = (unsigned char)data[byteIndex];
				byteIndex++;
			}

			int value = ((int)bytes[bytesPerSample-1])<<((bytesPerSample-1)*8);
			for (int k=bytesPerSample-2; k>-1; k--) {
				value |= ((unsigned int)(unsigned char)bytes[k])<<(k*8);
			}
			buffer[j][i] = (float)value * scaler;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsAudioFile::loadSampleData(void)
{
	if(m_sampleDataLoaded) {
		return 0;
	}

	//if(position() != m_dataPosition) {
	//	aLog("filePosition() != m_dataPosition, setting position"); 
		setPosition(fromBeginning, m_dataPosition);
	//}
	
	m_data = (char*)malloc(m_dataSize);
	
	int br = readBytes(m_dataSize, m_data);
	if(br) {
		m_sampleDataLoaded = true;
	} else {
		free(m_data);
		//return 0;
	}

	return br;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsAudioFile::debugFormat(void)
{
	aLog("channels = %d", m_channels);
	aLog("sampleRate = %f", m_sampleRate);
	aLog("bitDepth = %d", m_bitDepth);
}

}