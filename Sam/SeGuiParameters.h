#ifndef SEGUIPARAMETERS_H
#define SEGUIPARAMETERS_H

#include <QWidget>
//#include "ui_SeGuiParameters.h"

#include "UiListContainer.h"
#include "SeGuiParameterProperties.h"

#include "SamArticulation.h"

#include "AStateManager.h"
using namespace Atom;

class SeGuiParameters : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SeGuiParameters(QWidget *parent = 0);
	~SeGuiParameters();

	void onState(AStateEvent *ev);

	void setArticulation(SamArticulation *articulation);

public slots:
	void rebuild(void);
	void setParameterName(int id, QString name);
	void setParameter(int id);
	void addParameter(void);
	void removeParameter(void);
	void createFilterCutOffParameter(void);
	void createMinimumVelocityParameter(void);
	void createSampleStartParameter(void);
	void createAttackParameter(void);
	void createReleaseEnvelopeParameters(void);
	void createCellSampleStartAdjustParameter(void);
	void createLayerSwitchParameters(void);

signals:
	void parameterListUpdated(void);

protected:
	void resizeEvent(QResizeEvent *ev);

private:
	//Ui::SeGuiParametersClass ui;
	SamArticulation *m_articulation;
	UiListContainer *m_listContainer;
	SeGuiParameterProperties *m_parameterProperties;
	int m_parameterId;
	/*CsGuiFunctionBox *fbParameterEditor;
	SeGuiParameterEditor *parameterEditor;

	SamArticulation *m_articulation;
	SamChannel *m_channel;
	SamParameter *m_parameter;
	int m_parameterId;*/
};

#endif // SEGUIPARAMETERS_H
