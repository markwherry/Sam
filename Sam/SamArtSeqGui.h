#ifndef SAMARTSEQGUI_H
#define SAMARTSEQGUI_H

#include <QWidget>
//#include "ui_SamArtSeqGui.h"

#include <QMenu>

#include "SamMusicTool.h"

#include "SamArtSeq.h"

class SamArtSeqGui : public QWidget
{
	Q_OBJECT

public:
	SamArtSeqGui(SamMusicTool *tool);
	~SamArtSeqGui();

public slots:
	void setStep(QAction *action);

protected:
	void paintEvent(QPaintEvent *ev);
	void mousePressEvent(QMouseEvent *ev); 

private:
	//Ui::SamArtSeqGui ui;
	SamArtSeq *m_tool;
	QMenu *m_menu;
	QList<QAction*> m_actions;
	int m_stepClicked;
};

#endif // SAMARTSEQGUI_H
