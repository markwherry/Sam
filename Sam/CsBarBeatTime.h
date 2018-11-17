#ifndef __CsBarBeatTime_h__
#define __CsBarBeatTime_h__

#include "CsTypes.h"

namespace Cs {

class CsBarBeatTime
{
public:
    CsBarBeatTime(int ppqnResolution, int bar = 0, int beat = 0, int sixteenth = 0, int tick = 0);
    ~CsBarBeatTime();

	inline int bar (void) const { return m_bar; }
	inline int beat (void) const { return m_beat; }
	inline int sixteenth (void) const { return m_sixteenth; }
	inline int tick (void) const { return m_tick; }

	void setBar (int bar) { m_bar = bar; }
	void setBeat (int beat) { m_beat = beat; }
	void setSixteenth (int sixteenth) { m_sixteenth = sixteenth; }
	void setTick (int tick) { m_tick = tick; }

	void setTime (int bar, int beat, int sixteenth, int tick) { m_bar = bar; m_beat = beat; m_sixteenth = sixteenth; m_tick = tick; }

	CsPpqnTime toTicks(int ppqnResolution, int numerator, int denominator);

	//QString getBBAsString (void);
	//QString getBBElementAsString (int element);

	void debug(void);

	static CsPpqnTime ticksFromNanos(CsNanoTime time, int ppqnResolution, float tempo);
	static CsNanoTime nanosFromTicks(CsPpqnTime ticks, int ppqnResolution, float tempo);
	static CsBarBeatTime bbFromTicks(CsPpqnTime time, int ppqnResolution, int numerator, int denominator);

private:
	int m_ppqnResolution;
    int m_bar;
	int m_beat;
	int m_sixteenth;
	int m_tick;
};

}

#endif // __CsBarBeatTime_h__
