#include "ExsInstrument.h"
#include <ALog.h>

#include <math.h>

ExsInstrument::ExsInstrument(AString filename)
: AFile(filename)
, m_parameters(0)
, m_numZones(0)
, m_numGroups(0)
, m_numAudioFiles(0)
{
	m_parameters = new ExsParameters();

	// Plot the curve used for envelope parameters
	m_envelopeCurve[0] = 0.0f;
	for(int i=0; i<127; i++) {
		//float value = (i+1)*(1.0/127);
		float t = (float)(i+1)/127;
		float x = (1-t)*0+(t*1.0); // Linear
		m_envelopeCurve[i+1] = pow(x, 4.0f) * 10000.0f;
	}

	m_percentCurve[0] = 0.0f;
	for(int i=0; i<127; i++) {
		//float value = (i+1)*(1.0/127);
		float t = (float)(i+1)/127;
		float x = (1-t)*0+(t*1.0); // Linear
		m_percentCurve[i+1] = pow(x, 2.0f) * 100.0f;
	}
}

ExsInstrument::~ExsInstrument()
{
	delete m_parameters;

	for(int i=0; i<zoneList.size(); i++) {
		delete zoneList[i];
	}
	zoneList.clear();

	//aLog("XeInstrument::~XeInstrument() Groups...");

	for(int i=0; i<groupList.size(); i++) {
		delete groupList[i];
	}
	groupList.clear();

	//aLog("XeInstrument::~XeInstrument() Samples...");

	for(int i=0; i<audioFileList.size(); i++) {
		delete audioFileList[i];
	}
	audioFileList.clear();
}

AStatus ExsInstrument::read(void)
{
	if(open()==AError) {
		return AError;
	}

	bool firstHeader = true;

	while(position() < size()) {
		char header[4];
		char headerExs1[4] = { 0x00, 0x01, 0x00, 0x00 };
		char headerExs2[4] = { 0x01, 0x01, 0x00, 0x00 };
		readBytes(4, header);

		if(firstHeader) {
			if(!compareChunkName(header, headerExs1) && !compareChunkName(header, headerExs2)) {
				aLog("EXS File Invalid.");
				close();
				return AError;
			}
			firstHeader = false;
		}

		bool useMsb = false;
		if(header[0]==0x00) { // Assume MSB values if version 1 EXS24 file. This is probably a bad idea, but we haven't had a TBOS/SOBT chunk yet...
			useMsb = true;
		}

		int lvalue;
		if(useMsb) {
			read32Msb(&lvalue);
		} else {
			read32Lsb(&lvalue);
		}
		aLog("size=%d", lvalue);
		long size = lvalue;
	
		// ???
		char b1, b2;
		readBytes(1, &b1);
		readBytes(1, &b2);
		setPosition(fromCurrent, 6);

		//if(header[0]==0x00) { // Original EXS24 instruments are not supported, only EXS24 MkII
		//	close();
		//	return AError;
		//}

		switch(header[3]) {
			case 0x00: // Instrument
				readInstrument(size);
				break;
			case 0x01: // Zone
				readZone(size);
				break;
			case 0x02: // Group
				readGroup(size);
				break;
			case 0x03: // Audio File
				readAudioFile(size, b1, b2);
				break;
			case 0x04: // Parameters
				readParameters(size);
				break;
			/*default:
				close();
				return AError;
				break;*/
		}
	}

	close();

	return AOk;
}

AStatus ExsInstrument::readInstrument(long dataSize)
{
	char chunkName[5] = { 0, 0, 0, 0, 0};
	unsigned long chunkSize = 0;
	char *data;
	int lvalue;
	bool useMsb = false;

	// SOBT/TBOS Chunk
	readChunkName(chunkName);
	if(compareChunkName(chunkName, "SOBT")) {
		aLog("SOBT");
		useMsb = true;
	}

	// Instrument name
	//data = (char*)malloc(64);
	readBytes(64, m_name);
	aLog("Instrument name = [%s]", m_name);
	//free(data);

	// ?
	setPosition(fromCurrent, 4);

	// Number of Zones
	if(useMsb) {
		read32Msb(&lvalue);
	} else {
		read32Lsb(&lvalue);
	}
	m_numZones = lvalue;
	aLog("Num Zones = %d", lvalue);

	// Number of Groups
	if(useMsb) {
		read32Msb(&lvalue);
	} else {
		read32Lsb(&lvalue);
	}
	m_numGroups = lvalue;
	aLog("Num Groups = %d", lvalue);

	// Number of Audio Files
	if(useMsb) {
		read32Msb(&lvalue);
	} else {
		read32Lsb(&lvalue);
	}
	m_numAudioFiles = lvalue;
	aLog("Num Audio Files = %d", lvalue);

	// ?
	setPosition(fromCurrent, 24); // 40 - 16

	return AOk;
}

AStatus ExsInstrument::readZone(long dataSize)
{
	char chunkName[5] = { 0, 0, 0, 0, 0};
	char *data;
	char byte;
	unsigned int ulvalue;
	int lvalue;
	bool useMsb = false;

	ExsZone *zone = new ExsZone();

	// SOBT/TBOS Chunk
	readChunkName(chunkName);
	if(compareChunkName(chunkName, "SOBT")) {
		aLog("SOBT");
		useMsb = true;
	}
	
	// Zone name
	//data = (char*)malloc(64);
	readBytes(64, zone->name);
	aLog("Zone name = [%s]", zone->name);
	//free(data);

	// ---
	// Data begins here

	// Zone Flags
	readBytes(1, &byte);
	dataSize -= 1;
	if(byte&0x01) zone->oneShot = true;
	if(byte&0x02) zone->disablePitch = true;
	if(byte&0x04) zone->reverse = true;
	if(byte&0x08) zone->useZoneVelocityRange = true;
	if(byte&0x40) zone->useZoneOutput = true;

	aLog("oneShot = %d", (int)zone->oneShot);
	aLog("disablePitch = %d", (int)zone->disablePitch);
	aLog("reverse = %d", (int)zone->reverse);
	aLog("useZoneVelocityRange = %d", (int)zone->useZoneVelocityRange);
	aLog("useZoneOutput = %d", (int)zone->useZoneOutput);

	// Pitch: Key
	readBytes(1, &byte);
	dataSize -= 1;
	zone->keyNote = byte;
	aLog("keyNote = %d", zone->keyNote);
	
	// Pitch: Fine
	readBytes(1, &byte);
	dataSize -= 1;
	zone->tuneCents = byte;
	aLog("tuneCents = %d", zone->tuneCents);
	
	readBytes(1, &byte); // pan
	dataSize -= 1;
	zone->pan = byte;
	aLog("pan = %d", zone->pan);
	
	readBytes(1, &byte); // Volume
	dataSize -= 1;
	zone->volume = byte;
	aLog("volume = %d", zone->volume);
	
	readBytes(1, &byte); // Scale
	dataSize -= 1;
	zone->scale = byte;
	aLog("scale = %d", zone->scale);
	
	readBytes(1, &byte);
	dataSize -= 1;
	zone->zoneRangeLo = byte;
	aLog("zoneRangeLo = %d", zone->zoneRangeLo);

	readBytes(1, &byte);
	dataSize -= 1;
	zone->zoneRangeHi = byte;
	aLog("zoneRangeHi = %d", zone->zoneRangeHi);

	readBytes(1, &byte); // ?????
	dataSize -= 1;

	readBytes(1, &byte);
	dataSize -= 1;
	zone->zoneVelocityRangeLo = byte;
	aLog("zoneVelocityRangeLo = %d", zone->zoneVelocityRangeLo);

	readBytes(1, &byte);
	dataSize -= 1;
	zone->zoneVelocityRangeHi = byte;
	aLog("zoneVelocityRangeHi = %d", zone->zoneVelocityRangeHi);

	readBytes(1, &byte); // ?????
	dataSize -= 1;

	// Start frame
	if(useMsb) {
		readU32Msb(&ulvalue);
	} else {
		readU32Lsb(&ulvalue);
	} 
	dataSize -= 4;
	zone->startFrame = ulvalue;
	aLog("start = %d", zone->startFrame);

	// End frame
	if(useMsb) {
		readU32Msb(&ulvalue);
	} else {
		readU32Lsb(&ulvalue);
	} 
	dataSize -= 4;
	zone->endFrame = ulvalue;
	aLog("end = %d", zone->endFrame);

	// Loop Start
	if(useMsb) {
		readU32Msb(&ulvalue);
	} else {
		readU32Lsb(&ulvalue);
	} 
	dataSize -= 4;
	zone->loopStart = ulvalue;
	aLog("loopStart = %d", zone->loopStart);

	// Loop End
	if(useMsb) {
		readU32Msb(&ulvalue);
	} else {
		readU32Lsb(&ulvalue);
	} 
	dataSize -= 4;
	zone->loopEnd = ulvalue;
	aLog("loopEnd = %d", zone->loopEnd);

	// Auto Crossfade
	if(useMsb) {
		readU32Msb(&ulvalue);
	} else {
		readU32Lsb(&ulvalue);
	} 
	dataSize -= 4;
	zone->autoCrossfade = ulvalue;
	aLog("autoCrossfade = %d", zone->autoCrossfade);

	readBytes(1, &byte); //?
	dataSize -= 1;
	aLog("loopTune = %d", byte);

	readBytes(1, &byte);
	dataSize -= 1;
	if(byte&0x01) zone->loopEnabled = true;
	if(byte&0x02) zone->loopEqualPower = true;
	aLog("loopEnabled = %d", (int)zone->loopEnabled);
	aLog("loopEqualPower = %d", (int)zone->loopEqualPower);
	
	setPosition(fromCurrent, 46);
	dataSize -= 46;

	readBytes(1, &byte);
	dataSize -= 1;
	zone->tuneSemitones = byte;
	aLog("tuneSemitones = %d", zone->tuneSemitones);

	readBytes(1, &byte); // ?????
	dataSize -= 1;
	//aLog("byte = %d", byte);

	readBytes(1, &byte);
	dataSize -= 1;
	zone->output = byte;
	aLog("output = %d", zone->output);

	// ???
	setPosition(fromCurrent, 5);
	dataSize -= 5;

	// Group Id
	if(useMsb) {
		read32Msb(&lvalue);
	} else {
		read32Lsb(&lvalue);
	}
	dataSize -= 4;
	zone->group = lvalue;
	aLog("group = %d", zone->group);

	// Audio File Id
	if(useMsb) {
		readU32Msb(&ulvalue);
	} else {
		readU32Lsb(&ulvalue);
	} 
	dataSize -= 4;
	zone->audioFileId = ulvalue;
	aLog("audioFileId = %d", zone->audioFileId);

	aLog("dataSize remaining = %d", dataSize);
	setPosition(fromCurrent, dataSize);

	
	/*if(dataSize > 96) {
		setPosition(fromCurrent, 8); // 24-16
	}*/
	// Version 1 Zones have 96 bytes (after header, TBOS, and the 64 chars)

	zoneList.append(zone);

	return AOk;
}

AStatus ExsInstrument::readGroup(long dataSize)
{
	char chunkName[5] = { 0, 0, 0, 0, 0};
	char *data;
	char byte;
	unsigned int ulvalue;
	int lvalue;
	bool useMsb = false;

	ExsGroup *group = new ExsGroup();

	// SOBT/TBOS Chunk
	readChunkName(chunkName);
	if(compareChunkName(chunkName, "SOBT")) {
		aLog("SOBT");
		useMsb = true;
	}
	
	// Group name
	//data = (char*)malloc(64);
	readBytes(64, group->name);
	aLog("Group name = [%s]", group->name);
	//free(data);

	readBytes(1, &byte);
	group->volume = byte;
	aLog("volume = %d", group->volume);

	readBytes(1, &byte);
	group->pan = byte;
	aLog("pan = %d", group->pan);

	readBytes(1, &byte);
	group->voices = byte; // 0 = Max (otherwise range is 1-16)
	aLog("voices = %d", group->voices);

	readBytes(1, &byte);
	if(byte&0x40) group->triggerReleaseToggle = true;
	aLog("triggerReleaseToggle = %d", (int)group->triggerReleaseToggle);
	
	readBytes(1, &byte);
	
	readBytes(1, &byte);
	group->selectRangeLo = byte;
	aLog("selectRangeLo = %d", group->selectRangeLo);

	readBytes(1, &byte);
	group->selectRangeHi = byte;
	aLog("selectRangeHi = %d", group->selectRangeHi);

	setPosition(fromCurrent, 9);

	if(useMsb) {
		read32Msb(&lvalue);
	} else {
		read32Lsb(&lvalue);
	}
	group->releaseDecay = lvalue;
	aLog("releaseDecay = %ld", group->releaseDecay);

	setPosition(fromCurrent, 21);
	
	readBytes(1, &byte);
	group->filterCutOffOffset = byte;
	aLog("filterCutOffOffset = %d", group->filterCutOffOffset);

	readBytes(1, &byte);

	readBytes(1, &byte); //?
	group->filterResonanceOffset = byte;
	aLog("filterResonanceOffset = %d", group->filterResonanceOffset);

	setPosition(fromCurrent, 12);

	if(useMsb) {
		read32Msb(&lvalue);
	} else {
		read32Lsb(&lvalue);
	}
	group->ampAttackOffset = lvalue;
	aLog("ampAttackOffset = %ld", group->ampAttackOffset);

	if(useMsb) {
		read32Msb(&lvalue);
	} else {
		read32Lsb(&lvalue);
	}
	group->ampDecayOffset = lvalue;
	aLog("ampDecayOffset = %ld", group->ampDecayOffset);

	if(useMsb) {
		read32Msb(&lvalue);
	} else {
		read32Lsb(&lvalue);
	}
	group->ampSustainOffset = lvalue;
	aLog("ampSustainOffset = %ld", group->ampSustainOffset);

	if(useMsb) {
		read32Msb(&lvalue);
	} else {
		read32Lsb(&lvalue);
	}
	group->ampReleaseOffset = lvalue;
	aLog("ampReleaseOffset = %ld", group->ampReleaseOffset);

	readBytes(1, &byte);

	readBytes(1, &byte);
	group->triggerOn = byte;
	aLog("triggerOn = %d", group->triggerOn); // 0 - Key Down, 1 - Key Release
	
	readBytes(1, &byte);
	group->output = byte;
	aLog("output = %d", group->output);

	readBytes(1, &byte);
	group->selectByNotePitch = byte;
	aLog("selectByNotePitch = %d", group->selectByNotePitch);

	setPosition(fromCurrent, 4);

	if(useMsb) {
		read32Msb(&lvalue);
	} else {
		read32Lsb(&lvalue);
	}
	group->selectByGroupNum = lvalue;
	aLog("selectByGroupNum = %d", group->selectByGroupNum);

	readBytes(1, &byte); //
	group->selectByMode[3];
	group->selectByMode[0] = byte; // 0 - Not assigned, 1 - Note, 2 - Group, 3 - Control, 4 - Bend, 5 - MIDI Channel
	aLog("selectByMode[0] = %d", group->selectByMode[0]);

	readBytes(1, &byte);
	group->selectByControlNum = byte;
	aLog("selectByControlNum = %d", group->selectByControlNum);

	readBytes(1, &byte);
	group->selectByControlRangeLo = byte;
	aLog("selectByControlRangeLo = %d", group->selectByControlRangeLo);

	readBytes(1, &byte);
	group->selectByControlRangeHi = byte;
	aLog("selectByControlRangeHi = %d", group->selectByControlRangeHi);

	readBytes(1, &byte);
	group->keyRangeLo = byte;
	aLog("keyRangeLo = %d", group->keyRangeLo);

	readBytes(1, &byte);
	group->keyRangeHi = byte;
	aLog("keyRangeHi = %d", group->keyRangeHi);

	readBytes(1, &byte);
	group->selectByMidiChannelChannel = byte;
	aLog("selectByMidiChannelChannel = %d", group->selectByMidiChannelChannel);

	readBytes(1, &byte); // ?????

	readBytes(1, &byte);
	group->selectByMode[1] = byte;
	aLog("selectByMode[1] = %d", group->selectByMode[1]);

	readBytes(1, &byte);
	group->selectByMode[2] = byte;
	aLog("selectByMode[2] = %d", group->selectByMode[2]);

	readBytes(1, &byte);
	group->selectByBendRangeLo = byte;
	aLog("selectByBendRangeLo = %d", group->selectByBendRangeLo);

	readBytes(1, &byte);
	group->selectByBendRangeHi = byte;
	aLog("selectByBendRangeHi = %d", group->selectByBendRangeHi);

	if(dataSize > 96){
	if(useMsb) {
		readU32Msb(&ulvalue);
	} else {
		readU32Lsb(&ulvalue);
	} 
	group->ampHoldOffset = ulvalue;
	aLog("ampHoldOffset = %ld", group->ampHoldOffset);
	
	if(useMsb) {
		read32Msb(&lvalue);
	} else {
		read32Lsb(&lvalue);
	}
	group->filterAttackOffset = lvalue;
	aLog("filterAttackOffset = %ld", group->filterAttackOffset);

	if(useMsb) {
		read32Msb(&lvalue);
	} else {
		read32Lsb(&lvalue);
	}
	group->filterDecayOffset = lvalue;
	aLog("filterDecayOffset = %ld", group->filterDecayOffset);

	if(useMsb) {
		read32Msb(&lvalue);
	} else {
		read32Lsb(&lvalue);
	}
	group->filterSustainOffset = lvalue;
	aLog("filterSustainOffset = %ld", group->filterSustainOffset);

	if(useMsb) {
		read32Msb(&lvalue);
	} else {
		read32Lsb(&lvalue);
	}
	group->filterReleaseOffset = lvalue;
	aLog("filterReleaseOffset = %ld", group->filterReleaseOffset);

	if(useMsb) {
		readU32Msb(&ulvalue);
	} else {
		readU32Lsb(&ulvalue);
	} 
	group->filterHoldOffset = ulvalue;
	aLog("filterHoldOffset = %ld", group->filterHoldOffset);
	}
	//setPosition(fromCurrent, 16);
	
	groupList.append(group);

	return AOk;
}

AStatus ExsInstrument::readAudioFile(long dataSize, char b1, char b2)
{
	char chunkName[5] = { 0, 0, 0, 0, 0};
	char *data;
	char byte;
	unsigned long ulvalue;
	long lvalue;

	ExsAudioFile *audioFile = new ExsAudioFile();

	// TBOS Chunk
	readChunkName(chunkName);
	
	// Audio File name
	//data = (char*)malloc(64);
	readBytes(64, audioFile->filename);
	aLog("AudioFile name = [%s]", audioFile->filename);
	//audioFile->filename = CsString(data);
	//free(data);

	//if(b2 == 0x01) {
		setPosition(fromCurrent, dataSize);//336);
	//} else {
	//	setPosition(fromCurrent, 592);
	//}

	audioFileList.append(audioFile);

	return AOk;
}

AStatus ExsInstrument::readParameters(long dataSize)
{
	char chunkName[5] = { 0, 0, 0, 0, 0};
	char *data;
	char byte;
	unsigned long ulvalue;
	long lvalue;

	// TBOS Chunk
	readChunkName(chunkName);

	// Zone name
	//data = (char*)malloc(64);
	readBytes(64, m_parameters->name);
	aLog("Parameters name = [%s]", m_parameters->name);
	//free(data);

	setPosition(fromCurrent, 104);

	readBytes(1, &byte);
	m_parameters->outputVolume = byte;
	aLog("outputVolume = %d", m_parameters->outputVolume);
	readBytes(1, &byte);

	readBytes(1, &byte);
	m_parameters->keyScale = byte;
	aLog("keyScale = %d", m_parameters->keyScale);
	readBytes(1, &byte);

	readBytes(1, &byte);
	m_parameters->pitchBendUp = byte;
	aLog("pitchBendUp = %d", m_parameters->pitchBendUp);
	readBytes(1, &byte);

	readBytes(1, &byte);
	m_parameters->pitchBendDown = byte;
	aLog("pitchBendDown = %d", m_parameters->pitchBendDown);
	readBytes(1, &byte);

	//setPosition(fromCurrent, 372-4-64);
	setPosition(fromCurrent, dataSize-104-4);

	return AOk;
}