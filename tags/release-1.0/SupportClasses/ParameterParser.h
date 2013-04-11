#ifndef ParamFileParser_H_INCLUDED
#define ParamFileParser_H_INCLUDED
#include <iostream>
#include <fstream>
#include "Parameters.h"
#include "CParameterSet.h"

using namespace std;

class ParameterParser
{
public:
	ParameterParser(CParameterTree *ParameterTree);
	~ParameterParser();
	void ParseLine(char *CurLine);
private:
	ParameterTypeT ParseTypeField(void);

	CParameterTree *mParameterTree;
	int mLineNumber;
	char *mCurLine;
	char *mCurTypeField;
	char *mCurNameField;
	char *mCurValueField;
	char *mCurSectionName;
};
#endif