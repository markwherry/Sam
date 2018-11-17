#ifndef __SamParameter_h__
#define __SamParameter_h__

#include "AModel.h"
#include "AString.h"
#include "AList.h"
#include "AXmlDocument.h"
using namespace Atom;

#include "CsCurve.h"
using namespace Cs;

class SamParameter : public AModel
{
public:
	SamParameter(AModel *parent);
	SamParameter(AModel *parent, const SamParameter& articulation);
	~SamParameter();

	enum SamParameterSource {
		LayerSelectMode = 10,
		LayerSelect     = 11,
		
		StreamGain = 20,
		StreamPanX = 21,
		StreamPanY = 22,
		StreamMute = 23,
		StreamSolo = 24,

		Articulation          = 30,
		ArticulationLock      = 31,
		ArticulationMode      = 32,
		ArticulationDefault   = 33,
		DefaultCfArticulation = 34,

		EnvelopeAttack       = 40,
		EnvelopeHold         = 41,
		EnvelopeDecay        = 42,
		EnvelopeSustain      = 43,
		EnvelopeRelease      = 44,
		EnvelopeAttackOffset = 45,

		SampleStart     = 50,
		CellSampleStart = 51,

		MinimumVelocity       = 60,
		MinimumVelocityInvert = 61,

		FilterCutOff = 70,

		MusicTool = 80, // 80-89 are reserved for Music Tools 0-9

		LayerMute = 90,

		SendGain = 100,

		ChannelMute = 110,
		ChannelSolo = 111,

		EnvelopeReleaseAttack       = 140,
		EnvelopeReleaseHold         = 141,
		EnvelopeReleaseDecay        = 142,
		EnvelopeReleaseSustain      = 143,
		EnvelopeReleaseRelease      = 144,
		//EnvelopeReleaseAttackOffset = 145
		

	};

	enum SamParameterType {
		sptTrigger = 0,
		sptToggle  = 1,
		sptRange   = 2
	};

	enum SamParameterMidiType {
		spmtNotePitch    = 0,
		spmtNoteVelocity = 1,
		spmtController   = 2
	};

	inline AString name(void) const { return m_name; }
	inline void setName(AString name) { m_name = name; }

	inline int source(void) const { return m_source; }
	inline void setSource(int source) { m_source = source; }

	inline int sourceControl(void) const { return m_sourceControl; }
	inline void setSourceControl(int sourceControl) { m_sourceControl = sourceControl; }

	inline int type(void) const { return m_type; }
	inline void setType(int type) { m_type = type; }

	inline int triggerValue(void) const { return m_triggerValue; }
	inline void setTriggerValue(int triggerValue) { m_triggerValue = triggerValue; }

	inline int midiType(void) const { return m_midiType; }
	inline void setMidiType(int midiType) { m_midiType = midiType; }

	inline int midiController(void) const { return m_midiController; }
	inline void setMidiController(int midiController) { m_midiController = midiController; }

	inline int midiValue(void) const { return m_midiValue; }
	inline void setMidiValue(int midiValue) { m_midiValue = midiValue; }

	inline bool isInverted(void) const { return m_inverted; }
	inline void setInverted(bool inverted) { m_inverted = inverted; }

	CsCurve* curve(void) { return m_curve; }
	void setCurveShape(int shape) { m_curve->setShape(shape); }

	float defaultValue(void) { return m_defaultValue; }
	void setDefaultValue(float defaultValue) { m_defaultValue = defaultValue; }

	void debug(void);

	// Load and save
	int readElement(AXmlDocument *document, AXmlElement *element);
	int writeElement(AXmlDocument *document, AXmlElement *element);

protected:
private:
	AString m_name;
	int m_source;
	int m_sourceControl;
	int m_type;
	int m_triggerValue;
	int m_midiType;
	int m_midiController;
	int m_midiValue;
	bool m_inverted;
	CsCurve *m_curve;
	float m_defaultValue;
};

#endif