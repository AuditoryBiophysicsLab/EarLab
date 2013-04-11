#include "Earlab.h"
#include "Logger.h"
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <memory.h>

using namespace std;

Logger::Logger()
{
	memset(this, 0, sizeof(Logger));
#ifdef USE_PTHREADS
	pthread_mutex_init(&mLoggingMutex, NULL);
#endif
}

void Logger::SetLogCallback(LogCallback theCallback)
{
#ifdef USE_PTHREADS
	pthread_mutex_lock(&mLoggingMutex);
#endif
	mCallback = theCallback;
#ifdef USE_PTHREADS
	pthread_mutex_unlock(&mLoggingMutex);
#endif
}

void Logger::Log(const char *Message, ...)
{
	va_list vl;

#ifdef USE_PTHREADS
	pthread_mutex_lock(&mLoggingMutex);
#endif
	va_start(vl, Message);
	vsprintf(mLogBuffer, Message, vl);
	va_end(vl);

	if (mCallback != NULL)
		(*mCallback)(mLogBuffer);
	else
	{
		cout << mLogBuffer << endl;
		cout.flush();
	}
#ifdef USE_PTHREADS
	pthread_mutex_unlock(&mLoggingMutex);
#endif
}

