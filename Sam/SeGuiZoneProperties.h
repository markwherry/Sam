#ifndef SEGUIZONEPROPERTIES_H
#define SEGUIZONEPROPERTIES_H

#include <QWidget>
//#include "ui_SeGuiZoneProperties.h"

#include "AStateManager.h"
using namespace Atom;

#include "UiList.h"
#include "SeGuiItemText.h"
#include "SeGuiItemValue.h"
#include "SeGuiItemBool.h"
#include "SeGuiItemCurve.h"
#include "SeGuiItemSelectOutput.h"

#include "SamZone.h"

class SeGuiZoneProperties : public QWidget
{
	Q_OBJECT

public:
	SeGuiZoneProperties(QWidget *parent, int category);
	~SeGuiZoneProperties();

	void setZone(SamZone *zone, int numZonesSelected);

public slots:
	void setValue(int propertyId, float value);
	void setState(int propertyId, bool state);
	void setText(int propertyId, QString text);

protected:
	void resizeEvent(QResizeEvent *event);

private:
	void initGeneral(void);
	void initLevelsAndOutputs(void);
	void initPitch(void);
	void initVelocity(void);
	void initTriggering(void);
	void initFilter(void);
	void initEnvelope1(void);
	void initEnvelope2(void);
	void initEnvelope3(void);
	void initTempoReleaseModulation(void);
	void initCrossfade(void);
	//Ui::SeGuiZonePropertiesClass ui;
	UiList *m_list;

	SamZone *m_zone;
	int m_category;

	SeGuiItemText *m_name;
	SeGuiItemText *m_description;
	//SeGuiItemValue *m_group;
	SeGuiItemValue *m_dynamicRef;
	SeGuiItemValue *m_sampleStart;

	SeGuiItemValue *m_gain;
	SeGuiItemValue *m_releaseGain;
	SeGuiItemValue *m_preLoopReleaseGain;
	SeGuiItemBool *m_enableZoneOutput;
	SeGuiItemSelectOutput *m_output;
	SeGuiItemSelectOutput *m_releaseOutput;

	SeGuiItemValue *m_pitchHi;
	SeGuiItemValue *m_pitchLo;
	SeGuiItemValue *m_fineTune;
	SeGuiItemBool *m_trackPitch;

	SeGuiItemValue *m_velocityHi;
	SeGuiItemValue *m_velocityLo;
	SeGuiItemBool *m_predictableVelocity;
	SeGuiItemValue *m_predictableVelocityCellIndex;
	SeGuiItemValue *m_velocitySampleStart;
	SeGuiItemValue *m_velocitySampleStartThreshold;

	SeGuiItemBool *m_oneShot;
	SeGuiItemValue *m_triggerMode;
	SeGuiItemValue *m_singleCellId;

	SeGuiItemValue *m_env1Attack;
	SeGuiItemValue *m_env1AttackVelocity;
	SeGuiItemValue *m_env1AttackVelocityThreshold;
	SeGuiItemValue *m_env1AttackOffset;
	SeGuiItemValue *m_env1AttackOffsetMode;
	SeGuiItemCurve *m_env1AttackCurve;
	SeGuiItemValue *m_env1Hold;
	SeGuiItemValue *m_env1Decay;
	SeGuiItemValue *m_env1Sustain;
	SeGuiItemValue *m_env1Release;
	SeGuiItemBool *m_env1AutoRelease;
	SeGuiItemCurve *m_env1ReleaseCurve;

	SeGuiItemValue *m_env2Attack;
	SeGuiItemCurve *m_env2AttackCurve;
	SeGuiItemValue *m_env2Hold;
	SeGuiItemValue *m_env2Decay;
	SeGuiItemValue *m_env2Sustain;
	SeGuiItemValue *m_env2Release;
	SeGuiItemCurve *m_env2ReleaseCurve;

	SeGuiItemValue *m_env3Attack;
	SeGuiItemCurve *m_env3AttackCurve;
	SeGuiItemValue *m_env3Hold;
	SeGuiItemValue *m_env3Decay;
	SeGuiItemValue *m_env3Sustain;
	SeGuiItemValue *m_env3Release;
	SeGuiItemCurve *m_env3ReleaseCurve;

	// Filter
	SeGuiItemBool *m_filterEnabled;
	SeGuiItemValue *m_filterCutOff;
	//SeGuiItemValue *m_cutOffController;
	SeGuiItemValue *m_minCutOff;
	//SeGuiItemValue *m_maxCutOff;
	SeGuiItemValue *m_resonance;
	SeGuiItemBool *m_filterVelocityScaling;
	SeGuiItemValue *m_filterVelocityDepth;
	SeGuiItemBool *m_filterVelocityFullRange;
	SeGuiItemBool *m_filterPitchTracking;
	SeGuiItemValue *m_filterPitchTrackOffset;
	SeGuiItemBool *m_filterEnvelope;
	SeGuiItemBool *m_filterParameter;

	SeGuiItemBool *m_tempoReleaseAdjustment;
	SeGuiItemValue *m_tempoThreshold;
	SeGuiItemValue *m_tempoReleaseDivisor;

	SeGuiItemValue *m_inStart;
	SeGuiItemValue *m_inEnd;
	SeGuiItemValue *m_outStart;
	SeGuiItemValue *m_outEnd;
	SeGuiItemValue *m_inCurveDepth;
	SeGuiItemValue *m_outCurveDepth;
};

#endif // SEGUIZONEPROPERTIES_H
