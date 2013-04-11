#ifndef _MODULESERVICES_H_INCLUDED
#define _MODULESERVICES_H_INCLUDED

#include <stdio.h>
#include "EarlabEnvironment.h"

void SetEnvironmentPointer(EarlabEnvironment *Environment);
FILE *OpenOutputFile(const char *name, const char *mode);
FILE *OpenInputFile(const char *name, const char *mode);
void CloseFile(FILE *fp);
void CopyStringContents(char **LocalString, const char *NewContents);
void FormFileName(const char *DirectoryName, const char *FileName, char *OutputBuffer, int OutputBufferLength);

#endif
