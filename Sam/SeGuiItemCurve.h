#ifndef SEGUIITEMCURVE_H
#define SEGUIITEMCURVE_H

#include "UiListItem.h"
//#include "ui_SeGuiItemCurve.h"

#include "UiCurve.h"
#include "UiSelectMenu.h"
#include "UiLabel.h"
#include "UiValue.h"

class SeGuiItemCurve : public UiListItem
{
	Q_OBJECT

public:
	SeGuiItemCurve(QWidget *parent, QString title, bool showStartAndEnd = true);
	~SeGuiItemCurve();

	void setShape(int id);
	void setStart(float start);
	void setEnd(float end);
	void setDepth(float q);

	void setPropertyIdShape(int propertyId) { m_shapePropertyId = propertyId; }
	void setPropertyIdStart(int propertyId) { m_startPropertyId = propertyId; }
	void setPropertyIdEnd(int propertyId) { m_endPropertyId = propertyId; }
	void setPropertyIdDepth(int propertyId) { m_depthPropertyId = propertyId; }

	void setCurveColor(QColor color) { curve->setColor(color); }

public slots:
	void internalSetShape(int id);
	void internalSetStart(float percentage);
	void internalSetEnd(float percentage);
	void internalSetDepth(float q);

signals:
	void shapeChanged(int propertyId, float id);
	void startChanged(int propertyId, float start);
	void endChanged(int propertyId, float end);
	void depthChanged(int propertyId, float q);

protected:
	void paintEvent(QPaintEvent *event);

private:
	//Ui::SeGuiItemCurveClass ui;
	UiCurve *curve;
	QString m_title;
	UiSelectMenu *selectShape;
	UiLabel *labelShape;
	UiLabel *labelStart;
	UiLabel *labelEnd;
	UiValue *valueStart;
	UiValue *valueEnd;
	int m_shapePropertyId;
	int m_startPropertyId;
	int m_endPropertyId;
	int m_depthPropertyId;
};

#endif // SEGUIITEMCURVE_H
