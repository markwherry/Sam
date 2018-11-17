#ifndef __SamPerformanceControls_h__
#define __SamPerformanceControls_h__

#include "AModel.h"
using namespace Atom;

class SamPerformanceControls : public AModel
{
public:
	SamPerformanceControls(AModel *parent);
	~SamPerformanceControls();

	// ---

	float envelopeAttack() { return m_envelopeAttack; }
	void setEnvelopeAttack(float envelopeAttack) { m_envelopeAttack = envelopeAttack; }

	float envelopeAttackOffset() { return m_envelopeAttackOffset; }
	void setEnvelopeAttackOffset(float envelopeAttackOffset) { m_envelopeAttackOffset = envelopeAttackOffset; }

	float envelopeHold() { return m_envelopeHold; }
	void setEnvelopeHold(float envelopeHold) { m_envelopeHold = envelopeHold; }

	float envelopeDecay() { return m_envelopeDecay; }
	void setEnvelopeDecay(float envelopeDecay) { m_envelopeDecay = envelopeDecay; }

	float envelopeSustain() { return m_envelopeSustain; }
	void setEnvelopeSustain(float envelopeSustain) { m_envelopeSustain = envelopeSustain; }

	float envelopeRelease() { return m_envelopeRelease; }
	void setEnvelopeRelease(float envelopeRelease) { m_envelopeRelease = envelopeRelease; }

	// ---

	float envelopeReleaseAttack() { return m_envelopeReleaseAttack; }
	void setEnvelopeReleaseAttack(float attack) { m_envelopeReleaseAttack = attack; }

	//float envelopeReleaseAttackOffset() { return m_envelopeReleaseAttackOffset; }
	//void setEnvelopeReleaseAttackOffset(float offset) { m_envelopeReleaseAttackOffset = offset; }

	float envelopeReleaseHold() { return m_envelopeReleaseHold; }
	void setEnvelopeReleaseHold(float hold) { m_envelopeReleaseHold = hold; }

	float envelopeReleaseDecay() { return m_envelopeReleaseDecay; }
	void setEnvelopeReleaseDecay(float decay) { m_envelopeReleaseDecay = decay; }

	float envelopeReleaseSustain() { return m_envelopeReleaseSustain; }
	void setEnvelopeReleaseSustain(float sustain) { m_envelopeReleaseSustain = sustain; }

	float envelopeReleaseRelease() { return m_envelopeReleaseRelease; }
	void setEnvelopeReleaseRelease(float release) { m_envelopeReleaseRelease = release; }

	// ---

	float sampleStart(void) { return m_sampleStart; }
	void setSampleStart(float sampleStart) { m_sampleStart = sampleStart; }

	float cellSampleStart(void) { return m_cellSampleStart; }
	void setCellSampleStart(float sampleStart) { m_cellSampleStart = sampleStart; }

	int minimumVelocity(void) { return m_minimumVelocity; }
	void setMinimumVelocity(float value);

	int filterCutOff(void) { return m_filterCutOff; }
	void setFilterCutOff(float value);

	float fFilterCutOff(void) { return m_fFilterCutOff; }

	void debug(void);

private:
	float m_envelopeAttack;
	float m_envelopeAttackOffset;
	float m_envelopeHold;
	float m_envelopeDecay;
	float m_envelopeSustain;
	float m_envelopeRelease;

	float m_envelopeReleaseAttack;
	//float m_envelopeReleaseAttackOffset;
	float m_envelopeReleaseHold;
	float m_envelopeReleaseDecay;
	float m_envelopeReleaseSustain;
	float m_envelopeReleaseRelease;

	float m_sampleStart;
	float m_cellSampleStart;
	float m_fFilterCutOff;
	int   m_minimumVelocity;
	int   m_filterCutOff;
};

#endif // __SamPerformanceControls_h__
