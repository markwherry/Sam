#ifndef __SamVersion_h__
#define __SamVersion_h__

#include "AModel.h"
using namespace Atom;

#include "SamArticulation.h"
#include "SamOutput.h"

class SamVersion : public AModel
{
public:
    SamVersion(AModel *parent);
	SamVersion(AModel *parent, const SamVersion& version, bool parametersOnly);
    ~SamVersion();

	SamArticulation* createArticulation(void);
	void addArticulation(SamArticulation *articulation);

	bool isPitchValid(int articulation, int layer, int pitch);

	// Accessors
	inline AString name (void) const { return m_name; }
	inline void setName (AString name) { m_name = name; }

	int numArticulations(void) { return articulationList.size(); }
	SamArticulation* articulation(int id) { return articulationList[id]; }
	int defaultArticulation(void) { return m_defaultArticulation; }
	int getArticulationTriggerId(int id);
	int getArticulationId(int triggerId);

	int numOutputs(void) { return outputList.size(); }
	SamOutput* output(int id) { return outputList[id]; }
	int outputListId(int id);
	void addOutput(SamOutput *output);
	SamOutput* createOutput(AString name);
	void deleteOutput(int id);

	//void setNumOutputs(int numOutputs) { m_numOutputs = numOutputs; }

	//inline QString outputName(int id) const { return m_outputName[id]; }
	//void setOutputName(int id, QString name) { m_outputName[id] = name; }

	int numVoices(void) { return m_numVoices; }
	void setNumVoices(int numVoices) { m_numVoices = numVoices; }

	int reference(void) { return m_reference; }
	void setReference(int reference) { m_reference = reference; }

	// Load and save
	int readElement(AXmlDocument *document, AXmlElement *element);
	int writeElement(AXmlDocument *document, AXmlElement *element);

private:
	int createOutputId(void);
	AString m_name;
	int m_defaultArticulation;
	//int m_numOutputs;
	int m_numVoices;
	int m_reference;
	//QString m_outputName[32];

	AList<SamArticulation*> articulationList;
	AList<SamOutput*> outputList;
	AXmlElement *m_musicToolElements[2];
	
};

#endif // SAMVERSION_H
