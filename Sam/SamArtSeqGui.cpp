#include "SamArtSeqGui.h"

#include <QPainter>
#include <QMouseEvent>

#include "SamGuiArticulationEditor.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamArtSeqGui::SamArtSeqGui(SamMusicTool *tool)
	: QWidget(0)
	, m_tool((SamArtSeq*)tool)
	, m_stepClicked(-1)
{
	//ui.setupUi(this);

	setFixedSize(774, 105);

	m_menu = new QMenu();
	connect(m_menu, SIGNAL(triggered(QAction*)), this, SLOT(setStep(QAction*)));

	QAction *noAction = m_menu->addAction("None");
	m_actions.append(noAction);
	m_menu->addSeparator();

	for(int i=0; i<m_tool->toolMaster()->numArticulations(); i++) {
		QAction *action = m_menu->addAction(m_tool->toolMaster()->articulationName(i).toChar());
		m_actions.append(action);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamArtSeqGui::~SamArtSeqGui()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamArtSeqGui::setStep(QAction *action)
{
	qDebug("SamArtSeqGui::setStep()");

	int artIndex = -1;

	if(action == m_actions[0]) {
		m_tool->setNumSteps(m_stepClicked);
	} else {
		for(int i=1; i<m_actions.size(); i++) {
			if(action == m_actions[i]) {
				artIndex = i-1;
				break;
			}
		}

		qDebug("%d, %d, %d", artIndex, m_stepClicked, m_tool->numSteps());

		if(artIndex > -1 && m_stepClicked > -1) {
			if(m_stepClicked <= m_tool->numSteps()) {
				m_tool->setStep(m_stepClicked, artIndex);
				if(m_stepClicked+1 > m_tool->numSteps()) {
					m_tool->setNumSteps(m_stepClicked+1);
				}
			}

			m_stepClicked = -1;
		}
	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamArtSeqGui::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.fillRect(0,0,width(),height(),QColor(174, 167, 154));

	p.setPen(QColor(0,0,0,60));

	int y=20;

	for(int i=0; i<5; i++) { 
		p.drawLine(0, y, width(), y);
		y+=6;
	}

	int x = 54;
	p.setPen(QColor(0,0,0));
	p.setBrush(QColor(0,0,0,0));

	for(int i=0; i<8; i++) {
		if(i >= m_tool->numSteps()) {
			p.setPen(QColor(0,0,0,120));
		} else {
			p.drawRect(x+1, 14+1, 36-2, 36-2);
			p.setFont(QFont("Calibri", 10));

			QString str = m_tool->toolMaster()->articulationName(m_tool->step(i)).toChar();
			/*if(str.contains(" ")) {
				for(int j=0; j<str.length(); j++) {
					if(str.at(j) == ' ') {
						QString strl = str.left(j);
						QString strr = str.right(str.length()-j-1);
						str = strl + QString("\n") + strr;
						break;
					}
				}
			}*/
			str = SamGuiArticulationEditor::formatArticulationName(str);
			p.drawText(x-18, 60, 72, 40, Qt::AlignTop|Qt::AlignHCenter, str);
		}
		p.drawRect(x, 14, 36, 36);

		

		x+=90;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamArtSeqGui::mousePressEvent(QMouseEvent *ev)
{
	int x = 54;
	int step = -1;

	for(int i=0; i<8; i++) {
		if(QRect(x, 14, 36, 36).contains(ev->pos())) {
			step = i;
			break;
		}
		x+=90;
	}

	if(step > -1) {
		m_stepClicked = step;
		m_menu->popup(ev->globalPos());
	}


}