#ifndef __CsCurve_h__
#define __CsCurve_h__

#include "ALog.h"

namespace Cs {

class CsCurve
{
public:
	CsCurve(int length = 128, int shape = Linear, float start = 0.0f, float end = 1.0f);
	CsCurve(const CsCurve& curve);
	~CsCurve();

	void invert(void);

	int length(void) { return m_length; }
	void setLength(int length) { m_length = length; calculateCurve(); }
	
	int shape(void) { return m_shape; }
	void setShape(int shape) { m_shape = shape; calculateCurve(); }

	float start(void) { return m_start; }
	void setStart(float start) { m_start = start; calculateCurve(); }

	float end(void) { return m_end; }
	void setEnd(float end) { m_end = end; calculateCurve(); }

	float depth(void) { return m_q; }
	void setDepth(float q);

	float point(int index) { return m_point[index]; }

	void print(void);

	enum Shapes {
		Linear = 0,
		LinearRoot = 1,
		LinearSquared = 2,
		Bezier = 3,
		BezierRoot = 4,
		BezierSquared = 5,
	};
protected:
private:
	void calculateCurve(void);

	float *m_point;
	float m_q;
	int m_length;
	int m_shape;
	float m_start;
	float m_end;
};

}

#endif