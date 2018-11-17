#ifndef SAMGUIINDICATORBLOCK_H
#define SAMGUIINDICATORBLOCK_H

#include <QWidget>
//#include "ui_SamGuiIndicatorBlock.h"

#include "UiIndicator.h"

class SamGuiIndicatorBlock : public QWidget
{
	Q_OBJECT

public:
	SamGuiIndicatorBlock(QWidget *parent, QString label, QColor color = QColor(255,255,255), int controller = -1);
	~SamGuiIndicatorBlock();

	void setValue(int value);

	int controller(void) { return m_controller; }

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiIndicatorBlockClass ui;
	QColor m_color;
	int m_value;
	QString m_label;
	int m_controller;
};

#endif // SAMGUIINDICATORBLOCK_H
