#include "SamElementPool.h"

#include "SamInstrument.h"
#include "SamEngine.h"

#include "ALog.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamElementPool::SamElementPool(AModel *parent, AString instrumentPath)
	: AModel(parent)
	, m_instrumentPath(instrumentPath)
	, m_size(0)
	, m_accelerated(false)
{
	aLog("SamElementPool::SamElementPool(%s)", instrumentPath.toChar());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamElementPool::~SamElementPool()
{
	aLog("SamElementPool::~SamElementPool()");

	for(int i=0; i<elementList.size(); i++) {
		delete elementList[i];
	}
	
	elementList.clear();

	aLog("Cleared elementList");

	for(int i=0; i<setList.size(); i++) {
		delete setList[i];
	}
	setList.clear();

	aLog("EXIT: SamElementPool::~SamElementPool()");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamElementPool::createSet(AString name, int streams)
{
	SamSet *set = new SamSet(this);

	set->setName(name);
	set->setStreams(streams);

	setList.append(set);

	return setList.size()-1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamElement* SamElementPool::element(int id)
{
	if(id>=0 && id<elementList.size()) {
		return elementList[id]; 
	}

	aLog("SamElementPool::element(%d) out of range", id);
	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamElement* SamElementPool::createElement(AString filename, bool loadSampleData)
{
	//aLog("SamElementPool::createElement()");
	SamElement *element = new SamElement(this, m_instrumentPath, filename);

	if(element->bitDepth() == 0) {
		aLog("Not found");
		delete element;
		return 0;
	}
	
	if(loadSampleData) {
		element->loadAudioData();
		if(!element->isAudioDataLoaded()) {
			aLog("Not found!");
			delete element;
			return 0;
		}

		m_size += element->size();
	}

	//if(!element->importAudioFile(m_instrumentPath, filename)) {
	//	aLog("Not found!");
	//	delete element;
	//	return 0;
	//}

	elementList.append(element);
	//aLog("EXIT: SamElementPool::createElement()");

	

	//aLog("m_size = %u, element->size() = %u", m_size, element->size());

	return element;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamElementPool::addElement(SamElement *element)
{
	elementList.append(element);

	return elementList.size()-1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamElementPool::recalculateSize(void)
{
	m_size = 0;
	SamElement *e = 0;
	for(AListIterator<SamElement*>item = elementList.begin(); item != elementList.end(); item++) {
		e = *item;
		m_size += e->size();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamElementPool::importElements(AString filename, ACommand *command)
{
	aLog("SamElementPool::importElements(%s)", filename.toChar());

	AFile file(filename);

	if(!file.open()) {
		return false;
	}

	char chunkName[4];
	file.readChunkName(chunkName);
	if(!file.compareChunkName(chunkName, "SamA")) {
		aLog("Not SamA");
		return false;
	}
	
	AInt32 version;
	file.read32Lsb(&version);
	aLog("version = %d", version);

	AUInt64 numElements;
	file.readU64Lsb(&numElements);
	aLog("numElements = %lld", numElements);

	unsigned int chunkSize;

	for(AUInt64 i = 0; i<numElements; i++) {
		file.readChunkName(chunkName);
		if(!file.compareChunkName(chunkName, "SamE")) {
			aLog("Not SamE");
			file.close();
			return false;
		}

		file.readU32Lsb(&chunkSize);

		//aLog("Elements %lld/%lld", i+1, numElements);
		SamElement *element = new SamElement(this, "", "");
		element->importElementFromChunk(&file);
		elementList.append(element);
		m_size += element->size();
		command->setProgress((float)i / (float)numElements);
	}

	file.close();

	aLog("RETURN: SamElementPool::importElements()");

	return true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamElementPool::exportElements(AString filename, ACommand *command)
{
	AFile file(filename);

	if(!file.create()) {
		return false;
	}

	file.writeChunkName("SamA");

	AInt32 version = 0;
	file.write32Lsb(version);

	file.writeU64Lsb(elementList.size());
	for(AUInt64 i=0; i<elementList.size(); i++) {
		elementList[i]->exportElementFromChunk(&file);
		command->setProgress((float)i / (float)elementList.size());
	}

	file.close();

	return true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamElementPool::readElement(AXmlDocument *document, AXmlElement *element, ACommand *command)
{
	if(element->numChildElements() < 2) {
		aLog("Warning: <ElementPool> does not contain the required Elements: <Sets>, <Elements>.");
	}

	// Read <Sets>
	// -----------
	AXmlElement *elementSets = element->childElement(0);
	if(elementSets->tag() != "Sets") {
		aLog("<Sets> tag not found. This tag should be the first child of <ElementPool>.");
		return 0;
	}
	
	aLog("<Sets> found.");
	
	for(int i=0; i<elementSets->numChildElements(); i++) {
		AXmlElement *elementSet = elementSets->childElement(i);
		SamSet *set = new SamSet(this);
		aLog("Reading <Set> %d", i);
		if(!set->readElement(document, elementSet)) {
			aLog("Error reading <Set>");
			return false;
		}
		setList.append(set);
	}

	// Read <Elements>
	// ---------------
	AXmlElement *elementElements = element->childElement(1);
	if(elementElements->tag() != "Elements") {
		aLog("<Elements> tag not found. This tag should follow <Sets>.");
		return 0;
	}
	
	aLog("<Elements> found.");

	m_accelerated = (bool)elementElements->attribute("accelerated", "0").toInt();

	if(m_accelerated) {
		aLog("Accelerated.");
		AString fn = ((SamInstrument*)parent())->filename(); aLog("1");
		AString filename = fn.first(fn.length()-1); aLog("2");
		filename.append("x"); aLog("3 [%d]", filename.length());
		if(!importElements(filename, command)) {
			return 0;
		}

	} else {
	
		int numElements = elementElements->numChildElements();

		for(int i=0; i<numElements; i++) {
			AXmlElement *elementElement = elementElements->childElement(i);
			//aLog("Reading <Element> %d", i);

			if(!elementElement->hasAttribute("audioFile")) {
				aLog("<Element> does not have 'audioFile' attribute.");
				return 0;
			}

			if(!elementElement->hasAttribute("set")) {
				aLog("<Element> does not have 'set' attribute.");
				return 0;
			}

			if(command) {
				AString af = elementElement->attribute("audioFile"); //aLog("1");
				int pos = af.findCharacter(L'//', AString::fromBack); //aLog("2");
				if(pos > -1) { //aLog("3");
					af = af.last(af.length()-pos-1);
				}
				command->setProgressLabel(1, af);
			}

			//aLog("Instrument path = %s",m_instrumentPath.toChar());
			SamElement *samElement = createElement(elementElement->attribute("audioFile"), false);
			if(samElement==0) {
				aLog("Error loading <Element> %d [%s].", i, elementElement->attribute("audioFile").toChar());
				command->setMessage(AString("There was a problem loading this Instrument because the following audio file could not be found:\n\n")+elementElement->attribute("audioFile"));
				//delete samElement; // This is already done in createElement
				return false;
			}

			samElement->setSet(elementElement->attribute("set").toInt());

			if(command) {
				command->setProgress((float)i/(float)numElements);
			}
		}

	}

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamElementPool::writeElement(AXmlDocument *document, AXmlElement *e, ACommand *command)
{
	// Sets
	AXmlElement *elementSets = document->createElement("Sets");
	for(int i=0; i<numSets(); i++) {
		AXmlElement *elementSet = document->createElement("Set");
		set(i)->writeElement(document, elementSet);
		elementSets->addChildElement(elementSet);
	}
	e->addChildElement(elementSets);

	// Elements
	AXmlElement *elementElements = document->createElement("Elements");
	if(m_accelerated) {
		AString fn = ((SamInstrument*)parent())->filename();
		AString filename = fn.first(fn.length()-1);
		filename.append("x");
		if(!exportElements(filename, command)) {
			return 0;
		}
		elementElements->setAttribute("accelerated", (int)m_accelerated);
		//elementElements->setAttribute("file", "path");
	} else {
		for(int i=0; i<numElements(); i++) {
			AXmlElement *elementElement = document->createElement("Element");
			elementElement->setAttribute("id", i);
			elementElement->setAttribute("audioFile", element(i)->filename());
			elementElement->setAttribute("set", element(i)->set());
			elementElements->addChildElement(elementElement);
		}
	}
	e->addChildElement(elementElements);

	return true;
}