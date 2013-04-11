#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "EarlabEnvironment.h"

EarlabEnvironment::EarlabEnvironment()
{
	memset(this, 0, sizeof(EarlabEnvironment));
	InputDirectoryPath = new char [10];
	strcpy(InputDirectoryPath, ".");
	OutputDirectoryPath = new char [10];
	strcpy(OutputDirectoryPath, ".");
}

EarlabEnvironment::~EarlabEnvironment()
{
	if (InputDirectoryPath != NULL)
		delete [] InputDirectoryPath;
	if (OutputDirectoryPath != NULL)
		delete [] OutputDirectoryPath;
}

void EarlabEnvironment::SetInputDirectoryPath(char *NewInputPath)
{
	CopyStringContents(&InputDirectoryPath, NewInputPath);
}

const char *EarlabEnvironment::GetInputDirectoryPath(void)
{
	return InputDirectoryPath;
}

void EarlabEnvironment::SetOutputDirectoryPath(char *NewOutputPath)
{
	CopyStringContents(&OutputDirectoryPath, NewOutputPath);
}

const char *EarlabEnvironment::GetOutputDirectoryPath(void)
{
	return OutputDirectoryPath;
}

void EarlabEnvironment::CopyStringContents(char **LocalString, const char *NewContents)
{
	if (*LocalString != NULL)
		delete [] *LocalString;

	*LocalString = new char [strlen(NewContents) + 1];
	strcpy(*LocalString, NewContents);
}
