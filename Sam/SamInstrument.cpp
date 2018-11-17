#include "SamInstrument.h"

#include "Alog.h"

#include "Sam.h"

#include <QUuid>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamInstrument::SamInstrument(AModel *parent)
: AModel(parent)
, m_elementPool(0)
, m_description(0)
, m_name("Instrument")
{
	// Create Basic Instrument Properties
	recreateUuid();

	m_description = new SamDescription(this);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamInstrument::~SamInstrument()
{
	aLog("+++++++++++++++++++++++++++++++++++++++++++++++++++++");
	aLog("++++++++++ SamInstrument::~SamInstrument() ++++++++++");
	aLog("+++++++++++++++++++++++++++++++++++++++++++++++++++++");

	delete m_description;

	if(m_elementPool) {
		delete m_elementPool;
	}

	for(int i=0; i<m_streamMixes.size(); i++) {
		delete m_streamMixes[i];
	}
	m_streamMixes.clear();

	for(int i=0; i<m_versions.size(); i++) {
		delete m_versions[i];
	}
	m_versions.clear();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamInstrument::setFilename(AString filename)
{
	aLog("SamInstrument::setFilename(%s)", filename.toChar());

	m_filename = filename;

	if(m_filename.length() > 0) {
		m_path = m_filename.path();
		aLog(" m_path = %s", m_path.toChar());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamInstrument::setName(AString name)
{
	m_name = name;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamInstrument::setUuid(AString uuid)
{
	m_uuid = uuid;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamInstrument::create(int mode, bool allMonoStreams, bool createVersion, AString instrumentPath)
{
	aLog("SamInstrument::create()");

	// Create Basic Instrument Properties
	recreateUuid();

	m_name = "Instrument";


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
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamInstrument::createFromBounceGroups(AString instrumentPath, AList<SamBounceGroup*> *bounceGroups)
{
	// Create Element Pool
	// -------------------
	if(m_elementPool) {
		delete m_elementPool;
	}
	m_elementPool = new SamElementPool(this, instrumentPath);


	// Create Sets
	// -----------
	int numStreams = 0;
	for(int i=0; i<bounceGroups->size(); i++) {
		SamBounceGroup *bg = bounceGroups->at(i);
		AString name = bg->prefix();
		if(name.length() < 1) {
			name = AString::number(i+1);
		}
		m_elementPool->createSet(name, bg->config().numSpeakers());
		numStreams += bg->config().numSpeakers();
	}

	// Create Stream Mix
	// -----------------
	SamStreamMix *streamMix = new SamStreamMix(this);
	streamMix->createFromBounceGroups(bounceGroups);
	m_streamMixes.append(streamMix);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamInstrument::createFromStreamMix(AString instrumentPath, SamElementPool *elementPool, SamStreamMix *streamMix)
{
	// Create Element Pool
	// -------------------
	if(m_elementPool) {
		delete m_elementPool;
	}
	m_elementPool = new SamElementPool(this, instrumentPath);

	// Create Sets
	// -----------
	for(int i=0; i<elementPool->numSets(); i++) {
		SamSet *set = elementPool->set(i);
		m_elementPool->createSet(set->name(), set->streams());
	}

	// Create Stream Mix
	// -----------------
	SamStreamMix *streamMix_ = new SamStreamMix(this, *streamMix);
	m_streamMixes.append(streamMix_);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamInstrument::addVersion(SamVersion *version)
{
	m_versions.append(version);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamInstrument::recreateUuid(void)
{
	QString uuid = QUuid::createUuid().toString();
	QByteArray ba = uuid.mid(1,uuid.length()-2).toAscii();
	m_uuid = ba.data();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamInstrument::numCells(void)
{
	int cells = 0;
	
	for(int h=0; h<numVersions(); h++) {
		SamVersion *v = version(h);
		for(int i=0; i<v->numArticulations(); i++) {
			SamArticulation *a = v->articulation(i);
			for(int j=0; j<a->numLayers(); j++) {
				SamLayer *l = a->layer(j);
				for(int k=0; k<l->numZones(); k++) {
					SamZone *z = l->zone(k);
					cells += z->numCells();
				}
			}
		}
	}

	return cells;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamInstrument::readElement(AXmlDocument *document, AXmlElement *element, ACommand *command)
{
	if(element->numChildElements()<4) {
		aLog("Warning: <Instrument> does not contain the required Elements: <Description>, <ElementPool>, <StreamMixes>, and at least one <Version>.");
	}

	// Read <Instrument> attributes
	// ----------------------------
	if(element->hasAttribute("uuid")) {
		m_uuid = element->attribute("uuid");
	} else {
		aLog("<Instrument> does not have 'uuid' attribute.");
		return 0;
	}

	if(element->hasAttribute("name")) {
		m_name = element->attribute("name");
	} else {
		aLog("<Instrument> does not have 'name' attribute.");
		return 0;
	}

	if(element->hasAttribute("formatVersion")) {
		aLog("formatVersion=[%s]",element->attribute("formatVersion").toChar());
		float formatVersion = element->attribute("formatVersion").toFloat();
		if(formatVersion==(float)0.7) {
			aLog("Compatible formatVersion (%f).", formatVersion);
		} else {
			aLog("Incompatible formatVersion %f (this application loads version 0.7 files).", formatVersion);
			return 0;
		}
	} else {
		aLog("<Instrument> does not have 'formatVersion' attribute.");
		return 0;
	}

	int bn = element->attribute("appBuildNumber", AString::number(BUILD_NUMBER)).toInt();
	if(bn > BUILD_NUMBER) {
		aLog("Instrument was saved with a newer version of Sam.");
		command->setMessage("The Instrument you're trying to open was saved in a newer version of Sam and cannot be opened in this version of the application.");
		return 0;
	}

	// Read <Description>
	// ------------------
	AXmlElement *elementDescription = element->childElement(0);
	if(elementDescription->tag() != "Description") {
		aLog("<Description> tag not found. This tag should be the first child of <Instrument>.");
		return 0;
	}
	
	aLog("<Description> found.");
	m_description->readElement(document, elementDescription);

	// Read <ElementPool>
	// ------------------
	AXmlElement *elementElementPool = element->childElement(1);
	if(elementElementPool->tag() != "ElementPool") {
		aLog("<ElementPool> tag not found. This tag should follow <Description>...</Description>.");
		return 0;
	}
	aLog("<ElementPool> found.");

	if(m_elementPool) {
		delete m_elementPool;
	}
	m_elementPool = new SamElementPool(this, path());
	if(!m_elementPool->readElement(document, elementElementPool, command)) {
		aLog("Error reading <ElementPool>.");
		return 0;
	}

	// Read <StreamMixes>
	// ------------------
	AXmlElement *elementStreamMixes = element->childElement(2);
	if(elementStreamMixes->tag() != "StreamMixes") {
		aLog("<StreamMixes> tag not found. This tag should follow <ElementPool>...</ElementPool>.");
		return 0;
	}
	aLog("<StreamMixes> found.");

	for(int i=0; i<elementStreamMixes->numChildElements(); i++) {
		AXmlElement *elementStreamMix = elementStreamMixes->childElement(i);
		SamStreamMix *streamMix = new SamStreamMix(this);
		aLog("Reading <StreamMix> %d", i);
		if(!streamMix->readElement(document, elementStreamMix)) {
			aLog("Error reading <StreamMix>");
			delete streamMix;
			return 0;
		}
		m_streamMixes.append(streamMix);
	}

	// Read <Versions>
	// ---------------
	AXmlElement *elementVersions = element->childElement(3);
	if(elementVersions->tag() != "Versions") {
		aLog("<Versions> tag not found. This tag should follow <StreamMixes>...</StreamMixes>.");
		return 0;
	}
	aLog("<Versions> found.");

	for(int i=0; i<elementVersions->numChildElements(); i++) {
		AXmlElement *elementVersion = elementVersions->childElement(i);
		SamVersion *version = new SamVersion(this);
		aLog("Reading <Version> %d", i);
		if(!version->readElement(document, elementVersion)) {
			aLog("Error reading <Version>");
			delete version;
			return false;
		}
		m_versions.append(version);
	}

	aLog("EXIT: SamInstrument::readElement()");

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamInstrument::writeElement(AXmlDocument *document, AXmlElement *element, ACommand *command)
{
	aLog("SamInstrument::saveElement()");

	element->setAttribute("name", m_name);
	element->setAttribute("uuid", m_uuid);
	element->setAttribute("formatVersion", 0.7f);
	element->setAttribute("savedWith", AString("Sam2:")+AString(VERSION_STRING));
	element->setAttribute("appBuildNumber", BUILD_NUMBER);

	// Description
	aLog("Saving <Description>");
	AXmlElement *elementDescription = document->createElement("Description");
	m_description->writeElement(document, elementDescription);
	element->addChildElement(elementDescription);

	// Element Pool
	aLog("Saving <ElementPool>");
	AXmlElement *elementElementPool = document->createElement("ElementPool");
	m_elementPool->writeElement(document, elementElementPool, command);
	element->addChildElement(elementElementPool);

	// StreamMixes
	aLog("Saving <StreamMixes>");
	AXmlElement *elementStreamMixes = document->createElement("StreamMixes");
	for(int i=0; i<numStreamMixes(); i++) {
		aLog("Saving <StreamMix #%d>", i);
		AXmlElement *elementStreamMix = document->createElement("StreamMix");
		streamMix(i)->writeElement(document, elementStreamMix);
		elementStreamMixes->addChildElement(elementStreamMix);
	}
	element->addChildElement(elementStreamMixes);

	// Versions
	aLog("Saving <Description>");
	AXmlElement *elementVersions = document->createElement("Versions");
	for(int i=0; i<numVersions(); i++) {
		aLog("Saving <Version> #%d", i);
		AXmlElement *elementVersion = document->createElement("Version");
		version(i)->writeElement(document, elementVersion);
		elementVersions->addChildElement(elementVersion);
	}
	element->addChildElement(elementVersions);

	return 1;
}