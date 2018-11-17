#ifndef __SamToolParameter_h__
#define __SamToolParameter_h__

#include "AModel.h"
#include "AString.h"
#include "ALog.h"
using namespace Atom;

class SamToolParameter
{
public:
	enum ToolParameterTypes {
		Discrete = 0x100
	};

	SamToolParameter(int id = -1, AString name = "", AString unit = "", int type = 0, float minValue = 0.0f, float maxValue = 0.0f, int displayPrecision = 2);
	SamToolParameter(const SamToolParameter &parameter);

	inline int id(void) { return m_id; }

	float value(void) { return m_value; }
	void setValue(float value) { m_value = value; }

	inline AString name(void) { return m_name; }
	inline AString unit(void) { return m_unit; }

	inline int type(void) { return m_type; }

	inline float minValue(void) { return m_minValue; }
	inline float maxValue(void) { return m_maxValue; }

	inline int displayPrecision(void) { return m_displayPrecision; }

	float normalisedFromValue(float value);
	float valueFromNormalised(float normalisedValue);

	static float round(const float &number, const int num_digits);

	int midiController(void) { return m_midiController; }
	void setMidiController(int controller);

	float midiMin(void) { return m_midiMin; }
	void setMidiMin(float value) { m_midiMin = value; }

	float midiMax(void) { return m_midiMax; }
	void setMidiMax(float value) { m_midiMax = value; }

protected:
private:
	int m_id;
	float m_value;
	AString m_name;
	AString m_unit;
	int m_type;
	float m_minValue;
	float m_maxValue;
	int m_displayPrecision;
	int m_midiController;
	float m_midiMin;
	float m_midiMax;
};

#endif