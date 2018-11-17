#include "CsCurve.h"

#include "ALog.h"

#include <math.h>

namespace Cs {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsCurve::CsCurve(int length, int shape, float start, float end)
	: m_length(length)
	, m_shape(shape)
	, m_start(start)
	, m_end(end)

	, m_point(0)
	, m_q(0.0f)
{
	calculateCurve();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsCurve::CsCurve(const CsCurve& curve)
{
	m_length = curve.m_length;
	m_shape = curve.m_shape;
	m_start = curve.m_start;
	m_end = curve.m_end;
	m_point = 0;
	m_q = curve.m_q;

	calculateCurve();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsCurve::~CsCurve()
{
	if(m_point) {
		free(m_point);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsCurve::invert(void)
{
	for(int i=0; i<m_length; i++) {
		m_point[i] = 1.0f - m_point[i];
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsCurve::setDepth(float q)
{
	m_q = q;
	
	calculateCurve();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsCurve::calculateCurve(void)
{
	if(m_point) {
		free(m_point);
	}

	m_point = (float*)malloc(sizeof(float)*m_length);

	//csDebug("CsCurve::calculateCurve() m_length = %d", m_length);

	for(int i=0; i<m_length; i++) {
		//float value = i*(1.0/128);
		
		float t = (float)i/(m_length-1);

		float b0;
		if(m_shape >= 3 && m_shape < 6) {
			b0 = (pow((1-t),2)*m_start) + (2*(t*(1-t)*m_q)) + pow(t,2)*m_end; // Bezier
		} else {
			b0 = (1-t)*m_start+(t*m_end); // Linear
		}

		if(m_shape == 1 || m_shape == 4) {	
			b0 = sqrt(b0);
		} else if(m_shape == 2 || m_shape == 5){
			b0 = pow(b0,2);
		}

		if(m_shape == 6) {
			b0 = sin(3.14159265358979323846f/2*b0);
		}

		if(b0<0.0)b0=0.0f;
		if(b0>1.0)b0=1.0f;

		m_point[i] = b0;
		
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsCurve::print(void)
{
	aLog("CsCurve::print()");

	for(int i=0; i<m_length; i++) {
		aLog("[%d] %f", i, m_point[i]);
	}
}

}