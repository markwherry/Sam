#ifndef __SamArticulation_h__
#define __SamArticulation_h__

#include "AModel.h"
#include "AString.h"
#include "AList.h"
#include "AXmlDocument.h"
using namespace Atom;

#include "CsCurve.h"
using namespace Cs;

#include "SamLayer.h"
#include "SamParameter.h"

class SamArticulation : public AModel
{
public:
	SamArticulation(AModel *parent);
	SamArticulation(AModel *parent, const SamArticulation& articulation, int elementIdOffset, bool parametersOnly);
	~SamArticulation();

	SamLayer* createLayer(void);

	// Accessors
	inline AString name (void) const { return m_name; }
	inline void setName (AString name) { m_name = name; }

	int id(void) const { return m_id; }
	void setId(int id) { m_id = id; }

	inline int zoneCrossfadeResponseTime(void) const { return m_zoneCrossfadeResponseTime; }
	void setZoneCrossfadeResponseTime(int time, void *from = 0, bool shouldUpdate = true);

	inline int zoneFilterResponseTime(void) const { return m_zoneFilterResponseTime; }
	void setZoneFilterResponseTime(int time, void *from = 0, bool shouldUpdate = true);

	int defaultStreamMix(void) { return m_defaultStreamMix; }
	void setDefaultStreamMix(int defaultStreamMix) { m_defaultStreamMix = defaultStreamMix; }

	int defaultLayer(void) { return m_defaultLayer; }
	void setDefaultLayer(int defaultLayer) { m_defaultLayer = defaultLayer; }

	enum SamLayerSelectMode {
		SingleLayer         = 0,
		CycleLayers         = 1,
		AllLayers           = 2,
		Legato              = 3,
		NumLayerSelectModes = 4,
	};

	int layerSelectMode(void) { return m_layerSelectMode; }
	void setLayerSelectMode(int layerSelectMode) { /*qDebug("setLayerSelectMode(%d)", layerSelectMode);*/ if(layerSelectMode>=0 && layerSelectMode <NumLayerSelectModes) m_layerSelectMode = layerSelectMode; }

	void addLayer(SamLayer *layer) { layerList.append(layer); }
	SamLayer* layer(int id) { return layerList[id]; }
	int numLayers(void) { return layerList.size(); }
	int layerIndex(SamLayer *layer);

	SamParameter* parameter(int id) { return parameterList[id]; }
	int numParameters(void) { return parameterList.size(); }
	SamParameter *createParameter(void);
	void addParameter(SamParameter *parameter);
	void removeParameter(int id);

	CsCurve* velocityCurve(void) { return m_curveVelocity; }
	CsCurve* velocityFilterCurve(void) { return m_curveVelocity; }
	CsCurve* curveCrossfadeGain(void) { return m_curveCrossfadeGain; }
	CsCurve* curveReleaseAttenuation(void) { return m_curveReleaseAttenuation; }

	

	int switchLayerTime(void) { return m_switchLayerTime; }
	void setSwitchLayerTime(int switchLayerTime) { m_switchLayerTime = switchLayerTime; }

	int legatoOverlap(void) { return m_legatoOverlap; }
	void setLegatoOverlap(int legatoOverlap) { m_legatoOverlap = legatoOverlap; }

	//int output(void) { return m_output; }
	int outputListId(void) { return m_outputListId; }
	void setOutput(int output) { m_output = output; }

	//int releaseOutput(void) { return m_releaseOutput; }
	int releaseOutputListId(void) { return m_releaseOutputListId; }
	void setReleaseOutput(int releaseOutput) { m_releaseOutput = releaseOutput; }

	void calculateOutputListIds(void);

	inline int polyphonicModulation(void) { return m_polyphonicModulation; }
	void setPolyphonicModulation(int polyphonicModulation) { m_polyphonicModulation = polyphonicModulation; }

	inline int maxAftertouch(void) { return m_maxAftertouch; }
	void setMaxAftertouch(int velocity) { m_maxAftertouch = velocity; }

	inline bool isVelocitySetsCrossfade(void) { return m_velocitySetsCrossfade; }
	void setVelocitySetsCrossfade(bool enabled) { m_velocitySetsCrossfade = enabled; }

	inline bool isVelocityCrossfadeDecay(void) { return m_velocityCrossfadeDecay; }
	void setVelocityCrossfadeDecay(bool enabled) { m_velocityCrossfadeDecay = enabled; }

	inline int velocityCrossfadeDecayTime(void) { return m_velocityCrossfadeDecayTime; }
	void setVelocityCrossfadeDecayTime(int time) { m_velocityCrossfadeDecayTime = time; }
	
	inline int velocityCrossfadeDecayTo(void) { return m_velocityCrossfadeDecayTo; }
	void setVelocityCrossfadeDecayTo(int velocity) { m_velocityCrossfadeDecayTo = velocity; }

	//inline int minimumVelocityController(void) { return m_minimumVelocityController; }
	//void setMinimumVelocityController(int controller) { m_minimumVelocityController = controller; }

	bool containsLoops(void);

	// Load and save
	int readElement(AXmlDocument *document, AXmlElement *element);
	int writeElement(AXmlDocument *document, AXmlElement *element);

	enum SamArticulationStateItem {
		ItemSelected                  = 0,
		ItemProperty                  = 1,
		ItemZoneCrossfadeResponseTime = 2,
		ItemZoneFilterResponseTime    = 3,
		ItemParameterAdded            = 4,
		ItemParameterRemoved          = 5
	};

//signals:
//	void parameterCreated(void);
//	void parameterDeleted(void);

private:
	AString m_name;
	int m_id;
	int m_zoneCrossfadeResponseTime;
	int m_zoneFilterResponseTime;
	int m_output;
	int m_outputListId;
	int m_releaseOutput;
	int m_releaseOutputListId;

	int m_defaultStreamMix;
	int m_defaultLayer;
	int m_layerSelectMode;
	int m_legatoOverlap;

	AList<SamLayer*> layerList;
	AList<SamParameter*> parameterList;

	CsCurve *m_curveVelocity;
	CsCurve *m_curveVelocityFilter;
	CsCurve *m_curveCrossfadeGain;
	CsCurve *m_curveReleaseAttenuation;

	

	int m_switchLayerTime;

	int m_polyphonicModulation;
	int m_maxAftertouch;
	bool m_velocitySetsCrossfade;
	bool m_velocityCrossfadeDecay;
	int m_velocityCrossfadeDecayTime;
	int m_velocityCrossfadeDecayTo;

	

	//int m_minimumVelocityController;
};

#endif // __SamArticulation_h__
