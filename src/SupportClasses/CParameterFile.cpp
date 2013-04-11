#include "Earlab.h"
/*
 * Older version kept for back compatibility
 */
#ifndef PARAMETER_SPACE

#include <iostream>
#include <fstream>
#include "CParameterFile.h"
#include "EarlabException.h"

using namespace std;

CParameterFile::CParameterFile()
{
}

CParameterFile::CParameterFile(const char *FileName)
{
	ReadParameterFile(FileName);
}

CParameterFile::~CParameterFile()
{
}

void CParameterFile::ReadParameterFile(const char *FileName)
{
	fstream InFile(FileName);
	char linebuf[2048];
	int lineNumber = 0;

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

#else
#include "CParameterFile.h"
using namespace std;

CParameterFile::CParameterFile() {

}

CParameterFile::CParameterFile(const char *FileName) {

}

CParameterFile::~CParameterFile() {

}

#endif

