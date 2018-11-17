#include "UiPianoKeyboard.h"

#include <QPainter>
#include <QMouseEvent>

#include "UiObject.h"

UiPianoKeyboard::UiPianoKeyboard(QWidget *parent, float pitchWidth, int depth, int numPitches, int firstMidiNote, KeyboardOrientation orientation)
	: QWidget(parent)
	, m_pitchWidth(pitchWidth)
	, m_depth(depth)
	, m_orientation(orientation)
	, m_numPitches(numPitches)
	, m_firstMidiNote(firstMidiNote)
	, m_numWhiteNotes(0)
	, m_style(0)
	, m_clickedOnPitch(-1)
	, m_leftButtonPressed(false)
{
	//ui.setupUi(this);

	float length = (pitchWidth * (float)m_numPitches) + 1.0f;

	setFixedSize((float)length, depth);

	m_keyboardLength = pitchWidth*m_numPitches;


/*if(firstMidiNote + numPitches > 127) {
		numPitches -= ((firstMidiNote + numPitches) - 127);
	}

	m_keyboardLength = m_pixelsPerPitch*m_numPitches;
	m_keyboardDepth = m_whiteNoteDepth;
	
	if(isHorizontal()) {
		setFixedSize(m_keyboardLength, m_keyboardDepth);
	} else {
		setFixedSize(m_keyboardDepth, m_keyboardLength);
	}*/

	for(int i=0; i<128; i++) {
		m_pitchDown[i] = 0;
		m_aftertouch[i] = 0;
		m_pitchActive[i] = true;
	}

	m_noteDownColor = QColor(220,220,220);
}

UiPianoKeyboard::~UiPianoKeyboard()
{

}

void UiPianoKeyboard::setNoteDown(int pitch, int velocity)
{
	//qDebug("UiPianoKeyboard::setNoteDown(%d, %d)", pitch, velocity);
	m_pitchDown[pitch] = velocity;
	update();
}

void UiPianoKeyboard::setPitchWidth(float pixels)
{
	m_pitchWidth = pixels;
	update();
}

/*void UiPianoKeyboard::setNoteVelocities(int* velocities)
{
	for(int i=0; i<128; i++) {
		m_pitchDown[i] = velocities[i];
	}
	update();
}*/

void UiPianoKeyboard::mouseMoveEvent(QMouseEvent *ev)
{
	if (ev->buttons()==Qt::LeftButton) {
		if(rect().contains(ev->pos())) {
			int pitch = pitchFromPoint(ev->x(), ev->y());
			if (pitch!=m_clickedOnPitch) {
				emit pitchReleased(m_clickedOnPitch, 0, ev->modifiers());
				emit pitchAftertouch(m_clickedOnPitch, 0);
				emit pitchPressed(pitch, 90, ev->modifiers());

				//setNoteDown(m_clickedOnPitch, false);
				//setNoteDown(pitch, true);

				m_clickedOnPitch = pitch;
			}
		} else {
			int y = -ev->y();
			if(y>-1) {
				//y -= 10;
				y /= 3;
				if(y > 127) {
					y = 127;
				}
				emit pitchAftertouch(m_clickedOnPitch, y);
			}

		}
	}
}

void UiPianoKeyboard::mousePressEvent(QMouseEvent *ev)
{
	//qDebug("%d", pitchFromPoint(ev->x(), ev->y()));

	//int pitch = pitchFromPoint(ev->x(), ev->y());
	//qDebug("pitch = %d", pitch);

	//emit pitchPressed(pitch, 100, ev->modifiers());


	m_clickedOnPitch = pitchFromPoint(ev->x(), ev->y());
	m_leftButtonPressed = false;

	if (ev->buttons()==Qt::LeftButton) {
		if(m_clickedOnPitch>-1) {
			//setNoteDown(m_clickedOnPitch, true);
			emit pitchPressed(m_clickedOnPitch, 90, ev->modifiers());
		}
		m_leftButtonPressed = true;
	}
}

void UiPianoKeyboard::mouseReleaseEvent(QMouseEvent *ev)
{
	if(m_leftButtonPressed && m_clickedOnPitch > -1) {
		//setNoteDown(m_clickedOnPitch, false);
		emit pitchReleased(m_clickedOnPitch, 0, ev->modifiers());
		emit pitchAftertouch(m_clickedOnPitch, 0);
	}
}

void UiPianoKeyboard::paintEvent(QPaintEvent *paintEvent)
{
	QPainter p(this);

	if(m_style==0) {
		p.setPen(QColor(30,30,30));
		p.setBrush(QColor(240,240,240));
	} else {
		p.setPen(QColor(201,201,216));
		p.setBrush(QColor(201,201,216));
	}
	p.drawRect(0,0,width()-1, height()-1);

	if(isHorizontal()) {
		p.setPen(QColor(110,110,110));
		p.drawLine(1,1,width()-2,1);
		p.setPen(QColor(180,180,180));
		p.drawLine(1,2,width()-2,2);
		p.drawLine(1, height()-2,width()-2,height()-2);
	} else {
		if(m_style==0) {
			p.setPen(QColor(110,110,110));
		} else {
			p.setPen(QColor(0,0,0));
		}
		p.drawLine(1,1,1,height()-2);
		if(m_style==0) {
			p.setPen(QColor(180,180,180));
		}
		p.drawLine(2,1,2,height()-2);
		p.drawLine(width()-2,1,width()-2,height()-2);
		if(m_style==1) {
			p.drawLine(width()-3,1,width()-3,height()-2);
		}
	}

	bool isBlackNote[12] = { false, true, false, true, false, false, true, false, true, false, true, false };

	int blackNoteDepth = (m_depth/5)*3;

	for(int i=0; i<m_numPitches; i++) {
		int noteInOctave = (m_firstMidiNote+i)%12;

		if(!isBlackNote[noteInOctave]) {
			float x = 3.0f;
			/*if(i!=0) {
				switch(noteInOctave) {
					case 2:
					case 4:
					case 7:
					case 9:
					case 11:
						x = blackNoteDepth + 1;
						break;
				}
			}*/
			
			float y = whiteNotePos(i, noteInOctave);
			int nnio = noteInOctave+nextWhiteNote(noteInOctave);
			if(nnio>11) {
				nnio=0;
			}
			float w = whiteNotePos(i+nextWhiteNote(noteInOctave), nnio)-y;

			if(isHorizontal()) {
				paintWhiteNote(p, m_firstMidiNote+i, y, x, w, m_depth-2);
				if(m_firstMidiNote+i == 60 && m_pitchDown[60] == 0) {
					p.fillRect(QRectF(y, x, w, m_depth-2.0f), QColor(0,0,0,50));
				}
				if(noteInOctave == 0) {
					p.setPen(QColor(0,0,0));
					int octave = ((m_firstMidiNote+i)/12) - 2;
					//p.drawRect(y, x+((m_depth/3)*2), w, m_depth/3);
					p.drawText(y, x+(m_depth/2), w, m_depth/2, Qt::AlignCenter|Qt::AlignVCenter, QString::number(octave));
				}
			} else {
				paintWhiteNote(p, m_firstMidiNote+i, x, y, (float)m_depth-2.f, w);
			}
		}
	}

	for(int i=0; i<m_numPitches; i++) {
		int noteInOctave = (m_firstMidiNote+i)%12;

		if(isBlackNote[noteInOctave]) {
			if(isHorizontal()) {
				paintBlackNote(p, m_firstMidiNote+i, i*m_pitchWidth, 1, m_pitchWidth, blackNoteDepth-1);
			} else {
				paintBlackNote(p, m_firstMidiNote+i, 1, m_keyboardLength-((i+1)*m_pitchWidth), blackNoteDepth-1, m_pitchWidth);
			}
		} 
	}

	return;
	
	
	
	
	//QPainter p(this);

	p.setPen(QColor(0,0,0));
	p.setBrush(QColor(0,0,0));
	p.drawRect(0,0,width()-1, height()-1);

	

	int x = 0;
	//bool isBlackNote[12] = { false, true, false, true, false, false, true, false, true, false, true, false };


	float d = m_pitchWidth/4.0f;

	//float whiteNoteWidth = ((float)m_pitchWidth * 12.0f) / 7.0f;
	
	float whiteNoteOffsets[12] = { 0, 0, -d*1, 0, -d*3, 0, 0, 0, 0, 0, 0, 0 };
	float whiteNoteWidths[12] = { d*3, 0, d*(1+1), 0, d*3, 0, 0, 0, 0, 0, 0, 0 };


	for(int i=m_firstMidiNote; i<m_firstMidiNote+m_numPitches; i++) {
		if(!isBlackNote[i%12]) {
			paintWhiteNote(p, i, x+whiteNoteOffsets[i%12], 0, 10+(int)whiteNoteWidths[i%12], height()-1);
			/*if(i == 60) {
				p.fillRect(x+whiteNoteOffsets[i%12], 0, 10+(int)whiteNoteWidths[i%12], height()-1, QColor(0,0,0,255));
			}*/
			
		}

		x += 10;
	}

	x = 0;
	int blackNoteHeight = (height()/5) * 3;
	for(int i=m_firstMidiNote; i<m_firstMidiNote+m_numPitches; i++) {
		if(isBlackNote[i%12]) {
			paintBlackNote(p, i, x,0,10,blackNoteHeight);
		}

		x += 10;
	}

	



	return;
}

void UiPianoKeyboard::paintWhiteNote(QPainter &p, int midiNote, float x, float y, float w, float h)
{
	if(m_pitchDown[midiNote] > 0) {
		p.setPen(m_noteDownColor);
		QColor c = m_noteDownColor;
		c.setAlpha(100+m_pitchDown[midiNote]);
		p.setBrush(c);
		p.drawRect(QRectF(x,y-2.0f,w,h+1.0f));

		if(m_aftertouch[midiNote] > 0) {
			float unit = (float)h/128.0f;
			float ah = unit * m_aftertouch[midiNote];
			p.fillRect(QRectF(x, y+h-ah, w, ah), QColor(255,255,255,125));
		}
	} else if(!m_pitchActive[midiNote]) {
		p.fillRect(QRectF(x, y, w, h), QColor(120,120,120));
	}

	if(isHorizontal()) {
		
		

		p.setPen(QColor(120,120,120));
		p.drawLine(QPointF(x,y),QPointF(x,h));

		if(m_pitchDown[midiNote] == 0 && m_pitchActive[midiNote]) {
			p.setPen(QColor(180,180,180));
			p.drawLine(QPointF(x+w-1.0f,y), QPointF(x+w-1.0f,h));
			
		}
		

		if(m_pitchDown[midiNote] > 0 || !m_pitchActive[midiNote]) {
			p.setPen(QColor(60,60,60));
		} else {
			p.setPen(QColor(255,255,255));
		}
		p.drawLine(QPointF(x+1.0f, y), QPointF(x+1.0f,h));
		

		/*if(m_noteDown[midiNote]) {
			p.setPen(QColor(255,255,255));)
			p.drawLine(x+2, h, x+w-2, h);
		}*/

		
	} else {
		if(m_style==0) {
			p.setPen(QColor(120,120,120));
		} else {
			p.setPen(QColor(0,0,0));
			p.drawLine(QPointF(x,y+1.0f), QPointF(w,y+1.0f));
		}
		p.drawLine(QPointF(x,y), QPointF(w,y));

		if(m_style==0 && m_pitchActive[midiNote]) {
			p.setPen(QColor(180,180,180));
			p.drawLine(QPointF(x,y-1.0f),QPointF(m_depth-2.0f,y-1.0f));
			p.setPen(QColor(255,255,255));
			p.drawLine(QPointF(x,y+1.0f),QPointF(m_depth-2.0f,y+1.0f));
		}
	}

	//if(!m_pitchActive[midiNote]) {
	//	p.fillRect(QRectF(x, y-2.0f, w, h+2.0f),QColor(0,0,0,150));
	//}
}

void UiPianoKeyboard::paintBlackNote(QPainter &p, int midiNote, float x, float y, float w, float h)
{
	p.setPen(QColor(0,0,0));
	if(m_pitchDown[midiNote] > 0) {
		p.fillRect(QRectF(x,y,w,h),QColor(255,255,255));
		QColor c = m_noteDownColor;
		c.setAlpha(100+m_pitchDown[midiNote]);
		p.setBrush(c);
		p.drawRect(x,y,w,h);
		if(m_aftertouch[midiNote] > 0) {
			float unit = (float)h/128.0f;
			float ah = unit * m_aftertouch[midiNote];
			p.fillRect(QRectF(x, y+h-ah, w, ah), QColor(255,255,255,125));
		}
	} else {
		p.setBrush(QColor(0,0,0));
		p.drawRect(x,y,w,h);
	}
	

	

	// Original Style
	if(m_style==0) {
		QColor color(255,255,255);
		QLinearGradient linearGradient(QPointF(0, y+3), QPointF(0, (y+3)+m_pitchWidth*2));
		if(isHorizontal()) {
			linearGradient = QLinearGradient(QPointF(x+3,0), QPointF((x+3)+m_pitchWidth*2,0));
		}

		int alphaOffset = 0;
		if(m_pitchDown[midiNote] > 0) {
				alphaOffset = -20;
			}

		color.setAlpha(90+alphaOffset);//90//40
		linearGradient.setColorAt(0, color);
		color.setAlpha(0);//50//5
		linearGradient.setColorAt(1, color);

		if(isHorizontal()) {
			p.setBrush(linearGradient);
			p.setPen(QColor(60,60,60,100+alphaOffset));

			int additionalHeight = 0;
			if(m_pitchDown[midiNote] >0) {
				additionalHeight = 3;
			}

			p.drawRect(x+3, 2, w-5, (h-1)-((h-1)/5)-2+additionalHeight);

			p.setPen(QColor(255,255,255,50+alphaOffset));
			p.drawLine(x+2, 2, x+2, (h-1)-3+additionalHeight);

			p.setPen(QColor(255,255,255,80+alphaOffset));
			p.drawLine(x+3, 2, x+3, (h-1)-((h-1)/5)-1+additionalHeight);

			p.setPen(QColor(255,255,255,60+alphaOffset));
			p.drawLine(x-1, 2, x-1, (h-1)-3+additionalHeight);
		} else {
			p.setBrush(linearGradient);
			p.setPen(QColor(60,60,60,100));
			p.drawRect(2, y+3, (w+1)-((w+1)/5)-2, h-5);

			p.setPen(QColor(255,255,255,50));
			p.drawLine(2, y+2, (w+1)-3, y+2);

			p.setPen(QColor(255,255,255,80));
			p.drawLine(2, y+3, (w+1)-((w+1)/5)-1, y+3);

			p.setPen(QColor(255,255,255,60));
			p.drawLine(2, y-1, (w+1)-3, y-1);
		}
	}

	if(!m_pitchActive[midiNote]) {
		p.fillRect(QRectF(x,y-2.0f,w,h+2.0f),QColor(0,0,0,150));
	}
}

float UiPianoKeyboard::whiteNotePos(int key, int noteInOctave)
{
	float y = key*m_pitchWidth;
	if(!isHorizontal()) {
		y = m_keyboardLength-(key*m_pitchWidth);
	}
	float offset = 0;
	switch(noteInOctave) {
		case 2:
			offset = ((m_pitchWidth/2.f)-(m_pitchWidth/6.f));
			break;
		case 4:
			offset = ((m_pitchWidth/2.f)+(m_pitchWidth/6.f));
			break;
		case 7:
			offset = ((m_pitchWidth/2.f)-(m_pitchWidth/6.f));
			break;
		case 9:
			offset = ((m_pitchWidth/2.f)+(m_pitchWidth/8.f));
			break;
		case 11:
			offset = ((m_pitchWidth/2.f)+(m_pitchWidth/4.f));
			break;
	}

	if(isHorizontal()) {
		y-=offset;
	} else {
		y+=offset;
	}

	return y;
}

int UiPianoKeyboard::nextWhiteNote(int noteInOctave)
{
	//int next[12] = { 2, 2, 4, 4, 5, 7, 7, 9, 9, 11, 11, 0 };
	int next[12] = { 2, 1, 2, 1,   1, 2, 1, 2, 1, 2, 1, 1 };

	return next[noteInOctave];

	/*0 - 2
	1 - 2
	2 - 4
	3 - 4
	4 - 5
	5 - 7
	6 - 7
	7 - 9
	8 - 9
	9 - 11
	10 - 11
	11 - 0*/
}

int UiPianoKeyboard::pitchFromPoint(int px, int py)
{
	bool isBlackNote[12] = { false, true, false, true, false, false, true, false, true, false, true, false };

	int blackNoteDepth = (m_depth/5)*3;

	// Black notes have priority because they're in the foreground
	
	if((isHorizontal() && py <= blackNoteDepth) || (!isHorizontal() && px <= blackNoteDepth)) {
		for(int i=0; i<m_numPitches; i++) {
			int noteInOctave = (m_firstMidiNote+i)%12;

			if(isBlackNote[noteInOctave]) {
				if(isHorizontal()) {
					if(px >= i*m_pitchWidth && px < (i*m_pitchWidth)+m_pitchWidth) {
						return i+m_firstMidiNote;
					}
					//paintBlackNote(p, m_firstMidiNote+i, i*m_pixelsPerPitch, 1, m_pixelsPerPitch, blackNoteDepth-1);
				} else {
					//paintBlackNote(p, m_firstMidiNote+i, 1, m_keyboardLength-((i+1)*m_pixelsPerPitch), blackNoteDepth-1, m_pixelsPerPitch);
					if(py >= m_keyboardLength-((i+1)*m_pitchWidth) && py < m_keyboardLength-((i+1)*m_pitchWidth)+m_pitchWidth) {
						return i+m_firstMidiNote;
					}
				}
			} 
		}
	}



	for(int i=0; i<m_numPitches; i++) {
		int noteInOctave = (m_firstMidiNote+i)%12;

		if(!isBlackNote[noteInOctave]) {
			int x = 3;
			int y = whiteNotePos(i, noteInOctave);
			int nnio = noteInOctave+nextWhiteNote(noteInOctave);
			if(nnio>11) {
				nnio=0;
			}
			int w = whiteNotePos(i+nextWhiteNote(noteInOctave), nnio)-y;

			if(isHorizontal()) {
				//paintWhiteNote(p, m_firstMidiNote+i, y, x, w, m_keyboardDepth-2);
				if(px >= y && px < y+w) {
					return i+m_firstMidiNote;
				}
			} else {
				//paintWhiteNote(p, m_firstMidiNote+i, x, y, m_keyboardDepth-2, w);
				if(py >= y && py < y+w) {
					return i+m_firstMidiNote;
				}
			}
		}
	}

	return -1;

	
}