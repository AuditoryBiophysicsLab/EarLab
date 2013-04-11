#include "Earlab.h"
#include <memory.h>
#include <string.h>
#include "EarlabException.h"
#include "ModuleServices.h"

#ifdef NOTWINDOWS
# define DIRSTR  "/"
# define DIRCHR  '/'
#else
# define DIRSTR  "\\"
# define DIRCHR  '\\'
#endif


static EarlabEnvironment *theEnvironment;

void SetEnvironmentPointer(EarlabEnvironment *Environment)
{
	theEnvironment = Environment;
}

FILE *OpenInputFile(const char *filename, const char *mode)
{
	char FileName[256];
	FILE *fp;
	int RetryCount = 0;

	FormFileName(theEnvironment->GetInputDirectoryPath(), filename, FileName, sizeof(FileName));
	//printf("OpenInputFile: File name: %s\n", FileName);
	//TODO: Check for NULL when the file is opened, and if it is NULL, sleep and retry
	//      a few times.  This is for multithreaded code to cooperate better with files
	//      and will be obviated when we get the shared environment stuff fully working
	fp = NULL;
	while (fp == NULL)
	{
		fp = fopen(FileName, mode);
		if (fp == NULL)
		{
			if (RetryCount > 10)
				throw EarlabException("OpenInputFile: Error opening input file %s, retry count exhausted", FileName);
#ifndef _WIN32
			sleep(1);
#endif
			RetryCount++;
		}
	}
	return fp;
}

FILE *OpenOutputFile(const char *filename, const char *mode)
{
	char FileName[256];
	FILE *fp;
	int RetryCount = 0;
	
	FormFileName(theEnvironment->GetOutputDirectoryPath(), filename, FileName, sizeof(FileName));
	//printf("OpenOutputFile: File name: %s\n", FileName);
	fp = NULL;
	while (fp == NULL)
	{
		fp = fopen(FileName, mode);
		if (fp == NULL)
		{
			if (RetryCount > 10)
				throw EarlabException("OpenInputFile: Error opening input file %s, retry count exhausted", FileName);
#ifndef _WIN32
			sleep(1);
#endif
			RetryCount++;
		}
	}
	return fp;
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
		throw new EarlabException("FormFileName: Error forming file name - directory path (%s) too int for output buffer", DirectoryName);
#ifdef _WIN32
	if (OutputBuffer[strlen(OutputBuffer) - 1] != DIRCHR)
	{
		strcat(OutputBuffer, DIRSTR);
		OutputBufferLength--;
	}
#else
	if (OutputBuffer[strlen(OutputBuffer) - 1] != '/')
	{
		strcat(OutputBuffer, "/");
		OutputBufferLength--;
	}
#endif
	if (OutputBufferLength <= 0)
		throw new EarlabException("FormFileName: Error forming file name - directory path (%s%c) too long for output buffer",
					   DirectoryName, DIRCHR);
	strncat(OutputBuffer, FileName, OutputBufferLength);
	OutputBufferLength -= (int)(strlen(FileName) + 1);
	if (OutputBufferLength <= 0)
		throw new EarlabException("FormFileName: Error forming file name - file name (%s) too int for output buffer", FileName);
}

void CopyStringContents(char **LocalString, const char *NewContents)
{
	if (*LocalString != NULL)
		delete [] *LocalString;

	*LocalString = new char [strlen(NewContents) + 1];
	strcpy(*LocalString, NewContents);
}
