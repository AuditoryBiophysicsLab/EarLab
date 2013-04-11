#include <memory.h>
#include <string.h>
#include "EarlabException.h"
#include "ModuleServices.h"

static EarlabEnvironment *theEnvironment;

void SetEnvironmentPointer(EarlabEnvironment *Environment)
{
	theEnvironment = Environment;
}

FILE *OpenInputFile(const char *filename, const char *mode)
{
	char FileName[256];
	FormFileName(theEnvironment->GetInputDirectoryPath(), filename, FileName, sizeof(FileName));
	return(fopen(FileName, mode));
}

FILE *OpenOutputFile(const char *filename, const char *mode)
{
	char FileName[256];
	FormFileName(theEnvironment->GetOutputDirectoryPath(), filename, FileName, sizeof(FileName));
	return(fopen(FileName, mode));
}

void CloseFile(FILE *fp)
{
	fclose(fp);
}

void FormFileName(const char *DirectoryName, const char *FileName, char *OutputBuffer, int OutputBufferLength)
{
	memset(OutputBuffer, 0, OutputBufferLength);
	strncpy(OutputBuffer, DirectoryName, OutputBufferLength);
	OutputBufferLength -= (int)(strlen(DirectoryName) + 1);
	if (OutputBufferLength <= 0)
		throw new EarlabException("FormFileName: Error forming file name - directory path (%s) too long for output buffer", DirectoryName);
	if (OutputBuffer[strlen(OutputBuffer) - 1] != '\\')
	{
		strcat(OutputBuffer, "\\");
		OutputBufferLength--;
	}
	if (OutputBufferLength <= 0)
		throw new EarlabException("FormFileName: Error forming file name - directory path (%s\\) too long for output buffer", DirectoryName);
	strncat(OutputBuffer, FileName, OutputBufferLength);
	OutputBufferLength -= (int)(strlen(FileName) + 1);
	if (OutputBufferLength <= 0)
		throw new EarlabException("FormFileName: Error forming file name - file name (%s) too long for output buffer", FileName);
}

void CopyStringContents(char **LocalString, const char *NewContents)
{
	if (*LocalString != NULL)
		delete [] *LocalString;

	*LocalString = new char [strlen(NewContents) + 1];
	strcpy(*LocalString, NewContents);
}
