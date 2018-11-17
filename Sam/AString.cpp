#include "AString.h"
#include "ALog.h"

namespace Atom {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString::AString()
: m_data(0)
, numCharactersAllocated(0)
, outCStr(0)
{
	//aLog("String::String()");	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString::AString(const AString& str)
: m_data(0)
, numCharactersAllocated(0)
, outCStr(0)
{
	//aLog("String::String(const)");
	if(str.m_data) {
		allocateString(str.m_data);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString::AString(char *str)
: m_data(0)
, numCharactersAllocated(0)
, outCStr(0)
{
	//aLog("String::String(char)");
	allocateString(str);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString::AString(wchar_t *str)
: m_data(0)
, numCharactersAllocated(0)
, outCStr(0)
{
	//aLog("String::String(wchar_t)");
	allocateString(str);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString::~AString()
{
	//csDebug("String::~String() [%s] numCharactersAllocated = %d", toChar(), numCharactersAllocated);
	clear();
	//csDebug("EXIT: String::~String()");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString& AString::operator=(const AString& other)
{
	//aLog("String::operator=(String)");

	if(this != &other ) {
		if(other.m_data) {
			allocateString(other.m_data);
		} else {
			clear();
		}
	} else {
		//csDebug("Self initialisation");
	}

	//csDebug("EXIT: String::operator=(String)");

	return *this;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool AString::operator==(const AString& str)
{
	return isEqualTo(str);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool AString::operator!=(const AString& str)
{
	return !isEqualTo(str);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString AString::operator+(const AString& str)
{
	AString retStr(m_data);
	retStr.append(str);
	return retStr;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
wchar_t AString::at(int position)
{
	if(position < 0 || position >= numCharactersAllocated) {
		return 0;
	}

	return m_data[position];
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool AString::isEqualTo(AString str)
{
	if(length() != str.length()) {
		//aLog("Length not equal");
		return false;
	}

	bool isEqual = true;

	for(int i=0; i<length(); i++) {
		if(m_data[i]!=str.m_data[i]) {
			//aLog("[%d] %c != %c", i, m_data[i], str.m_data[i]);
			return false;
		}
	}

	return isEqual;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AString::append(AString str)
{
	//aLog("String::append(AString)");

	if(str.length()<1) {
		//aLog("length < 1");
		return;
	}

	//aLog("numCharactersAllocated = %d", numCharactersAllocated);

	m_data = (wchar_t*)realloc(m_data, (numCharactersAllocated+str.length()+1)*sizeof(wchar_t));
	
	//aLog("size = %d", (sizeof(wchar_t)*(numCharactersAllocated-2)));
	memcpy(m_data+numCharactersAllocated, str.m_data, sizeof(wchar_t)*str.length());

	numCharactersAllocated += str.length();

	m_data[numCharactersAllocated] = 0;

	//outCStr = 0;

	//aLog("m_data = %s", toChar());

	//aLog("numCharactersAllocated = %d", numCharactersAllocated);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AString::append(wchar_t ch)
{
	//aLog("String::append(wchar_t)");

	m_data = (wchar_t*)realloc(m_data, (numCharactersAllocated+1+1)*sizeof(wchar_t));
	numCharactersAllocated++;

	m_data[numCharactersAllocated-1] = ch;
	m_data[numCharactersAllocated] = 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AString::clear()
{
	if(m_data) {
		free(m_data);
		m_data = 0;
	}

	if(outCStr) {
		free(outCStr);
		outCStr = 0;
	}
	numCharactersAllocated = 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AString::insert(int pos, AString str)
{
	aLog("AString::insert(%d, %s)", pos, str.toChar());

	if(str.length()<1) {
		//aLog("length < 1");
		return;
	}

	m_data = (wchar_t*)realloc(m_data, (numCharactersAllocated+str.length()+1)*sizeof(wchar_t));
	
	//aLog("size = %d", (sizeof(wchar_t)*(numCharactersAllocated-2)));
	
	memcpy(m_data+pos+str.length(), m_data+pos, sizeof(wchar_t)*str.length());

	memcpy(m_data+pos, str.m_data, sizeof(wchar_t)*str.length());

	numCharactersAllocated += str.length();

	m_data[numCharactersAllocated] = 0;

	//outCStr = 0;

	aLog("m_data = %s", toChar());

	//aLog("numCharactersAllocated = %d", numCharactersAllocated);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AString::remove(int pos, int num)
{
	aLog("AString::remove(pos=%d, num=%d)", pos, num);
	int newLength = numCharactersAllocated - num;
	aLog("newLength = %d", newLength);

	memcpy(m_data+pos, m_data+pos+num, sizeof(wchar_t)*(numCharactersAllocated-(pos+num)));
	aLog("m_data = %s", toChar());

	m_data = (wchar_t*)realloc(m_data, sizeof(wchar_t)*(newLength + 1));
	numCharactersAllocated = newLength;
	m_data[numCharactersAllocated] = 0;

	aLog("m_data = %s (%d)", toChar(), numCharactersAllocated);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int AString::findCharacter(wchar_t character, int from)
{
	if(from == fromFront) {
		for(int i=0; i<numCharactersAllocated; i++) {
			if(m_data[i] == character) {
				return i;
			}
		}
	} else if(from == fromBack) {
		for(int i=numCharactersAllocated-1; i>-1; i--) {
			//aLog("[%d] %d %d", i, m_data[i], character);
			if(m_data[i] == character) {
				return i;
			}
		}
	}

	return -1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString AString::first(int numCharacters)
{
	if(numCharacters > numCharactersAllocated) {
		numCharacters = numCharactersAllocated;
	}

	AString str;
	str.allocate(numCharacters);
	memcpy(str.m_data, m_data, sizeof(wchar_t)*numCharacters);
	//for(int i=0; i<numCharacters; i++) {
	//	str.m_data[i] = m_data[i];
	//}
	//str.m_data[numCharacters] = 0;

	return str;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString AString::last(int numCharacters)
{
	//aLog("AString::last(%d)", numCharacters);
	if(numCharacters > numCharactersAllocated) {
		numCharacters = numCharactersAllocated;
	}

	AString str;
	str.allocate(numCharacters);
	//memcpy(str.m_data, m_data+(sizeof(wchar_t)*numCharactersAllocated-numCharacters), sizeof(wchar_t)*numCharacters);
	int counter = 0;
	for(int i=numCharactersAllocated-numCharacters; i<numCharactersAllocated; i++) {
		str.m_data[counter] = m_data[i];
		counter++;
	}
	str.m_data[numCharacters] = 0;

	//aLog("EXIT: AString::last()");
	return str;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
char *AString::toChar(void)
{
	if(!m_data) {
		return 0;
	}

	if(outCStr) {
		free(outCStr);
	}
	outCStr = (char*)malloc(sizeof(char)*(numCharactersAllocated+1));

	WideCharToMultiByte(CP_ACP, 0, m_data, -1, outCStr, numCharactersAllocated, NULL, NULL);

	outCStr[numCharactersAllocated] = 0;

	return outCStr;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString AString::path(void)
{
	int p = findCharacter(L'//', fromBack);

	if(p<0) {
		p = findCharacter(L'\\', fromBack);
	}

	if(p<0) {
		return "";
	}
	
	return first(p+1);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int AString::toInt(void)
{
	int n = 0;
	int ten = 1;

	for(int i=numCharactersAllocated-1; i>=0; i--) {
		//aLog("[%d] %c (%d)", i, m_data[i], m_data[i]);
		if((m_data[i] < 48 || m_data[i] >57) && (m_data[i] == 45 && i > 0)) {
			return 0;
		}
		
		if(m_data[i] == 45 && i==0) {
			n = -n;
		} else {
			n += (((int)m_data[i]-48)*ten);
			ten *= 10;
		}
	}

	return n;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float AString::toFloat(void)
{
	if(length() > 0) {
		return (float)atof(toChar());
	}

	return 0.0f;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
//http://clc-wiki.net/wiki/K%26R2_solutions:Chapter_3:Exercise_4
AString AString::number(int n)
{
	AString str;
	int i, sign;
    sign = n;
    
	i = 0;
	do {
		str.append( abs(n % 10) + '0');
	} while ( n /= 10 );
	if (sign < 0)
       str.append('-');
    
	//s[i] = '\0';
    str.reverse();

	return str;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AString::reverse(void)
{
	wchar_t c;
	int i, j;
    for(i = 0, j = numCharactersAllocated-1; i < j; i++, j--) {
		c = m_data[i];
		m_data[i] = m_data[j];
		m_data[j] = c;
    }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AString::allocate(int numCharacters)
{
	m_data = (wchar_t*)malloc((numCharacters+1)*sizeof(wchar_t));
	numCharactersAllocated = numCharacters;
	m_data[numCharacters] = 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AString::allocateString(char *str)
{
	int length = (int)strlen(str);
	//aLog("String::allocateString() length=%d", length);
	if(length<1) {
		clear();
		return;
	}

	m_data = (wchar_t*)malloc((length+1)*sizeof(wchar_t));
	numCharactersAllocated = length;

	MultiByteToWideChar(CP_ACP, 0, str, -1, m_data, length);

	m_data[length] = 0;

	outCStr = 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AString::allocateString(wchar_t *str)
{
	//aLog("AString::allocateString()");

	int length = (int)wcslen(str);
	if(length<1) {
		//clear();
		return;
	}
	//aLog("String::allocateString() length=%d", length);

	m_data = (wchar_t*)malloc((length+1)*sizeof(wchar_t));
	numCharactersAllocated = length;

	memcpy(m_data, str, sizeof(wchar_t)*length);

	m_data[length] = 0;

	outCStr = 0;
}

}