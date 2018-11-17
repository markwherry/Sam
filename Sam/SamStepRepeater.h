#ifndef __SamStepRepeater_h__
#define __SamStepRepeater_h__

#include "SamMusicTool.h"

class SamStepRepeater : public SamMusicTool
{
public:
	SamStepRepeater(SamToolMaster *toolMaster);
	~SamStepRepeater();

	static SamTool* createInstance(SamToolMaster *toolMaster) { return (SamTool*)new SamStepRepeater(toolMaster); }
	static AString uuid(void) { return "AD540906-F4AC-45ae-B183-7A76D65D2132"; }
	static AString name(void) { return "Step Repeater"; }

	QWidget* createEditor(void);

	virtual void processEvent(SamEvent *ev);
	virtual void processRange(CsPpqnTime from, CsPpqnTime to);

	float parameterValueNormalised(int id);
	void setParameterValueNormalised(int id, float value);

	enum Parameters {
		pmrNumSteps = 1000,
		pmrStep     = 1001, // to 1128
		pmrMute     = 2001,
		pmrAccent   = 3001
	};

	const static int maxSteps = 64;

protected:
private:
	void playNote(int pitch, int velocity, int start, int length);

	struct Step {
		int velocity;
		bool mute;
		bool accent;
		int divisor;
		float gate;
	};

	Step m_steps[maxSteps];
	int m_numSteps;

	CsPpqnTime m_lastTo;
	int m_numNotes;
	int m_startTime;
	bool m_pitch[128];
	int m_stepLength;
	bool m_active;
	int m_position;
	int m_stepCounter;
	float m_velocityModulator;
	int m_humanise;
};

#endif