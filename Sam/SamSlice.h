#ifndef __SamSlice_h__
#define __SamSlice_h__

#include "AModel.h"
#include "AList.h"
#include "AXmlDocument.h"
using namespace Atom;

class SamSlice : public AModel
{
public:
	SamSlice(int samplePos = 0, int ppqnPos = 0, int length = 0);

	int samplePos(void) { return m_samplePos; }
	void setSamplePos(int samplePos) { m_samplePos = samplePos; }

	int ppqnPos(void) { return m_ppqnPos; }
	void setPpqnPos(int ppqnPos) { m_ppqnPos = ppqnPos; }

	int length(void) { return m_length; }
	void setLength(int length) { m_length = length; }

	int readElement(AXmlDocument *document, AXmlElement *element);
	int writeElement(AXmlDocument *document, AXmlElement *element);

protected:
private:
	int m_samplePos;
	int m_ppqnPos;
	int m_length;
};

#endif