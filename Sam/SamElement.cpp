#include "SamElement.h"

#include "ALog.h"
#include "AMemoryManager.h"
#include "APrecisionTimer.h"
#include "ATypes.h"
using namespace Atom;

#include "CsAudioFile.h"
using namespace Cs;

#include <ipps.h>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamElement::SamElement(AModel *parent, AString path, AString filename)
	: AModel(parent)
	, m_path(path)
	, m_filename(filename)
	, m_id(0)
	, m_streams(0)
	, m_set(0)
	, m_lengthFrames(0)
	, m_bitDepth(0)
	, m_loopStart(0)
	, m_loopEnd(0)
	, m_fineTune(0)
	, m_size(0)
	, m_audioDataLoaded(false)
{

	/*for(int i=0; i<8; i++) {
		audioData[i] = 0;
		fAudioData[i] = 0;
	}*/

	CsAudioFile audioFile(m_path + m_filename);
	
	if(!audioFile.open()) {
		aLog("File could not be opened.");
		return;
	}
	//csDebug("File open.");

	if(!audioFile.read(false)) {
		aLog("File could not be read.");
		return;
	}
	
	m_bitDepth = audioFile.bitDepth();
	//m_sampleRate = (unsigned long)audioFile.sampleRate(); // What on earth was this for? (Duplicated on the line after next)
	m_streams = audioFile.channels();
	m_sampleRate = audioFile.sampleRate();
	m_loopStart = audioFile.loopStart();
	m_loopEnd = audioFile.loopEnd();
	m_fineTune = audioFile.fineTune();

	m_lengthFrames = audioFile.lengthFrames();

	audioFile.close();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamElement::~SamElement()
{
	//qDebug("SamElement::~SamElement()");

	/*for(int i=0; i<m_streams; i++) {
		if(audioData[i]) {
			VirtualFree(audioData[i], sizeof(Core::Int16)*m_lengthFrames, MEM_DECOMMIT);
		}
		if(audioData[i]) {
			VirtualFree(fAudioData[i], sizeof(float)*m_lengthFrames, MEM_DECOMMIT);
		}
	}*/

	/*if(m_lengthFrames > 0 && m_streams > 0) {
		for(int i=0; i<m_streams; i++) {
			for(int j=0; j<m_numBlocks; j++) {
				aFree(m_blocks[i][j].data, calculateBlockDataSize(16, m_blocks[i][j].compressionMode));
			}
			aFree(m_blocks[i], sizeof(SamElementBlock)*m_numBlocks);
		}

		aFree(m_blocks, sizeof(SamElementBlock*)*m_streams);
	}*/

	unloadAudioData();

	//qDebug("EXIT: SamElement::~SamElement()");
}
/*
void SamElement::create(int streams, int bitDepth, int lengthFrames)
{
	m_streams = streams;
	m_bitDepth = bitDepth;
	m_lengthFrames = lengthFrames;
	allocateAudioData(m_streams, m_bitDepth, m_lengthFrames, true);
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamElement::loadAudioData(void)
{
	aLog("$$$$ SamElement::loadAudioData() %s", m_filename.toChar());
	if(m_audioDataLoaded == true) { aLog("Already loaded."); return; }

	CsAudioFile audioFile(m_path + m_filename);
	
	if(!audioFile.open()) {
		aLog("File could not be opened.");
		return;
	}
	//csDebug("File open.");

	if(!audioFile.read(false)) {
		aLog("File could not be read.");
		return;
	}
	//csDebug("File read.");

	/*if(m_audioFile->loadSampleData()) {
		qDebug("Sample data loaded");
	} else {
		qDebug("Sample data could not be loaded.");
	}//*/

	
	m_bitDepth = audioFile.bitDepth();
	//m_sampleRate = (unsigned long)audioFile.sampleRate(); // What on earth was this for? (Duplicated on the line after next)
	m_streams = audioFile.channels();
	m_sampleRate = audioFile.sampleRate();
	m_loopStart = audioFile.loopStart();
	m_loopEnd = audioFile.loopEnd();
	m_fineTune = audioFile.fineTune();

	const int bytesPerSample = m_bitDepth/8;
	const int maxBlockDataSize = bytesPerSample * m_streams * SamElementBlock::size;

	char *readBuffer = (char*)malloc(maxBlockDataSize);

	m_lengthFrames = audioFile.lengthFrames();
	m_numBlocks = (m_lengthFrames/SamElementBlock::size)+1;

	//if(m_numBlocks > 16) m_numBlocks = 16;

	// Allocate the blocks and temp buffers
	m_blocks = (SamElementBlock**)aMalloc(sizeof(SamElementBlock*)*m_streams);

	char **compressedBuffer = (char**)malloc(sizeof(char*)*m_streams);
	char **uncompressedBuffer = (char**)malloc(sizeof(char*)*m_streams);
	for(int i=0; i<m_streams; i++) {
		m_blocks[i] = (SamElementBlock*)aMalloc(sizeof(SamElementBlock)*m_numBlocks);
		compressedBuffer[i] = (char*)malloc(SamElementBlock::size);
		uncompressedBuffer[i] = (char*)malloc(bytesPerSample*SamElementBlock::size);
	}


	// Working variables
	int diff[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int lastDiff[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int previous[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int previousDiff[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	int bufferCompressCount[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	int sampleCounter = 0;

	for(int i=0; i<m_numBlocks; i++) {

		for(int k=0; k<m_streams; k++) {
			memset(compressedBuffer[k], 0, SamElementBlock::size);
			memset(uncompressedBuffer[k], 0, bytesPerSample*SamElementBlock::size);
		}
		
		//aLog("Block %d/%d", i+1, m_numBlocks);
		int cbi[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		int bi[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

		memset(readBuffer, 0, maxBlockDataSize);
		int bytesRead = audioFile.readData(i * SamElementBlock::size, SamElementBlock::size, readBuffer);
		//aLog("bytesRead = %d", bytesRead);
		int byteIndex = 0;
		bool overflow[8] = { false, false, false, false, false, false, false, false };

		for(int j=0; j<m_streams; j++) {
			m_blocks[j][i].previousSample = previous[j];
			m_blocks[j][i].previousDiff = previousDiff[j];
		}

		//qDebug("A");
		

		for(int j=0; j<SamElementBlock::size; j++) {

			//qDebug("blockSample[%d]",j );

			for(int k=0; k<m_streams; k++) {

				char bytes[8];
				for(int l=0; l<bytesPerSample; l++) {
					bytes[l] = (unsigned char)readBuffer[byteIndex];
					uncompressedBuffer[k][bi[k]] = readBuffer[byteIndex];
					byteIndex++;
					bi[k]++;
				}

				int sample = ((int)bytes[bytesPerSample-1])<<((bytesPerSample-1)*8);
				for (int l=bytesPerSample-2; l>-1; l--) {
					sample |= ((unsigned int)(unsigned char)bytes[l])<<(l*8);
				}
				//sample /= 8;

				/*if(i==21 && k==0) {
					aLog("sample[%d] = %d", j, sample);
				}*/

				if(bytesPerSample == 2) {
					//aLog("Using compression...");
					lastDiff[k] = previous[k] - sample;
					diff[k] = previousDiff[k] - lastDiff[k];

					compressedBuffer[k][cbi[k]] = (char)diff[k];
					cbi[k]++;

					if(diff[k]>127 || diff[k] <-127) {
						overflow[k] = true;
					}

					previous[k] = sample;
					previousDiff[k] = lastDiff[k];
				}

			}

			//sampleCounter++; //aLog("sc= %d", sampleCounter);
			//if(sampleCounter == m_lengthFrames) {
			//	//aLog("BREAK sampleCounter == ", m_lengthFrames);
			//	break;
			//}

		}

		//qDebug("B");
		
		for(int j=0; j<m_streams; j++) {
			//qDebug("Finishing channel %d", j);
			if(!overflow[j] && bytesPerSample <= 2){  //aLog("G");
				//aLog("COMPRESS");
				bufferCompressCount[j]++;  //qDebug("H");
				m_blocks[j][i].compressionMode = SamElementBlock::Compressed16Bit;  //qDebug("I");
				m_blocks[j][i].data = (char*)aMalloc(SamElementBlock::size);  //qDebug("J");
				memcpy(m_blocks[j][i].data, compressedBuffer[j], SamElementBlock::size);  //qDebug("K");
				m_size += SamElementBlock::size;
			} else { //qDebug("C");
				if(bytesPerSample == 3) {
					m_blocks[j][i].compressionMode = SamElementBlock::Uncompressed24Bit; //qDebug("D");
				} else {
					m_blocks[j][i].compressionMode = SamElementBlock::Uncompressed16Bit;
				}
				m_blocks[j][i].data = (char*)aMalloc(bytesPerSample*SamElementBlock::size); //qDebug("E");
				memcpy(m_blocks[j][i].data, uncompressedBuffer[j], bytesPerSample*SamElementBlock::size); //qDebug("F");
				m_size += (bytesPerSample*SamElementBlock::size);
			}
		}
		

	}

	/*aLog("======================================================");
	for(int i=0; i<m_streams; i++) {
		aLog("%d out of %d blocks can be fully compressed", bufferCompressCount[i], m_numBlocks);
	}
	aLog("======================================================"); //*/

	audioFile.close();

	free(readBuffer);

	
	for(int i=0; i<m_streams; i++) {
		free(compressedBuffer[i]);
		free(uncompressedBuffer[i]);
	}

	free(compressedBuffer);
	free(uncompressedBuffer);

	m_audioDataLoaded = true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamElement::unloadAudioData(void)
{
	if(m_audioDataLoaded == false) { aLog("Already unloaded."); return; }

	if(m_lengthFrames > 0 && m_streams > 0) {
		for(int i=0; i<m_streams; i++) {
			for(int j=0; j<m_numBlocks; j++) {
				aFree(m_blocks[i][j].data, calculateBlockDataSize(16, m_blocks[i][j].compressionMode));
				m_blocks[i][j].data = 0;
			}
			aFree(m_blocks[i], sizeof(SamElementBlock)*m_numBlocks);
			m_blocks[i] = 0;
		}

		aFree(m_blocks, sizeof(SamElementBlock*)*m_streams);
		m_blocks = 0;

		m_lengthFrames = 0;
		m_streams = 0;
	}

	m_audioDataLoaded = false;
	m_size = 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamElement::importAudioFile(AString path, AString filename)
{
	//aLog("SamElement::importAudioFile(%s, %s)", path.toChar(), filename.toChar());
	//csDebug("XeSample::importAudioFile()");
	//csDebug("path=%s", path.toChar());
	//aLog("filename=%s", filename.toChar());

	m_path = path;
	m_filename = filename;
	
	CsAudioFile audioFile(m_path + m_filename);
	
	if(!audioFile.open()) {
		aLog("File could not be opened.");
		return 0;
	}
	//csDebug("File open.");

	if(!audioFile.read(false)) {
		aLog("File could not be read.");
		return 0;
	}
	//csDebug("File read.");

	/*if(m_audioFile->loadSampleData()) {
		qDebug("Sample data loaded");
	} else {
		qDebug("Sample data could not be loaded.");
	}//*/

	
	m_bitDepth = audioFile.bitDepth();
	m_sampleRate = (unsigned long)audioFile.sampleRate();
	m_streams = audioFile.channels();
	m_sampleRate = audioFile.sampleRate();
	m_loopStart = audioFile.loopStart();
	m_loopEnd = audioFile.loopEnd();

	const int bytesPerSample = m_bitDepth/8;
	const int maxBlockDataSize = bytesPerSample * m_streams * SamElementBlock::size;

	char *readBuffer = (char*)malloc(maxBlockDataSize);

	m_lengthFrames = audioFile.lengthFrames();
	m_numBlocks = (m_lengthFrames/SamElementBlock::size)+1;

	//if(m_numBlocks > 16) m_numBlocks = 16;

	// Allocate the blocks and temp buffers
	m_blocks = (SamElementBlock**)aMalloc(sizeof(SamElementBlock*)*m_streams);

	char **compressedBuffer = (char**)malloc(sizeof(char*)*m_streams);
	char **uncompressedBuffer = (char**)malloc(sizeof(char*)*m_streams);
	for(int i=0; i<m_streams; i++) {
		m_blocks[i] = (SamElementBlock*)aMalloc(sizeof(SamElementBlock)*m_numBlocks);
		compressedBuffer[i] = (char*)malloc(SamElementBlock::size);
		uncompressedBuffer[i] = (char*)malloc(bytesPerSample*SamElementBlock::size);
	}


	// Working variables
	int diff[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int lastDiff[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int previous[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int previousDiff[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	int bufferCompressCount[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };


	for(int i=0; i<m_numBlocks; i++) {

		for(int k=0; k<m_streams; k++) {
			memset(compressedBuffer[k], 0, SamElementBlock::size);
			memset(uncompressedBuffer[k], 0, bytesPerSample*SamElementBlock::size);
		}
		
		//aLog("Block %d/%d", i+1, m_numBlocks);
		int cbi[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		int bi[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

		memset(readBuffer, 0, maxBlockDataSize);
		int bytesRead = audioFile.readData(i * SamElementBlock::size, SamElementBlock::size, readBuffer);
		//aLog("bytesRead = %d", bytesRead);
		int byteIndex = 0;
		bool overflow[8] = { false, false, false, false, false, false, false, false };

		for(int j=0; j<m_streams; j++) {
			m_blocks[j][i].previousSample = previous[j];
			m_blocks[j][i].previousDiff = previousDiff[j];
		}

		//qDebug("A");

		for(int j=0; j<SamElementBlock::size; j++) {

			//qDebug("blockSample[%d]",j );

			for(int k=0; k<m_streams; k++) {

				char bytes[8];
				for(int l=0; l<bytesPerSample; l++) {
					bytes[l] = (unsigned char)readBuffer[byteIndex];
					uncompressedBuffer[k][bi[k]] = readBuffer[byteIndex];
					byteIndex++;
					bi[k]++;
				}

				int sample = ((int)bytes[bytesPerSample-1])<<((bytesPerSample-1)*8);
				for (int l=bytesPerSample-2; l>-1; l--) {
					sample |= ((unsigned int)(unsigned char)bytes[l])<<(l*8);
				}
				//sample /= 8;

				/*if(i==21 && k==0) {
					aLog("sample[%d] = %d", j, sample);
				}*/

				if(bytesPerSample == 2) {
					//aLog("Using compression...");
					lastDiff[k] = previous[k] - sample;
					diff[k] = previousDiff[k] - lastDiff[k];

					compressedBuffer[k][cbi[k]] = (char)diff[k];
					cbi[k]++;

					if(diff[k]>127 || diff[k] <-127) {
						overflow[k] = true;
					}

					previous[k] = sample;
					previousDiff[k] = lastDiff[k];
				}

			}

			

		}

		//qDebug("B");
		
		for(int j=0; j<m_streams; j++) {
			//qDebug("Finishing channel %d", j);
			if(!overflow[j] && bytesPerSample <= 2){  //aLog("G");
				//aLog("COMPRESS");
				bufferCompressCount[j]++;  //qDebug("H");
				m_blocks[j][i].compressionMode = SamElementBlock::Compressed16Bit;  //qDebug("I");
				m_blocks[j][i].data = (char*)aMalloc(SamElementBlock::size);  //qDebug("J");
				memcpy(m_blocks[j][i].data, compressedBuffer[j], SamElementBlock::size);  //qDebug("K");
				m_size += SamElementBlock::size;
			} else { //qDebug("C");*/
				if(bytesPerSample == 3) {
					m_blocks[j][i].compressionMode = SamElementBlock::Uncompressed24Bit; //qDebug("D");
				} else {
					m_blocks[j][i].compressionMode = SamElementBlock::Uncompressed16Bit;
				}
				m_blocks[j][i].data = (char*)aMalloc(bytesPerSample*SamElementBlock::size); //qDebug("E");
				memcpy(m_blocks[j][i].data, uncompressedBuffer[j], bytesPerSample*SamElementBlock::size); //qDebug("F");
				m_size += (bytesPerSample*SamElementBlock::size);
			}
		}
		

	}

	/*aLog("======================================================");
	for(int i=0; i<m_streams; i++) {
		aLog("%d out of %d blocks can be fully compressed", bufferCompressCount[i], m_numBlocks);
	}
	aLog("======================================================"); //*/

	audioFile.close();

	free(readBuffer);

	
	for(int i=0; i<m_streams; i++) {
		free(compressedBuffer[i]);
		free(uncompressedBuffer[i]);
	}

	free(compressedBuffer);
	free(uncompressedBuffer);

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamElement::importElementFromChunk(AFile *file)
{
	//aLog("SamElement::importElementFromChunk()");

	char chunkName[4];
	AUInt32 chunkSize;

	//char version = 0;
	//file->readBytes(1, &version);

	file->readU32Lsb(&m_id);

	//AU32Int pathLength;
	//file->readU32Lsb(&pathLength);
	//char *path = malloc(pathLength);
	//file->readBytes(pathLength, path);
	//m_path = path;
	//free(path);

	AUInt32 filenameLength;
	file->readU32Lsb(&filenameLength);
	char *filename = (char*)malloc(filenameLength);
	file->readBytes(filenameLength, filename);
	m_filename = filename;
	free(filename);

	file->read32Lsb(&m_streams); //aLog("m_streams = %d", m_streams);
	file->read32Lsb(&m_set);

	file->read32Lsb(&m_lengthFrames); 
	file->read32Lsb(&m_bitDepth); //aLog("m_bitDepth = %d", m_bitDepth);
	file->readFloatLsb(&m_sampleRate); //aLog("m_sampleRate = %f", m_sampleRate);
	file->read32Lsb(&m_loopStart);
	file->read32Lsb(&m_loopEnd);
	file->readU64Lsb(&m_size);

	file->read32Lsb(&m_numBlocks);

	// Allocate the blocks
	m_blocks = (SamElementBlock**)aMalloc(sizeof(SamElementBlock*)*m_streams);
	for(int i=0; i<m_streams; i++) {
		m_blocks[i] = (SamElementBlock*)aMalloc(sizeof(SamElementBlock)*m_numBlocks);
	}



	int blockCount = 0;
	int channelCount = 0;
	//while(file->readChunkName(chunkName)) {
	for(int i=0; i<m_numBlocks * m_streams; i++) {
		file->readChunkName(chunkName); //aLog("chunkName=[%s]", chunkName);
		if(file->compareChunkName(chunkName, "SamB")) {
			//csDebug("XeSB");
			file->readU32Lsb(&chunkSize);

			file->readBytes(1, &m_blocks[channelCount][blockCount].compressionMode);
			
			file->read32Lsb(&m_blocks[channelCount][blockCount].previousSample);
			file->read32Lsb(&m_blocks[channelCount][blockCount].previousDiff);
			
			int dataSize = calculateBlockDataSize(m_bitDepth, m_blocks[channelCount][blockCount].compressionMode);
			m_blocks[channelCount][blockCount].data = (char*)aMalloc(dataSize);
			file->readBytes(dataSize, m_blocks[channelCount][blockCount].data);

			channelCount++;
			if(channelCount >= m_streams) {
				blockCount++;
				channelCount = 0;
			}
		} else {
			aLog("SamB not found.");
			return false;
		}
	}







	// Allocate the blocks
	/*m_blocks = (SamElementBlock**)aMalloc(sizeof(SamElementBlock*)*m_streams);

	char **compressedBuffer = (char**)malloc(sizeof(char*)*m_streams);
	char **uncompressedBuffer = (char**)malloc(sizeof(char*)*m_streams);
	for(int i=0; i<m_streams; i++) {
		m_blocks[i] = (SamElementBlock*)aMalloc(sizeof(SamElementBlock)*m_numBlocks);
		compressedBuffer[i] = (char*)malloc(SamElementBlock::size);
		uncompressedBuffer[i] = (char*)malloc(bytesPerSample*SamElementBlock::size);
	}

	for(int i=0; i<m_numBlocks; i++) {
		for(int j=0; j<m_streams; j++) {
		}
	}*/

	return true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamElement::exportElementFromChunk(AFile *file)
{
	file->writeChunkName("SamE");
	file->writeU32Lsb(0); // Chunk Size
	//char version = 1;
	//file->writeBytes(1, &version);

	file->writeU32Lsb(m_id); //unsigned int m_id; (4)

	//file->writeU32Lsb(m_path.length());
	//file->writeBytes(m_path.length(), m_path.toChar()); //AString m_path;
	file->writeU32Lsb(m_filename.length());
	file->writeBytes(m_filename.length(), m_filename.toChar()); //AString m_filename;

	file->write32Lsb(m_streams); // int m_streams;
	file->write32Lsb(m_set); // int m_set;
	
	file->write32Lsb(m_lengthFrames); //int m_lengthFrames;
	file->write32Lsb(m_bitDepth); //int m_bitDepth;
	file->writeFloatLsb(m_sampleRate); //float m_sampleRate;
	file->write32Lsb(m_loopStart); //int m_loopStart;
	file->write32Lsb(m_loopEnd); //int m_loopEnd;
	file->writeU64Lsb(m_size); //unsigned long long m_size;

	file->write32Lsb(m_numBlocks); //int m_numBlocks;
	for(int i=0; i<m_numBlocks; i++) {
		for(int j=0; j<m_streams; j++) {
			int dataSize = calculateBlockDataSize(m_bitDepth, m_blocks[j][i].compressionMode);

			file->writeChunkName("SamB");
			file->writeU32Lsb(0); // Chunk Size
			//version = 1;
			//file->writeBytes(1, &version);

			file->writeBytes(1, &m_blocks[j][i].compressionMode); //char compressionMode;
			

			file->write32Lsb(m_blocks[j][i].previousSample); //long previousSample;
			file->write32Lsb(m_blocks[j][i].previousDiff); //long previousDiff;
			
			file->writeBytes(dataSize, m_blocks[j][i].data); //char *data;
		}
	}

	return true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamElement::calculateBlockDataSize(int bitDepth, char compressionMode)
{
	int dataSize = 0;

	switch(compressionMode) {
		case SamElementBlock::Uncompressed16Bit:
		case SamElementBlock::Uncompressed24Bit:
			dataSize = (bitDepth/8)*SamElementBlock::size;
			break;
		case SamElementBlock::Compressed16Bit:
			dataSize = SamElementBlock::size;
			break;
	}

	return dataSize;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamElement::decompressBlock(SamElementBlock *block, float *buffer)
{
	float *pBuffer = buffer;
	float scaler = 1.0f / 32767.0f;
	int bytesPerSample = m_bitDepth/8;
	if(bytesPerSample == 3) {
		scaler = 1.0f/8388608.0f;
	}
	char *data = block->data;
	int k = SamElementBlock::size;

	//APrecisionTimer t;
	//t.start();

	switch(block->compressionMode) {
		case SamElementBlock::Compressed16Bit:
			{
				
				int x = block->previousSample;
				int dx = block->previousDiff;
				
				/*for(int k=0; k<2048; k++) {
					dx-=block->data[k];
					x-=dx;

					buffer[k] = (float) x * scaler;
					//buffer[k] *= 8;
				}*/

				
				
				while(k--) {
					dx -= *data++;
					x -= dx;

					*buffer++ = x;// * scaler;
				}

				
			}
			break;
		case SamElementBlock::Uncompressed16Bit:
			{
				//APrecisionTimer t;
				//t.start();
				while(k--) {
					int sample = (unsigned char)*data++;
					sample |= ((int)*data++) << 8;
					*buffer++ = sample;// * scaler;
				}
				//t.stop();
				//aLog("duration = %f (bps=%d)", t.duration(), bytesPerSample);
			}
			break;
		case SamElementBlock::Uncompressed24Bit:
			{
				//APrecisionTimer t;
				//t.start();

				//int byteIndex = 0;
				while(k--) {
					/*char bytes[8];
					for(int l=0; l<bytesPerSample; l++) {
						//qDebug("byteIndex = %d", byteIndex);
						bytes[l] = (unsigned char)block->data[byteIndex];
						byteIndex++;
					}

					int sample = ((int)bytes[bytesPerSample-1])<<((bytesPerSample-1)*8);
					for (int l=bytesPerSample-2; l>-1; l--) {
						sample |= ((unsigned int)(unsigned char)bytes[l])<<(l*8);
					}*/
					
					//if(bytesPerSample == 2) {
					//	buffer[k] = (float)sample * (1.0f/32768.0f);
					//} else if(bytesPerSample == 3) {
					//	buffer[k] = (float)sample * (1.0f/8388608.0f);
					//}

					//int sample = (unsigned char)*data++;
					//sample |= ((int)*data++) << 8;
					//for(int i=1; i<bytesPerSample; i++) {
					//	sample |= ((int)*data++) << (8*i);
					//}

					//sample |= ((int)*data++) << 8;
					//sample |= ((int)*data++) << 16;

					//int sample = ((int)data[byteIndex+2])<<((bytesPerSample-1)*8);
					//sample |= ((unsigned char)data[byteIndex+1])<<(1*8);
					//sample |= ((unsigned char)data[byteIndex+0])<<(0*8);

					//int sample = ((unsigned char)data[byteIndex+0])<<(0*8);
					//sample |= ((unsigned char)data[byteIndex+1])<<(1*8);
					//sample |= ((int)data[byteIndex+2])<<((bytesPerSample-1)*8);

					int sample = ((unsigned char)*data++)<<(0*8);
					sample |= ((unsigned char)*data++)<<8;
					sample |= ((int)*data++)<<16;

					

					*buffer++ = sample;// * scaler;

					//byteIndex+=3;
				}

				//t.stop();
				//aLog("duration = %f (bps=%d)", t.duration(), bytesPerSample);
			}
			break;
	}

	ippsMulC_32f_I(scaler, pBuffer, SamElementBlock::size);

	//t.stop();
	//aLog("SamElement::decompressBlock() duration = %f (bps=%d)", t.duration(), bytesPerSample);
}

/*
bool SamElement::importAudioFile(Core::String path, Core::String audioFile)
{
	aLog("SamElement::importAudioFile()");

	m_path = path;
	m_filename = audioFile;

	bool ret = true;

	Core::AudioFile file(m_path+m_filename);
	if(!file.read(true)) {
	//if(!file.open(QString(path+audioFile).toStdString(),true)) {
		aLog("Error opening audio file.");	
		return false;
	}

	aLog("Audio file open ok.");

	//file.setFilePositionBySample(0);

	m_streams = file.channels();
	m_lengthFrames = file.lengthFrames();
	m_bitDepth = file.bitDepth();
	m_loopStart = file.loopStart();
	m_loopEnd = file.loopEnd();

	if (m_streams>0 && m_streams<9) {
		aLog ("Loading sample data... bitDepth=%d", m_bitDepth);

		allocateAudioData(m_streams, m_bitDepth, m_lengthFrames);

		for (unsigned long i=0; i<m_lengthFrames; i++) {
			for (int j=0; j<m_streams; j++) {
				if(m_bitDepth==16) {
					Core::Int16 sample;
					file.readNextSampleAsInt16(&sample);
					audioData[j][i] = sample;
				} else if(m_bitDepth==24) {

					Core::Int16 sample;
					float fSample;
					file.readNextSampleAsFloat(&fSample);
					double sc = (double)((double)0x7fffL) + .49999;
					audioData[j][i] = (CsInt16)(fSample*sc);
					//float sample;
					//file.readNextSampleAsFloat(&sample);
					//fAudioData[j][i] = sample;

				}
			}
		}

		m_bitDepth = 16;//file.resolution();

		qDebug ("Sample data loaded.");
		//m_sampleDataLoaded = true;
		ret = true;
	} else {
		qDebug ("Sample data has more than 8 streams.");
		ret = false;
	}

	file.close(true);

	return ret;
}

void SamElement::exportAudioFile(Core::String path, Core::String audioFile)
{
	qDebug("SamElement::exportAudioFile()");

	m_path = path;
	m_filename = audioFile;

	CsAudioFile file;

	if(!file.create(QString(path+audioFile).toStdString())) {
		qDebug("Error creating audio file.");	
		return;
	}

	file.setFormat(kCsAFFWave);
	file.setChannels(m_streams);
	file.setResolution(m_bitDepth);
	file.setSampleRate(44100.0);
	file.setSizeData(m_lengthFrames*(m_bitDepth/8)*m_streams);
	file.writeHeader();

	if(m_bitDepth==16) {
		for(int i=0; i<m_lengthFrames; i++) {
			for(int j=0; j<m_streams; j++) {
				//qDebug("Writing frame %d/%d and stream %d/%d", i+1, m_lengthFrames, j+1, m_streams);
				//file.write16LSB(audioData[j][i]);
			}
		}
	} else {
		qDebug("Unsupported bit depth (%d)", m_bitDepth);
	}

	file.close(false); 
}

bool SamElement::allocateAudioData(int streams, int bitDepth, int lengthFrames, bool zeroData)
{
	for (int i=0; i<streams; i++) {
		bool r = false;
		if(bitDepth==16) {
			audioData[i] = (CsInt16*)VirtualAlloc(NULL, sizeof(CsInt16)*lengthFrames, MEM_COMMIT, PAGE_READWRITE);
			r = VirtualLock (audioData[i], sizeof(CsInt16)*lengthFrames);
		} else if(bitDepth==24) {
			//fAudioData[i] = (float*)VirtualAlloc(NULL, sizeof(float)*lengthFrames, MEM_COMMIT, PAGE_READWRITE);
			//r = VirtualLock (fAudioData[i], sizeof(float)*lengthFrames);
			audioData[i] = (CsInt16*)VirtualAlloc(NULL, sizeof(CsInt16)*lengthFrames, MEM_COMMIT, PAGE_READWRITE);
			r = VirtualLock (audioData[i], sizeof(CsInt16)*lengthFrames);
		}
		if (!audioData[i]) {
			qDebug("VirtualAlloc Fail");
			//ErrorExit(L"VirtualAlloc reserve failed");
		} else {
			qDebug("VirtualAlloc OK");
		}

		if (r) {
			qDebug("VirtualLock OK");
		} else {
			qDebug("VirtualLock Fail");
		}

		if(zeroData) {
			if(bitDepth==16) {
				memset(audioData[i], 0, sizeof(CsInt16)*lengthFrames);
			} else if(bitDepth==24) {
				//memset(fAudioData[i], 0, sizeof(float)*lengthFrames);
				memset(audioData[i], 0, sizeof(CsInt16)*lengthFrames);
			}
		}
	}

	return true;
}*/


// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamElementBlock* SamElement::block(int stream, int index)
{
	return &m_blocks[stream][index];
}