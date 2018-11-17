#include "CsMidiName.h"

Atom::AString CsMidiName::pitchToName(int pitch, int middleCOctave, int namingScheme)
{
	Atom::AString noteName[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

	int noteNumber = pitch%12;
	int octave = pitch/12;

	Atom::AString str = noteName[noteNumber];
	char buffer[4];
	str.append(itoa((octave+middleCOctave)-5,buffer,10));
		//QString::number((octave+middleCOctave)-5);

	return str;
}

Atom::AString CsMidiName::controllerToName(int controller, int type)
{
	static Atom::AString controllerName[256] = {
		"Bank Select MSB", "Bank Select MSB",
		"Modulation Wheel", "Modulation",
		"Breath", "Breath",
		"Controller 3", "Ctrl 3",
		"Foot Pedal", "Foot",
		"Portamento", "Portamento",
		"Data Entry", "Data Entry",
		"Volume", "Volume",

		// 8
		"Balance", "Balance",
		"Controller 9", "Ctrl 9",
		"Pan", "Pan",
		"Expression", "Expression",
		"Effect Control 1", "Effect 1",
		"Effect Control 2", "Effect 2",
		"Controller 14", "Ctrl 14",
		"Controller 15", "Ctrl 15",

		// 16
		"General Purpose Slider 1", "General 1",
		"General Purpose Slider 2", "General 2",
		"General Purpose Slider 3", "General 3",
		"General Purpose Slider 4", "General 4",
		"Controller 20", "Ctrl 20",
		"Controller 21", "Ctrl 21",
		"Controller 22", "Ctrl 22",
		"Controller 23", "Ctrl 23",

		// 24
		"Controller 24", "Ctrl 24",
		"Controller 25", "Ctrl 25",
		"Controller 26", "Ctrl 26",
		"Controller 27", "Ctrl 27",
		"Controller 28", "Ctrl 28",
		"Controller 29", "Ctrl 29",
		"Controller 30", "Ctrl 30",
		"Controller 31", "Ctrl 31",

		// 32
		"Bank Select LSB", "Bank Select LSB",
		"Modulation Wheel LSB", "Modulation LSB",
		"Breath LSB", "Breath LSB",
		"Controller 35", "Ctrl 35",
		"Foot Pedal LSB", "Foot LSB",
		"Portamento LSB", "Portamento LSB",
		"Data Entry LSB", "Data Entry LSB",
		"Volume LSB", "Volume LSB",

		// 40
		"Balance LSB", "Balance LSB",
		"Controller 41", "Ctrl 41",
		"Pan LSB", "Pan LSB",
		"Expression LSB", "Expression LSB",
		"Effect Control 1 LSB", "Effect 1 LSB",
		"Effect Control 2 LSB", "Effect 2 LSB",
		"Controller 46", "Ctrl 46",
		"Controller 47", "Ctrl 47",

		// 48
		"Controller 48", "Ctrl 48",
		"Controller 49", "Ctrl 49",
		"Controller 50", "Ctrl 50",
		"Controller 51", "Ctrl 51",
		"Controller 52", "Ctrl 52",
		"Controller 53", "Ctrl 53",
		"Controller 54", "Ctrl 54",
		"Controller 55", "Ctrl 55",

		// 56
		"Controller 56", "Ctrl 56",
		"Controller 57", "Ctrl 57",
		"Controller 58", "Ctrl 58",
		"Controller 59", "Ctrl 59",
		"Controller 60", "Ctrl 60",
		"Controller 61", "Ctrl 61",
		"Controller 62", "Ctrl 62",
		"Controller 63", "Ctrl 63",

		// 64
		"Sustain Pedal", "Sustain",
		"Portamento Switch", "Porta Switch",
		"Sostenuto Pedal", "Sostenuto",
		"Soft Pedal", "Soft Pedal",
		"Legato Pedal", "Legato Pedal",
		"Hold 2 Pedal", "Hold 2 Pedal",
		"Sound Variation", "Variation",
		"Sound Timbre", "Timbre",

		// 72
		"Sound Release Time", "Release",
		"Sound Attack Time", "Attack",
		"Sound Brightness", "Brightness",
		"Sound Control 6", "Sound Ctrl 6",
		"Sound Control 7", "Sound Ctrl 7",
		"Sound Control 8", "Sound Ctrl 8",
		"Sound Control 9", "Sound Ctrl 9",
		"Sound Control 10", "Sound Ctrl 10",

		// 80
		"General Purpose Button 1", "General 5",
		"General Purpose Button 2", "General 6",
		"General Purpose Button 3", "General 7",
		"General Purpose Button 4", "General 8",
		"Controller 84", "Ctrl 84",
		"Controller 85", "Ctrl 85",
		"Controller 86", "Ctrl 86",
		"Controller 87", "Ctrl 87",

		// 88
		"Controller 88", "Ctrl 88",
		"Controller 89", "Ctrl 89",
		"Controller 90", "Ctrl 90",
		"Effects Level", "Effects",
		"Tremolo Level", "Tremolo",
		"Chorus Level", "Chorus",
		"Celeste Level", "Celeste",
		"Phaser Level", "Phaser",

		// 96
		"Data Button Increment", "Data Button +",
		"Data Button Decrement", "Data Button -",
		"Non-registered Parameter LSB", "Non-reg Par LSB",
		"Non-registered Parameter MSB", "Non-reg Par MSB",
		"Registered Parameter LSB", "Reg Par LSB",
		"Registered Parameter MSB", "Reg Par MSB",
		"Controller 102", "Ctrl 102",
		"Controller 103", "Ctrl 103",

		// 104
		"Controller 104", "Ctrl 104",
		"Controller 105", "Ctrl 105",
		"Controller 106", "Ctrl 106",
		"Controller 107", "Ctrl 107",
		"Controller 108", "Ctrl 108",
		"Controller 109", "Ctrl 109",
		"Controller 110", "Ctrl 110",
		"Controller 111", "Ctrl 111",

		// 112
		"Controller 112", "Ctrl 112",
		"Controller 113", "Ctrl 113",
		"Controller 114", "Ctrl 114",
		"Controller 115", "Ctrl 115",
		"Controller 116", "Ctrl 116",
		"Controller 117", "Ctrl 117",
		"Controller 118", "Ctrl 118",
		"Controller 119", "Ctrl 119",

		// 120
		"All Sound Off", "All Sound Off",
		"All Controllers Off", "Controllers Off",
		"Local Keyboard Switch", "Local Switch",
		"All Notes Off", "All Notes Off",
		"Omni Mode Off", "Omni Mode Off",
		"Omni Mode On", "Omni Mode On",
		"Mono Operation", "Mono Operation",
		"Poly Operation", "Poly Operation"
	};

	if(controller<0 || controller>127)
		return "";

	int ctrl = (controller*2)+type;

	return controllerName[ctrl];
}