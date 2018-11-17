#ifndef SAMGUIVSTPLUGINMANAGER_H
#define SAMGUIVSTPLUGINMANAGER_H

#include <QWidget>
//#include "ui_SamGuiVstPluginManager.h"

#include "UiCheckBox.h"

class SamGuiVstPluginManager : public QWidget
{
	Q_OBJECT

public:
	SamGuiVstPluginManager(QWidget *parent = 0);
	~SamGuiVstPluginManager();

public slots:
	void setUse(bool state);

protected:
	void closeEvent(QCloseEvent *ev);
	void paintEvent(QPaintEvent *ev);
	void showEvent(QShowEvent *ev);

private:
	//Ui::SamGuiVstPluginManager ui;
	QPixmap *m_pixmap;
	UiCheckBox *m_checkUse;
	bool m_initialState;
};

#endif // SAMGUIVSTPLUGINMANAGER_H
