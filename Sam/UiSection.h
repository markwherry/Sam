#ifndef UISECTION_H
#define UISECTION_H

#include <QWidget>
//#include "ui_UiSection.h"

#include "UiLabel.h"

class UiSection : public QWidget
{
	Q_OBJECT

public:
	UiSection(QWidget *parent, QString title);
	~UiSection();

protected:
	void paintEvent(QPaintEvent *ev);
	void resizeEvent(QPaintEvent *ev);

private:
	//Ui::UiSectionClass ui;
	QString m_title;
	UiLabel *m_labelTitle;
};

#endif // UISECTION_H
