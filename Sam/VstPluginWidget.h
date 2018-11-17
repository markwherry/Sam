#ifndef VSTPLUGINWIDGET_H
#define VSTPLUGINWIDGET_H

#include <QWidget>
//#include "ui_VstPluginWidget.h"

#include <Windows.h>

class VstPluginWidget : public QWidget
{
	Q_OBJECT

public:
	VstPluginWidget(QWidget *parent = 0);
	~VstPluginWidget();

	inline HWND hWnd(void) const { return m_hWnd; }

public slots:
	void setVisible (bool visible);

protected:
	void resizeEvent (QResizeEvent *ev);

private:
	//Ui::VstPluginWidget ui;
	HWND m_hWnd;
};

#endif // VSTPLUGINWIDGET_H
