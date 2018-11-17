#include "SamOscillator.h"
#include "SamChannel.h"

#include "AMemoryManager.h"
#include "ALog.h"

#define USE_IPP_C
#ifdef USE_IPP_C
#include <ipps.h>
#endif

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamOscillator::SamOscillator(SamVoice *voice, int numStreams, SamStreamMix *streamMix, int setId)
: m_voice(voice)
, m_numStreams(numStreams)
, m_streamMix(streamMix)
, m_setId(setId)
, m_element(0)
, decompressionBufferBlockIndex(-1)
, m_samplePointer(0.0f)
, m_pitchModulationBuffer(0)
, m_release(false)
//, m_streamCoefficients(0)
{
	//aLog("SamOscillator::SamOscillator(%d)", numStreams);

	m_channel = m_voice->channel();

	decompressionBuffer = (float**)aMalloc(sizeof(float*)*numStreams);
	for(int i=0; i<numStreams; i++) {
		//decompressionBuffer[i] = (float*)malloc(sizeof(float)*blockSize);
		decompressionBuffer[i] = (float*)aMalloc((sizeof(float)*SamElementBlock::size));
	}

	//osc = new AOscillator(44100.0);

	frame = (float**)aMalloc(sizeof(float*)*m_numStreams);
	for(int i=0; i<m_numStreams; i++) {
		frame[i] = (float*)aMalloc(sizeof(float)*4);
		frame[i][0] = 0;
	}

	inputBuffer = (float**)aMalloc(sizeof(float*)*m_numStreams);
	m_maxInputBufferSize = sizeof(float)*(440*8/55)*m_channel->audioProcessInfo().bufferSize;
	m_maxInputBufferSizeFloat = m_maxInputBufferSize/4;
	for(int i=0; i<m_numStreams; i++) {
		inputBuffer[i] = (float*)aMalloc(m_maxInputBufferSize);
		memset(inputBuffer[i], 0, m_maxInputBufferSize);
	}

	

	//aLog("EXIT: SamOscillator::SamOscillator()");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamOscillator::~SamOscillator()
{
	for(int i=0; i<m_numStreams; i++) {
		aFree(decompressionBuffer[i], sizeof(float)*SamElementBlock::size);
		aFree(frame[i], sizeof(float)*4);
		aFree(inputBuffer[i], m_maxInputBufferSize);
	}
	aFree(decompressionBuffer, sizeof(float*)*m_numStreams);
	aFree(frame, sizeof(float*)*m_numStreams);
	aFree(inputBuffer, sizeof(float*)*m_numStreams);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamOscillator::assign(SamCell *cell, SamElement *element, int offset, bool release)
{
	m_cell = cell;
	m_element = element;
	decompressionBufferBlockIndex = -1;
	m_samplePointer = 0.0f + (double)offset;
	m_release = release;
	//aLog("assign length frames = %d", element->lengthFrames());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamOscillator::advanceSamplePointer(CsSampleBuffer *buffer, int frameOffset)
{
	for(int i=frameOffset; i<buffer->numFrames; i++) {
		/*if(m_pitchModulationBuffer[i] == 1.0f) {
			m_samplePointer += m_pitchModulationBuffer[i];
			if(m_cell->isLoop()) {
				if(m_samplePointer >= m_cell->loopEnd()) {
					m_samplePointer = m_cell->loopStart();
				}
			}
		} else {
			m_samplePointer += m_pitchModulationBuffer[i];
			if(m_cell->isLoop()) {
				if(m_samplePointer >= m_cell->loopEnd()) {
					m_samplePointer = m_cell->loopStart();
				}
			}
		}*/
		m_samplePointer += m_pitchModulationBuffer[i];
		if(m_cell->isLoop() && !m_release) {
			if(m_samplePointer >= m_cell->loopEnd()) {
				m_samplePointer = m_cell->loopStart();
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamOscillator::renderBuffer(CsSampleBuffer *buffer, int frameOffset)
{
	//aLog("SamOscillator::renderBuffer(frameOffset = %d)", frameOffset);
	//advanceSamplePointer(buffer, frameOffset);
	//return;

	// How many output samples?
	// ------------------------
	int numOutputSamples = buffer->numFrames - frameOffset;
	//aLog("numOutputSamples = %d", numOutputSamples);

	
	// How many input samples do we need?
	// ----------------------------------
	float numInputSamples = m_voice->numInputSamplesRequired(numOutputSamples);
	//aLog("numInputSamples = %f", numInputSamples);

	if(numInputSamples != (float)numOutputSamples) {
		//aLog("numInputSamples != (float)numOutputSamples");
		numInputSamples += 4;
	}

	if(numInputSamples >= (m_maxInputBufferSize/4)) {
		//aLog("inputSamples >= m_maxInputBufferSize");
		return;
	}

	//aLog("m_samplePointer = %f", m_samplePointer);
	unsigned int sp = (unsigned int)m_samplePointer;

	//if((!m_cell->isLoop() || m_release) && sp + numInputSamples >= m_cell->lengthFrames()) {
	//	aLog("sp = %d, numInputSamples = %f, lf = %d", sp, numInputSamples, m_cell->lengthFrames());
	//	numInputSamples = m_element->lengthFrames();
	//}

	if((!m_cell->isLoop() || m_release) && sp + numInputSamples >= m_element->lengthFrames()) {
		//aLog("sp = %d, numInputSamples = %f, lf = %d", sp, numInputSamples, m_cell->lengthFrames());
		numInputSamples = m_element->lengthFrames() - sp;
	}

	//if((!m_cell->isLoop() && sp + numInputSamples >= m_cell->lengthFrames()) || (m_release && sp + numInputSamples >= m_cell->lengthFrames())) {
	//	numInputSamples = m_element->lengthFrames

	for(int i=0; i<m_numStreams; i++) {
		//memset(inputBuffer[i], 0, m_maxInputBufferSize);
		ippsSet_32f(0, inputBuffer[i], m_maxInputBufferSizeFloat);
	}

	// Render the input buffer
	unsigned int inputBufferPos = 0;
	unsigned int numSamplesToGet = 0;
	int remainingSamples = numInputSamples;
	//while(inputBufferPos < numInputSamples) {
	while(remainingSamples > 0) {
		//aLog("%d < %d", inputBufferPos, numInputSamples);
		//aLog("remainingSamples = %d", remainingSamples);
		if(m_cell->isLoop() && !m_release && sp + numInputSamples >= m_cell->loopEnd()) {
			numSamplesToGet = m_cell->loopEnd() - sp;
		} else {
			numSamplesToGet = numInputSamples;
		}
		fillBuffer(sp, numSamplesToGet, inputBuffer, inputBufferPos);
		inputBufferPos += numSamplesToGet;
		sp += numSamplesToGet;
		if(m_cell->isLoop() && !m_release && sp >= m_cell->loopEnd()) {
			sp = m_cell->loopStart();
		}
		remainingSamples -= numSamplesToGet;
	}

	//fillBuffer(sp, numInputSamples, inputBuffer);

	m_inputSamplePointer = m_samplePointer - (int)m_samplePointer;

	for(int i=frameOffset; i<buffer->numFrames; i++) {

		sp = (unsigned int)m_inputSamplePointer;

		if(m_pitchModulationBuffer[i] > 0.99999f && m_pitchModulationBuffer[i] < 1.00001f) { //aLog("HERE %f", m_pitchModulationBuffer[i]); // == 1.0f
			for(int j=0; j<m_numStreams; j++) {
				for(int k=0; k<buffer->numStreams; k++) {
					buffer->buffer32[k][i] += ((inputBuffer[j][sp]  ) * m_channel->m_streamCoefficients[m_setId][j][k]);
				}
			}
		} else {
			
			float x[8][4];
			
			//if(sp >= numInputSamples) {
			//	aLog("m_inputSamplePointer = %f (%d)", m_inputSamplePointer, numInputSamples);
			//}

			for(int j=0; j<m_numStreams; j++) {
				for(int k=0; k<4; k++) {
					x[j][k] = inputBuffer[j][sp+k];
				}
			}

			double fraction = m_inputSamplePointer - (int)m_inputSamplePointer;
			for(int j=0; j<m_numStreams; j++) {
				float a,b,c;
				a = (3.0f*(x[j][1]-x[j][2])-x[j][0]+x[j][3])*0.5f;
				b = 2.0f * x[j][2]+x[j][0]-(5.0f*x[j][1]+x[j][3])*0.5f;
				c = (x[j][2]-x[j][0])*0.5f;
				//buffer->buffer32[j][i] += ((((a*fraction)+b)*fraction+c)*fraction+x[j][1])*0.5;
				//pan(((((a*fraction)+b)*fraction+c)*fraction+x[j][1])*0.5, j, buffer, i);
				for(int k=0; k<buffer->numStreams; k++) {
					buffer->buffer32[k][i] += ((((((a*fraction)+b)*fraction+c)*fraction+x[j][1])  ) * m_channel->m_streamCoefficients[m_setId][j][k]);
					//aLog("[%d][%d] %f", k, i, buffer->buffer32[k][i]);
				}
			}//*/
		}

		

		m_samplePointer += m_pitchModulationBuffer[i];
		m_inputSamplePointer += m_pitchModulationBuffer[i];
		
		if(m_cell->isLoop() && !m_release) {
			if((unsigned int)m_samplePointer >= m_cell->loopEnd()) {
				m_samplePointer = (float)m_cell->loopStart();
			}
		} else if((unsigned int)m_samplePointer >= m_element->lengthFrames()) {
			m_samplePointer = m_element->lengthFrames();
			break;
		} 

	}

	//for(int i=0; i<buffer->numFrames; i++) {
	//		aLog("[%d][%d] %f", 0, i, buffer->buffer32[0][i]);
	//	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamOscillator::isSamplePointerOutOfRange(void)
{
	if(m_element) {
		if(m_samplePointer < m_element->lengthFrames()) { 
			return false;
		} //aLog("OOR %f < %d", m_samplePointer, m_element->lengthFrames());
	}

	return true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamOscillator::fillBuffer(int fromFrame, int numFrames, float **buffer, int outputIndex)
{
	//aLog("SamOscillator::fillBuffer(%d, %d, %d)", fromFrame, numFrames, outputIndex);

	if(fromFrame < 0) {
		aLog("!*!*!*! fromFrame < 0");
		return 0;
	}

	int firstBlockToPlay = (fromFrame)/2048;
	int lastBlockToPlay = (fromFrame+(numFrames-1))/2048;
	//csDebug("Here!1 (%d)", m_sample);
	if(firstBlockToPlay >= m_element->numBlocks() || lastBlockToPlay >= m_element->numBlocks()) {
		aLog("firstBlockToPlay = %d, numBlocks() = %d, lastBlockToPlay = %d", firstBlockToPlay, m_element->numBlocks(), lastBlockToPlay);
		//aLog("EXIT:A");
		return 0;
	}
	//csDebug("Here!2");
	int numBlocksToPlay = (lastBlockToPlay - firstBlockToPlay) + 1;

	//aDebug("firstBlockToPlay=%d, numBlocksToPlay=%d", firstBlockToPlay, numBlocksToPlay);

	int firstFrameWithinBlock = fromFrame - (firstBlockToPlay*2048);
	//aDebug("firstFrameWithinBlock = %d", firstFrameWithinBlock);

	//int outputIndex = 0;

	for(int i=firstBlockToPlay; i<firstBlockToPlay+numBlocksToPlay; i++) {
		// If the block we need has already been decompressed, there's no need to get it again
		if(i != decompressionBufferBlockIndex) {
			//aLog("---------- Getting new block %d ----------", i);
			for(int j=0; j<m_element->numStreams(); j++) {			
				//m_element->decompressBlock(&m_element->blocks()[j][i], decompressionBuffer[j]);
				m_element->decompressBlock(m_element->block(j,i), decompressionBuffer[j]);
				decompressionBufferBlockIndex = i;
			}
		} else {
			//cDebug("Block we need is cached. (%d)", decompressionBufferBlockIndex);
		}

		int numFramesToRender = numFrames-outputIndex;
		if(firstFrameWithinBlock+numFramesToRender>=2048) {
			numFramesToRender = 2048-firstFrameWithinBlock;
		}
		/*if(fromFrame < 5 || fromFrame >= 44095) {
		aLog("Rendering from frame %d to %d (%d)", outputIndex, outputIndex+numFramesToRender, numFramesToRender);
		aLog(" Rendering from frame %d to %d in block %d", firstFrameWithinBlock, firstFrameWithinBlock+numFramesToRender, i);
		}*/
		for(int j=0; j<m_element->numStreams(); j++) {
			/*int frameIndex = outputIndex;
			for(int k=firstFrameWithinBlock; k<firstFrameWithinBlock+numFramesToRender; k++) {
				buffer[j][frameIndex] = decompressionBuffer[j][k];
				//if(fromFrame >= 43962-16 && fromFrame <= 44100) {
				//	aLog("[%d][%d][%f]", fromFrame, j, decompressionBuffer[j][k]);
				//}
				frameIndex++;
			}*/
			//memcpy(buffer[j]+outputIndex, decompressionBuffer[j]+firstFrameWithinBlock, numFramesToRender*sizeof(float));
			ippsCopy_32f(decompressionBuffer[j]+firstFrameWithinBlock, buffer[j]+outputIndex, numFramesToRender);
		}

		outputIndex += numFramesToRender;
		firstFrameWithinBlock = 0;

		
	}

	return 1;
}