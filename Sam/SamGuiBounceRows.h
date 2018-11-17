#ifndef SAMGUIBOUNCEROWS_H
#define SAMGUIBOUNCEROWS_H

#include <QWidget>
//#include "ui_SamGuiBounceRows.h"

#include "SamGuiBounceRow.h"

class SamGuiBounceRows : public QWidget
{
	Q_OBJECT

public:
	SamGuiBounceRows(QWidget *parent = 0);
	~SamGuiBounceRows();

	bool isStreamSelected(int row, int streamIndex) { return m_rows[row]->isStreamSelected(streamIndex); }

	void setChannel(SamChannel *channel);

	void setActive(int row, bool active);

	SamGuiBounceRow* row(int index) { return m_rows[index]; }

private:
	//Ui::SamGuiBounceRowsClass ui;
	SamChannel::Ptr m_channel;
	SamGuiBounceRow *m_header;
	SamGuiBounceRow *m_rows[8];
};

#endif // SAMGUIBOUNCEROWS_H
