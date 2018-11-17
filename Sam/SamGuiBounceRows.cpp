#include "SamGuiBounceRows.h"

SamGuiBounceRows::SamGuiBounceRows(QWidget *parent)
	: QWidget(parent)
	, m_channel(0)
{
	//ui.setupUi(this);

	int y = 0;
	m_header = new SamGuiBounceRow(this, -1);
	y += m_header->height();
	for(int i=0; i<8; i++) {
		m_rows[i] = new SamGuiBounceRow(this, i);
		m_rows[i]->move(0, y);
		y+=(m_rows[i]->height()-1);
	}

	setFixedHeight(y+1);
}

SamGuiBounceRows::~SamGuiBounceRows()
{

}

void SamGuiBounceRows::setChannel(SamChannel *channel)
{
	m_channel = channel;

	m_header->setChannel(channel);
	for(int i=0; i<8; i++) {
		m_rows[i]->setChannel(channel);
	}

	setFixedWidth(m_rows[0]->width());
}

void SamGuiBounceRows::setActive(int row, bool active)
{
	m_rows[row]->setActive(active);
}