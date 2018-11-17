#ifndef SAMGUIARTICULATIONVECTOR_H
#define SAMGUIARTICULATIONVECTOR_H

#include <QWidget>
//#include "ui_SamGuiArticulationVector.h"

class SamGuiArticulationVector : public QWidget
{
	Q_OBJECT

public:
	SamGuiArticulationVector(QWidget *parent = 0);
	~SamGuiArticulationVector();

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiArticulationVectorClass ui;
};

#endif // SAMGUIARTICULATIONVECTOR_H
