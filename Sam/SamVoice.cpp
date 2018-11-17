#include "SamVoice.h"

#include "SamChannel.h"
#include "SamInstrument.h"
#include "SamEngine.h"
#include "SamProject.h"

#include "AMath.h"
#include <math.h>

//#define USE_FW_V

#ifdef USE_FW_V
#include <fwSignal.h>
#endif

#define USE_IPP_V
#ifdef USE_IPP_V
#include <ipps.h>
#endif

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamVoice::SamVoice(SamChannel* channel)
	: m_channel(channel)
	, m_id(-1)
	, m_pitchBendModulator(0)
	, m_playing(false)
	, m_sustained(false)
	, m_stolen(0)
	, m_pitchModulationBuffer(0)
	, m_pitchBendUp(0.0f)
	, m_pitchBendDn(0.0f)
	, m_zone(0)
	, m_filterEnabled(false)
	, m_lastExpressionValue(1.0f)
	, m_lastZoneCrossfadeValue(1.0f)
	, m_currentZoneCrossfadeMidiValue(0)
	, m_outputIndex(0)
	, m_startOffset(0)
	, m_envelopeOffset(0)
	, m_pitchPowBuffer(0)
	, m_twoBuffer(0)
	, m_reqPitchBuffer(0)
	, m_pitchMustUpdate(true)
	, m_triggeredPitch(-1)
	, m_sampleRateRatio(1.0f)
	//, m_audioToolInsert(0)
{
	//aLog("SamVoice::SamVoice");

	for(int i=0; i<maxOscillators; i++) {
		m_oscillators[i] = 0;
	}

	m_numOscillators = m_channel->instrument()->elementPool()->numSets();
	for(int i=0; i<m_numOscillators; i++) {
		//aLog("Allocating oscillator %d/%d", i+1, m_numOscillators);
		m_oscillators[i] = new SamOscillator(this, m_channel->instrument()->elementPool()->set(i)->streams(), m_channel->streamMix(), i);
	}

	m_envelope = new CsEnvelopeAhdsr(44100.0, 256);
	m_filterEnvelope = new CsEnvelopeAhdsr(44100.0, 256);

	m_filter = new CsSynthFilter(44100.0, m_channel->streamMix()->speakerConfig().numSpeakers());

	m_amplifier = new CsAmplifier(44100);
	m_amplifier->addModulationSource(m_envelope);
	m_amplifier->addModulationSource(m_channel->expressionModulator());

	m_zoneCrossfadeModulator = new CsModulator(44100.0, samEngine->audioProcessInfo().bufferSize, 32, false, 256, 0.0f);
	m_filterModulator = new CsModulator(44100.0, samEngine->audioProcessInfo().bufferSize, 32, false, 4410, 1.0f);

	m_filterModulationSource = new CsModulationSource(44100.0, samEngine->audioProcessInfo().bufferSize, 32, false);

	m_ampZoneCf = new CsAmplifier(44100);
	m_amplifier->addModulationSource(m_zoneCrossfadeModulator);


	
	// Pitch Modulation
	m_mscPitch = new CsModulationSource(44100.0, samEngine->audioProcessInfo().bufferSize, 32, false);
	m_modPitch = new CsModulator(44100.0, samEngine->audioProcessInfo().bufferSize, 32, false, 4410, 1.0f);
	m_lfoPitch = new CsLfo(44100.0, samEngine->audioProcessInfo().bufferSize);


	/*if(m_channel->perVoiceFxToolUuid().length() > 0) { aLog("Yes, we have uuid");
		//m_audioToolInsert = new SamAudioToolInsert((AModel*)channel, -1);
		//m_audioToolInsert->setAudioTool(m_channel->perVoiceFxToolUuid());
	}*/


	//aLog("EXIT: SamVoice::SamVoice()");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamVoice::~SamVoice()
{
	delete m_amplifier;
	delete m_zoneCrossfadeModulator;
	delete m_filter;
	delete m_filterModulator;
	delete m_filterEnvelope;
	delete m_envelope;
	for(int i=0; i<m_numOscillators; i++) {
		delete m_oscillators[i];
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamVoice::assign(int id, SamEvent *ev, SamCell *cell, bool release, float gainAdjustment, float releaseCutOff)//XeInstrument *instrument, XeZone *zone, CsSeqEvent *ev);//, float velocityAmplitude);
{
	//aLog("SamVoice::assign() timestamp = %f", ev->timestamp);
	//aLog("SamVoice::assign() id = %d", id);

	m_id = id;
	m_cell = cell;
	//aLog("m_cell->sampleRate() = %f", m_cell->sampleRate());
	m_sampleRateRatio = m_cell->sampleRate() / 44100.0f;
	m_release = release;
	m_triggeredPitch = ev->triggeredPitch;

	// Make basic assignments
	// ----------------------
	m_zone = (SamZone*)m_cell->parent(); //if(m_zone->cfInEnd()!=127) return;
	m_layer = (SamLayer*)m_zone->parent();
	m_articulationIndex = ev->articulationIndex;
	m_articulation = (SamArticulation*)m_layer->parent();

	m_defaultArticulationIndex = m_channel->currentDefaultArticulationIndex();
	m_layerIndex = ev->layerIndex;
	
	m_startFrame = ev->delta;
	//aLog("m_startFrame = %d", m_startFrame);
	m_endFrame = m_audioProcessInfo.bufferSize-1;
	
	m_pitch = ev->midiValue1;
	m_velocity = ev->midiValue2;
	
	m_velocityAmplitude = m_articulation->velocityCurve()->point(m_velocity) * m_channel->midiNoteGain(m_pitch);
	//aLog("m_velocity = %d, m_velocityAmplitude = %f", m_velocity, m_velocityAmplitude);

	m_velocityAmplitude *= (m_zone->gainLinear() * gainAdjustment * m_layer->gainLinear());
	//aLog("     m_velocityAmplitude = %f (%f * %f)", m_velocityAmplitude, m_zone->gainLinear(), gainAdjustment);

	

	m_timestamp = ev->timestamp;

	m_playing = true;
	m_releasing = m_release;
	m_sustained = false;
	m_stolen = 0;

	
	m_pitchMustUpdate = true;
	
	if(m_channel->isPitchBend()) {
		m_pitchBendUp = (float)m_channel->pitchBendUp();
		m_pitchBendDn = (float)m_channel->pitchBendDown();
	} else {
		m_pitchBendUp = (float)m_layer->pitchBendUp();
		m_pitchBendDn = (float)m_layer->pitchBendDown();
	}

	

	m_lastZoneCrossfade = m_channel->currentZoneCrossfade()+m_channel->aftertouch(m_pitch);
	//int cc = m_channel->aftertouch(m_pitch);
	//if(!m_layer->isShortNoteMode()) {
	//	cc += m_channel->currentZoneCrossfade();
	//}
	
	if(m_layer->zoneSelectMode() == 1) {
		m_zoneCrossfade = true;
		
		m_lastZoneCrossfadeValue = setZoneCrossfade(m_channel->currentZoneCrossfade()+m_channel->aftertouch(m_pitch));
		//m_lastZoneCrossfadeValue = setZoneCrossfade(cc);
		m_zoneCrossfadeModulator->resetValue(m_lastZoneCrossfadeValue);
		if(isZoneCrossfadeSilent() && m_layer->isShortNoteMode()) {
			SamVoice::release();
			return;
		}
	} else {
		m_zoneCrossfade = false;
		m_lastZoneCrossfadeValue = 1.0f;
		m_zoneCrossfadeModulator->resetValue(1.0f);
		//m_zoneCrossfadeModulator->update();
	}

	//if(m_channel->isArticulationMode()) {
	//	m_articulationCrossfade = true;
	//} else {
	//	m_articulationCrossfade = false;
	//}
	m_articulationCrossfade = m_channel->isArticulationMode();
	


	// Calculate root frequency
	// ------------------------
	//m_rootPitchFrequency = 8.1758 * pow(2.0f,((float)(m_cell->noteBase()*100.0f)/1200.0f));
	m_rootPitchFrequency = samEngine->midiFrequencyTable(m_cell->noteBase());
	if(m_zone->isTrackPitch()) {
		m_tuning = (m_pitch*100.0f)+m_cell->fineTune() + m_zone->fineTune() + m_layer->fineTune() + ev->tuning;
	} else {
		m_tuning = (m_cell->noteBase()*100.0f)+m_cell->fineTune() + m_zone->fineTune() + m_layer->fineTune() + ev->tuning;
	}
	//aLog("m_pitch = %d, m_cell->noteBase() = %d", m_pitch, m_cell->noteBase());
	if(m_release) {
		float requiredPitchFreq = 0.0f;//8.1758;
		if(m_channel->currentPitchBend() > 0.0) {
			//requiredPitchFreq *= pow(2.0f,((float)(ev->initialPitchMod+m_tuning+(m_pitchBendUp*m_channel->currentPitchBend()))/1200.0f));
			requiredPitchFreq = samEngine->centsToFrequency(ev->initialPitchMod+m_tuning+(m_pitchBendUp*m_channel->currentPitchBend()));
		} else {
			//requiredPitchFreq *= pow(2.0f,((float)(ev->initialPitchMod+m_tuning+(m_pitchBendDn*m_channel->currentPitchBend()))/1200.0f));
			requiredPitchFreq = requiredPitchFreq = samEngine->centsToFrequency(ev->initialPitchMod+m_tuning+(m_pitchBendDn*m_channel->currentPitchBend()));
		}
		float inc = requiredPitchFreq/m_rootPitchFrequency;
		//float incSr = 18000/44100;
		for(int i=0; i<m_audioProcessInfo.bufferSize; i++) {
			m_pitchModulationBuffer[i] = (inc * m_sampleRateRatio);//(inc * incSr);
		}
	}


	// Calculate sample start offset
	// -----------------------------
	m_startOffset = ev->startOffset;
	m_envelopeOffset = 0;

	if(m_channel->isDownbeatOffset()) {
		m_startOffset += m_layer->downbeatSampleOffset();
		//aLog("Downbeat Offset: m_startOffset = %d", m_startOffset);
	} else {
		//aLog("No Downbeat Offset");
		if(m_layer->downbeatSampleOffset() > 0) {
			m_envelopeOffset = m_layer->downbeatSampleOffset() + m_layer->downbeatEnvelopeOffset();
			if(m_envelopeOffset<0) {
				m_envelopeOffset = 0;
			}
			//aLog("Downbeat Offset: m_envelopeOffset = %d", m_envelopeOffset);
		}
	}

	
	if(!m_release) {
		// Velocity modulation
		if(m_velocity >= m_zone->velocitySampleStartThreshold() && m_zone->velocitySampleStart() > 0) {
			double startStep = (float)m_zone->velocitySampleStart()/(127.0f-(float)m_zone->velocitySampleStartThreshold());
			//qDebug("startStep=%f", startStep);
			m_startOffset += (int)((m_velocity-m_zone->velocitySampleStartThreshold()) * startStep);
		}
		
		// Zone Sample Start
		m_startOffset += (m_zone->sampleStart() * m_channel->performanceControls(m_articulationIndex)->sampleStart());
		
		//aLog("Cell m_startOffset = %d", m_startOffset);

		// Cell Sample Start
		m_startOffset += (cell->sourceStart() * m_channel->performanceControls(m_articulationIndex)->cellSampleStart());

		//aLog("m_startOffset = %d (ss=%f, css=%f)", m_startOffset, m_channel->performanceControls()->sampleStart(), m_channel->performanceControls()->cellSampleStart());
	}
	

	// Assign Oscillators and calculate release differences
	// ----------------------------------------------------
	int envelopeIndex = 0;
	if(m_release) {
		for(int i=0; i<m_cell->numElements(); i++) {
			m_oscillators[i]->assign(m_cell, m_cell->elementRelease(i), m_cell->releaseStart(), true);
		}
		envelopeIndex = 1;
	} else {
		for(int i=0; i<m_cell->numElements(); i++) {
			m_oscillators[i]->assign(m_cell, m_cell->element(i), m_startOffset, false);
		}
	}


	// Configure the envelope
	// ----------------------
	//aLog("envelopeIndex = %d", envelopeIndex);

	// Assign the envelope parameters from the zone to local variables
	m_envAttack   = m_zone->envelope(envelopeIndex)->attack();
	m_envHold     = m_zone->envelope(envelopeIndex)->hold();
	m_emvDecay    = m_zone->envelope(envelopeIndex)->decay();
	m_envSustain  = m_zone->envelope(envelopeIndex)->sustain();
	m_envReleaseT = m_zone->envelope(envelopeIndex)->release();
	

	float attackVelocity = m_zone->envelope(envelopeIndex)->attackVelocity();
	int attackVelocityThreshold = m_zone->envelope(envelopeIndex)->attackVelocityThreshold();

	if(envelopeIndex == 0 && m_velocity < attackVelocityThreshold && attackVelocity < m_envAttack) {
		double attackRange = m_envAttack-attackVelocity;
		//aLog("attackRange = %f = %f - %f", attackRange, attack, attackVelocity);
		double attackStep = attackRange/(float)attackVelocityThreshold;
		//aLog("attackStep = %f = %f/%f", attackStep, attackRange, 127.0f);
		m_envAttack = m_envAttack - (attackStep * (float)m_velocity);
		//aLog("attack = %f (%f)", attack, attackStep * (float)m_midiVelocity);
	} else if(envelopeIndex == 0 && m_velocity >= attackVelocityThreshold) {
		m_envAttack = attackVelocity;
	}

	

	//qDebug("%f - %f - %f - %f - %f", attack, hold, decay, sustain, releaseT);

	// Tempo modulates release
	float tempoReleaseModifier = 0.0f;
	if(m_zone->isTempoReleaseAdjustment() && samEngine->project()->tempo() > m_zone->tempoThreshold()) {
		//qDebug("release=%d  oneShot=%d  hasRelease=%d", release, m_zone[0]->isOneShot(), !cell->hasRelease());
		if(release || m_zone->isOneShot() || !cell->hasRelease()) {
			//qDebug("Yes, modify release by tempo.");
			// (((240 - 120)/120)*(1000 - (1000/2)))
			tempoReleaseModifier = (((samEngine->project()->tempo() - m_zone->tempoThreshold())/m_zone->tempoThreshold()) * (m_envReleaseT - (m_envReleaseT/m_zone->tempoReleaseDivisor())));
			if(m_envReleaseT - tempoReleaseModifier < 0) {
				tempoReleaseModifier = m_envReleaseT;
			}
		}
	}
	//aLog("tempoReleaseModifier = %f", tempoReleaseModifier);
	m_envReleaseT -= tempoReleaseModifier;

	
	if(envelopeIndex == 0) {
		float attackOffset = (float)ev->attackOffset;// * 44.1f; //0.0;
		if(m_zone->attackOffset() > 0.0) {
			//qDebug("attackOffset > 0.0");
			switch(m_zone->attackOffsetMode()) {
				case 0: // Switch
					if(m_channel->performanceControls(m_articulationIndex)->envelopeAttackOffset()<1.0) {
						attackOffset = m_zone->attackOffset();
					}
					break;
				case 1: // Scale
					attackOffset = m_zone->attackOffset() * (1.0-m_channel->performanceControls(m_articulationIndex)->envelopeAttackOffset());
					break;
			}
			//aLog("attackOffset = %f (pc = %f)", attackOffset, m_channel->performanceControls()->envelopeAttackOffset());
		}

		// Scale the envelope stages by the current performance control settings
		m_envAttack   *= m_channel->performanceControls(m_articulationIndex)->envelopeAttack();
		m_envAttack += attackOffset;
		m_envHold     *= m_channel->performanceControls(m_articulationIndex)->envelopeHold();
		m_emvDecay    *= m_channel->performanceControls(m_articulationIndex)->envelopeDecay();
		m_envSustain  *= m_channel->performanceControls(m_articulationIndex)->envelopeSustain();
		m_envReleaseT *= m_channel->performanceControls(m_articulationIndex)->envelopeRelease();
	} else if(envelopeIndex == 1) {
		m_envAttack   *= m_channel->performanceControls(m_articulationIndex)->envelopeReleaseAttack();
		//m_envAttack += attackOffset;
		m_envHold     *= m_channel->performanceControls(m_articulationIndex)->envelopeReleaseHold();
		m_emvDecay    *= m_channel->performanceControls(m_articulationIndex)->envelopeReleaseDecay();
		m_envSustain  *= m_channel->performanceControls(m_articulationIndex)->envelopeReleaseSustain();
		m_envReleaseT *= m_channel->performanceControls(m_articulationIndex)->envelopeReleaseRelease();
	}


	//aLog("m_envAttack = %f", m_envAttack);

	// Finally set the envelope
	m_envelope->setEnvelope(
		m_envAttack,
		m_envHold,
		m_emvDecay,
		m_envSustain,
		m_envReleaseT,
		m_zone->envelope(envelopeIndex)->attackShape(),
		m_zone->envelope(envelopeIndex)->decayShape(),
		m_zone->envelope(envelopeIndex)->releaseShape(),
		m_zone->envelope(envelopeIndex)->attackDepth(),
		m_zone->envelope(envelopeIndex)->decayDepth(),
		m_zone->envelope(envelopeIndex)->releaseDepth()
	);
	//m_envelope->debug();

	if(m_release) {
		m_envelope->setAutoRelease();
	} else {
		m_envelope->setAutoRelease(m_zone->envelope(envelopeIndex)->isAutoRelease());
	}

	m_envelope->setOffset(m_envelopeOffset);


	if(m_zone->isFilterEnvelope()) {
		m_filterEnvelope->setEnvelope(
			m_zone->envelope(2)->attack(),
			m_zone->envelope(2)->hold(),
			m_zone->envelope(2)->decay(),
			m_zone->envelope(2)->sustain(),
			m_zone->envelope(2)->release(),
			m_zone->envelope(2)->attackShape(),
			m_zone->envelope(2)->decayShape(),
			m_zone->envelope(2)->releaseShape(),
			m_zone->envelope(2)->attackDepth(),
			m_zone->envelope(2)->decayDepth(),
			m_zone->envelope(2)->releaseDepth()
		);
		m_filterEnvelope->setOffset(m_envelopeOffset);
		//m_filterEnvelope->debug();
	}

	// Assign the initial filter value
	// -------------------------------
	m_filterEnabled = m_zone->isFilterEnabled();
	if(m_filterEnabled) {
		m_filter->reset();
		if(m_zone->isFilterParameter()) {
			//m_filterModulator->resetValue(m_channel->performanceControls()->fFilterCutOff());
			//m_voices[i]->setFilterCutOff(m_performanceControls->filterCutOff());
			setFilterCutOff(m_channel->performanceControls(m_articulationIndex)->filterCutOff());
			m_filterModulator->resetValue(m_filterModulator->targetValue());
		} else {
			m_filterModulator->resetValue(1.0f);
		}

		// Resonance
		m_filter->setResonance(1.41421356-(1.31421356 * m_zone->resonance())); //float resonanceRange = 1.31421356; // (sqrt(2) = 1.41421356) - 0.1;

		//aLog("Filter enabled: assigning voice properties");
		if(m_releasing) {// && m_zone->cutOffController() > 0) {
			m_cutOff = releaseCutOff; //(int)(m_zone->minCutOff()+((m_zone->cutOff()-m_zone->minCutOff())*m_channel->zoneCrossfadeModulator()->value()));
			m_filter->setCutOff(m_cutOff);
		} else {
			m_cutOff = m_zone->cutOff();
			if(m_zone->isFilterPitchTracking()) {
				int p = m_pitch + m_zone->filterPitchTrackOffset();
				if(p < 0) p = 0;
				if(p > 127) p = 127;
				m_cutOff = samEngine->midiFrequencyTable(p);
			}
			
			if(m_zone->isFilterVelocityScaling()) {
				float range = m_cutOff - m_zone->minCutOff();
				//m_cutOff = m_zone->minCutOff() + ((range / 127.0f) * m_velocity);
				//m_cutOff = m_zone->minCutOff() + (range * m_articulation->velocityFilterCurve()->point(m_velocity));
				//aLog("velocity = %d (%d-%d)", m_velocity, m_zone->midiVelocityLow(),m_zone->midiVelocityHigh());
				float mr = 0;
				if(m_zoneCrossfade || m_zone->isFilterVelocityFullRange()) {
					mr = (1.0f/127.0f) * m_velocity;
				} else {
					mr = (1.0f/(m_zone->midiVelocityHigh()-m_zone->midiVelocityLow())) * (m_velocity-m_zone->midiVelocityLow());
				}
				//aLog("mr = %f", mr);
				mr *= m_zone->filterVelocityDepth();
				mr = AMath::pow(mr, 2);
				//aLog("mr = %f", mr);
				m_cutOff = m_zone->minCutOff() + (range * mr);
			}

			//setFilterCutOff(m_channel->currentZoneCrossfade());
			//m_filterModulator->resetValue(m_filterModulator->targetValue());
			//aLog("m_cutOff = %f", m_cutOff);
		}
		
		//if(m_zone->cutOffController() > 0) {
		//	setFilterCutOff(m_channel->currentZoneCrossfade());
		//	m_filterModulator->resetValue(m_filterModulator->targetValue());
		//} else {
		//	m_filter->setCutOff(m_cutOff);
		//}
		//m_cutOffController = m_zone->cutOffController();
	}


	// Amplifier
	// ---------
	if(m_release) {
		m_amplifier->setGain(m_velocityAmplitude *
							 //m_channel->midiNoteGain(m_pitch) * This is already accounted for in m_velocityAmplitude
							 m_channel->expressionModulator()->value() *
							 m_articulation->curveReleaseAttenuation()->point(m_lastZoneCrossfade) *
							 m_lastZoneCrossfadeValue);
	} else {
		m_amplifier->setGain(m_velocityAmplitude * m_channel->midiNoteGain(m_pitch));
	}
	m_amplifier->setRelease(m_release);
	m_ampZoneCf->setRelease(m_release);
	
	// Initialise these values with the current expression and crossfade values -- useful if this voice is a release.
	//m_lastExpressionValue = m_channel->expressionModulator()->value();


	// Calculate the output index
	// --------------------------
	if(release) {
		m_outputIndex = m_articulation->releaseOutputListId();
		if(m_zone->isEnableZoneOutput()) {
			m_outputIndex = m_zone->releaseOutputListId();
		}
	} else {
		m_outputIndex = m_articulation->outputListId();
		if(m_zone->isEnableZoneOutput()) {
			m_outputIndex = m_zone->outputListId();
		}
	}

	if(m_outputIndex >= m_channel->numAudioOutputs()) {
		m_outputIndex = 0;
	}
	if(m_outputIndex < 0) { 
		m_outputIndex = 0;
	}

	m_lfoPitch->reset();

	m_modPitch->resetValue(ev->initialPitchMod);


	// Reset Per-voice FX
	/*if(m_audioToolInsert) {
		if(m_audioToolInsert->isActive()) {
			m_audioToolInsert->resetTool();
			int zone = 0;
			if(m_velocity >= m_channel->perVoiceVelocitySplit()) {
				zone = 1;
			}
			char group = m_channel->perVoiceGroup(m_pitch, zone); //aLog("zone = %d", zone);
			for(int i=0; i<m_audioToolInsert->audioTool()->numParameters(); i++) {
				for(int j=0; j<m_audioToolInsert->numInstances(); j++) {
					SamToolParameter *pr = m_channel->perVoiceParameter(group, i);
					m_audioToolInsert->audioTool(j)->setParameterValueNormalised(pr->id(), pr->value());
					//aLog("[%d] %f", pr->id(), pr->value());
				}
			}
		}
	}*/
	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamVoice::release(void)
{
	//aLog("SamVoice::release()");

	m_playing   = false;
	m_sustained = false;
	m_stolen    = 0;
	m_id        = -1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamVoice::end(int endFrame)
{
	//aLog("SamVoice::end(%d)",endFrame);
	//env->setReleased (true);
	
	//release();
	//return;

	if(m_zone->isOneShot() && !m_cell->isLoop()) {
		return;
	}

	m_releasing = true;

	if(endFrame<1){
		m_envelope->setReleased();
		m_filterEnvelope->setReleased();
	}else{
		m_envelope->setReleaseCounter(endFrame);
		m_filterEnvelope->setReleaseCounter(endFrame);
	}
	
	m_endFrame = endFrame;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamVoice::render(CsSampleBuffer *buffer)
{
	//aLog("SamVoice::render() %d, %d", buffer->numFrames, buffer->numStreams);
	// Zero voice buffer
	// -----------------
	for(int i=0; i<buffer->numStreams; i++) {
/*#ifdef USE_FW_V
		fwsZero_32f(buffer->buffer32[i], buffer->numFrames);
#else*/
#ifdef USE_IPP_V
		ippsZero_32f(buffer->buffer32[i], buffer->numFrames);
#else
		memset(buffer->buffer32[i], 0, buffer->numFrames * sizeof(float));
#endif
	}

	//if(m_release) { aLog("SamVoice::render() nf = %d, ns = %d", buffer->numFrames, buffer->numStreams); }

	if(m_filterEnabled) {// && m_zone->cutOffController() == 1) {
		//aLog("Updating filter modulator");
		m_filterModulator->update();
		//m_filterModulator->debug();
	}


	if(m_zoneCrossfade) {
		m_zoneCrossfadeModulator->update();

		if(m_zoneCrossfadeModulator->firstValue()+m_zoneCrossfadeModulator->lastValue() < (float)0.000001f) {
			//aLog("Not playing voice.");
			m_outOfRange = 0;
			for(int i=0; i<m_numOscillators; i++) {
				if(i<m_cell->numElements()) { // This is a nasty workaround to stop a previous workaround from crashing.
					m_oscillators[i]->advanceSamplePointer(buffer, m_startFrame);
					if(m_oscillators[i]->isSamplePointerOutOfRange()) {
						m_outOfRange++;
					}
				}
			}
			//m_envelope->advance(buffer, m_startFrame);
			m_envelope->advance(buffer->numFrames - m_startFrame);
			m_startFrame = 0;
			if(m_envelope->finished() || m_outOfRange >= m_numOscillators) {
				release();
			}
			return false;
		}
	}


	//m_zoneCrossfadeModulator->debug();
	
	

	// Calculate tuning
	// ---------------- 
	/*if(!m_release) { //&& m_pitchMustUpdate
		int pos = 0;
		//m_numInputSamplesRequired = 0;
		for(int i=0; i<buffer->numFrames/m_pitchBendModulator->period(); i++) {
			//float requiredPitchFreq = 8.1758 * pow (2,((float)((m_pitch*100.0f)+m_zone->fineTune()+(((float)m_parent->parameters()->pitchBendUp()*100.0f)*m_pitchBendModulator->modulationBuffer()[i]))/1200.0f));
			float requiredPitchFreq = 0.0f;
			if(m_pitchBendModulator->buffer()[i] > 0) {
				requiredPitchFreq = 8.1758 * pow(2.0f,((float)(m_tuning+(m_pitchBendUp*m_pitchBendModulator->buffer()[i]))/1200.0f));
				//requiredPitchFreq = 8.1758 * pow (2,((float)((m_pitch*100.0f)+m_cell->fineTune())/1200.0f));
			} else {
				requiredPitchFreq = 8.1758 * pow(2.0f,((float)(m_tuning+(m_pitchBendDn*m_pitchBendModulator->buffer()[i]))/1200.0f));
				//requiredPitchFreq = 8.1758 * pow (2,((float)((m_pitch*100.0f)+m_cell->fineTune())/1200.0f));
			}
			//float incSr = 1.0f;//11001.0f/44100.0f;
			float inc = requiredPitchFreq/m_rootPitchFrequency;
			
			for(int j=0; j<4; j++) {
				m_pitchModulationBuffer[pos] = (inc * m_sampleRateRatio);
				//aLog("%f = (%f * %f)", m_pitchModulationBuffer[pos],inc, incSr);

				//m_pitchModulationBuffer[pos] *= m_lfoPitch->buffer()[pos];

				pos++;
			}
			//m_numInputSamplesRequired += (inc * 4);
		}//
		//aLog("m_numInputSamplesRequired = %f (%d)", m_numInputSamplesRequired, (int)m_numInputSamplesRequired);
		m_pitchMustUpdate = false;
	}*/

	m_modPitch->update(m_startFrame);
	m_lfoPitch->update(m_startFrame);

	if(!m_release) {
		int pos = 0;
		for(int i=0; i<buffer->numFrames; i++) {
			float mod = m_modPitch->buffer()[i];//m_lfoPitch->buffer()[i] * 1200.0f;
			//aLog("mod[%d] = %f", i, mod);
			//float requiredPitchFreq = 8.1758 * pow(2.0f,((float)(m_tuning + mod)/1200.0f));
			if(m_pitchBendModulator->buffer()[i] > 0.0f) {
				mod += (m_pitchBendModulator->buffer()[i] * m_pitchBendUp);
			} else {
				mod += (m_pitchBendModulator->buffer()[i] * m_pitchBendDn);
			}
			float requiredPitchFreq = samEngine->centsToFrequency(m_tuning + mod);
			float inc = requiredPitchFreq/m_rootPitchFrequency;
			m_pitchModulationBuffer[i] = (inc * m_sampleRateRatio);
		}
	}



	/*for(int i=0; i<buffer->numFrames/m_pitchBendModulator->period(); i++) {
		if(m_pitchBendModulator->buffer()[i] > 0) {
			m_pitchPowBuffer[i] = ((float)(m_tuning+(m_pitchBendUp*m_pitchBendModulator->buffer()[i]))/1200.0f);
		} else {
			m_pitchPowBuffer[i] = ((float)(m_tuning+(m_pitchBendDn*m_pitchBendModulator->buffer()[i]))/1200.0f);
		}
	}
	fwsPow_32f_A11(m_twoBuffer, m_pitchPowBuffer, m_reqPitchBuffer, buffer->numFrames/m_pitchBendModulator->period());
	fwsMulC_32f_I(8.1758, m_reqPitchBuffer, buffer->numFrames/m_pitchBendModulator->period());
	fwsDivC_32f_I(m_rootPitchFrequency, m_reqPitchBuffer, buffer->numFrames/m_pitchBendModulator->period());

	for(int i=0; i<buffer->numFrames/m_pitchBendModulator->period(); i++) {
		//float inc = m_reqPitchBuffer[i]/m_rootPitchFrequency;
		for(int j=0; j<4; j++) {
			m_pitchModulationBuffer[pos] = m_reqPitchBuffer[i];
			pos++;
		}
	}//*/


	// Render synth components
	// -----------------------
	m_outOfRange = 0;
	for(int i=0; i<m_numOscillators; i++) {
		//cDebug("Render: m_startFrame = %d", m_startFrame);
		//aLog("%d/%d < %d", i, m_numOscillators, m_cell->numElements());
		if(i<m_cell->numElements()) { // This is a nasty workaround to stop a previous workaround from crashing.
			//aLog("%d < %d", i, m_cell->numElements());
			if(m_channel->totalSetGain(i) > (float)0.000001f && m_oscillators[i]->element()->isAudioDataLoaded()) { // This might be a naive workaround... Is there a better way consider we're compiling for speed rather than precision?
				//aLog("Rendering osc %d (%f > %f)", i, m_channel->totalSetGain(i), 0.000001f);
				m_oscillators[i]->renderBuffer(buffer, m_startFrame);
				//for(int z=0;z<buffer->numFrames;z++) aLog("%f %d", buffer->buffer32[z], z);
			} else { //aLog("advance");
				m_oscillators[i]->advanceSamplePointer(buffer, m_startFrame);
			}
			if(m_oscillators[i]->isSamplePointerOutOfRange()) {
				m_outOfRange++; //aLog("outofrange");
			}
			//aLog("After.");
		}
	}
	//aLog("After.After.");

	//if(m_release) {
	//	aLog("%f", m_oscillators[0]->samplePointer());
	//}

	//if(m_parent->parameters()->isFilterEnabled() && m_parent->parameters()->filterCutOff() < 1.0) {

	if(m_filterEnabled) {
		m_filterModulationSource->copy(m_filterModulator);
		
		if(m_zone->isFilterEnvelope()) {
			//aLog("filter envelope");
			m_filterEnvelope->update(m_startFrame);
			m_filterModulationSource->add(m_filterEnvelope);
			
			//m_filterModulator->debug();
			//m_filterModulator->add(m_filterEnvelope);
			//m_filterModulator->debug();
		}
		if(m_releasing) { // No cut-off controller -- static cut-off throughout entire buffer (m_cutOffController == 0 || )
			//aLog("Here");
			m_filter->renderBuffer(buffer, m_startFrame);
		} else { //if(m_cutOffController > 0 && !m_releasing) { // Use modulation buffer

			//m_filter->setCutoff((44100.0/2.0f) * ((1.0f/127.0f)*m_channel->currentCrossfade()));
			//m_filter->renderBuffer(buffer, m_startFrame);
			
			//m_filterModulator->debug();
			//APrecisionTimer tf; tf.start();
			m_filter->renderBufferWithModulation(buffer, m_startFrame, m_filterModulationSource, m_zone->minCutOff(), m_cutOff);
			//tf.stop(); aLog("tf = %f", tf.duration());
			//m_filter->renderBuffer(buffer, m_startFrame);
			//aLog("modbuffer filter last cutoff = %f", m_filter->cutOff());
		}
		m_lastCutOff = m_filter->cutOff();
		//aLog("m_filter->cutOff() = %f", m_filter->cutOff());
	}

	//m_envelope->renderBuffer(buffer, m_startFrame);
	m_envelope->update(m_startFrame);
	

	// Main amplifier (velocity + expression + envelope)
	// -------------------------------------------------
	m_amplifier->renderBuffer(buffer, m_startFrame);


	// Handle crossfades if this voice is not a release voice
	// ------------------------------------------------------
	if(m_zoneCrossfade) {
		m_ampZoneCf->renderBufferWithoutConstantGain(buffer, m_startFrame);
	}


	// Expression & Crossfades
	// -----------------------
	/*if(!m_release) {
		int sampleIndex = 0;
		if(m_zoneCrossfade) {
			for(int i=0; i<buffer->numFrames/m_channel->expressionModulator()->period(); i++) {
				for(int j=0; j<buffer->numStreams; j++) {
					for(int k=0; k<m_channel->expressionModulator()->period(); k++) {
						buffer->buffer32[j][sampleIndex+k] *= (m_channel->expressionModulator()->buffer()[i] * m_zoneCrossfadeModulator->buffer()[i] * m_velocityAmplitude);
					}
				}
				sampleIndex += m_channel->expressionModulator()->period();
			}
		} else {
			
#ifdef USE_FW
			for(int i=0; i<buffer->numStreams; i++) {
				fwsMulC_32f_I(m_velocityAmplitude, buffer->buffer32[i], buffer->numFrames);
			}
#else
			for(int i=0; i<buffer->numFrames; i++) {
				for(int j=0; j<buffer->numStreams; j++) {
					buffer->buffer32[j][i] *= m_velocityAmplitude;
				}
			}
#endif
		
		}

		// These values need to go here, because we don't want to update them after voice is releasing
		// But should they go somewhere else? Could the default be a problem? Maybe for a release voice, but probably otherwise not since a voice has to start before it releases.
		// Maybe these values should be initialised when the voice is assigned?
		m_lastExpressionValue = m_channel->expressionModulator()->value();
		if(m_zoneCrossfade) {
			m_lastZoneCrossfadeValue = m_zoneCrossfadeModulator->value();
		}

	} else {
		//aLog("m_lastZoneCrossfadeValue = %f", m_lastZoneCrossfadeValue);

		//aLog("m_articulation->curveReleaseAttenuation()->point(%d) = %f", m_lastZoneCrossfade, m_articulation->curveReleaseAttenuation()->point(m_lastZoneCrossfade));
#ifdef USE_FW
		for(int i=0; i<buffer->numStreams; i++) {
			fwsMulC_32f_I(m_lastExpressionValue * m_lastZoneCrossfadeValue * m_velocityAmplitude * m_articulation->curveReleaseAttenuation()->point(m_lastZoneCrossfade), buffer->buffer32[i], buffer->numFrames);
		}
#else
		for(int i=0; i<buffer->numFrames; i++) {
			for(int j=0; j<buffer->numStreams; j++) {
				buffer->buffer32[j][i] *= (m_lastExpressionValue * m_lastZoneCrossfadeValue * m_velocityAmplitude * m_articulation->curveReleaseAttenuation()->point(m_lastZoneCrossfade));
			}
		}
#endif
	}//*/

	



	// Articulation Crossfade
	// ----------------------
	if(m_channel->isArticulationMode()) {
		if(m_articulationIndex == m_defaultArticulationIndex) {
			int sampleIndex = 0;
			for(int i=0; i<buffer->numFrames/m_channel->articulationCrossfadeModulator()->period(); i++) {
				for(int j=0; j<buffer->numStreams; j++) {
					for(int k=0; k<m_channel->articulationCrossfadeModulator()->period(); k++) {
						buffer->buffer32[j][sampleIndex+k] *= (1.0f-m_channel->articulationCrossfadeModulator()->buffer()[i]);
					}
				}
				sampleIndex += m_channel->articulationCrossfadeModulator()->period();
			}
		} else {
			int sampleIndex = 0;
			for(int i=0; i<buffer->numFrames/m_channel->articulationCrossfadeModulator()->period(); i++) {
				for(int j=0; j<buffer->numStreams; j++) {
					for(int k=0; k<m_channel->articulationCrossfadeModulator()->period(); k++) {
						buffer->buffer32[j][sampleIndex+k] *= m_channel->articulationCrossfadeModulator()->buffer()[i];
					}
				}
				sampleIndex += m_channel->articulationCrossfadeModulator()->period();
			}
		}
	}
	
	/*if(m_audioToolInsert) {
		if(m_audioToolInsert->isActive()) {
			m_audioToolInsert->process(buffer, buffer, 1, 1, 0, 0);
		}
	}*/
	
	
	m_startFrame = 0;

	if(m_envelope->finished() || m_outOfRange >= m_numOscillators) {
		release();
	}

	if(m_stolen > 0) {
		//aLog("m_stolen = %d", m_stolen);
		//float gain = 1.0;
		//float inc = -1.0/buffer->numFrames;

		//int frames = m_stolen;
		//if(frames > buffer->numFrames) {
		//	frames = buffer->numFrames;
		//}

		//aLog("frames = %d", frames);
		for(int i=0; i<buffer->numFrames; i++) {
			for(int j=0; j<buffer->numStreams; j++) {
				buffer->buffer32[j][i] *= m_stolenGain;
			}
			
			m_stolen--;
			if(m_stolen < 1) {
				m_stolenGain = 0;
			} else {
				m_stolenGain += m_stolenInc;
			}
		}

		if(m_stolen < 1) {
			//aLog("Release stolen voice");
			release();
		}
	}

	//aLog("%f", buffer->buffer32[0]);

	return true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamVoice::setStolen(int count)
{
	m_stolen = count;

	m_stolenGain = 1.0;
	m_stolenInc = -1.0 / m_stolen;

	if(m_zoneCrossfade) {
		if(isZoneCrossfadeSilent()) {
			release();
		}
	}

	//aLog("setStolen(%d) %f, %f", m_stolen, m_stolenGain, m_stolenInc);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*int SamVoice::stolen(void)
{
	return m_stolen;

	//if(m_stolen > 0) {
	//	return true;
	//}
	//
	//return false;
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamVoice::setAudioProcessInfo(CsAudioProcessInfo audioProcessInfo)
{
	aLog("SamVoice::setAudioProcessInfo(numFrames = %d)", audioProcessInfo.bufferSize);

	m_audioProcessInfo = audioProcessInfo;

	if(!m_pitchModulationBuffer) {
		free(m_pitchModulationBuffer);
	}
	m_pitchModulationBuffer = (float*)malloc(sizeof(float)*audioProcessInfo.bufferSize);
	for(int i=0; i<m_numOscillators; i++) {
		m_oscillators[i]->setPitchModulationBuffer(m_pitchModulationBuffer);
	}

	if(m_pitchPowBuffer) {
		free(m_pitchPowBuffer);
	}
	m_pitchPowBuffer = (float*)malloc(sizeof(float)*(audioProcessInfo.bufferSize/m_pitchBendModulator->period()));
	if(m_twoBuffer) {
		free(m_twoBuffer);
	}
	m_twoBuffer = (float*)malloc(sizeof(float)*(audioProcessInfo.bufferSize/m_pitchBendModulator->period()));
	for(int i=0; i<audioProcessInfo.bufferSize/m_pitchBendModulator->period(); i++) {
		m_twoBuffer[i] = 2.0f;
	}
	if(m_reqPitchBuffer) {
		free(m_reqPitchBuffer);
	}
	m_reqPitchBuffer = (float*)malloc(sizeof(float)*(audioProcessInfo.bufferSize/m_pitchBendModulator->period()));

	m_envelope->setSampleRate(audioProcessInfo.sampleRate);
	m_envelope->setBufferSize(audioProcessInfo.bufferSize);

	m_filterEnvelope->setSampleRate(audioProcessInfo.sampleRate);
	m_filterEnvelope->setBufferSize(audioProcessInfo.bufferSize);

	m_zoneCrossfadeModulator->setBufferSize(audioProcessInfo.bufferSize);
	m_filterModulator->setBufferSize(audioProcessInfo.bufferSize);
	m_filterModulationSource->setBufferSize(audioProcessInfo.bufferSize);

	
	m_mscPitch->setBufferSize(audioProcessInfo.bufferSize);
	m_modPitch->setBufferSize(audioProcessInfo.bufferSize);
	m_lfoPitch->setBufferSize(audioProcessInfo.bufferSize);

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SamVoice::updateStreamMixer(void)
{
	for(int i=0; i<m_numOscillators; i++) {
		m_oscillators[i]->updateStreamCoefficients();
	}
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamVoice::framePosition(void) const
{
	if(m_numOscillators > 0) {
		return (int)m_oscillators[0]->samplePointer();
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamVoice::setReleaseInPlace(void)
{
	//aLog("SamVoice::setReleaseInPlace()"); //return;

	// This is not sample accurate -- will sound different depending on buffer sizes... Does it matter?

	/*if(!m_zone) {
		aLog("Zone is NULL");
		return;
	}//*/

	if(m_zoneCrossfade && isZoneCrossfadeSilent()) {
		return;
	}

	m_envelope->setEnvelope(
		m_zone->envelope(1)->attack(),
		m_zone->envelope(1)->hold(),
		m_zone->envelope(1)->decay(),
		m_zone->envelope(1)->sustain(),
		m_zone->envelope(1)->release(),
		m_zone->envelope(1)->attackShape(),
		m_zone->envelope(1)->decayShape(),
		m_zone->envelope(1)->releaseShape(),
		m_zone->envelope(1)->attackDepth(),
		m_zone->envelope(1)->decayDepth(),
		m_zone->envelope(1)->releaseDepth(),
		m_envelope->gain() // Attack Gain
	);
	//m_envelope->debug();

	m_envelope->setAutoRelease();
	m_envelope->setCurrentStage(1);
	m_releasing = true;
	//m_release = true; // Should this be added?

	m_amplifier->setRelease(true);
	
	if(m_zoneCrossfade) {
	//float g = m_velocityAmplitude * m_channel->midiNoteGain(m_pitch) * m_channel->expressionModulator()->value();
	
	//	m_amplifier->
		//g *= setZoneCrossfade(m_channel->currentZoneCrossfade());//m_channel->zoneCrossfadeModulator()->value();
		//g *= m_articulation->curveReleaseAttenuation()->point(m_lastZoneCrossfade);
		//g *= m_lastZoneCrossfadeValue;

		
			m_amplifier->setGain(m_amplifier->gain() *
								 setZoneCrossfade(m_channel->currentZoneCrossfade()+m_channel->aftertouch(m_pitch)) *
								 m_channel->expressionModulator()->value());
		

		/*m_amplifier->setGain(m_velocityAmplitude *
							 //m_channel->midiNoteGain(m_pitch) *
							 m_channel->expressionModulator()->value() *
							 m_articulation->curveReleaseAttenuation()->point(m_lastZoneCrossfade) *
							 setZoneCrossfade(m_channel->currentZoneCrossfade()+m_channel->aftertouch(m_pitch)));*/
	} else {
		m_amplifier->setGain(m_amplifier->gain() *
			                 //setZoneCrossfade(m_channel->currentZoneCrossfade()+m_channel->aftertouch(m_pitch)) *
							 m_channel->expressionModulator()->value());
	}

	//m_amplifier->setGain(g);

	m_ampZoneCf->setRelease(true);
	// Somewhere along the line, a gain value will need to be set here.
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamVoice::setPitchModulation(float cents)
{
	m_modPitch->setTargetValue(cents);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float SamVoice::setZoneCrossfade(int zoneCrossfade)
{
	//aLog("SamVoice::setZoneCrossfade(%d)", zoneCrossfade);

	if(m_articulation->isVelocitySetsCrossfade()) {
		//zoneCrossfade += m_velocity;
		zoneCrossfade = m_velocity;
	}

	if(zoneCrossfade>127)zoneCrossfade = 127;
	if(zoneCrossfade < 0) zoneCrossfade = 0;

	m_currentZoneCrossfadeMidiValue = zoneCrossfade;
	float cf = 0.0f;
	float curveDepth = 0.0f;

	if (zoneCrossfade >= m_zone->cfInStart() && zoneCrossfade <= m_zone->cfOutEnd()) {
		
		int fadeInRange = m_zone->cfInEnd() - m_zone->cfInStart();
		int fadeOutRange = m_zone->cfOutEnd() - m_zone->cfOutStart();

		// Are we in the fadeIn range?
		if (zoneCrossfade >= m_zone->cfInStart() && zoneCrossfade <= m_zone->cfInEnd()) { // Yes we should fade-in
			
			if(fadeInRange==0) {
				cf = 1.0;
			} else {
				cf = (zoneCrossfade - m_zone->cfInStart())*(1.0 / fadeInRange);
			}

			curveDepth = m_zone->cfInCurveDepth();
		
		// Are we in the fadeOut range?
		} else if (zoneCrossfade >= m_zone->cfOutStart() && zoneCrossfade <= m_zone->cfOutEnd()) { // Yes we should fade=out

			if(fadeOutRange==0) {
				cf = 1.0;
			} else {
				cf = (((m_zone->cfOutStart()) - zoneCrossfade) + fadeOutRange) * (1.0 / fadeOutRange);
			}

			curveDepth = m_zone->cfOutCurveDepth();

		} else {
			cf = 1.0;
		}

		cf += curveDepth * 0.828427 * (cf-cf*cf); // Rainer's curve adjustment from SLPT
	}

	
	cf *= m_articulation->curveCrossfadeGain()->point(zoneCrossfade);
	m_zoneCrossfadeModulator->setTargetValue(cf);

	// Calculate the filter value

	return cf;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamVoice::setZoneCrossfadeResponseTime(int time)
{
	m_zoneCrossfadeModulator->setRate(time);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamVoice::setZoneFilterResponseTime(int time)
{
	//aLog("SamVoice::setZoneFilterResponseTime(%d)", time);

	m_filterModulator->setRate(time);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamVoice::setFilterCutOff(int controllerValue)
{
	if(m_articulation->isVelocitySetsCrossfade()) {
		//controllerValue += m_velocity;
		controllerValue = m_velocity;
	}

	if(controllerValue>127) controllerValue = 127;
	if(controllerValue < 0) controllerValue = 0;

	float scaled = 1.0f;

	if(m_zoneCrossfade) {
		// If it's a crossfading zone, we get our range from the crossfade values
		scaled = 1.0f / (float)(m_zone->cfOutStart() - m_zone->cfInStart());
		if(controllerValue > m_zone->cfOutStart()) {
			controllerValue = m_zone->cfOutStart();
		}
		if(controllerValue < m_zone->cfInStart()) {
			controllerValue = m_zone->cfInStart();
		}
		scaled *= (controllerValue-m_zone->cfInStart());
	} else {
		// Otherwise, get the range values from the velocity -- No, just use the full range.
		scaled = 1.0f / 127; //(float)(m_zone->midiVelocityHigh() - m_zone->midiVelocityLow());
		scaled *= controllerValue;
	}
	//aLog("scaled = %f", scaled);
	m_filterModulator->setTargetValue(scaled);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamVoice::isZoneCrossfadeSilent(void)
{
	//aLog("SamVoice::isZoneCrossfadeSilent() %f + %f = %f", m_zoneCrossfadeModulator->firstValue(), m_zoneCrossfadeModulator->lastValue(), m_zoneCrossfadeModulator->firstValue()+m_zoneCrossfadeModulator->lastValue());

	if(m_zoneCrossfadeModulator->firstValue()+m_zoneCrossfadeModulator->lastValue() < (float)0.000001f) {
		return true;
	}

	return false;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
float SamVoice::numInputSamplesRequired(int numOutputSamples)
{
	if(numOutputSamples >= m_audioProcessInfo.bufferSize) {
		numOutputSamples = m_audioProcessInfo.bufferSize;
	}
	
	m_numInputSamplesRequired = 0;
	for(int i=0; i<numOutputSamples; i++) {
		m_numInputSamplesRequired += m_pitchModulationBuffer[i];
		//aLog("[%d] m_numInputSamplesRequired = %f += m_pitchModulationBuffer[i] = %f", i, m_numInputSamplesRequired, m_pitchModulationBuffer[i]);
	}

	return m_numInputSamplesRequired+1.0f; //((float)(int)((m_numInputSamplesRequired)+1)); //ceil(m_numInputSamplesRequired);
	//return m_numInputSamplesRequired;
	//return ((float)(int)(m_numInputSamplesRequired + 0.5f)); //floor(m_numInputSamplesRequired + 0.5);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------