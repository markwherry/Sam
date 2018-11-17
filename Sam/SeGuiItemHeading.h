#ifndef SEGUIITEMHEADING_H
#define SEGUIITEMHEADING_H

#include "UiListItem.h"
//#include "ui_SeGuiItemHeading.h"

class SeGuiItemHeading : public UiListItem
{
	Q_OBJECT

public:
	SeGuiItemHeading(QWidget *parent, QString heading);
	~SeGuiItemHeading();

	void setBackgroundColor(QColor backgroundColor) { m_backgroundColor = backgroundColor; }

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SeGuiItemHeadingClass ui;
	QString m_heading;
	QColor m_backgroundColor;
};

#endif // SEGUIITEMHEADING_H
