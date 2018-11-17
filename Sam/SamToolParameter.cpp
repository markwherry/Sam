#include "SamToolParameter.h"

#include <math.h>

#include "SamEngine.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamToolParameter::SamToolParameter(int id, AString name, AString unit, int type, float minValue, float maxValue, int displayPrecision)
	: m_id(id) 
	, m_name(name)
	, m_unit(unit)
	, m_type(type)
	, m_minValue(minValue)
	, m_maxValue(maxValue)
	, m_displayPrecision(displayPrecision)
	, m_midiController(-1)
	, m_midiMin(0.0f)
	, m_midiMax(1.0f)
{
	//csDebug("SamToolParameter::SamToolParameter()");
	//m_name = name;
	//m_unit = unit;
	//csDebug("EXIT: SamToolParameter::SamToolParameter()");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamToolParameter::SamToolParameter(const SamToolParameter &parameter)
{
	m_id = parameter.m_id;
	m_name = parameter.m_name;
	m_unit = parameter.m_unit;
	m_type = parameter.m_type;
	m_minValue = parameter.m_minValue;
	m_maxValue = parameter.m_maxValue;
	m_displayPrecision = parameter.m_displayPrecision;
	m_midiController = parameter.m_midiController;
	m_midiMin = parameter.m_midiMin;
	m_midiMax = parameter.m_midiMax;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float SamToolParameter::normalisedFromValue(float value)
{
	float range = m_maxValue - m_minValue;
	float diff = m_maxValue - value;

	return (1.0f/range) * (range - diff);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float SamToolParameter::valueFromNormalised(float normalisedValue)
{
	return round(m_minValue+((m_maxValue-m_minValue)*normalisedValue), m_displayPrecision);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float SamToolParameter::round(const float &number, const int num_digits)
{
	float doComplete5i, doComplete5(number * powf(10.0f, (float) (num_digits + 1)));

	if(number < 0.0f) {
		doComplete5 -= 5.0f;
	} else {
		doComplete5 += 5.0f;
	}
	doComplete5 /= 10.0f;
	modff(doComplete5, &doComplete5i);
    
	return doComplete5i / powf(10.0f, (float) num_digits);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamToolParameter::setMidiController(int controller)
{
	m_midiController = controller;
}