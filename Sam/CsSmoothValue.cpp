#include "CsSmoothValue.h"

namespace Cs {

CsSmoothValue::CsSmoothValue(int frames)
: m_frames(frames)
, m_value(1.0f)
, m_moving(false)
, m_step(0.0f)
, m_counter(0)
{
}

float CsSmoothValue::value(void)
{
	if(m_moving) {
		return nextFrame();
	}

	return m_value;
}

void CsSmoothValue::updateValue(float value, int frame)
{
	if(value == m_value) {
		m_moving = false;
		return;
	}

	float diff = value - m_value;
	m_step = diff/m_frames;
	m_moving = true;
	m_counter = m_frames;
}

float CsSmoothValue::nextFrame(void)
{
	m_value += m_step;
	m_counter--;
	if(m_counter < 1) {
		m_moving = false;
	}
	return m_value;
}

}