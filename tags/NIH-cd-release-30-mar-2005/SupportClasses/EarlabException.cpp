#include "EarlabException.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

EarlabException::EarlabException(const char *FormatString, ...)
{
	va_list vl;

	va_start(vl, FormatString);
	vsprintf(mExceptionString, FormatString, vl);
	va_end(vl);
}

EarlabException::~EarlabException()
{
}

const char *EarlabException::GetExceptionString(void) const
{
	return mExceptionString;
}
