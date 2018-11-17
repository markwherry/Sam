#ifndef __CsSmpteTime_h__
#define __CsSmpteTime_h__

#include "CsTypes.h"

namespace Cs {

class CsSmpteTime
{
public:
    CsSmpteTime(double fps, int subFrames = 100);
    ~CsSmpteTime();

	void setTimeInSmpte (int hour, int minute, int second, int frame, int subFrame);
	void setTimeInNanos (CsNanoTime timeNano);
	void setTimeInSamples (CsSampleTime timeSamples, double sampleRate);

	void debug (void);

	inline int hour (void) const { return m_hour; }
	inline int minute (void) const { return m_minute; }
	inline int second (void) const { return m_second; }
	inline int frame (void) const { return m_frame; }
	inline int subFrame (void) const { return m_subFrame; }

	//QString getTCElementAsString (int _element);
	//QString getTCAsString (bool _withSubFrames);

private:
    double round(const double &number, const int num_digits);
	unsigned int m_hour;
	unsigned int m_minute;
	unsigned int m_second;
	unsigned int m_frame;
	unsigned int m_subFrame;
	double m_fps;
	unsigned int m_subFrames;
};

}

#endif // __CsSmpteTime_h__
