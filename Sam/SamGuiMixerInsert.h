#ifndef SAMGUIMIXERINSERT_H
#define SAMGUIMIXERINSERT_H

#include <QWidget>
//#include "ui_SamGuiMixerInsert.h"

#include "SamChannel.h"

#include "SamGuiSelectAudioTool.h"

class SamGuiMixerInsert : public QWidget
{
	Q_OBJECT

public:
	SamGuiMixerInsert(QWidget *parent, SamChannel *channel, int outputIndex, int insertIndex);
	~SamGuiMixerInsert();

protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void mousePressEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);
	void resizeEvent(QResizeEvent *ev);

private:
	//Ui::SamGuiMixerInsert ui;

	SamChannel *m_channel;
	int m_outputIndex;
	int m_insertIndex;

	SamGuiSelectAudioTool *m_selectAudioTool;

	QPixmap *m_pixmap;

	bool m_mouseOver;
};

#endif // SAMGUIMIXERINSERT_H
