#include <stdio.h>
#include <memory.h>
#include <stdarg.h>
#include <string.h>
#include "EarlabModule.h"
#include "EarlabException.h"

EarlabModule::EarlabModule()
{
	memset(this, 0, sizeof(EarlabModule));
}

EarlabModule::~EarlabModule()
{
	if (mModuleName != NULL)
		delete [] mModuleName;
}

void EarlabModule::SetModuleName(char *ModuleName)
{
	CopyStringContents(&mModuleName, ModuleName);
}

void EarlabModule::SetLogger(Logger *TheLogger)
{
	mLogger = TheLogger;
}

void EarlabModule::Log(const char *Message, ...)
{
	va_list vl;
	char LogBuf[1024];

	va_start(vl, Message);
	vsprintf(LogBuf, Message, vl);
	va_end(vl);

	if (mLogger != NULL)
		mLogger->Log(LogBuf);
}
