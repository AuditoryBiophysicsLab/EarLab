#include "Logger.h"
#include <iostream>
#include <stdarg.h>
#include <memory.h>

using namespace std;

Logger::Logger()
{
	memset(this, 0, sizeof(Logger));
}

void Logger::SetLogCallback(LogCallback theCallback)
{
	mCallback = theCallback;
}

void Logger::Log(const char *Message, ...)
{
	va_list vl;

	va_start(vl, Message);
	vsprintf(mLogBuffer, Message, vl);
	va_end(vl);

	if (mCallback != NULL)
		(*mCallback)(mLogBuffer);
	else
		cout << mLogBuffer << endl;
}

