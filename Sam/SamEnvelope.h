#ifndef __SamEnvelope_h__
#define __SamEnvelope_h__

#include "AModel.h"
#include "AXmlDocument.h"
using namespace Atom;

class SamEnvelope : public AModel
{
public:
	SamEnvelope(AModel *parent, float attackStart = 0, float attack = 0, float hold = 0, float decay = 0, float sustain = 1.0, float release = 0);
	SamEnvelope(AModel *parent, const SamEnvelope& envelope);
	~SamEnvelope();

	void print(void);

	// Accessors
	inline float attackStart(void) const { return m_attackStart; }
	inline void setAttackStart(float attackStart) { m_attackStart = attackStart; }

	inline float attack(void) const { return m_attack; }
	inline void setAttack(float attack) { m_attack = attack; }

	inline float attackVelocity(void) const { return m_attackVelocity; }
	inline void setAttackVelocity(float attackVelocity) { m_attackVelocity = attackVelocity; }

	inline int attackVelocityThreshold(void) const { return m_attackVelocityThreshold; }
	inline void setAttackVelocityThreshold(int attackVelocityThreshold) { m_attackVelocityThreshold = attackVelocityThreshold; }

	inline float hold(void) const { return m_hold; }
	inline void setHold(float hold) { m_hold = hold; }

	inline float decay(void) const { return m_decay; }
	inline void setDecay(float decay) { m_decay = decay; }

	inline float sustain(void) const { return m_sustain; }
	inline void setSustain(float sustain) { m_sustain = sustain; }

	inline float release(void) const { return m_release; }
	inline void setRelease(float release) { m_release = release; }

	inline int attackShape(void) const { return m_attackShape; }
	inline void setAttackShape(int attackShape) { m_attackShape = attackShape; }

	inline float attackDepth(void) const { return m_attackDepth; }
	inline void setAttackDepth(float attackDepth) { m_attackDepth = attackDepth; }

	inline int decayShape(void) const { return m_decayShape; }
	inline void setDecayShape(int decayShape) { m_decayShape = decayShape; }

	inline int decayDepth(void) const { return m_decayDepth; }
	inline void setDecayDepth(int decayDepth) { m_decayDepth = decayDepth; }

	inline int releaseShape(void) const { return m_releaseShape; }
	inline void setReleaseShape(int releaseShape) { m_releaseShape = releaseShape; }

	inline int releaseDepth(void) const { return m_releaseDepth; }
	inline void setReleaseDepth(int releaseDepth) { m_releaseDepth = releaseDepth; }

	bool isAutoRelease(void) const { return m_autoRelease; }
	void setAutoRelease(bool autoRelease) { m_autoRelease = autoRelease;}

	// Load and save
	int readElement(AXmlDocument *document, AXmlElement *element);
	int writeElement(AXmlDocument *document, AXmlElement *element);

private:
	float m_attackStart;
	float m_attack;
	float m_attackVelocity;
	int m_attackVelocityThreshold;
	float m_hold;
	float m_decay;
	float m_sustain;
	float m_release;
	bool m_autoRelease;

	int m_attackShape;
	int m_decayShape;
	int m_releaseShape;

	float m_attackDepth;
	float m_decayDepth;
	float m_releaseDepth;
};

#endif // __SamEnvelope_h__
