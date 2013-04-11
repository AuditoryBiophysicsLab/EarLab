#ifndef __EARLAB_MODULE_H_INCLUDED
#define __EARLAB_MODULE_H_INCLUDED

#include <stdarg.h>
#include <stdio.h>
#include "Logger.h"
#include "EarlabEnvironment.h"
#include "XmlMetadata.h"
#include "ModuleServices.h"
#include "EarlabModuleInfo.h"
#include "EarlabParameterInfo.h"
#include "EarlabIOStreamInfo.h"
#include "XMLInfo.h"

class EarlabModule : XMLInfo
{
public:
	EarlabModule();
	~EarlabModule();

	void SetEnvironment(EarlabEnvironment *theEnvironment) {::SetEnvironmentPointer(theEnvironment);}
	
	void SetModuleName(char *ModuleName);
	inline char *GetModuleName(void) {return mModuleName;}
	void SetLogger(Logger *TheLogger);
	void WriteXMLDescription(FILE *file);

protected:
	void Log(const char *Message, ...);
	const char *ModuleName() {return mModuleName;}
	void RegisterModuleInfo(EarlabModuleInfo *ModuleInfo);
	void RegisterParameterInfo(EarlabParameterInfo *ParameterInfo);
	void RegisterInputInfo(EarlabIOStreamInfo *InputInfo);
	void RegisterOutputInfo(EarlabIOStreamInfo *OutputInfo);

private:
	int mParameterCount, mInputCount, mOutputCount;
	Logger *mLogger;
	char *mModuleName;
	EarlabParameterInfo **mParameterInfo;
	EarlabIOStreamInfo **mInputInfo, **mOutputInfo;
	XMLInfo *mXmlParameters, *mXmlInputs, *mXmlOutputs;
};

#endif
