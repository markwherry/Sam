#include "ALog.h"

#include <stdio.h>

namespace Atom {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void ALog::output(const wchar_t *format, ...)
{
	wchar_t	buf[4096], *p = buf;
	va_list	args;

	va_start(args, format);
	//p += _vsnwprintf(p, sizeof buf - 1, format, args);
	p += _vsnwprintf_s(p, sizeof(buf) - 1, _TRUNCATE, format, args);
	va_end(args);

	while ( p > buf  &&  isspace(p[-1]) )
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p   = '\0';

	OutputDebugString(buf);
	wprintf (buf);
	//printf (buf);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void ALog::output(const char *format, ...)
{
	char buf[4096], *p = buf;
	va_list	args;

	va_start(args, format);
	//p += _vsnprintf(p, sizeof buf - 1, format, args);
	p += _vsnprintf_s(p, sizeof(buf)- 1, _TRUNCATE, format, args);
	va_end(args);

	while ( p > buf  &&  isspace(p[-1]) )
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p   = '\0';

	WCHAR wbuf[4096];
	int bufLen = (int)strlen(buf) + 1;
	MultiByteToWideChar (CP_ACP, 0, buf, bufLen, wbuf, sizeof(wbuf));

	OutputDebugString(wbuf);
	printf(buf);
}

}