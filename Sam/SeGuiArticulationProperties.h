#ifndef SEGUIARTICULATIONPROPERTIES_H
#define SEGUIARTICULATIONPROPERTIES_H

#include <QWidget>
//#include "ui_SeGuiArticulationProperties.h"

#include "UiList.h"
#include "SeGuiItemHeading.h"
#include "SeGuiItemText.h"
#include "SeGuiItemValue.h"
#include "SeGuiItemBool.h"
#include "SeGuiItemCurve.h"
#include "SeGuiItemSelectOutput.h"

#include "SamVersion.h"

class SeGuiArticulationProperties : public QWidget
{
	Q_OBJECT

public:
	SeGuiArticulationProperties(QWidget *parent = 0);
	~SeGuiArticulationProperties();

	void setVersion(SamVersion *version, SamArticulation *articulation);

public slots:
	void setValue(int propertyId, float value);
	void setState(int propertyId, bool state);
	void setText(int propertyId, QString text);

protected:
	void resizeEvent(QResizeEvent *event);

private:
	//Ui::SeGuiArticulationPropertiesClass ui;
	SamVersion *m_version;
	SamArticulation *m_articulation;

	UiList *m_list;

	SeGuiItemText *m_name;
	SeGuiItemValue *m_triggerId;
	SeGuiItemValue *m_zoneCrossfadeResponseTime;
	SeGuiItemValue *m_zoneFilterResponseTime;
	SeGuiItemValue *m_minimumVelocityController;
	SeGuiItemBool *m_polyphonicModulation;
	//SeGuiItemValue *m_maxAftertouch;

	SeGuiItemHeading *m_velocityCrossfade;
	SeGuiItemBool *m_velocitySetsCrossfade;
	//SeGuiItemBool *m_velocityCrossfadeDecay;
	//SeGuiItemValue *m_velocityCrossfadeDecayTime;
	//SeGuiItemValue *m_velocityCrossfadeDecayTo;

	SeGuiItemHeading *m_curvesAndReleases;
	SeGuiItemCurve *m_velocityCurve;
	SeGuiItemCurve *m_velocityFilterCurve;
	SeGuiItemCurve *m_crossfadeCurve;
	SeGuiItemCurve *m_releaseCurve;

	SeGuiItemHeading *m_outputs;
	SeGuiItemSelectOutput *m_output;
	SeGuiItemSelectOutput *m_releaseOutput;

	SeGuiItemHeading *m_layerControls;
	SeGuiItemValue *m_defaultLayer;
	SeGuiItemValue *m_layerSelectMode;
	SeGuiItemValue *m_switchLayerTime;
	SeGuiItemValue *m_legatoOverlap;
};

#endif // SEGUIARTICULATIONPROPERTIES_H
