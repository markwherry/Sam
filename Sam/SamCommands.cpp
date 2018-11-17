#include "SamCommands.h"
#include "SamEngine.h"
#include "SamProject.h"
#include "SamProjectCommands.h"
#include "SamRealTimeCommands.h"

#include "Sam.h"

#include "ALog.h"
#include "AFile.h"
using namespace Atom;

#include "ExsInstrument.h"
#include "SfzInstrument.h"

#include <QFile>
#include <QDir>
#include <QMessageBox>

#include "CsAudioFile.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
NewInstrument::NewInstrument(AList<SamSet*> *setList)
{
	setName("New Instrument");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void NewInstrument::execute(ACommandManager *commandManager, int id)
{
	aLog("SamInstrument::create()");

	/*SamInstrument *instrument = new SamInstrument(samEngine);

	// Create Basic Instrument Properties
	instrument->recreateUuid();
	instrument->setName("Instrument");


	// Create Element Pool
	if(m_elementPool!=0) {
		delete m_elementPool;
	}
	m_elementPool = new SamElementPool(this, instrumentPath);

	if(allMonoStreams) {
		for(int i=0; i<mode; i++) {
			m_elementPool->createSet(AString::number(i), 1);
		}
	} else {
		if(mode==2) {
			m_elementPool->createSet("Stereo", 2);
		} else if(mode==4) {
			m_elementPool->createSet("Front", 2);
			m_elementPool->createSet("Rear", 2);
		} else if(mode==6) {
			m_elementPool->createSet("Close", 2);
			m_elementPool->createSet("Room", 2);
			m_elementPool->createSet("Far", 2);
		} else if(mode==12) {
			m_elementPool->createSet("Frnt LR", 2);
			m_elementPool->createSet("Room LR", 2);
			m_elementPool->createSet("Glry Tree Frnt LR", 2);
			m_elementPool->createSet("Glry Tree Rear LR", 2);
			m_elementPool->createSet("Frnt C / Room C", 2);
			m_elementPool->createSet("Glry C / LFE", 2);
		}
	}


	// Create Stream Mix
	SamStreamMix *streamMix = new SamStreamMix(this);
	if(allMonoStreams) {
		streamMix->createMonoStreamMix(mode);
	} else {
		if(mode==2) {
			streamMix->createStereoStreamMix();
		} else if(mode==4) {
			streamMix->createQuadStreamMix(2);
		} else if(mode==6) {
			streamMix->createQuadStreamMix(3);
		} else if(mode==12) {
			streamMix->createQuadStreamMix(6);
		}
	}
	m_streamMixes.append(streamMix);


	// Create Version
	if(createVersion) {
		SamVersion *version = new SamVersion(this);
		version->setName("Instrument");
		version->createArticulation();
		version->createOutput("Output");
		m_versions.append(version);
	}*/
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
LoadInstruments::LoadInstruments(AList<AString> *files, int numFiles, bool createChannels)
: m_files(files)
, m_numFiles(numFiles)
, m_createChannels(createChannels)
{
	setName("Load Instruments");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void LoadInstruments::execute(ACommandManager *commandManager, int id)
{
	ACommand::execute(commandManager, id);

	aLog("SamCommandLoadInstruments::execute()");

	QString errorMessage = ""; 

	AList<SamInstrument*> instruments;

	for (int i=0; i<m_numFiles; i++) {
		if(m_numFiles == 1) {
			setProgressLabel(0, "Loading instrument...");
		} else {
			setProgressLabel(0, "Loading instruments...");
		}
		SamInstrument *instrument = 0;
		
		if(m_files->at(i).last(3).isEqualTo("exs")) {
			instrument = importExsInstrument(m_files->at(i));
		} else if(m_files->at(i).last(3).isEqualTo("sfz")) {
			instrument = importSfzInstrument(m_files->at(i));
		} else {
			instrument = loadInstrument(m_files->at(i));
		}
						
		if(!instrument) {
			aLog("ERROR LOADING INSTRUMENT");
			//setFeedback(true);
			//setMessage("Error Loading Instrument");
			//setMessageType(

			if(errorMessage.length() == 0) {
				errorMessage = "<b>The following Instruments could not be loaded:</b><br><br>";
			}

			errorMessage += m_files->at(i).toChar();
			errorMessage += "<br>";

			if(m_numFiles>1) {
				/*int ret = QMessageBox::warning(0,"Sam", "There was an error loading the instrument:<br>"+instrumentList[i]+"<br>Do you want to continue loading other instruments, or stop loading instruments?", "Continue", "Abort");
				qDebug ("Message box returned %d", ret);
				if (ret==1) {
					break;
				}*/
			} else {
				//QMessageBox::warning(0,"Sam", "There was an error loading the instrument:<br>"+instrumentList[i], "OK");
			}
		} else {
			aLog("Instrument loaded successfully.");
			if(m_createChannels) {
				samEngine->project()->commandManager()->executeInBackground(new CreateChannel(instrument));
			}
			instruments.append(instrument);
		}

		
	}

	for(int i=0; i<instruments.size(); i++) {
		if(samEngine->isLoadSampleDataWithInstrument()) {
			//aLog("$$$$ Loading Sample Data for %s", instruments[i]->name().toChar());
			for(int j=0; j<instruments[i]->elementPool()->numSets(); j++) { //aLog("$$$$ set %d/%d", j+1, instruments[i]->elementPool()->numSets());
				samGui->commandManager()->execute(new UpdateSetElementStates(instruments[i], j, true));
			}
		} else {
			//aLog("$$$$ NOT Loading Sample Data for %s", instruments[i]->name().toChar());
		}
	}

	m_files->clear();
	delete m_files;

	if(errorMessage.length() > 0) {
		setFeedback(true);
		QByteArray baem = errorMessage.toLatin1();
		setMessage(baem.data());
	}

	aLog("EXIT: SamCommandLoadInstruments::execute()");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamInstrument* LoadInstruments::loadInstrument(AString filename)
{
	aLog("SamCommandLoadInstruments::loadInstrument(%s)", filename.toChar());

	AFile file(filename);

	if(!file.open()) {
		return 0;
	}

	char *data = (char*)malloc(file.size()+1);
	file.readBytes(file.size(), data);
	data[file.size()] = 0;

	file.close();

	AXmlDocument doc;
	doc.setSource(data);
	if(!doc.rootElement()) {
		aLog("!doc.element()");
		return 0;
	}

	if(!doc.rootElement()->hasAttribute("uuid")) {
		aLog("Instrument UUID cannot be read.");
		return 0;
	}

	setProgressLabel(0, AString("Loading ")+doc.rootElement()->attribute("name"));

	aLog("Valid Instrument file.");

	SamInstrument *instrument = samEngine->instrumentFromUuid(doc.rootElement()->attribute("uuid"));
	if(instrument) {
		return instrument;
	}

	instrument = new SamInstrument(samEngine);
	instrument->setFilename(filename);
	if(!instrument->readElement(&doc, doc.rootElement(), this)) {
		aLog("Instrument file could not be read.");
		delete instrument;
		return 0;
	}
	aLog("Instrument read ok.");

	samEngine->addInstrument(instrument);
	aLog("Instrument appended.");

	//samGui->addRecentInstrument(filename);

	

	return instrument;
}

SamInstrument* LoadInstruments::importExsInstrument(AString filename)
{
	aLog("SamInstrumentManager::importExsInstrument(%s)", filename.toChar());

	ExsInstrument *exs = new ExsInstrument(filename);
	SamInstrument *instrument = 0;

	if(exs->read()) {
		// Find the sample path
		//int pathEndPos = filename.findCharacter('\\', AString::fromBack);
		//AString path = filename.first(pathEndPos+1);
		AString path = filename.path();
		aLog("path = %s", path.toChar());

		instrument = new SamInstrument(samEngine);
		instrument->create(2, false, true, path);

		aLog("*** exs->name()=[%s]", exs->name());
		instrument->setName(exs->name());
		instrument->version(0)->setName(exs->name());
		instrument->setUuid(filename); // This could be useful to see if the same instrument is imported twice
		//instrument->elementPool()->setInstrumentPath("S:/AIR3 Drum Kit Original/");

		for(int i=0; i<exs->numAudioFiles(); i++) {
			aLog("importing... [%s]", exs->audioFile(i)->filename);

			/*AString af = elementElement->attribute("audioFile"); //aLog("1");
			int pos = af.findCharacter(L'//', AString::fromBack); //aLog("2");
			if(pos > -1) { //aLog("3");
				af = af.last(af.length()-pos-1);
			}*/
			setProgressLabel(1, exs->audioFile(i)->filename);

			SamElement *element = instrument->elementPool()->createElement(exs->audioFile(i)->filename, true);
			if(!element) {
				aLog("An error occurred with this element.");
			}

			setProgress((float)i/(float)exs->numAudioFiles());
		}

		SamLayer *layer = instrument->version(0)->articulation(0)->layer(0);

		for(int i=0; i<exs->numZones(); i++) {
			SamZone *zone = new SamZone(layer);

			SamCell *cell = new SamCell(zone);
			cell->addElement(exs->zone(i)->audioFileId,-1);
			cell->setNoteBase(exs->zone(i)->keyNote);
			cell->setFineTune(exs->zone(i)->tuneCents);
			zone->addCell(cell);

			zone->setMidiNoteLow(exs->zone(i)->zoneRangeLo);
			zone->setMidiNoteHigh(exs->zone(i)->zoneRangeHi);

			/*if(exs->zone(i)->useZoneVelocityRange) {
				aLog("Using EXS Zone velocity range");
				zone->setMidiVelocityLow(exs->zone(i)->zoneVelocityRangeLo);
				zone->setMidiVelocityHigh(exs->zone(i)->zoneVelocityRangeHi);
			} else*/
			if(exs->zone(i)->group>-1) {
				aLog("Using EXS Group velocity range");
				if(exs->zone(i)->group < exs->numGroups()) {
					ExsGroup* exsGroup = exs->group(exs->zone(i)->group);
					zone->setMidiVelocityLow(exsGroup->selectRangeLo);
					zone->setMidiVelocityHigh(exsGroup->selectRangeHi);
				} else {
					aLog("Group could not be found.");
				}
			} else {
				aLog("Using EXS Zone velocity range");
				zone->setMidiVelocityLow(exs->zone(i)->zoneVelocityRangeLo);
				zone->setMidiVelocityHigh(exs->zone(i)->zoneVelocityRangeHi);
			}

			layer->addZone(zone);
		}

		samEngine->addInstrument(instrument);

		//sam->setPendingInstrument(instrument);
		
		/*for(int i=0; i<exs->numAudioFiles(); i++) {
			//CsString filename = CsString("D:\\PianoTest\\") + exs->audioFile(i)->filename;
			AString filename(exs->audioFile(i)->filename);
			int pathEndPos = filename.findCharacter('\\', CsString::fromBack);
			AString path = filename.first(pathEndPos+1);
			//CsString filename = CsString("D:\\Triton\\000 Noisy Stabber\\") + exs->audioFile(i)->filename;
			AString audioFilename = path + exs->audioFile(i)->filename;
			aLog("importing... [%s]", audioFilename.toChar());
			XeSample *sample = new XeSample();
			sample->importAudioFile(audioFilename);
			m_samples.append(sample);
		}

		for(int i=0; i<exs->numGroups(); i++) {
			XeGroup *group = new XeGroup();
			//group->setName(exs->group(i)->name);
			m_groups.append(group);
		}

		for(int i=0; i<exs->numZones(); i++) {
			XeZone *zone = new XeZone();
			//aLog("zone[%d] afid=%d, lo=%d, hi=%d", i, exs->zone(i)->audioFileId, exs->zone(i)->zoneRangeLo, exs->zone(i)->zoneRangeHi);
			zone->setSampleId(exs->zone(i)->audioFileId);
			zone->setRootNote(exs->zone(i)->keyNote);
			zone->setMidiNoteLo(exs->zone(i)->zoneRangeLo);
			zone->setMidiNoteHi(exs->zone(i)->zoneRangeHi);
			if(exs->zone(i)->useZoneVelocityRange) {
				zone->setMidiVelocityLo(exs->zone(i)->zoneVelocityRangeLo);
				zone->setMidiVelocityHi(exs->zone(i)->zoneVelocityRangeHi);
			} else if(exs->zone(i)->group>-1) {
				if(exs->zone(i)->group < exs->numGroups()) {
					ExsGroup* exsGroup = exs->group(exs->zone(i)->group);
					zone->setMidiVelocityLo(exsGroup->selectRangeLo);
					zone->setMidiVelocityHi(exsGroup->selectRangeHi);
				}
			}
			zone->setFineTune(exs->zone(i)->tuneCents);
			m_zones.append(zone);
		}*/

	} else {
		aLog("Can't read EXS file.");
	}

	delete exs;

	//samGui->addRecentInstrument(filename);

	return instrument;
}

SamInstrument* LoadInstruments::importSfzInstrument(AString filename)
{
	aLog("LoadInstruments::importSfzInstrument(%s)", filename.toChar());

	Sfz::Instrument *sfz = new Sfz::Instrument(filename);
	SamInstrument *instrument = 0;

	if(sfz->read()) {
		AString path = filename.path();
		aLog("path = %s", path.toChar());

		instrument = new SamInstrument(samEngine);
		instrument->create(2, false, true, path);

		SamLayer *layer = instrument->version(0)->articulation(0)->layer(0);

		aLog("numRegions = %d", sfz->numRegions());
		for(int i=0; i<sfz->numRegions(); i++) {
			aLog("Region %d/%d", i+1, sfz->numRegions());

			Sfz::Region *r = sfz->region(i);

			AString sfn;
			if(sfz->default_path.length() > 0) {
				sfn.append(sfz->default_path);
			}
			sfn.append(r->sample);

			aLog("importing... [%s]", sfn.toChar());

			
			setProgressLabel(1, sfn);

			SamElement *element = instrument->elementPool()->createElement(sfn, true);
			if(!element) {
				aLog("An error occurred with this element.");
				setMessage(AString("The SFZ file could not be imported because the following file could not be loaded:\n")+sfn);
				delete instrument;
				delete sfz;
				return 0;
			}

			SamZone *zone = new SamZone(layer);

			SamCell *cell = new SamCell(zone);
			cell->addElement(instrument->elementPool()->numElements()-1, -1);
			cell->setNoteBase(r->pitch_keycenter);
			cell->setFineTune(r->tune);
			//cell->setLoopStart(element->loopStart());
			//cell->setLoopEnd(element->loopEnd());
			//if(cell->loopEnd() > cell->loopStart()) {
			//	cell->setLoop(true);
			//}

			zone->addCell(cell);

			zone->setMidiNoteLow(r->lokey);
			zone->setMidiNoteHigh(r->hikey);
			zone->setMidiVelocityLow(r->lovel);
			zone->setMidiVelocityHigh(r->hivel);

			layer->addZone(zone);

			setProgress((float)i/(float)sfz->numRegions());
		}

		//aLog("*** exs->name()=[%s]", exs->name());
		//instrument->setName(exs->name());
		//instrument->version(0)->setName(exs->name());
		//instrument->setUuid(filename);
	}

	delete sfz;

	return instrument;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
SaveInstrument::SaveInstrument(SamInstrument *instrument, AString filename, bool newUuid)
: m_filename(filename)
, m_instrument(instrument)
, m_newUuid(newUuid)
{
	setName("Save Instrument");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SaveInstrument::execute(ACommandManager *commandManager, int id)
{
	ACommand::execute(commandManager, id);

	AXmlDocument doc;

	AXmlElement *element = doc.createElement("Instrument");
	doc.setRootElement(element);
	if(m_newUuid) {
		m_instrument->recreateUuid();
	}

	m_instrument->setFilename(m_filename);
	m_instrument->writeElement(&doc, element, this);

	AString str = doc.toString();
	//aDebug("[%s]", str.toChar());

	AFile file(m_filename);

	if(!file.create()) {
		return;
	}

	file.writeBytes(str.length(), str.toChar());
	file.close();

	//m_instrument->setFilename(m_filename);

	//samGui->addRecentProject(m_filename);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
ReplaceInstrument::ReplaceInstrument(SamInstrument *instrumentToReplace, AString withFile)
: m_instrumentToReplace(instrumentToReplace)
, m_withFile(withFile)
{
	setName("Replace Instrument");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void ReplaceInstrument::execute(ACommandManager *commandManager, int id)
{
	aLog("---------- ReplaceInstrument::execute() ----------");

	
	//Sleep(1000);
	//return;

	SamInstrument *dummy = new SamInstrument(samEngine);
	dummy->create(2, false, true, "");

	//AList<SamChannel*> channels;
	for(int i=0; i<samEngine->project()->numChannels(); i++) {
		SamChannel *channel = samEngine->project()->channel(i);
		if(channel->instrument() == m_instrumentToReplace) {
			aLog("Channel %d needs replacing", i+1);
			//channels.append(channel);
			samEngine->executeRealTimeCommand(new RtcAssignInstrument(channel, dummy)); // Don't reassign to 0; reassign to a dummy instrument! :-)
		}
	}

	//samEngine->project()->deselectChannels();

	Sleep(1000);

	bool removeInstrument = true;
	for(int i=0; i<samEngine->project()->numChannels(); i++) {
		if(samEngine->project()->channel(i)->instrument() == m_instrumentToReplace) {
			removeInstrument = false;
			break;
		}
	}

	if(removeInstrument) {
		aLog("Instrument could be removed.");
		samEngine->removeInstrument(m_instrumentToReplace);
	} else aLog("!!! Instrument could not be removed");

	//Load the new Instrument
	AList<AString> *files = new AList<AString>;
	files->append(m_withFile);
	samGui->commandManager()->execute(new LoadInstruments(files, 1, false));

	SamInstrument *newInstrument = 0;
	for(int i=0; i<samEngine->numInstruments(); i++) {
		if(samEngine->instrument(i)->filename() == m_withFile) {
			newInstrument = samEngine->instrument(i);
			aLog("YES we found a new Instrument");
			break;
		}
	}
	//samEngine->project()->commandManager()->
	


	if(!newInstrument) {
		return;
	}

	for(int i=0; i<samEngine->project()->numChannels(); i++) {
		SamChannel *channel = samEngine->project()->channel(i);
		if(channel->instrument() == dummy) {
			aLog("Channel %d needs replacing", i+1);
			
			samEngine->executeRealTimeCommand(new RtcAssignInstrument(channel, newInstrument)); // Don't reassign to 0; reassign to a dummy instrument! :-)
		}
	}

	//delete dummy;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
NewProject::NewProject()
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void NewProject::execute(ACommandManager *commandManager, int id)
{
	samEngine->project()->clear();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
SaveProject::SaveProject(AString filename)
: m_filename(filename)
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SaveProject::execute(ACommandManager *commandManager, int id)
{
	ACommand::execute(commandManager, id);

	AXmlDocument doc;

	AXmlElement *element = doc.createElement("Project");
	doc.setRootElement(element);

	AXmlElement *elementInstruments = doc.createElement("Instruments");
	for(int i=0; i<samEngine->numInstruments(); i++) {
		AXmlElement *elementInstrument = doc.createElement("Instrument");
		elementInstrument->setAttribute("name", samEngine->instrument(i)->name());
		elementInstrument->setAttribute("uuid", samEngine->instrument(i)->uuid());
		elementInstrument->setAttribute("filename", samEngine->instrument(i)->filename());
		elementInstruments->addChildElement(elementInstrument);
	}
	element->addChildElement(elementInstruments);

	samEngine->project()->writeElement(&doc, element);

	AString str = doc.toString();
	//aDebug("[%s]", str.toChar());

	AFile file(m_filename);

	if(!file.create()) {
		return;
	}

	file.writeBytes(str.length(), str.toChar());
	file.close();

	//samGui->setCurrentProjectFilename(m_filename);
	//samGui->addRecentProject(m_filename);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
OpenProject::OpenProject(AString filename)
: m_filename(filename)
{
	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void OpenProject::execute(ACommandManager *commandManager, int id)
{
	ACommand::execute(commandManager, id);

	aLog("OpenProject::execute()");

	samEngine->project()->clear();

	while(samEngine->project()->numChannels() > 0 && samEngine->project()->numAudioHardwareBuses() > 0) {
		Sleep(1);
	}

	AFile file(m_filename);

	if(!file.open()) {
		return;
	}

	char *data = (char*)malloc(file.size()+1);
	file.readBytes(file.size(), data);
	data[file.size()] = 0;

	file.close();

	AXmlDocument doc;
	doc.setSource(data);
	if(doc.rootElement()) {
		if(doc.rootElement()->tag() == "Project") {
			
			//setProgressLabel(0, AString("Loading ")+doc.rootElement()->attribute("name"));
			samEngine->project()->readElement(&doc, doc.rootElement(), this);
			
		} else {
			aLog("Project tag cannot be read.");
		}
	} else {
		aLog("!doc.element()");
	}

	free(data);

	//samGui->setCurrentProjectFilename(m_filename);
	//samGui->addRecentProject(m_filename);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
BuildDatabase::BuildDatabase()
{
	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void BuildDatabase::execute(ACommandManager *commandManager, int id)
{
	QDir dir("S:\\Sam Instruments\\First Instrument");

	//int c;
	//countDirectory(dir, c);
	//qDebug("count = %d", c);
	
	parseDirectory(dir);
}

void BuildDatabase::parseDirectory(QDir dir)
{
	//dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	dir.setSorting(QDir::Name);// | QDir::Reversed);

	QFileInfoList list = dir.entryInfoList();
    
	for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
		qDebug("%s", qPrintable(fileInfo.fileName()));
		qDebug("%s", qPrintable(fileInfo.absoluteFilePath()));

		if(fileInfo.isDir() && fileInfo.fileName()!="." && fileInfo.fileName()!="..") {
			parseDirectory(QDir(fileInfo.absoluteFilePath()));
		} else if(fileInfo.fileName().contains(".sam")) {
			QByteArray baName = fileInfo.fileName().toAscii();
			QByteArray baFilename = fileInfo.absoluteFilePath().toAscii();
			samEngine->database()->addEntry(new SamDatabaseEntry("0", baName.data(), baFilename.data()));
		}

	}
}

void BuildDatabase::countDirectory(QDir dir, int &count)
{
	QFileInfoList list = dir.entryInfoList();
    
	for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
		//qDebug("%s", qPrintable(fileInfo.fileName()));
		//qDebug("%s", qPrintable(fileInfo.absoluteFilePath()));

		count++;
		if(fileInfo.isDir() && fileInfo.fileName()!="." && fileInfo.fileName()!="..") {
			countDirectory(QDir(fileInfo.absoluteFilePath()), count);
		}

	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
SetInstrumentStreamMix::SetInstrumentStreamMix(SamChannel *channel, bool saveInstrument)
: m_channel(channel)
, m_saveInstrument(saveInstrument)
{
	//setFeedback(false);
}

void SetInstrumentStreamMix::execute(ACommandManager *commandManager, int id)
{
	m_channel->instrument()->streamMix(0)->setStreamMix(m_channel->streamMix());

	if(m_saveInstrument) {
		//m_channel->instrument()->
	}

	setMessageType(1);
	setMessage("The Instrument's Stream Mix was updated.");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
SetStreamMixPreset::SetStreamMixPreset(SamChannel *channel, int presetIndex)
: m_channel(channel)
, m_presetIndex(presetIndex)
{
}

void SetStreamMixPreset::execute(ACommandManager *commandManager, int id)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
CopyLayerFromInstrument::CopyLayerFromInstrument(SamInstrument *src, SamInstrument *dst)
: m_srcInstrument(src)
, m_dstInstrument(dst)
{
}

void CopyLayerFromInstrument::execute(ACommandManager *commandManager , int id)
{
	ACommand::execute(commandManager, id);

	aLog("CopyLayerFromInstrument::execute()");

	setProgressLabel(0, "Copying Layer from Instrument...");

	//SamInstrument *instrument = samEngine->project()->instrument(selectInstrument->selectedItem());

	if(!m_srcInstrument || !m_dstInstrument) {
		aLog("Invalid instrument");
		return;
	}

	//SamLayer *dest = m_instrument->version(0)->layer(0);//old
	SamLayer *src = m_srcInstrument->version(0)->articulation(0)->layer(0);

	// Create a new Group in the destination Layer
	//int numGroups = dest->numGroups();//old

	//dest->createGroup(numGroups, src->group(0)->name());

	SamElementPool *destPool = m_dstInstrument->elementPool();
	SamElementPool *srcPool = m_srcInstrument->elementPool();

	// Transfer the Elements
	int numOriginalElements = destPool->numElements();
	int numNewElements = srcPool->numElements();

	for(int i=0; i<numNewElements; i++) {
		SamElement *element = destPool->createElement(srcPool->element(i)->filename(), true);
		if(!element) {
			//QMessageBox::information(this, "Sam", "Merge failed while trying to copy Elements.");
			return;
		}
		element->setSet(srcPool->element(i)->set());
		setProgress((float)i/(float)numNewElements);
	}

	// Transfer the Layer
	SamLayer *newLayer = new SamLayer(m_dstInstrument->version(0)->articulation(0), *src, numOriginalElements);
	m_dstInstrument->version(0)->articulation(0)->addLayer(newLayer);

	samGui->signalEditorRefresh();

	//QMessageBox::information(this, "Sam", "Merge complete.");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
MergeLoadedInstruments::MergeLoadedInstruments()
{
}

void MergeLoadedInstruments::execute(ACommandManager *commandManager, int id)
{
	ACommand::execute(commandManager, id);

	aLog("Sam::debugMergeLoadedInstruments()");

	setProgressLabel(0, "Merging loaded Instruments...");

	int numInstruments = samEngine->numInstruments();

	if(numInstruments<2) {
		aLog("You need at least two instruments loaded for this command to work.");
		return;
	}

	SamInstrument *dstInstrument = samEngine->instrument(0);

	for(int i=1; i<numInstruments; i++) {
		SamInstrument *srcInstrument = samEngine->instrument(i);
		aLog("++++++++++++++++++++ Start Instrument (%s) ++++++++++++++++++++", srcInstrument->name().toChar());
		

		if(!srcInstrument) {
			aLog("Invalid instrument");
			return;
		}

		SamArticulation *articulation = srcInstrument->version(0)->articulation(0);

		if(!articulation) {
			aLog("Invalid articulation");
			return;
		}


		SamElementPool *destPool = dstInstrument->elementPool();
		SamElementPool *srcPool = srcInstrument->elementPool();

		// Transfer the Elements
		int numOriginalElements = destPool->numElements();
		int numNewElements = srcPool->numElements();

		for(int j=0; j<numNewElements; j++) {
			SamElement *element = destPool->createElement(srcPool->element(j)->filename(), false);
			if(!element) {
				QMessageBox::information(0, "Sam", "Merge failed while trying to copy Elements.");
				return;
			}
			element->setSet(srcPool->element(j)->set());
			setProgress((float)j/(float)numNewElements);
		}

		// Transfer the Articulation
		SamArticulation *newArticulation = new SamArticulation(dstInstrument->version(0), *articulation, numOriginalElements, false);
		//newArticulation->setId(m_instrument->version(0)->numArticulations());
		dstInstrument->version(0)->addArticulation(newArticulation);
		aLog("++++++++++++++++++++ End Instrument (%d) ++++++++++++++++++++", i);
	}

	dstInstrument->version(0)->setName("**Merged**");

	//QMessageBox::information(this, "Sam", "Merge Articulation complete.");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------

ExportArticulationAsInstrument::ExportArticulationAsInstrument(SamInstrument *instrument, SamArticulation *articulation, AString filename, bool xmlOnly)
	: m_instrument(instrument)
	, m_articulation(articulation)
	, m_filename(filename)
	, m_xmlOnly(xmlOnly)
{
}

void ExportArticulationAsInstrument::execute(ACommandManager *commandManager, int id)
{
	ACommand::execute(commandManager, id);

	setProgressLabel(0, "Exporting Articulation as Instrument...");

	// Create a directory for the sample data
	// --------------------------------------
	QFileInfo fi(m_filename.toChar());
	QString filename = fi.fileName();
	qDebug("filename = %s", qPrintable(filename));
	if(filename.contains(".sam")) {
		filename.remove(".sam");
	}
	qDebug("filename = %s", qPrintable(filename));
	QDir dir(fi.path());

	if(!m_xmlOnly) {
		QDir dir2(fi.path()+"/"+filename);
		if(dir2.exists()) {
			qDebug("Directory exists.");
			setMessage("<b>The folder for the sample data could not be created.</b><br>This is because a folder with the name you entered for<br>the instrument already exists.<br><br>Try again with a different name for the Sam Instrument file.");
			return;
		}
		dir.mkdir(filename);
	}

	QString path = fi.path()+"/";
	QByteArray baPath = path.toLatin1();

	// Create Instrument
	// -----------------
	SamInstrument *instrument = new SamInstrument(samEngine);
	instrument->createFromStreamMix(baPath.data(), m_instrument->elementPool(), m_instrument->streamMix(0));

	// Version
	SamVersion *version = new SamVersion(instrument, *m_instrument->version(), true);
	instrument->addVersion(version);

	// Articulation
	SamArticulation *articulation = new SamArticulation(version, *m_articulation, 0, true);
	version->addArticulation(articulation);

	// Layers
	for(int i=0; i<m_articulation->numLayers(); i++) { aLog("Layer %d of %d", i, m_articulation->numLayers());

		SamLayer *originalLayer = m_articulation->layer(i);
		SamLayer *layer = new SamLayer(m_articulation, *originalLayer, 0, true);
		articulation->addLayer(layer);

		// Zones
		for(int j=0; j<originalLayer->numZones(); j++) { aLog("Zone %d of %d", j, originalLayer->numZones());

			SamZone *originalZone = originalLayer->zone(j);
			SamZone *zone = new SamZone(layer, *originalZone, 0, true);
			layer->addZone(zone);

			// Cells
			for(int k=0; k<originalZone->numCells(); k++) { aLog("Cell %d of %d", k, originalZone->numCells());

				SamCell *originalCell = originalZone->cell(k);
				SamCell *cell = new SamCell(zone, *originalCell, 0, true);
				zone->addCell(cell);

				// Elements
				for(int l=0; l<originalCell->numElements(); l++) { aLog("Element %d of %d", l, originalCell->numElements());

					//qDebug("Element %d of %d", instrument->elementPool()->numElements(), m_instrument->elementPool()->numElements());

					SamElement *originalElement = originalCell->element(l);
					//SamElement *element = new SamElement(cell, baPath.data(), originalElement->filename());

					QString pathless = QString(originalElement->filename().toChar());

					int lio = pathless.lastIndexOf("/");
					if(lio > -1) {
						qDebug("Fixing path");
						pathless = pathless.right(pathless.length()-lio-1);
					}

					QString afn = QString(fi.path()+"/"+filename+"/"+pathless);
					QByteArray ba = afn.toLatin1();

					QString sfn = QString(originalElement->path().toChar()) + QString(originalElement->filename().toChar());
					
					//afn.replace("/", "\\");
					//sfn.replace("/", "\\");
					
					if(!m_xmlOnly) {
						qDebug("Copying file [%s] to [%s]", qPrintable(sfn), qPrintable(afn));
						if(!QFile::copy(sfn, afn)) {
							qDebug("Copy fail");
						} else {
							qDebug("Copy ok");
						}
					}

					int srcId = -1;
					int relId = -1;

					srcId = instrument->elementPool()->numElements();

					QString sefn = QString(filename+"/"+ pathless);
					if(m_xmlOnly) {
						sefn = QString(originalElement->filename().toChar());
					}
					QByteArray bas = sefn.toLatin1();
					SamElement *elementSrc = instrument->elementPool()->createElement(bas.data(), false);
					elementSrc->setSet(originalElement->set());

					// Release (if there is one)
					if(originalCell->hasRelease() && !originalCell->isSourceRelease()) {
						SamElement *originalElementRelease = originalCell->elementRelease(l);

						pathless = QString(originalElementRelease->filename().toChar());

						lio = pathless.lastIndexOf("/");
						if(lio > -1) {
							qDebug("Fixing path");
							pathless = pathless.right(pathless.length()-lio-1);
						}
						QString rfn = QString(fi.path() + "/" + filename + "/" + pathless); //QString(originalElementRelease->filename().toChar()));

						

						//QByteArray rba = rfn.toLatin1();

						QString rsfn = QString(originalElementRelease->path().toChar()) + QString(originalElementRelease->filename().toChar());
						
						//rfn.replace("/", "\\");
						//rsfn.replace("/", "\\");
						
						if(!m_xmlOnly) {
							aLog("Copying file [%s] to [%s]", qPrintable(sfn), qPrintable(afn));
							if(!QFile::copy(rsfn, rfn)) {
								qDebug("Copy fail");
							} else {
								qDebug("Copy ok");
							}
						}

						relId = instrument->elementPool()->numElements();
						
						QString rsefn = QString(filename+"/"+ pathless);
						if(m_xmlOnly) {
							rsefn = QString(originalElementRelease->filename().toChar());
						}
						QByteArray rbas = rsefn.toLatin1();

						SamElement *elementRel = instrument->elementPool()->createElement(rbas.data(), false);
						elementRel->setSet(originalElementRelease->set());

					} else if(originalCell->hasRelease() && originalCell->isSourceRelease()) {
						relId = srcId;
					}

					if(!cell->addElement(srcId, relId)) {
						qDebug("BIG FUCKING PROBLEM!");
						return;
					}

					setProgress((float)instrument->elementPool()->numElements() / (float)m_instrument->elementPool()->numElements());
				}

			}
		}
	}

	aLog("Done and saving...");
	commandManager->execute(new SaveInstrument(instrument, m_filename, true));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------

ExportElementPool::ExportElementPool(SamInstrument *instrument, SamArticulation *articulation, AString path)
	: m_instrument(instrument)
	, m_articulation(articulation)
	, m_path(path)
{
}

void ExportElementPool::execute(ACommandManager *commandManager, int id)
{
	ACommand::execute(commandManager, id);

	setProgressLabel(0, "Exporting Element Pool...");

	// Create a directory for the sample data
	// --------------------------------------
	QFileInfo fi(m_path.toChar());
	QString filename = fi.fileName();
	qDebug("filename = %s", qPrintable(filename));
	//if(filename.contains(".sam")) {
	//	filename.remove(".sam");
	//}
	qDebug("filename = %s", qPrintable(filename));
	QDir dir(fi.path());

	QDir dir2(fi.path()+"/"+filename);
	if(dir2.exists()) {
		qDebug("Directory exists.");
		setMessage("<b>The folder for the sample data could not be created.</b><br>This is because a folder with the name you entered for already exists.<br><br>Try again with a different name for the Sam Instrument file.");
		return;
	}
	dir.mkdir(filename);

	QString path = fi.path()+"/";
	QByteArray baPath = path.toLatin1();

	// Parse the Instrument
	// --------------------

	int ec = 0;

	// Layers
	for(int i=0; i<m_articulation->numLayers(); i++) { aLog("Layer %d of %d", i, m_articulation->numLayers());
		SamLayer *layer = m_articulation->layer(i);

		// Zones
		for(int j=0; j<layer->numZones(); j++) { aLog("Zone %d of %d", j, layer->numZones());
			SamZone *zone = layer->zone(j);

			// Cells
			for(int k=0; k<zone->numCells(); k++) { aLog("Cell %d of %d", k, zone->numCells());
				SamCell *cell = zone->cell(k);

				// Elements
				for(int l=0; l<cell->numElements(); l++) { aLog("Element %d of %d", l, cell->numElements());
					SamElement *element = cell->element(l);

					// Create the filename;
					// Format: name-rootpitch-notelo-notehi-vello-velhi(-rr-speaker).wav
					QString builtname = QString(layer->name().toChar()) +
						                "-" +
										QString::number(cell->noteBase()) +
										"-" +
										QString::number(zone->midiNoteLow()) +
										"-" +
										QString::number(zone->midiNoteHigh()) +
										"-" +
										QString::number(zone->midiVelocityLow()) +
										"-" +
										QString::number(zone->midiVelocityHigh());

					//if(zone->numCells() > 1) { // Round robin
						builtname += ("-"+QString::number(k));
					//}

					if(m_instrument->elementPool()->numSets() > 1) { // Multiple speakers
						builtname += ("-"+QString::number(element->set()));
					}

					builtname += ".wav";

					QString afn = QString(fi.path()+"/"+filename+"/"+builtname);
					qDebug("%s", qPrintable(afn));
					QByteArray ba = afn.toLatin1();


					if(!writeElementToFile(element, cell, ba.data())) {
						return;
					}

					if(cell->hasRelease() && !cell->isSourceRelease()) {
						SamElement *elementRelease = cell->elementRelease(l);

						QString builtnamer = QString(layer->name().toChar()) +
						                "-" +
										QString::number(cell->noteBase()) +
										"-" +
										QString::number(zone->midiNoteLow()) +
										"-" +
										QString::number(zone->midiNoteHigh()) +
										"-" +
										QString::number(zone->midiVelocityLow()) +
										"-" +
										QString::number(zone->midiVelocityHigh());

						//if(zone->numCells() > 1) { // Round robin
							builtnamer += ("-"+QString::number(k));
						//}

						if(m_instrument->elementPool()->numSets() > 1) { // Multiple speakers
							builtnamer += ("-"+QString::number(element->set()));
						}

						builtnamer += "-r";

						builtnamer += ".wav";

						QString afnr = QString(fi.path()+"/"+filename+"/"+builtnamer);
						qDebug("%s", qPrintable(afnr));
						QByteArray bar = afnr.toLatin1();

						if(!writeElementToFile(elementRelease, cell, bar.data())) {
							return;
						}
					}

					setProgress((float)ec++ / (float)m_instrument->elementPool()->numElements());


					
				}

			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int ExportElementPool::writeElementToFile(SamElement *element, SamCell *cell, AString filename)
{
	// Copy the Element to an Audio Buffer
	CsAudioBuffer elementAudioBuffer(element->numStreams(), element->numBlocks() * SamElementBlock::size);

	float *decompressionBuffer[8];

	for(int i=0; i<8; i++) {
		decompressionBuffer[i] = (float*)aMalloc((sizeof(float)*SamElementBlock::size));
	}

	int sp = 0; // Sample pointer

	for(int i=0; i<element->numBlocks(); i++) {
		for(int j=0; j<element->numStreams(); j++) {

			float *outputStream = elementAudioBuffer.stream(j);

			element->decompressBlock(element->block(j,i), decompressionBuffer[j]);

			for(int k=0; k<SamElementBlock::size; k++) {
				outputStream[sp+k] = decompressionBuffer[j][k];	
			}
		}
		sp += SamElementBlock::size;
	}

	// Now write the Audio Buffer as an audio file
	CsAudioFile file(filename);

	if(!file.create()) {
		aLog("Error creating audio file.");	
		return 0;
	}

	file.setFormat(CsAudioFile::Wave);
	file.setChannels(elementAudioBuffer.numStreams());
	file.setBitDepth(24);
	file.setSampleRate(44100.0);
	file.setLengthFrames(elementAudioBuffer.numFrames());

	file.setMidiUnityNote(cell->noteBase());
	file.setMidiPitchFraction(((double)UINT_MAX * (cell->fineTune() / 100.0)));//(int)cell->fineTune());
	if(cell->isLoop()) {
		file.setLoopStart(cell->loopStart());
		file.setLoopEnd(cell->loopEnd());
	}

	file.writeHeader();
	file.writeAudioBuffer(&elementAudioBuffer);
	file.close();

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
MoveCopyZones::MoveCopyZones(SamLayer *layer, int semitones, bool transposeRootNote, bool copy)
: m_layer(layer)
, m_semitones(semitones)
, m_transposeRootNote(transposeRootNote)
, m_copy(copy)
{
}

void MoveCopyZones::execute(ACommandManager *commandManager, int id)
{
	ACommand::execute(commandManager, id);

	if(m_semitones == 0) {
		setMessage("Semitones needs to be set to a value higher or lower than 0 for the Zones to be moved or copied.");
		return;
	}

	AList<SamZone*> newZones;

	for(int i=0; i<m_layer->numZones(); i++) {
		if(m_layer->zone(i)->isSelected()) {
			SamZone *zone = m_layer->zone(i);

			if(m_copy) {
				SamZone *z = new SamZone(m_layer, *zone);
				newZones.append(z);
			}

			zone->setMidiNoteLow(zone->midiNoteLow() + m_semitones);
			zone->setMidiNoteHigh(zone->midiNoteHigh() + m_semitones);
			if(m_transposeRootNote) {
				for(int j=0; j<zone->numCells(); j++) {
					SamCell *cell = zone->cell(j);
					cell->setNoteBase(cell->noteBase() + m_semitones);
				}
			}
			zone->setCreatedByCopy();

			
		}
	}

	if(m_copy) {
		for(int i=0; i<newZones.size(); i++) {
			m_layer->addZone(newZones[i], true);
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
PasteZoneSettings::PasteZoneSettings(SamZone *zone, char *settings, SamLayer *layer)
: m_zone(zone)
, m_layer(layer)
{
	memcpy(m_settings, settings, 11);
}

void PasteZoneSettings::execute(ACommandManager *commandManager, int id)
{
	ACommand::execute(commandManager, id);

	for(int i=0; i<m_layer->numZones(); i++) {
		SamZone *z = m_layer->zone(i);
		
		if(z->isSelected()) {
			if(m_settings[ZoneSampleStart]) {
				z->setSampleStart(m_zone->sampleStart());
			}
			if(m_settings[LevelsAndOutputs]) {
				z->setGain              (m_zone->gain());
				z->setReleaseGain       (m_zone->releaseGain());
				z->setPreLoopReleaseGain(m_zone->preLoopReleaseGain());
			}
			if(m_settings[Pitch]) {
				//z->setMidiNoteLow(m_zone->midiNoteLow());
				//z->setMidiNoteHigh(m_zone->midiNoteHigh());

				z->setTrackPitch(m_zone->isTrackPitch());
				z->setFineTune  (m_zone->fineTune());
			}
			if(m_settings[Velocity]) {
				//z->setMidiVelocityLow(m_zone->midiVelocityLow());
				//z->setMidiVelocityHigh(m_zone->midiVelocityHigh());
				z->setPredictableVelocity         (m_zone->isPredictableVelocity());
				z->setPredictableVelocityCellIndex(m_zone->predictableVelocityCellIndex());
				z->setVelocitySampleStart         (m_zone->velocitySampleStart());
				z->setVelocitySampleStartThreshold(m_zone->velocitySampleStartThreshold());
			}
			if(m_settings[Triggering]) {
				z->setOneShot     (m_zone->isOneShot());
				z->setTriggerMode (m_zone->triggerMode());
				z->setSingleCellId(m_zone->singleCellId());
			}
			if(m_settings[Filter]) {
				z->setFilterEnabled(m_zone->isFilterEnabled());
				z->setCutOff       (m_zone->cutOff());
				z->setResonance    (m_zone->resonance());

				z->setFilterPitchTracking   (m_zone->isFilterPitchTracking());
				z->setFilterPitchTrackOffset(m_zone->filterPitchTrackOffset());

				z->setFilterVelocityScaling  (m_zone->isFilterVelocityScaling());
				z->setFilterVelocityDepth    (m_zone->filterVelocityDepth());
				z->setFilterVelocityFullRange(m_zone->isFilterVelocityFullRange());

				z->setMinCutOff(m_zone->minCutOff());
				z->setFilterEnvelope(m_zone->isFilterEnvelope());
				z->setFilterParameter(m_zone->isFilterParameter());
			}
			if(m_settings[Envelope1]) {
				z->envelope(0)->setAttack                 (m_zone->envelope(0)->attack());
				z->envelope(0)->setAttackVelocity         (m_zone->envelope(0)->attackVelocity());
				z->envelope(0)->setAttackVelocityThreshold(m_zone->envelope(0)->attackVelocityThreshold());

				z->setAttackOffset(m_zone->attackOffset());
				z->setAttackOffsetMode(m_zone->attackOffsetMode());

				z->envelope(0)->setAttackShape(m_zone->envelope(0)->attackShape());
				z->envelope(0)->setAttackDepth(m_zone->envelope(0)->attackDepth());

				z->envelope(0)->setHold(m_zone->envelope(0)->hold());

				z->envelope(0)->setDecay(m_zone->envelope(0)->decay());

				z->envelope(0)->setSustain(m_zone->envelope(0)->sustain());

				z->envelope(0)->setRelease     (m_zone->envelope(0)->release());
				z->envelope(0)->setAutoRelease (m_zone->envelope(0)->isAutoRelease());
				z->envelope(0)->setReleaseShape(m_zone->envelope(0)->releaseShape());
				z->envelope(0)->setReleaseDepth(m_zone->envelope(0)->releaseDepth());
			}
			if(m_settings[Envelope2]) {
				z->envelope(1)->setAttack     (m_zone->envelope(1)->attack());
				z->envelope(1)->setAttackShape(m_zone->envelope(1)->attackShape());
				z->envelope(1)->setAttackDepth(m_zone->envelope(1)->attackDepth());

				z->envelope(1)->setHold(m_zone->envelope(1)->hold());

				z->envelope(1)->setDecay(m_zone->envelope(1)->decay());

				z->envelope(1)->setSustain(m_zone->envelope(1)->sustain());

				z->envelope(1)->setRelease     (m_zone->envelope(1)->release());
				z->envelope(1)->setReleaseShape(m_zone->envelope(1)->releaseShape());
				z->envelope(1)->setReleaseDepth(m_zone->envelope(1)->releaseDepth());
			}
			if(m_settings[Envleope3]) {
				z->envelope(2)->setAttack     (m_zone->envelope(2)->attack());
				z->envelope(2)->setAttackShape(m_zone->envelope(2)->attackShape());
				z->envelope(2)->setAttackDepth(m_zone->envelope(2)->attackDepth());

				z->envelope(2)->setHold(m_zone->envelope(2)->hold());

				z->envelope(2)->setDecay(m_zone->envelope(2)->decay());

				z->envelope(2)->setSustain(m_zone->envelope(2)->sustain());

				z->envelope(2)->setRelease     (m_zone->envelope(2)->release());
				z->envelope(2)->setReleaseShape(m_zone->envelope(2)->releaseShape());
				z->envelope(2)->setReleaseDepth(m_zone->envelope(2)->releaseDepth());
			}
			if(m_settings[TempoReleaseModulation]) {
				z->setTempoReleaseAdjustment(m_zone->isTempoReleaseAdjustment());
				z->setTempoThreshold        (m_zone->tempoThreshold());
				z->setTempoReleaseDivisor   (m_zone->tempoReleaseDivisor());
			}
			if(m_settings[Crossfade]) {
				z->setCfInStart (m_zone->cfInStart());
				z->setCfInEnd   (m_zone->cfInEnd());
				z->setCfOutStart(m_zone->cfOutStart());
				z->setCfOutEnd  (m_zone->cfOutEnd());

				z->setCfInCurveDepth (m_zone->cfInCurveDepth());
				z->setCfOutCurveDepth(m_zone->cfOutCurveDepth());
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------

SetSetName::SetSetName(SamSet *set, AString name)
: m_set(set)
, m_name(name)
{
}

void SetSetName::execute(ACommandManager *commandManager, int id)
{
	ACommand::execute(commandManager, id);

	m_set->setName(m_name);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------

AssignSourceToRelease::AssignSourceToRelease(SamLayer * layer)
: m_layer(layer)
{
}

void AssignSourceToRelease::execute(ACommandManager *commandManager, int id)
{
	ACommand::execute(commandManager, id);

	if(!m_layer) return;

	for(int i=0; i<m_layer->numZones(); i++) {
		if(m_layer->zone(i)->isSelected()) {
			SamZone *zone = m_layer->zone(i);
			for(int j=0; j<zone->numCells(); j++) {
				SamCell *cell = zone->cell(j);
				cell->assignSourceToRelease();
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------

UnassignRelease::UnassignRelease(SamLayer * layer)
: m_layer(layer)
{
}

void UnassignRelease::execute(ACommandManager *commandManager, int id)
{
	ACommand::execute(commandManager, id);

	if(!m_layer) return;

	for(int i=0; i<m_layer->numZones(); i++) {
		if(m_layer->zone(i)->isSelected()) {
			SamZone *zone = m_layer->zone(i);
			for(int j=0; j<zone->numCells(); j++) {
				SamCell *cell = zone->cell(j);
				cell->clearReleaseElements();
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------

EnableLoops::EnableLoops(SamLayer * layer, bool enabled)
: m_layer(layer)
, m_enabled(enabled)
{
}

void EnableLoops::execute(ACommandManager *commandManager, int id)
{
	ACommand::execute(commandManager, id);

	if(!m_layer) return;

	for(int i=0; i<m_layer->numZones(); i++) {
		if(m_layer->zone(i)->isSelected()) {
			SamZone *zone = m_layer->zone(i);
			for(int j=0; j<zone->numCells(); j++) {
				SamCell *cell = zone->cell(j);
				cell->setLoop(m_enabled);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------

UpdateLoops::UpdateLoops(SamLayer * layer)
: m_layer(layer)
{
}

void UpdateLoops::execute(ACommandManager *commandManager, int id)
{
	ACommand::execute(commandManager, id);

	if(!m_layer) return;

	for(int i=0; i<m_layer->numZones(); i++) {
		if(m_layer->zone(i)->isSelected()) {
			SamZone *zone = m_layer->zone(i);
			for(int j=0; j<zone->numCells(); j++) {
				SamCell *cell = zone->cell(j);
				if(cell->numElements() > 0) {
					SamElement *element = cell->element(0);
					cell->setLoopStart(element->loopStart());
					cell->setLoopEnd(element->loopEnd());
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
UpdateSetElementStates::UpdateSetElementStates(SamInstrument *instrument, int setId, bool load)
	: m_instrument(instrument)
	, m_setId(setId)
	, m_load(load)
{
	setName("Update Set Element States");
	aLog("UpdateSetElementStates::UpdateSetElementStates(%d, %d)", setId, (int)load);
}

void UpdateSetElementStates::execute(ACommandManager *commandManager, int id)
{
	ACommand::execute(commandManager, id);

	if(m_load) {
		setProgressLabel(0, AString("Loading Set for ") + m_instrument->name());
	} else {
		setProgressLabel(0, AString("Unloading Set for ") + m_instrument->name());	
	}
	setProgressLabel(1, m_instrument->elementPool()->set(m_setId)->name());

	SamElementPool *ep = m_instrument->elementPool();

	float ne = (float)ep->numElements(); 

	//aLog("$$$$ %s ne = %d setid = %d m_load = %d", ((SamInstrument*)ep->parent())->name().toChar(), ep->numElements(), m_setId, (int)m_load);

	for(int i=0; i<ep->numElements(); i++) {
		if(ep->element(i)->set() == m_setId) {
			if(m_load) {
				ep->element(i)->loadAudioData();
			} else {
				ep->element(i)->unloadAudioData();
			}
		}
		setProgress((float)i/ne);
	}

	ep->recalculateSize();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
ImportSampleFolder::ImportSampleFolder(SamLayer *layer, QString path, int numTokens, int tokens[], int numSpeakers)
	: m_layer(layer)
	, m_path(path)
	, m_numTokens(numTokens)
	, m_numSpeakers(numSpeakers)
{
	for(int i=0; i<m_numTokens; i++) {
		m_tokens[i] = tokens[i];
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void ImportSampleFolder::execute(ACommandManager *commandManager, int id)
{
	QDir dir(m_path);
	//QString sampleDirectory = dir.dirName();
	QByteArray baDn = dir.dirName().toLatin1();
	AString sampleDirectory = baDn.data();
	qDebug("sampleDirectory=%s", qPrintable(dir.dirName()));
	dir.cdUp();
	
	QString instrumentPath = dir.path() + "/";
	qDebug("instrumentPath=%s", qPrintable(instrumentPath));
	//editorWidget->instrument()->elementPool()->setInstrumentPath(instrumentPath + "/");
	SamInstrument *instrument = (SamInstrument*)m_layer->parent()->parent()->parent();
	QByteArray baInstrumentPath = instrumentPath.toLatin1();
	instrument->elementPool()->setInstrumentPath(baInstrumentPath.data());

	QList<SimpleCellZoneDefinition> defList;

	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	dir.setSorting(QDir::Name);// | QDir::Reversed);

	dir.setPath(m_path);
    QFileInfoList list = dir.entryInfoList();
    
	//int runningId = editorWidget->instrument()->elementPool()->numElements(); // Was 0
	int runningId = instrument->elementPool()->numElements(); // Was 0


	for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
		if (fileInfo.fileName().contains(".wav")) {
			
				qDebug("%s", qPrintable(fileInfo.fileName()));

				QList<QString> tokens;
				buildList(fileInfo.fileName(), tokens);



				qDebug("tokens.size() = %d", tokens.size());

				if(tokens.size() == m_numTokens) {
					//for(int j=0; j<tokens.size(); j++) {
					//	qDebug("[%s]", qPrintable(tokens[j]));

						
					//}

					SimpleCellZoneDefinition def;
					buildDefinition(tokens, def, m_tokens, m_numSpeakers);

					//def.file = QString(sampleDirectory + "/" + fileInfo.fileName());
					def.file = sampleDirectory;
					def.file.append("/");
					QByteArray baFi = fileInfo.fileName().toLatin1();
					def.file.append(baFi.data());

					defList.append(def);
				}  else {
					qDebug("*** Ignoring sample");
				}
		}
	}

	//instrument->elementPool()->createElement(sampleDirectory + "/" + fileInfo.fileName(), true); // Was already commented

	/*if(mode==0) {
		for(int i=0; i<defList.size(); i++) {
			processDefinition(instrument->elementPool(), layer, defList[i], runningId);
		}
	} else if(mode==1) {
		for(int i=0; i<defList.size(); i++) {
			for(int j=0; j<64; j++) {
				if(defList[i].variation==j) {
					processDefinition(instrument->elementPool(), layer, defList[i], runningId);
				}
			}
		}
	} else if(mode==2) {

		for(int i=0; i<defList.size(); i++) {
			if(defList[i].speaker==0) {
				qDebug("defList[i].speaker==0");
				processDefinition(instrument->elementPool(), layer, defList[i], runningId);
			}
		}

		for(int i=0; i<defList.size(); i++) {
			if(defList[i].speaker==1) {
				qDebug("defList[i].speaker==1");
				processDefinition(editorWidget->instrument()->elementPool(), layer, defList[i], runningId);
			}
		}

	} else if(mode==3) {
		for(int i=0; i<defList.size(); i++) {
			for(int j=0; j<64; j++) {
				if(defList[i].variation==j && defList[i].speaker==0) {
					qDebug("defList[i].variation==j && defList[i].speaker==0");
					processDefinition(editorWidget->instrument()->elementPool(), layer, defList[i], runningId);
				} else {
					qDebug("Ignoring item %d (speaker = %d)", i, defList[i].speaker);
				}
			}
		}
		for(int i=0; i<defList.size(); i++) {
			for(int j=0; j<64; j++) {
				if(defList[i].variation==j && defList[i].speaker==1) {
					qDebug("defList[i].variation==j && defList[i].speaker==1");
					processDefinition(editorWidget->instrument()->elementPool(), layer, defList[i], runningId);
				}
			}
		}
	} else if(mode==4) {

		for(int i=0; i<defList.size(); i++) {
			if(defList[i].speaker==0) {
				qDebug("defList[i].speaker==0");
				processDefinition(editorWidget->instrument()->elementPool(), layer, defList[i], runningId);
			}
		}

		for(int i=0; i<defList.size(); i++) {
			if(defList[i].speaker==1) {
				qDebug("defList[i].speaker==1");
				processDefinition(editorWidget->instrument()->elementPool(), layer, defList[i], runningId);
			}
		}

		for(int i=0; i<defList.size(); i++) {
			if(defList[i].speaker==2) {
				qDebug("defList[i].speaker==2");
				processDefinition(editorWidget->instrument()->elementPool(), layer, defList[i], runningId);
			}
		}
			
	} else if(mode==5) {*/

		for(int i=0; i<defList.size(); i++) {
			for(int j=0; j<64; j++) {
			if(defList[i].variation==j && defList[i].speaker==0) {
				qDebug("defList[i].speaker==0");
				processDefinition(instrument->elementPool(), m_layer, defList[i], runningId);
			}
			}
		}

		for(int i=0; i<defList.size(); i++) {
			for(int j=0; j<64; j++) {
				if(defList[i].variation==j && defList[i].speaker==1) {
					qDebug("defList[i].speaker==1");
					processDefinition(instrument->elementPool(), m_layer, defList[i], runningId);
				}
			}
		}

		for(int i=0; i<defList.size(); i++) {
			for(int j=0; j<64; j++) {
				if(defList[i].variation==j && defList[i].speaker==2) {
					qDebug("defList[i].speaker==2");
					processDefinition(instrument->elementPool(), m_layer, defList[i], runningId);
				}
			}
		}
			
	/*} else if(mode==6) {

		for(int i=0; i<defList.size(); i++) {
			if(defList[i].speaker==0) {
				qDebug("defList[i].speaker==0");
				processDefinition(editorWidget->instrument()->elementPool(), layer, defList[i], runningId);
			}
		}

		for(int i=0; i<defList.size(); i++) {
			if(defList[i].speaker==1) {
				qDebug("defList[i].speaker==1");
				processDefinition(editorWidget->instrument()->elementPool(), layer, defList[i], runningId);
			}
		}

		for(int i=0; i<defList.size(); i++) {
			if(defList[i].speaker==2) {
				qDebug("defList[i].speaker==2");
				processDefinition(editorWidget->instrument()->elementPool(), layer, defList[i], runningId);
			}
		}

		for(int i=0; i<defList.size(); i++) {
			if(defList[i].speaker==3) {
				qDebug("defList[i].speaker==3");
				processDefinition(editorWidget->instrument()->elementPool(), layer, defList[i], runningId);
			}
		}

		for(int i=0; i<defList.size(); i++) {
			if(defList[i].speaker==4) {
				qDebug("defList[i].speaker==4");
				processDefinition(editorWidget->instrument()->elementPool(), layer, defList[i], runningId);
			}
		}

		for(int i=0; i<defList.size(); i++) {
			if(defList[i].speaker==5) {
				qDebug("defList[i].speaker==5");
				processDefinition(editorWidget->instrument()->elementPool(), layer, defList[i], runningId);
			}
		}
	} else if(mode==7) {
		for(int i=0; i<defList.size(); i++) {
			processDefinition(editorWidget->instrument()->elementPool(), layer, defList[i], runningId, true);
		}
	} else if(mode==8) {

		qDebug("===== mode 8 ====================");
		for(int i=0; i<defList.size(); i++) {
			if(defList[i].speaker==0) {
				qDebug("***** defList[i].speaker==0 %d", runningId);
				processDefinition(editorWidget->instrument()->elementPool(), layer, defList[i], runningId, true);
			}
		}

		for(int i=0; i<defList.size(); i++) {
			if(defList[i].speaker==1) {
				qDebug("***** defList[i].speaker==1 %d", runningId);
				processDefinition(editorWidget->instrument()->elementPool(), layer, defList[i], runningId, true);
			}
		}

		for(int i=0; i<defList.size(); i++) {
			if(defList[i].speaker==2) {
				qDebug("***** defList[i].speaker==2 %d", runningId);
				processDefinition(editorWidget->instrument()->elementPool(), layer, defList[i], runningId, true);
			}
		}
			
	} else if(mode==9) {

		for(int i=0; i<defList.size(); i++) {
			if(defList[i].speaker==0) {
				qDebug("defList[i].speaker==0");
				processDefinition(editorWidget->instrument()->elementPool(), layer, defList[i], runningId, true);
			}
		}

		for(int i=0; i<defList.size(); i++) {
			if(defList[i].speaker==1) {
				qDebug("defList[i].speaker==1");
				processDefinition(editorWidget->instrument()->elementPool(), layer, defList[i], runningId, true);
			}
		}

	}//*/

	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void ImportSampleFolder::buildList(QString in, QList<QString> &tokens)
{
	const QChar separators[] = { ' ', ',', '-' };

	QString currentToken;
	for(int i=0; i<in.length(); i++) {

		if(in.at(i)=='.') {
			if(currentToken.length()>0) {
				tokens.append(currentToken);
			}
			return;
		}
		
		bool isSeparator = false;
		for(int j=0; j<3; j++) {
			if(in.at(i)==separators[j]) {
				isSeparator = true;
			}
		}

		if(isSeparator) {
			tokens.append(currentToken);
			currentToken.clear();
		} else {
			currentToken.append(in.at(i));
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void ImportSampleFolder::buildDefinition(QList<QString> &listToken, SimpleCellZoneDefinition &def, int *tokenDef, int speakerMode)
{
	aLog("----- ImportSampleFolder::buildDefinition() -----");
	//int tokenDef[8] = { defIgnore, defNoteRoot, defNoteLo, defNoteHi, defVelLo, defVelHi, defVariation, defSpeaker };
	//int tokenDef[7] = { defIgnore, defNoteRoot, defNoteLo, defNoteHi, defVelLo, defVelHi, defSpeaker };
	//int tokenDef[6] = { defIgnore, defNoteRoot, defNoteLo, defNoteHi, defVelLo, defVelHi };
	
	def.noteRoot = -1;
	def.noteLo = -1;
	def.noteHi = -1;
	def.velLo = -1;
	def.velHi = -1;
	def.variation = -1;
	def.speaker = -1;

	for(int i=0; i<listToken.size(); i++) {
		switch(tokenDef[i]) {
			case TkNoteRoot:
				def.noteRoot = listToken[i].toInt();
				break;
			case TkNoteLo:
				def.noteLo = listToken[i].toInt();
				break;
			case TkNoteHi:
				def.noteHi = listToken[i].toInt();
				break;
			case TkVelLo:
				def.velLo = listToken[i].toInt();
				if(def.velLo<1) {
					def.velLo = 1;
				}
				break;
			case TkVelHi:
				def.velHi = listToken[i].toInt();
				break;
			case TkVariation:
				qDebug("Variation=[%s]", qPrintable(listToken[i]));
				def.variation = listToken[i].toInt();
				break;
			case TkSpeaker:
				if(speakerMode == 2) {
					if(listToken[i].contains("fr", Qt::CaseInsensitive)) {
						def.speaker = 0;
					} else if(listToken[i].contains("sr", Qt::CaseInsensitive)) {
						def.speaker = 1;
					}
				} else if(speakerMode == 3) {
					if(listToken[i].contains("cl", Qt::CaseInsensitive)) {
						def.speaker = 0;
					} else if(listToken[i].contains("rm", Qt::CaseInsensitive)) {
						def.speaker = 1;
					} else if(listToken[i].contains("sr", Qt::CaseInsensitive)) {
						def.speaker = 2;
					}
				} else if(speakerMode == 6) {
					if(listToken[i].contains("cl", Qt::CaseInsensitive)) {
						def.speaker = 0;
					} else if(listToken[i].contains("rm", Qt::CaseInsensitive)) {
						def.speaker = 1;
					} else if(listToken[i].contains("sr", Qt::CaseInsensitive)) {
						def.speaker = 2;
					} else if(listToken[i].contains("ss", Qt::CaseInsensitive)) {
						def.speaker = 3;
					} else if(listToken[i].contains("cf", Qt::CaseInsensitive)) {
						def.speaker = 4;
					} else if(listToken[i].contains("cs", Qt::CaseInsensitive)) {
						def.speaker = 5;
					}
				}
				break;
		}
		
	}

	aLog("%d Tokens (R=%d, L=%d, H=%d, l=%d, h=%d, V=%d, S=%d)", listToken.size(), def.noteRoot, def.noteLo, def.noteHi, def.velLo, def.velHi, def.variation, def.speaker);
	aLog("-------------------------------------------------");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void ImportSampleFolder::processDefinition(SamElementPool *ep, SamLayer *layer, SimpleCellZoneDefinition def, int &runningId, bool addRelease)
{
	qDebug("processDefinition()");

	SamElement *element = ep->createElement(def.file, true);

	SamElement *elementRelease = 0;
	if(addRelease) {
		AString fileRelease = def.file;
		fileRelease.insert(fileRelease.length()-4, "-R");
		//qDebug("Adding release %s", qPrintable(fileRelease));
		aLog("Adding release %s", fileRelease.toChar());
		elementRelease = ep->createElement(fileRelease, true);
	}

	SamZone *zone = layer->existingZone(def.noteLo, def.noteHi, def.velLo, def.velHi);

	if(!zone) {
		qDebug("Creating Zone...");
		zone = new SamZone(layer);
		layer->addZone(zone);

		if(def.noteLo > -1 && def.noteHi > -1) {
			zone->setMidiNoteRange(def.noteLo, def.noteHi);
		}

		if(def.velLo > -1 && def.velHi > -1) {
			zone->setMidiVelocityRange(def.velLo, def.velHi);
		}

		//zone->setTrackPitch(false);
		//zone->setOneShot(true);
	}

	SamCell *cell = 0;


	/*if(def.variation < 1 && def.speaker < 1) { // If there's no variation (-1) or this is the first Cell (0)
		aLog("Creating Cell...");
		cell = new SamCell(zone);
		zone->addCell(cell);
		
		if(def.noteRoot>-1) {
			cell->setNoteBase(def.noteRoot);
		}
		cell->setLoopStart(element->loopStart());
		cell->setLoopEnd(element->loopEnd());
		cell->setFineTune(element->fineTune());

	} else {
	
		
	
	}


	if(cell) {
		if(addRelease) {
			cell->addElement(runningId,runningId+1);
			runningId+=2;
		} else {
			cell->addElement(runningId,-1);
			runningId++;
		}
	} else {
		runningId++;
	}




	return;*/

	qDebug("def.speaker = %d", def.speaker);
	
	if(def.speaker < 1) {
		qDebug("Creating Cell...");
		cell = new SamCell(zone);
		zone->addCell(cell);
		if(def.noteRoot>-1) {
			cell->setNoteBase(def.noteRoot);
		}
		cell->setLoopStart(element->loopStart());
		cell->setLoopEnd(element->loopEnd());
		cell->setFineTune(element->fineTune());
	} else {
		qDebug("Adding Cell to existing Element... (%d)", def.speaker);
		element->setSet(def.speaker); // Was 1

		qDebug("def.variation-1=%d",def.variation-1);
		/*cell = zone->cell(def.variation-1);	
		qDebug("After: def.variation-1=%d",def.variation-1);*/

		if(def.variation>0) {
			cell = zone->cell(def.variation-1);
		} else {
			cell = zone->cell(0);
		}
	}
	
	if(addRelease) {
		cell->addElement(runningId,runningId+1);
		runningId+=2;
	} else {
		cell->addElement(runningId,-1);
		runningId++;
	}

	qDebug("EXIT: processDefinition()");
}