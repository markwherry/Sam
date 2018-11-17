#ifndef SAMGUIBOUNCEROW_H
#define SAMGUIBOUNCEROW_H

#include <QWidget>
//#include "ui_SamGuiBounceRow.h"

#include "SamChannel.h"

class SamGuiBounceRow : public QWidget
{
	Q_OBJECT

public:
	SamGuiBounceRow(QWidget *parent, int id);
	~SamGuiBounceRow();

	bool isStreamSelected(int streamIndex) { return m_streamSelected[streamIndex]; }
	int numStreamsSelected(void);
	void setActive(bool active);
	void setChannel(SamChannel *channel);

protected:
	void mousePressEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiBounceRowClass ui;
	SamChannel::Ptr m_channel;
	int m_id;
	bool m_active;
	bool m_header;
	bool m_streamSelected[128];
};

#endif // SAMGUIBOUNCEROW_H
