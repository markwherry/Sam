#ifndef __ALog_h__
#define __ALog_h__

#include "APlatform.h"

namespace Atom {

class ALog
{
public:
	static void output(const wchar_t *format, ...);
	static void output(const char *format, ...);

protected:
private:
};

}

#define aLog Atom::ALog::output

// CsAssert
// A custom assert
// See:
// http://www.ddj.com/dept/cpp/184403861
// http://www.devarticles.com/c/a/Cplusplus/C-plus-plus-Preprocessor-Always-Assert-Your-Code-Is-Right/3/

#define CSASSERT \
if (false) {} else struct CsAssert { \
	int m_line; \
	CsAssert (int line=__LINE__) : m_line(line) {} \
CsAssert (bool isOK, char const *message="")\
{\
	if (!isOK){ \
	aLog ("%s\n",message); \
	aLog ("test=false at line:%d in file: %s\n",CsAssert().m_line,__FILE__); \
	abort(); \
	}\
}\
}CsAsserter=CsAssert

#endif