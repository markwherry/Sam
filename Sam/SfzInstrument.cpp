// SfzInstrument.cpp

#include "SfzInstrument.h"

#include "ALog.h"

namespace Sfz {

Definition::Definition()
{
	initWithDefaultValues();
}

void Definition::initWithDefaultValues(void)
{
		
	// Sample Definition
	// sample = "";

	// Input Controls
	lochan = 1;     hichan = 16;
	lokey = 0;      hikey = 127;
	lovel = 0;      hivel = 127;
	lobend = -8192; hibend = 8192;
	lochanaft = 0;  hichanaft = 127;
	lopolyaft = 0;  hipolyaft = 127;
	lorand = 0.0f;  hirand = 1.0f;
	lobpm  = 0.0f;  hibpm = 500.0f;
	seq_length = 1;
	seq_position = 1;
	sw_lokey = 0;   sw_hikey = 127;
	sw_last = 0;
	sw_down = 0;
	sw_up = 0;
	sw_previous = 0; // None
	sw_vel = "current";
	trigger = "attack";
	group = 0;
	off_by = 0;
	off_mode = "fast";
	on_loccN = -1;  on_hiccN = -1;

	// Performance Parameters
	// Sample Player
	delay = 0.0f;
	delay_random = 0.0f;
	delay_ccN = 0.0f;
	offset = 0;
	offset_random = 0;
	offset_ccN = 0;
	end = 0;
	count = 0;
	loop_mode = "no_loop";
	loop_start = 0;
	loop_end = 0;
	sync_beats = 0.0f;
	sync_offset = 0.0f;

	// Pitch
	transpose = 0;
	tune = 0;
	pitch_keycenter = 60;
	pitch_keytrack = 100;
	pitch_veltrack = 0;
	pitch_random = 0;
	bend_up = 200;
	bend_down = -200;
	bend_step = 1;

	// Pitch EG
	/*float pitcheg_delay;
	float pitcheg_start;
	float pitcheg_attack;
	float pitcheg_hold;
	float pitcheg_decay;
	float pitcheg_sustain;
	float pitcheg_release;
	int pitcheg_depth;
	float pitcheg_vel2delay;
	float pitcheg_vel2attack;
	float pitcheg_vel2hold;
	float pitcheg_vel2decay;
	float pitcheg_vel2sustain;
	float pitcheg_vel2release;
	int pitcheg_vel2depth;

	// Pitch LFO
	float pitchlfo_delay;
	float pitchlfo_fade;
	float pitchlfo_freq;
	int pitchlfo_depth;
	int pitchlfo_depthccN;
	int pitchlfo_depthchanaft;
	int pitchlfo_depthpolyaft;
	float pitchlfo_freqccN;
	float pitchlfo_freqchanaft;
	float pitchlfo_freqpolyaft;

	// Filter
	AString fil_type;
	float cutoff;
	int cutoff_ccN;
	int cutoff_chanaft;
	int cutoff_polyaft;
	float resonance;
	int fil_keytrack;
	int fil_keycenter;
	int fil_veltrack;
	int fil_random;

	// Filter EG
	float fileg_delay;
	float fileg_start;
	float fileg_attack;
	float fileg_hold;
	float fileg_decay;
	float fileg_sustain;
	float fileg_release;
	int fileg_depth;
	float fileg_vel2delay;
	float fileg_vel2attack;
	float fileg_vel2hold;
	float fileg_vel2decay;
	float fileg_vel2sustain;
	float fileg_vel2release;
	int fileg_vel2depth;

	// Filter LFO
	float fillfo_delay;
	float fillfo_fade;
	float fillfo_freq;
	int fillfo_depth;
	int fillfo_depthccN;
	int fillfo_depthchanaft;
	int fillfo_depthpolyaft;
	float fillfo_freqccN;
	float fillfo_freqchanaft;
	float fillfo_freqpolyaft;*/

	// Amplifier
	volume = 0.0f;
	pan = 0.0f;
	width = 0.0f;
	position = 0.0f;
	amp_keytrack = 0.0f;
	amp_keycenter = 60;
	amp_veltrack = 100.0f;
	amp_velcurve_1 = 1.0f;  amp_velcurve_127 = 1.0f;
	amp_random = 0.0f;
	rt_decay = 0.0f;
	output = 0;
	gain_ccN = 0.0f;
	/*int xfin_lokey, xfin_hikey;
	int xfout_lokey, xfout_hikey;
	int xf_keycurve;
	int xfin_lovel, xfin_hivel;
	int xfout_lovel, xfout_hivel;
	AString xf_velcurve;
	int xfin_loccN, xfin_hiccN;
	int xfout_loccN, xfout_hiccN;
	AString xf_cccurve;

	// Amplifier EG
	float ampeg_delay;
	float ampeg_start;
	float ampeg_attack;
	float ampeg_hold;
	float ampeg_decay;
	float ampeg_sustain;
	float ampeg_release;
	float ampeg_vel2delay;
	float ampeg_vel2attack;
	float ampeg_vel2hold;
	float ampeg_vel2decay;
	float ampeg_vel2sustain;
	float ampeg_vel2release;
	float ampeg_delayccN;
	float ampeg_startccN;
	float ampeg_attackccN;
	float ampeg_holdccN;
	float ampeg_decayccN;
	float ampeg_sustainccN;
	float ampeg_releaseccN;


	// Amplifier LFO
	float amplfo_delay;
	float amplfo_fade;
	float amplfo_freq;
	int amplfo_depth;
	int amplfo_depthccN;
	int amplfo_depthchanaft;
	int amplfo_depthpolyaft;
	float amplfo_freqccN;
	float amplfo_freqchanaft;
	float amplfo_freqpolyaft;

	// Equalizer
	float eq1_freq, eq2_freq, eq3_freq;
	float eq1_freqccN, eq2_freqccN, eq3_freqccN;
	float eq1_vel2freq, eq2_vel2freq, eq3_vel2freq;
	float eq1_bw, eq2_bw, eq3_bw;
	float eq1_bwccN, eq2_bwccN, eq3_bwccN;
	float eq1_gain, eq2_gain, eq3_gain;
	float eq1_gainccN, eq2_gainccN, eq3_gainccN;
	float eq1_vel2gain, eq2_vel2gain, eq3_vel2gain;

	// Effects
	float effect1;
	float effect2;*/



}

Instrument::Instrument(AString filename)
: AFile(filename)
{
}

AStatus Instrument::read(void)
{
	aLog("Sfz::Instrument::read()");
	if(open()==AError) {
		aLog("Error opening.");
		return AError;
	}

	char *data = (char*)malloc(size()+1);
	readBytes(size(), data);
	data[size()] = 0;

	close();

	AString def(data);

	//aLog("length = %d", def.length(), def.toChar());


	AList<AString> lines;
	AString line;
	for(int i=0; i<def.length(); i++) {
		if(def[i]==10 || def[i]==13) {
			if(line.length() > 0) {
				lines.append(line);
				line.clear();
			}
		} else {
			line.append(def[i]);
		}
	}

	AString token;
	Definition *currentDef = 0;

	for(int i=0; i<lines.size(); i++) {
		//aLog("%d [%s]", i, lines[i].toChar());
		AString line = lines[i];

		int commentPos = line.findCharacter('/', AString::fromFront);

		if(commentPos > -1) {
			//aLog("commentPos = %d", commentPos);
			if(commentPos == 0) {
				line.clear();
			} else {
				line = line.first(commentPos);
			}
			//aLog("No comment. [%s]", line.toChar());
		}

		for(int j=0; j<line.length(); j++) {
			
			if(line[j] == 32 || j == line.length()-1) {
				
				bool tokenComplete = true;

				if(j == line.length()-1) {
					token.append(line[j]);
				} else {
					AString remainder = line.last(line.length()-j);
					//aLog("remainder=[%s]", remainder.toChar());
					if(remainder.findCharacter('=', AString::fromFront) < 0) {
						token.append(line[j]);
						tokenComplete = false;
					}


				}

				if(tokenComplete) {
					aLog("[%s]", token.toChar());
					

					if(token.isEqualTo("<control>")) {
						aLog("<control>");
					} else if(token.isEqualTo("<group>")) {
						aLog("<group>");
						Group *group = new Group();
						m_groups.append(group);
						currentDef = (Definition*)group;
					} if(token.isEqualTo("<region>")) {
						aLog("<region>");
						Region *region = new Region();
						m_regions.append(region);
						currentDef = (Definition*)region;
					} else {
						parseCommand(currentDef, token);
					}

					token.clear();
				}

			} else {
				token.append(line[j]);
			}
		}
	}

	/*enum {
		nowhere   = 0,
		inHeader  = 1,
		inComment = 2,
		inCommand = 3
	};

	
	int where = nowhere;
	AString header;
	AString command;
	Definition *currentDef = 0;
	for(int i=0; i<def.length(); i++) {
		aLog("[%c] %d", def[i], def[i]);
		switch(def[i]) {
			case '/':
				if(def[i+=1] == '/') {
					aLog("Comment");
					where = inComment;
				}
				break;
			case 13:
			case 10:
				//aLog("New Line");
				if(where == inCommand) {
					aLog("Command [%s]", command.toChar());
					parseCommand(currentDef, command);
				}
				where = nowhere;
				break;
			case '<':
				//aLog("Header");
				where = inHeader;
				header.clear();
				break;
			case '>':
				if(where == inHeader) {
					aLog("Header <%s>", header.toChar());
					if(header.isEqualTo("region")) {
						Region *region = new Region();
						m_regions.append(region);
						currentDef = (Definition*)region;
					} else if(header.isEqualTo("group")) {
						Group *group = new Group();
						m_groups.append(group);
						currentDef = (Definition*)group;
					}
					where = nowhere;
				}

				break;
			default:
				if(where == nowhere) {
					if(def[i] != 10 && def[i] != 13 && def[i] != ' ') {
						where = inCommand;
						command.clear();
					}
				}

				if(where == inHeader) {
					header.append(def[i]);
				} else if(where == inCommand) {
					command.append(def[i]);
				}

				break;

		}
	}*/

	return AOk;
}

void Instrument::parseCommand(Definition *def, AString command)
{
	aLog("Instrument::parseCommand(%d, %s)", def, command.toChar());
	int pos = command.findCharacter('=', AString::fromFront);
	if(pos < 1) {
		return;
	}
	AString opcode = command.first(pos);
	AString value = command.last(command.length()-pos-1);

	aLog("opcode[%s] value[%s]", opcode.toChar(), value.toChar());

	// Control
	if(opcode.isEqualTo("default_path")) {
		default_path = value;
		if(default_path[default_path.length()-1] == '\\') {
			default_path = default_path.first(default_path.length()-1);
			default_path.append('/');
		}

	}

	if(!def) return;

	//
	if(opcode.isEqualTo("sample")) def->sample = value;
	else if(opcode.isEqualTo("lokey")) { def->lokey = value.toInt(); aLog("lokey = %d", def->lokey); }
	else if(opcode.isEqualTo("hikey")) { def->hikey = value.toInt(); aLog("hikey = %d", def->hikey); }
	else if(opcode.isEqualTo("pitch_keycenter")) { def->pitch_keycenter = value.toInt(); aLog("pitch_keycenter = %d", def->hikey); }
	else if(opcode.isEqualTo("tune")) def->tune = value.toInt();
}

}