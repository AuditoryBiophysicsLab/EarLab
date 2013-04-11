#ifndef __MODULE_SERVICES_H_INCLUDED
#define __MODULE_SERVICES_H_INCLUDED

#include "EarlabEnvironment.h"
#include <stdio.h>

void SetEnvironmentPointer(EarlabEnvironment *Environment);
FILE *OpenOutputFile(const char *name, const char *mode);
FILE *OpenInputFile(const char *name, const char *mode);
void CloseFile(FILE *fp);
void CopyStringContents(char **LocalString, const char *NewContents);
void FormFileName(const char *DirectoryName, const char *FileName, char *OutputBuffer, int OutputBufferLength);

#endif
