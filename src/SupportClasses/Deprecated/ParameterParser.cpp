#include <fstream>
#include "ParameterParser.h"
#include "Parameters.h"

using namespace std;

#define INPUT_BUFFER_MAX 16384

static struct
{
	ParameterTypeT BaseType;
	char *Aliases[10];
} TypeAliases[] = 
{
	{Boolean,		{"bool", "boolean", NULL}},
	{Integer,		{"int", "integer", NULL}},
	{Double,		{"double", "float", "real", NULL}},
	{String,		{"string", "char", NULL}},
	{IntVector,		{"intvector", "intarray", "intvec", "intarr", NULL}},
	{DoubleVector,	{"doublevector", "doublearray", "floatvector", "floatarray", "doublevec", "doublearr", "floatvec", "floatarr", NULL}},
	{Comment,		{"//", "comment", ";", "#", NULL}},
	{Section,		{"[]", NULL}},
	{Uninitialized,	{NULL}},
};

ParameterParser::ParameterParser(CParameterTree *ParseTree)
{
	mCurTypeField = NULL;
	mCurNameField = NULL;
	mCurValueField = NULL;
	mCurSectionName = NULL;
	mLineNumber = 0;
	mCurSectionName = new char[50];
	strcpy(mCurSectionName, "Default");
	mParameterTree = ParseTree;
}

ParameterParser::~ParameterParser()
{
	if (mCurSectionName != NULL)
		delete [] mCurSectionName;
}


void ParameterParser::ParseLine(char *CurLine)
{
	ParameterTypeT CurType = Comment;
	CParameter *CurParameter;
	CParameterValue *CurValue;
	char *CurName, *CurValueString;

	mLineNumber++;
	mCurTypeField = strtok(mCurLine, " \t");
	CurType = ParseTypeField();

	switch (CurType)
	{
	case Section:
		if (mCurSectionName != NULL)
			delete [] mCurSectionName;
		mCurSectionName = new char [strlen(mCurTypeField)];
		strcpy(mCurSectionName, mCurTypeField);
		return;
	case Comment:
		return;
	case Boolean:
		CurName = strtok(NULL, " \t");
		CurValueString = strtok(NULL, " \t");
		CurValue = new CParameterValue(ParseBooleanValue(CurValueString));
		break;
	case Integer:
		CurName = strtok(NULL, " \t");
		CurValueString = strtok(NULL, " \t");
		CurValue = new CParameterValue(ParseIntegerValue(CurValueString));
		break;
	case Double:
		CurName = strtok(NULL, " \t");
		CurValueString = strtok(NULL, " \t");
		CurValue = new CParameterValue(ParseDoubleValue(CurValueString));
		break;
	case String:
		CurName = strtok(NULL, " \t");
		CurValueString = strtok(NULL, " \t");
		CurValue = new CParameterValue(CurValueString);
		break;
	case IntVector:
		CurName = strtok(NULL, " \t");
		CurValueString = strtok(NULL, "[");
		IntVector = ParseIntVectorValue(CurValueString, &VectorLength)
		CurValue = new CParameterValue(IntVector, VectorLength);
		break;
	case DoubleVector:
		CurName = strtok(NULL, " \t");
		CurValueString = strtok(NULL, "[");
		DoubleVector = ParseDoubleVectorValue(CurValueString, &VectorLength)
		CurValue = new CParameterValue(DoubleVector, VectorLength);
		break;
	case Uninitialized:
	default:
		sprintf(ErrStr, "Syntax error at line %d: \"%s\"", mLineNumber, Line);
		throw ErrStr;
	}
}

ParameterTypeT ParameterParser::ParseTypeField(void)
{
	int i, j;
	
	// Check for section name
	if ((mCurTypeField[0] == '[') && (mCurTypeField[strlen(mCurTypeField) - 1] == ']'))
	{
		// Replace the closing bracket with a NUL to terminate the section name string
		mCurTypeField[strlen(mCurTypeField) - 1] = '\0';
		mCurTypeField += 1;
		return Section;
		// Now, mCurTypeField points to the section name, which is NUL-terminated
	}
	for (i = 0; TypeAliases[i].BaseType != Uninitialized; i++)
	{
		for (j = 0; TypeAliases[i].Aliases[j] != NULL; j++)
		{
			if (stricmp(TypeAliases[i].Aliases[j], mCurTypeField) == 0)
				return TypeAliases[i].BaseType;
		}
	}
	return Uninitialized;
}
