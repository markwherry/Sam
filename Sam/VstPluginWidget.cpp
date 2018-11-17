#include "VstPluginWidget.h"

#include <QResizeEvent>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
VstPluginWidget::VstPluginWidget(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

	m_hWnd = CreateWindow(L"Static", 0, WS_CHILD, 0, 0, 100,100, (HWND)parent->winId(), 0, 0, 0);
	if(m_hWnd) {
		qDebug("m_hWnd OK");
		ShowWindow(m_hWnd, 1);
	} else {
		qDebug ("m_hWnd is NULL");
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
VstPluginWidget::~VstPluginWidget()
{
	if(m_hWnd) {
		DestroyWindow(m_hWnd);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void VstPluginWidget::setVisible (bool visible)
{
	QWidget::setVisible (visible);
	ShowWindow(m_hWnd, (int)visible);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void VstPluginWidget::resizeEvent (QResizeEvent *ev)
{
	SetWindowPos(m_hWnd, HWND_TOP,x(),y(), ev->size().width(), ev->size().height(),0);
}