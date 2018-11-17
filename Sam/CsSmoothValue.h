#ifndef __CsSmoothValue__
#define __CsSmoothValue__

namespace Cs {

class CsSmoothValue
{
public:
	CsSmoothValue(int frames);

	float value(void);
	void updateValue(float value, int frame);

protected:
private:
	float nextFrame(void);

	int m_frames;
	float m_value;
	bool m_moving;
	float m_step;
	int m_counter;
};

}

#endif