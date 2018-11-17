#ifndef SAMGUIOUTPUTHEADER_H
#define SAMGUIOUTPUTHEADER_H

#include <QWidget>
//#include "ui_SamGuiOutputHeader.h"

class SamGuiOutputHeader : public QWidget
{
	Q_OBJECT

public:
	SamGuiOutputHeader(QWidget *parent = 0);
	~SamGuiOutputHeader();

	void setHighlight(int output);

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiOutputHeader ui;

	bool m_highlights[256];
};

#endif // SAMGUIOUTPUTHEADER_H
