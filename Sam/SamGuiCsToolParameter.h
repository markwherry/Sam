#ifndef SAMGUICSTOOLPARAMETER_H
#define SAMGUICSTOOLPARAMETER_H

#include <QWidget>
//#include "ui_SamGuiCsToolParameter.h"

#include "SamTool.h"
#include "SamToolParameter.h"

#include "UiFader.h"
#include "UiValue.h"

class SamGuiCsToolParameter : public QWidget
{
	Q_OBJECT

public:
	SamGuiCsToolParameter(QWidget *parent, SamTool *tool, SamToolParameter* parameter);
	~SamGuiCsToolParameter();

	SamToolParameter* parameter(void) { return m_parameter; }

	void setValueNormalised(float value);

public slots:
	void updateFromSlider(float value, int flags);
	void updateFromValue(float value);
	void learn(void);
	void unlearn(void);

signals:
	void parameterChanged(int id, float value);

protected:
	void mousePressEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiCsToolParameterClass ui;
	SamTool *m_tool;
	SamToolParameter *m_parameter;
	UiFader *m_fader;
	UiValue *m_value;
	QMenu *m_menuLearn;

	QAction *m_actionLearn;
	QAction *m_actionUnlearn;
};

#endif // SAMGUICSTOOLPARAMETER_H
