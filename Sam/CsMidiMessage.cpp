#include "CsMidiMessage.h"

#include "APlatform.h"

namespace Cs {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsMidiMessage::CsMidiMessage(int t)
: type(t)
, length(0)
, timestamp(0)
, handle(0)
, device(0)
, port(0)
, channel(0)
//, user(0)
{
	memset(data, 0, 256);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsMidiMessage::identify(void)
{
	if(data[0] < 0xF0) {
		channel = data[0] & 0x0F;

		type = data[0] & 0xF0;
		if(type<0x80) {
			type = Unidentified;
		}
	} else {
		type = data[0];
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsMidiMessage::defineLength(void)
{
	if(type == Unidentified) {
		identify();
	}

	switch(type) {
		case NoteOff:
			length = 3;
			break;
		case NoteOn:
			length = 3;
			break;
		case Aftertouch:
			length = 3;
			break;
		case Controller:
			length = 3;
			break;
		case ProgramChange:
			length = 2;
			break;
		case ChannelPressure:
			length = 2;
			break;
		case PitchBend:
			length = 3;
			break;
	}

	return length;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
unsigned short CsMidiMessage::twoByteValue(void)
{
	unsigned short value = (unsigned short)data[2];
	value <<= 7;
	value |= (unsigned short)data[1];

	return value;
}

void CsMidiMessage::buildFirstByte(void)
{
	data[0] = type + channel;
}

void CsMidiMessage::buildFromWinMme(DWORD dwParam)
{
	data[0]   = dwParam & 0x000000FF;
	data[1]   = (dwParam>>8) & 0x000000FF;
	data[2]   = (dwParam>>16) & 0x000000FF;

	defineLength();
}

}