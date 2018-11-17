#ifndef UICOMMANDPROGRESS_H
#define UICOMMANDPROGRESS_H

#include <QWidget>
//#include "ui_UiCommandProgress.h"

#include "UiProgressBar.h"
#include "UiLabel.h"

class UiCommandProgress : public QWidget
{
	Q_OBJECT

public:
	UiCommandProgress(QWidget *parent = 0);
	~UiCommandProgress();

	void clear(void);
	void setProgress(int commandId, float progress);
	void setProgressLabel(int commandId, int labelId, QString text);

protected:
	void closeEvent(QCloseEvent *ev);
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::UiCommandProgressClass ui;
	UiProgressBar *progressBar;
	UiLabel *label[2];
};

#endif // UICOMMANDPROGRESS_H
