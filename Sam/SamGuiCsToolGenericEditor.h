#ifndef SAMGUICSTOOLGENERICEDITOR_H
#define SAMGUICSTOOLGENERICEDITOR_H

#include <QWidget>
//#include "ui_SamGuiCsToolGenericEditor.h"

#include "SamMusicTool.h"
#include "SamGuiCsToolParameter.h"

class SamGuiCsToolGenericEditor : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiCsToolGenericEditor(QWidget *parent, SamTool *tool);
	~SamGuiCsToolGenericEditor();

	void onState(AStateEvent *ev);

	void updateParameters(void);

public slots:
	void sendParameter(int id, float value);
	void updateAssignedParameter(void);
	void updateChangedParameter(int id);

signals:
	void parameterAssigned(void);
	void parameterChanged(int id);

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiCsToolGenericEditorClass ui;
	SamTool *m_tool;
	QList<SamGuiCsToolParameter*> m_parameters;
};

#endif // SAMGUICSTOOLGENERICEDITOR_H
