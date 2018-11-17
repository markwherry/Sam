#ifndef __SamArtSeq_h__
#define __SamArtSeq_h__

#include "SamMusicTool.h"

class SamArtSeq : public SamMusicTool
{
public:
	SamArtSeq(SamToolMaster *toolMaster);
	~SamArtSeq();

	static SamTool* createInstance(SamToolMaster *toolMaster) { return (SamTool*)new SamArtSeq(toolMaster); }
	static AString uuid(void) { return "8FC771A8-A190-403D-A506-3072BE56CBED"; }
	static AString name(void) { return "Articulation Sequencer"; }

	virtual void processEvent(SamEvent *ev);

	float parameterValueNormalised(int id);
	void setParameterValueNormalised(int id, float value);

	QWidget* createEditor(void);

	int step(int index) { return m_steps[index]; }
	void setStep(int index, int artIndex) { m_steps[index] = artIndex; }

	int numSteps(void) { return m_numSteps; }
	void setNumSteps(int steps);

	int currentStep(void) { return m_currentStep; }

	enum Parameters {
		pmrStep1    = 1000,
		pmrStep2    = 1001,
		pmrStep3    = 1002,
		pmrStep4    = 1003,
		pmrStep5    = 1004,
		pmrStep6    = 1005,
		pmrStep7    = 1006,
		pmrStep8    = 1007,
		pmrNumSteps = 1008
	};

	static const int maxSteps = 8;

protected:
private:
	int m_steps[maxSteps];
	int m_numSteps;
	int m_currentStep;
};

#endif