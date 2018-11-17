#ifndef __AString_h__
#define __AString_h__

#include "APlatform.h"

namespace Atom {

class AString
{
public:
	AString();
	AString(const AString& str);
	AString(char *str);
	AString(wchar_t *str);
	~AString();

	AString& operator=(const AString& str);
	bool operator==(const AString& str);
	bool operator!=(const AString& str);
	AString operator+(const AString& str);
	wchar_t operator[](int index) { return at(index); }


	wchar_t at(int position);

	bool isEqualTo(AString str);
	void append(AString str);
	void append(wchar_t ch);
	void clear();
	void insert(int pos, AString str);
	void remove(int pos, int num = 1);

	int findCharacter(wchar_t character, int from);
	enum {
		fromFront = 0,
		fromBack = 1
	};

	AString first(int numCharacters);
	AString last(int numCharacters);

	const wchar_t* data(void) { return m_data; }
	char* toChar(void);

	int length(void) { return numCharactersAllocated; }

	AString path(void);

	int toInt(void);
	float toFloat(void);
	static AString number(int n);
	void reverse(void);

protected:
private:
	void allocate(int numCharacters);
	void allocateString(char *str);
	void allocateString(wchar_t *str);
	wchar_t *m_data;
	char *outCStr;
	int numCharactersAllocated;
};

}

#endif