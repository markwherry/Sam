#include "CsBarBeatTime.h"
#include <ALog.h>

namespace Cs {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsBarBeatTime::CsBarBeatTime(int ppqnResolution, int bar, int beat, int sixteenth, int tick)
	: m_ppqnResolution(ppqnResolution)
	, m_bar(bar)
	, m_beat(beat)
	, m_sixteenth(sixteenth)
	, m_tick(tick)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsBarBeatTime::~CsBarBeatTime()
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*QString CsBarBeatTime::getBBElementAsString (int element)
{
	QString str;

	int value;

	if (element==0)
		value = m_bar;
	if (element==1)
		value = m_beat;
	if (element==2)
		value = m_sixteenth;
	if (element==3)
		value = m_tick;
	
	str.append (QString::number(value+1));

	return str;
}*/

/*QString CsBarBeatTime::getBBAsString (void)
{
	QString str;

	for (int i=0; i<4; i++)
	{
		str.append (getBBElementAsString(i));
		if (i!=3)
			str.append("|");
	}

	return str;
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsBarBeatTime::debug(void)
{
	aLog("CsBarBeatTime::debug (%d|%d|%d|%d)", m_bar, m_beat, m_sixteenth, m_tick);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsPpqnTime CsBarBeatTime::ticksFromNanos(CsNanoTime time, int ppqnResolution, float tempo)
{
	//double t = (double)time;

	double ticksPerMinute = (double)ppqnResolution * (double)tempo;
	//qDebug ("ticksPerMinute=%f\n",ticksPerMinute);
	double ticksPerSecond = (double)ticksPerMinute / (double)60;
	//qDebug ("ticksPerSecond=%f\n",ticksPerSecond);
	double ticksPerNano = (double)ticksPerSecond / (double)1000000000;
	//qDebug ("ticksPerNano=%f\n",ticksPerNano);
	
	//CSDEBUG("ticksPerSecond = %f\n", ticksPerNano * 1000000000);

	double outDouble = ticksPerNano * time;
	
	CsPpqnTime outInteger = (CsPpqnTime)outDouble;

	if(outDouble-outInteger>0)
		return outInteger+1;

	return outDouble;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsNanoTime CsBarBeatTime::nanosFromTicks(CsPpqnTime ticks, int ppqnResolution, float tempo)
{
	//qDebug ("CsProject::getAbsoluteTimeFromTicks ()\n");

	double qnPerSec = (double)tempo/(double)60;
	//qDebug ("qnPerSec = %f\n", qnPerSec);

	double ticksPerSec = (double)ppqnResolution * qnPerSec;
	//qDebug ("ticksPerSec = %f\n", ticksPerSec);

	double nsPerTick = (double)1000000000 / ticksPerSec;
	//qDebug ("nsPerTick = %f\n", nsPerTick);

	//qDebug ("%f\n", nsPerTick*_ticks);
	CsNanoTime nsTime = (CsNanoTime)((double)nsPerTick*ticks);

	return nsTime;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsBarBeatTime CsBarBeatTime::bbFromTicks(CsPpqnTime time, int ppqnResolution, int numerator, int denominator)
{
	CsBarBeatTime bbTime(ppqnResolution);

	double ticksRemaining = (double)time;

	double beatDivisor = (double)denominator / 4;

	double ticksPerBeat = (double)ppqnResolution/beatDivisor;

	double ticksPerBar  = ticksPerBeat * numerator;

	double sixteenthDivisor = 16/denominator;
	
	double ticksPerSixteenth = ticksPerBeat/sixteenthDivisor;

	bool counting = true;
	while (counting)
	{
		//qDebug ("[Bar] ticksRemaining = %d (%d<0)", ticksRemaining, (ticksRemaining-ticksPerBar));
		if (ticksRemaining-ticksPerBar<0)
		{
			//qDebug ("FALSE");
			counting=false;
		}
		else
		{
			//qDebug ("-");
			ticksRemaining-=ticksPerBar;
			bbTime.setBar (bbTime.bar()+1);
		}
	}

//bbTime.debug ();
	
counting = true;
	while (counting)
	{
		if (ticksRemaining-ticksPerBeat<0)
		{
			counting=false;
		}
		else
		{
			ticksRemaining-=ticksPerBeat;
			bbTime.setBeat (bbTime.beat()+1);
		}
	}

	//int leftOverBeats
	//if (bbTimer.beat()>)


//bbTime.debug ();
	counting = true;
	while (counting)
	{
		if (ticksRemaining-ticksPerSixteenth<0)
		{
			counting=false;
		}
		else
		{
			ticksRemaining-=ticksPerSixteenth;
			bbTime.setSixteenth (bbTime.sixteenth()+1);
		}
	}

	bbTime.setTick (ticksRemaining);

	return bbTime;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsPpqnTime CsBarBeatTime::toTicks(int ppqnResolution, int numerator, int denominator)
{
	CsPpqnTime returnTime = 0;

	CsPpqnTime ticksPerDenominator = (denominator/4)*ppqnResolution; 
	returnTime += m_bar*(ticksPerDenominator*numerator);

	returnTime += m_beat * ticksPerDenominator;

	if(denominator<16) returnTime += m_sixteenth * (ppqnResolution/4);

	returnTime+= m_tick;

	return returnTime;
}

}