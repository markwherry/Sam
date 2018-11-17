#ifndef __SfzInstrument_h__
#define __SfzInstrument_h__

// http://www.cakewalk.com/devXchange/article.aspx?aid=108

#include <AFile.h>
#include <AList.h>
#include <ATypes.h>
using namespace Atom;

namespace Sfz {

	class Definition {
	public:
		Definition();
		void initWithDefaultValues(void);
		
		// Sample Definition
		AString sample;

		// Input Controls
		int lochan, hichan;
		int lokey, hikey; // key
		int lovel, hivel;
		int lobend, hibend;
		int lochanaft, hichanaft;
		int lopolyaft, hipolyaft;
		float lorand, hirand;
		float lobpm, hibpm;
		int seq_length;
		int seq_position;
		int sw_lokey, sw_hikey;
		int sw_last;
		int sw_down;
		int sw_up;
		int sw_previous;
		AString sw_vel;
		AString trigger;
		int group;
		int off_by;
		AString off_mode;
		int on_loccN, on_hiccN;

		// Performance Parameters
		// Sample Player
		float delay;
		float delay_random;
		float delay_ccN;
		int offset;
		int offset_random;
		int offset_ccN;
		int end;
		int count;
		AString loop_mode;
		int loop_start;
		int loop_end;
		float sync_beats;
		float sync_offset;

		// Pitch
		int transpose;
		int tune;
		int pitch_keycenter;
		int pitch_keytrack;
		int pitch_veltrack;
		int pitch_random;
		int bend_up;
		int bend_down;
		int bend_step;

		// Pitch EG
		float pitcheg_delay;
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
		float fillfo_freqpolyaft;

		// Amplifier
		float volume;
		float pan;
		float width;
		float position;
		float amp_keytrack;
		int amp_keycenter;
		float amp_veltrack;
		float amp_velcurve_1, amp_velcurve_127;
		float amp_random;
		float rt_decay;
		int output;
		float gain_ccN;
		int xfin_lokey, xfin_hikey;
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
		float effect2;


	
	};

	class Region : public Definition {
	};

	class Group : public Definition {
	};

	class Instrument : public AFile {
	public:
		Instrument(AString filename);

		AStatus read(void);

		int numRegions(void) { return m_regions.size(); }
		Region* region(int index) { return m_regions[index]; }

		enum {
			headerUnknown = 0,
			headerControl = 1,
			headerGroup   = 2,
			headerRegion  = 3
		};

		AString default_path;

	private:
		void parseCommand(Definition *def, AString command);

		AList<Group*> m_groups;
		AList<Region*> m_regions;
	};

}

#endif