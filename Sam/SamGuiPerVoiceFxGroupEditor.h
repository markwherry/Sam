#/*ifndef SAMGUIPERVOICEFXGROUPEDITOR_H
#define SAMGUIPERVOICEFXGROUPEDITOR_H

#include <QWidget>
//#include "ui_SamGuiPerVoiceFxGroupEditor.h"

#include "SamChannel.h"

class SamGuiPerVoiceFxGroupEditor : public QWidget
{
	Q_OBJECT

public:
	SamGuiPerVoiceFxGroupEditor(QWidget *parent = 0);
	~SamGuiPerVoiceFxGroupEditor();

	void setChannel(SamChannel *channel);

protected:
	void mouseDoubleClickEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiPerVoiceFxGroupEditor ui;

	int validatePitch(int x);
	int validateZone(int y);

	SamChannel *m_channel;

	bool m_adjustVelocity;
	int m_pixel;
	int m_clickVelocity;
};

#endif // SAMGUIPERVOICEFXGROUPEDITOR_H*/
