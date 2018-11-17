#include "SamEnvelope.h"

#include "ALog.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamEnvelope::SamEnvelope(AModel *parent, float attackStart, float attack, float hold, float decay, float sustain, float release)
	: AModel(parent)
	, m_attackStart(attackStart)
	, m_attackVelocity(attackStart)
	, m_attackVelocityThreshold(127)
	, m_attack(attack)
	, m_hold(hold)
	, m_decay(decay)
	, m_sustain(sustain)
	, m_release(release)
	, m_attackShape(0)
	, m_decayShape(0)
	, m_releaseShape(0)
	, m_attackDepth(0.5)
	, m_decayDepth(0.5)
	, m_releaseDepth(0.5)
	, m_autoRelease(false)
{
	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamEnvelope::SamEnvelope(AModel *parent, const SamEnvelope& envelope)
	: AModel(parent)
{
	m_attackStart = envelope.m_attackStart;
	m_attackVelocity = envelope.m_attackVelocity;
	m_attackVelocityThreshold = envelope.m_attackVelocityThreshold;
	m_attack = envelope.m_attack;
	m_hold = envelope.m_hold;
	m_decay = envelope.m_decay;
	m_sustain = envelope.m_sustain;
	m_release = envelope.m_release;
	m_attackShape = envelope.m_attackShape;
	m_decayShape = envelope.m_decayShape;
	m_releaseShape = envelope.m_releaseShape;
	m_attackDepth = envelope.m_attackDepth;
	m_decayDepth = envelope.m_decayDepth;
	m_releaseDepth = envelope.m_releaseDepth;
	m_autoRelease = envelope.m_autoRelease;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamEnvelope::~SamEnvelope()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamEnvelope::print(void)
{
	aLog("SamEnvelope: (%f, %f/%f, %f, %f, %f, %f", m_attackStart, m_attack, m_attackVelocity, m_hold, m_decay, m_sustain, m_release);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamEnvelope::readElement(AXmlDocument *document, AXmlElement *element)
{
	if(element->hasAttribute("attackStart")) {
		setAttackStart(element->attribute("attackStart").toFloat());
	} else {
		aLog("Error: <Envelope> does not have 'attackStart' attribute.");
		return false;
	}

	if(element->hasAttribute("attack")) {
		setAttack(element->attribute("attack").toFloat());
	} else {
		aLog("Error: <Envelope> does not have 'attack' attribute.");
		return false;
	}

	if(element->hasAttribute("attackVelocity")) {
		setAttackVelocity(element->attribute("attackVelocity").toFloat());
	} else {
		setAttackVelocity(m_attack);
		aLog("Warning: <Envelope> does not have 'attackVelocity' attribute.");
		//return false;
	}

	if(element->hasAttribute("attackVelocityThreshold")) {
		setAttackVelocityThreshold(element->attribute("attackVelocityThreshold").toInt());
	} else {
		aLog("Warning: <Envelope> does not have 'attackVelocityThreshold' attribute.");
		//return false;
	}

	if(element->hasAttribute("attackShape")) {
		setAttackShape(element->attribute("attackShape").toInt());
	} else {
		aLog("Warning: <Envelope> does not have 'attackShape' attribute.");
		//return false;
	}

	if(element->hasAttribute("attackDepth")) {
		setAttackDepth(element->attribute("attackDepth").toFloat());
	} else {
		aLog("Warning: <Envelope> does not have 'attackDepth' attribute.");
		//return false;
	}

	if(element->hasAttribute("hold")) {
		setHold(element->attribute("hold").toFloat());
	} else {
		aLog("Error: <Envelope> does not have 'hold' attribute.");
		return false;
	}

	if(element->hasAttribute("decay")) {
		setDecay(element->attribute("decay").toFloat());
	} else {
		aLog("Error: <Decay> does not have 'decay' attribute.");
		return false;
	}

	if(element->hasAttribute("decayShape")) {
		setDecayShape(element->attribute("decayShape").toInt());
	} else {
		aLog("Error: <DecayShape> does not have 'decay' attribute.");
		//return false;
	}

	if(element->hasAttribute("decayDepth")) {
		setDecayDepth(element->attribute("decayDepth").toFloat());
	} else {
		aLog("Error: <DecayDepth> does not have 'decay' attribute.");
		//return false;
	}

	if(element->hasAttribute("sustain")) {
		setSustain(element->attribute("sustain").toFloat());
	} else {
		aLog("Error: <Envelope> does not have 'sustain' attribute.");
		return false;
	}

	if(element->hasAttribute("release")) {
		setRelease(element->attribute("release").toFloat());
	} else {
		aLog("Error: <Envelope> does not have 'release' attribute.");
		return false;
	}

	if(element->hasAttribute("releaseShape")) {
		setReleaseShape(element->attribute("releaseShape").toInt());
	} else {
		aLog("Warning: <Envelope> does not have 'releaseShape' attribute.");
		//return false;
	}

	if(element->hasAttribute("releaseDepth")) {
		setReleaseDepth(element->attribute("releaseDepth").toFloat());
	} else {
		aLog("Warning: <Envelope> does not have 'releaseDepth' attribute.");
		//return false;
	}

	if(element->hasAttribute("autoRelease")) {
		setAutoRelease((bool)element->attribute("autoRelease").toInt());
	} else {
		aLog("Warning: <Envelope> does not have 'autoRelease' attribute.");
		//return false;
	}

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamEnvelope::writeElement(AXmlDocument *document, AXmlElement *element)
{
	element->setAttribute("attackStart", m_attackStart);
	element->setAttribute("attack", m_attack);
	if(m_attack!=m_attackVelocity) {
		element->setAttribute("attackVelocity", m_attackVelocity);
	}
	if(m_attackVelocityThreshold<127) {
		element->setAttribute("attackVelocityThreshold", m_attackVelocityThreshold);
	}
	element->setAttribute("attackShape", m_attackShape);
	element->setAttribute("attackDepth", m_attackDepth);
	element->setAttribute("hold", m_hold);
	element->setAttribute("decay", m_decay);
	element->setAttribute("decayDepth", m_decayDepth);
	element->setAttribute("decayShape", m_decayShape);
	element->setAttribute("sustain", m_sustain);
	element->setAttribute("release", m_release);
	element->setAttribute("releaseShape", m_releaseShape);
	element->setAttribute("releaseDepth", m_releaseDepth);
	element->setAttribute("autoRelease", (int)m_autoRelease);

	return 1;
}