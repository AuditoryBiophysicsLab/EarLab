#include <iostream>
#include <fstream>
#include "CParameterFile.h"
#include "EarlabException.h"

using namespace std;

CParameterFile::CParameterFile()
{
	mFileName = NULL; 
}

CParameterFile::CParameterFile(char *FileName)
{
	mFileName = NULL; 
	ReadParameterFile(FileName);
}

CParameterFile::~CParameterFile()
{
	if (mFileName != NULL)
		delete [] mFileName;
}

void CParameterFile::ReadParameterFile(char *FileName)
{
	fstream InFile(FileName);
	char linebuf[2048];
	int lineNumber = 0;

	if (mFileName != NULL)
		delete [] mFileName;

	mFileName = new char[strlen(FileName) + 1];
	strcpy(mFileName, FileName);

	if (!InFile.is_open())
		throw EarlabException("Error opening specified parameter file \"%s\"", FileName);
	try
	{
		lineNumber++;
		InFile.getline(linebuf, sizeof(linebuf), '\n');

		while (!InFile.eof())
		{
			ParseLine(linebuf);
			lineNumber++;
			InFile.getline(linebuf, sizeof(linebuf), '\n');
		}
	}
	catch (EarlabException e)
	{
		InFile.close();
		throw EarlabException("Error while processing file %s at line %d: %s", FileName, lineNumber, e.GetExceptionString());
	}
	catch (char *e)
	{
		InFile.close();
		throw EarlabException("Error while processing file %s at line %d: %s", FileName, lineNumber, e);
	}

}
