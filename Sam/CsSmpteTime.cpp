#include "CsSmpteTime.h"
#include "ALog.h"

#include <math.h>

namespace Cs {

CsSmpteTime::CsSmpteTime(double fps, int subFrames)
{
	m_fps       = fps;
	m_subFrames = subFrames;
	setTimeInSmpte (0,0,0,0,0);
}

CsSmpteTime::~CsSmpteTime()
{

}

void CsSmpteTime::setTimeInSmpte (int hour, int minute, int second, int frame, int subFrame)
{
	m_hour     = hour;
	m_minute   = minute;
	m_second   = second;
	m_frame    = frame;
	m_subFrame = subFrame;
}

void CsSmpteTime::setTimeInNanos (CsNanoTime _timeNano)
{
	//qDebug ("CsSMPTETime::setTimeAsNano (%lld)\n", _timeNano);
	setTimeInSmpte (0,0,0,0,0);

	double nanoPerFrame = 1000000000/m_fps;
	//qDebug ("nanoPerFrame=%f\n", nanoPerFrame);

	double nanoPerSubFrame = 1000000000/(m_fps*m_subFrames);
	//qDebug ("nanoPerSubFrame=%f\n", nanoPerSubFrame);

	double nanoPerSecond;
	if (m_fps==29.97)
	{
		//qDebug ("fps==29.97 (%f)\n",fps);
		nanoPerSecond = round(nanoPerFrame*30,1);
	}
	else
		nanoPerSecond = round(nanoPerFrame*m_fps,1);
	double nanoPerMinute = nanoPerSecond*60;
	double nanoPerHour   = nanoPerMinute*60;

	double nanoRemaining = (double)_timeNano;
	//qDebug ("%f\n",nanoRemaining);
	//qDebug ("%lld\n", _timeNano);

	bool counting = true;
	while (counting)
	{
		if (nanoRemaining-nanoPerHour<0)
		{
			counting=false;
		}
		else
		{
			nanoRemaining-=nanoPerHour;
			m_hour++;
		}
	}

	counting=true;
	while (counting)
	{
		if (nanoRemaining-nanoPerMinute<0)
		{
			counting=false;
		}
		else
		{
			nanoRemaining-=nanoPerMinute;
			m_minute++;
		}
	}

	counting=true;
	while (counting)
	{
		if (nanoRemaining-nanoPerSecond<0)
		{
			counting = false;
		}
		else
		{
			nanoRemaining-=nanoPerSecond;
			m_second++;
		}
	}

	counting=true;
	while (counting)
	{
		if (nanoRemaining-nanoPerFrame<0)
		{
			counting = false;
		}
		else
		{
			nanoRemaining-=nanoPerFrame;
			m_frame++;
		}
	}

	counting=true;
	while (counting)
	{
		if (nanoRemaining-nanoPerSubFrame<0)
		{
			counting = false;
		}
		else
		{
			nanoRemaining-=nanoPerSubFrame;
			m_subFrame++;
		}
	}
}

void CsSmpteTime::setTimeInSamples(CsSampleTime _timeSamples, double _sampleRate)
{
	aLog("fps=(%f)\n",m_fps);
	//qDebug ("CsSMPTETime::setTimeInSamples (%lld)\n", _timeSamples);
	setTimeInSmpte (0,0,0,0,0);

	double samplesPerFrame = _sampleRate/m_fps;
	//qDebug ("nanoPerFrame=%f\n", nanoPerFrame);

	double samplesPerSubFrame = _sampleRate/(m_fps*m_subFrames);
	//qDebug ("nanoPerSubFrame=%f\n", nanoPerSubFrame);

	double samplesPerSecond;
	if (m_fps==29.97)
	{
		//qDebug ("fps==29.97 (%f)\n",fps);
		samplesPerSecond = (double)round((float)samplesPerFrame*30,1);
	}
	else
		samplesPerSecond = (double)round((float)samplesPerFrame*m_fps,1);

	double samplesPerMinute = samplesPerSecond*60;
	double samplesPerHour   = samplesPerMinute*60;

	double samplesRemaining = (double)_timeSamples;

	bool counting = true;
	while (counting)
	{
		if (samplesRemaining-samplesPerHour<0)
		{
			counting=false;
		}
		else
		{
			samplesRemaining-=samplesPerHour;
			m_hour++;
		}
	}

	counting=true;
	while (counting)
	{
		if (samplesRemaining-samplesPerMinute<0)
		{
			counting=false;
		}
		else
		{
			samplesRemaining-=samplesPerMinute;
			m_minute++;
		}
	}

	counting=true;
	while (counting)
	{
		if (samplesRemaining-samplesPerSecond<0)
		{
			counting = false;
		}
		else
		{
			samplesRemaining-=samplesPerSecond;
			m_second++;
		}
	}

	counting=true;
	while (counting)
	{
		if (samplesRemaining-samplesPerFrame<0)
		{
			counting = false;
		}
		else
		{
			samplesRemaining-=samplesPerFrame;
			m_frame++;
		}
	}

	counting=true;
	while (counting)
	{
		if (samplesRemaining-samplesPerSubFrame<0)
		{
			counting = false;
		}
		else
		{
			samplesRemaining-=samplesPerSubFrame;
			m_subFrame++;
		}
	}
}

void CsSmpteTime::debug(void)
{
	aLog("%d:%d:%d:%d.%d\n", m_hour, m_minute, m_second, m_frame, m_subFrame);
}

/*QString CsSmpteTime::getTCElementAsString (int _element)
{
	QString str;

	int value;

	if (_element==0)
		value = m_hour;
	if (_element==1)
		value = m_minute;
	if (_element==2)
		value = m_second;
	if (_element==3)
		value = m_frame;
	if (_element==4)
		value = m_subFrame;

	if (value<10)
		str.append ("0");
	
	str.append (QString::number(value));

	return str;
}*/

/*QString CsSmpteTime::getTCAsString (bool _withSubFrames)
{
	QString str;

	//s << getTCElementAsString(0) << ":" << getTCElementAsString(1) << ":" << getTCElementAsString(2) << ":" << getTCElementAsString(3);

	//if (_withSubFrames)
	//	s << "." << getTCElementAsString(4);

	for (int i=0; i<4; i++)
	{
		str.append (getTCElementAsString(i));
		if (i!=3)
			str.append(":");
	}

	if (_withSubFrames)
	{
		str.append(".");
		str.append (getTCElementAsString (4));
	}

	return str;
}*/

double CsSmpteTime::round(const double &number, const int num_digits)
{
    float doComplete5i, doComplete5(number * powf(10.0f, (double) (num_digits + 1)));
    
    if(number < 0.0f)
        doComplete5 -= 5.0f;
    else
        doComplete5 += 5.0f;
    
    doComplete5 /= 10.0f;
    modff(doComplete5, &doComplete5i);
    
    return doComplete5i / powf(10.0f, (double) num_digits);
}

}