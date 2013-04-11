#include <stdio.h>
#include <memory.h>
#include <stdarg.h>
#include <string.h>
#include "EarlabModule.h"
#include "EarlabException.h"

#ifndef _WIN32
void *__dso_handle = 0;
#endif

EarlabModule::EarlabModule() : XMLInfo("EarlabModule")
{
	mModuleName = NULL;
	mParameterCount = 0;
	mInputCount = 0;
	mOutputCount = 0;
	mLogger = NULL;
	mXmlParameters = mXmlInputs = mXmlOutputs = NULL;
	CleanupXmlElement = true;

	mParameterInfo = new EarlabParameterInfo *[EarlabMaxParametersPerModule];
	memset(mParameterInfo, 0, sizeof(EarlabParameterInfo *) * EarlabMaxParametersPerModule);
	mInputInfo = new EarlabIOStreamInfo *[EarlabMaxIOStreamCount];
	memset(mInputInfo, 0, sizeof(EarlabIOStreamInfo *) * EarlabMaxIOStreamCount);
	mOutputInfo = new EarlabIOStreamInfo *[EarlabMaxIOStreamCount];
	memset(mOutputInfo, 0, sizeof(EarlabIOStreamInfo *) * EarlabMaxIOStreamCount);
}

EarlabModule::~EarlabModule()
{
	int i;
	if (mModuleName != NULL)
		delete [] mModuleName;

	if (mParameterInfo != NULL)
	{
		for (i = 0; i < EarlabMaxParametersPerModule; i++)
			if (mParameterInfo[i] != NULL)
				delete mParameterInfo[i];
		delete [] mParameterInfo;
	}

	if (mInputInfo != NULL)
	{
		for (i = 0; i < EarlabMaxIOStreamCount; i++)
			if (mInputInfo[i] != NULL)
				delete mInputInfo[i];
		delete [] mInputInfo;
	}

	if (mOutputInfo != NULL)
	{
		for (i = 0; i < EarlabMaxIOStreamCount; i++)
			if (mOutputInfo[i] != NULL)
				delete mOutputInfo[i];
		delete [] mOutputInfo;
	}

	if (mXmlParameters != NULL)
		delete mXmlParameters;
	if (mXmlInputs != NULL)
		delete mXmlInputs;
	if (mXmlOutputs != NULL)
		delete mXmlOutputs;
}

void EarlabModule::RegisterModuleInfo(EarlabModuleInfo *ModuleInfo)
{
	AddElement(ModuleInfo->XmlElement());
}

void EarlabModule::RegisterParameterInfo(EarlabParameterInfo *ParameterInfo)
{
	if (mXmlParameters == NULL)
	{
		mXmlParameters = new XMLInfo("Parameters");
		AddElement(mXmlParameters->XmlElement());
	}
	mXmlParameters->AddElement(ParameterInfo->XmlElement());
	mParameterInfo[mParameterCount++] = ParameterInfo;
}

void EarlabModule::RegisterInputInfo(EarlabIOStreamInfo *InputInfo)
{
	if (mXmlInputs == NULL)
	{
		mXmlInputs = new XMLInfo("Inputs");
		AddElement(mXmlInputs->XmlElement());
	}
	mXmlInputs->AddElement(InputInfo->XmlElement());
	mInputInfo[mInputCount++] = InputInfo;
}

void EarlabModule::RegisterOutputInfo(EarlabIOStreamInfo *OutputInfo)
{
	if (mXmlOutputs == NULL)
	{
		mXmlOutputs = new XMLInfo("Outputs");
		AddElement(mXmlOutputs->XmlElement());
	}
	mXmlOutputs->AddElement(OutputInfo->XmlElement());
	mOutputInfo[mOutputCount++] = OutputInfo;
}

void EarlabModule::SetModuleName(char *ModuleName)
{
	CopyStringContents(&mModuleName, ModuleName);
}

void EarlabModule::SetLogger(Logger *TheLogger)
{
	mLogger = TheLogger;
}

void EarlabModule::WriteXMLDescription(FILE *file)
{
	XmlElement()->Print(file, 0);
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
