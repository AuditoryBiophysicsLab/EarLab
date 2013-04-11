#ifndef __EARLAB_MODULE_H_INCLUDED
#define __EARLAB_MODULE_H_INCLUDED

#include <stdarg.h>
#include <stdio.h>
#include "Logger.h"
#include "EarlabEnvironment.h"
#include "XmlMetadata.h"
#include "ModuleServices.h"

class EarlabModule
{
public:
	EarlabModule();
	~EarlabModule();

	void SetEnvironment(EarlabEnvironment *theEnvironment) {::SetEnvironmentPointer(theEnvironment);}
	
	void SetModuleName(char *ModuleName);
	void SetLogger(Logger *TheLogger);

protected:
	void Log(const char *Message, ...);
	const char *ModuleName() {return mModuleName;}

private:
	Logger *mLogger;
	char *mModuleName;
};

#endif
