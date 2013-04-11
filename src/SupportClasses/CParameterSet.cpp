#include "Earlab.h"
#ifndef PARAMETER_SPACE
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "CParameterSet.h"
#include "EarlabException.h"

using namespace std;

static bool is_valid_integer_char(char which);
static bool is_valid_double_char(char which);

#ifdef _WIN32
#define strcasecmp stricmp
#endif

static struct
{
	ParameterTypeT BaseType;
	char *Aliases[10];
} TypeAliases[] =
{
	{Boolean,               {"bool", "boolean", NULL}},
	{Integer,               {"int", "integer", NULL}},
	{Double,                {"double", "float", "real", NULL}},
	{String,                {"string", "char", NULL}},
	{IntVector,             {"intvector", "intarray", "intvec", "intarr", NULL}},
	{DoubleVector,  {"doublevector", "doublearray", "floatvector", "floatarray", "doublevec", "doublearr", "floatvec", "floatarr", NULL}},
	{Comment,               {"//", "comment", ";", "#", NULL}},
	{Section,               {"[]", NULL}},
	{Uninitialized, {NULL}},
};

CParameterSet::CParameterSet()
{
	mFirstSection = NULL;
}

CParameterSet::~CParameterSet()
{
	if (mFirstSection != NULL)
		delete mFirstSection;
}

ParameterStatus CParameterSet::GetParameter(const char *SectionName, const char *ParameterName, bool &RetVal, const bool DefVal)
{
	CParameter *TheParameter;

	try
	{
		TheParameter = GetParameter(SectionName, ParameterName);
		if (TheParameter != NULL)
			return TheParameter->Value().GetValue(RetVal);
		else
		{
			RetVal = DefVal;
			return Defaulted;
		}
	}
	catch (EarlabException e)
	{
		throw EarlabException("GetParameter: Error retrieving value of parameter %s in section [%s]: %s", ParameterName, SectionName, e.GetExceptionString());
	}
}

ParameterStatus CParameterSet::GetParameter(const char *SectionName, const char *ParameterName, int &RetVal, const int DefVal)
{
	CParameter *TheParameter;

	try
	{
		TheParameter = GetParameter(SectionName, ParameterName);
		if (TheParameter != NULL)
			return TheParameter->Value().GetValue(RetVal);
		else
		{
			RetVal = DefVal;
			return Defaulted;
		}
	}
	catch (EarlabException e)
	{
		throw EarlabException("GetParameter: Error retrieving value of parameter %s in section [%s]: %s", ParameterName, SectionName, e.GetExceptionString());
	}
}

ParameterStatus CParameterSet::GetParameter(const char *SectionName, const char *ParameterName, double &RetVal, const double DefVal)
{
	CParameter *TheParameter;

	try
	{
		TheParameter = GetParameter(SectionName, ParameterName);
		if (TheParameter != NULL)
			return TheParameter->Value().GetValue(RetVal);
		else
		{
			RetVal = DefVal;
			return Defaulted;
		}
	}
	catch (EarlabException e)
	{
		throw EarlabException("GetParameter: Error retrieving value of parameter %s in section [%s]: %s", ParameterName, SectionName, e.GetExceptionString());
	}
}

ParameterStatus CParameterSet::GetParameter(const char *SectionName, const char *ParameterName, char *RetVal, int RetValLen, const char *DefVal)
{
	CParameter *TheParameter;

	try
	{
		TheParameter = GetParameter(SectionName, ParameterName);
		if (TheParameter != NULL)
			return TheParameter->Value().GetValue(RetVal, RetValLen);
		else
		{
			if (DefVal == NULL) {
				RetVal[0] = '\0';
				return Failure;
			}
			else
			{
				strncpy(RetVal, DefVal, RetValLen);
				return Defaulted;
			}
		}
	}
	catch (EarlabException e)
	{
		throw EarlabException("GetParameter: Error retrieving value of parameter %s in section [%s]: %s", ParameterName, SectionName, e.GetExceptionString());
	}
}

ParameterStatus CParameterSet::GetParameter(const char *SectionName, const char *ParameterName, int *RetVal, int &RetValLen, const int *DefVal, const int DefValLen)
{
	CParameter *TheParameter;

	try
	{
		TheParameter = GetParameter(SectionName, ParameterName);
		if (TheParameter != NULL)
			return TheParameter->Value().GetValue(RetVal, RetValLen);
		else
		{
			int i;

			for (i = 0; (i < RetValLen) && (i < DefValLen); i++)
				RetVal[i] = DefVal[i];
			RetValLen = i;
			if (DefValLen > RetValLen)
				return BufferOverflowDefaulted;
			else
				return Defaulted;
		}
	}
	catch (EarlabException e)
	{
		throw EarlabException("GetParameter: Error retrieving value of parameter %s in section [%s]: %s", ParameterName, SectionName, e.GetExceptionString());
	}
}

ParameterStatus CParameterSet::GetParameter(const char *SectionName, const char *ParameterName, double *RetVal, int &RetValLen, const double *DefVal, const int DefValLen)
{
	CParameter *TheParameter;
	int i;

	try
	{
		TheParameter = GetParameter(SectionName, ParameterName);
		if (TheParameter != NULL)
			return TheParameter->Value().GetValue(RetVal, RetValLen);
		else
		{
			for (i = 0; (i < RetValLen) && (i < DefValLen); i++)
				RetVal[i] = DefVal[i];
			return Defaulted;
		}
	}
	catch (EarlabException e)
	{
		throw EarlabException("GetParameter: Error retrieving value of parameter %s in section [%s]: %s", ParameterName, SectionName, e.GetExceptionString());
	}
}

CParameter *CParameterSet::GetParameter(const char *SectionName, const char *ParameterName)
{
	CSection *CurSection;
	CParameter *CurParameter;

	CurSection = mFirstSection->Find(SectionName);
	if (CurSection != NULL)
		CurParameter = CurSection->FindParameter(ParameterName);
	else
		CurParameter = NULL;
	return CurParameter;
}

void CParameterSet::AddParameter(char *SectionName, char *ParameterName, CParameterValue &Value)
{
	CSection *CurSection;

	if (mFirstSection == NULL)
		mFirstSection = new CSection(SectionName);

	CurSection = mFirstSection->Find(SectionName);

	if (CurSection == NULL)
	{
		CurSection = new CSection(SectionName);
		mFirstSection->AddAtEnd(CurSection);
	}

	CurSection->AddParameter(ParameterName, Value);
}

CSection *CParameterSet::GetFirstSection(void) const
{
	return mFirstSection;
}

CSection *CParameterSet::GetNextSection(CSection *CurSection) const
{
	return CurSection->GetNext();
}

CSection *CParameterSet::FindSection(const char *SectionName)
{
	if (mFirstSection == NULL)
		return NULL;
	else
		return mFirstSection->Find(SectionName);
}

CSection *CParameterSet::AddSection(char *SectionName)
{
	if (mFirstSection == NULL)
	{
		mFirstSection = new CSection(SectionName);
		return mFirstSection;
	}
	else
	{
		CSection *newSection = new CSection(SectionName);
		mFirstSection->AddAtEnd(newSection);
		return newSection;
	}
}

bool CParameterSet::ParameterExists(char *SectionName, char *ParameterName)
{
	if (GetParameter(SectionName, ParameterName) != NULL)
		return true;
	else
		return false;
}

void CParameterSet::ParseLine(char *CurLine)
{
	ParameterTypeT CurType = Comment;
	CParameterValue *CurValue;
	char *CurTypeString, *CurName, *CurValueString;
	int VectorLength;
	int *IntVectorValue;
	double *DoubleVectorValue;
	char *CurField;

	CurField = Trim(CurLine);
	if (CurField == NULL)
		return; // Empty line, just ignore it

	CurType = ParseTypeField(CurField);
	if (CurType == Section)
	{
		memset(mCurSectionName, 0, sizeof(mCurSectionName));
		for (unsigned int i = 1; (i < strlen(CurLine)) && (i < sizeof(mCurSectionName)); i++)
			if (CurLine[i] != ']')
				mCurSectionName[i - 1] = CurLine[i];
			else
				break;
		//strncpy(mCurSectionName, &CurLine[1], strlen(CurLine - 2));
		mCurSection = new CSection(mCurSectionName);
		if (mFirstSection == NULL)
			mFirstSection = mCurSection;
		else
			mFirstSection->AddAtEnd(mCurSection);
		return;
	}
	else
	{
		CurTypeString = FirstField(CurField);
		CurType = ParseTypeField(CurTypeString);
	}

	switch (CurType)
	{
	case Comment:
		return;
	case Boolean:
		CurName = NextField(CurTypeString);
		CurValueString = NextField(CurName);
		CurValue = new CParameterValue(ParseBooleanValue(CurValueString));
		break;
	case Integer:
		CurName = NextField(CurTypeString);
		CurValueString = NextField(CurName);
		CurValue = new CParameterValue(ParseIntegerValue(CurValueString));
		break;
	case Double:
		CurName = NextField(CurTypeString);
		CurValueString = NextField(CurName);
		CurValue = new CParameterValue(ParseDoubleValue(CurValueString));
		break;
	case String:
		CurName = NextField(CurTypeString);
		CurValueString = LastField(CurName);
		CurValue = new CParameterValue(ParseStringValue(CurValueString));
		break;
	case IntVector:
		CurName = NextField(CurTypeString);
		CurValueString = LastField(CurName);
		IntVectorValue = ParseIntVectorValue(CurValueString, &VectorLength);
		CurValue = new CParameterValue(IntVectorValue, VectorLength);
		break;
	case DoubleVector:
		CurName = NextField(CurTypeString);
		CurValueString = LastField(CurName);
		DoubleVectorValue = ParseDoubleVectorValue(CurValueString, &VectorLength);
		CurValue = new CParameterValue(DoubleVectorValue, VectorLength);
		break;
	case Uninitialized:
	default:
		throw EarlabException("Syntax error while processing file: Unknown data type specifier: %s", CurTypeString);
	}

	mCurSection->AddParameter(CurName, *CurValue);
}

ParameterTypeT CParameterSet::ParseTypeField(char *StringVal)
{
	if ((StringVal[0] == '[') && (StringVal[strlen(StringVal) - 1] == ']'))
		return Section;

	for (int i = 0; TypeAliases[i].BaseType != Uninitialized; i++)
	{
		for (int j = 0; TypeAliases[i].Aliases[j] != NULL; j++)
		{
			if (strcasecmp(StringVal, TypeAliases[i].Aliases[j]) == 0)
				return TypeAliases[i].BaseType;
		}
	}
	return Uninitialized;
}

bool CParameterSet::ParseBooleanValue(char *StringVal)
{
	if ((strcasecmp(StringVal, "true") == 0) || (strcasecmp(StringVal, "yes") == 0))
		return true;
	if ((strcasecmp(StringVal, "false") == 0) || (strcasecmp(StringVal, "no") == 0))
		return false;

	throw EarlabException("Syntax error parsing boolean value");
}

int CParameterSet::ParseIntegerValue(char *StringVal)
{
	return strtol(StringVal, NULL, 10);
}

double CParameterSet::ParseDoubleValue(char *StringVal)
{
	return strtod(StringVal, NULL);
}

char *CParameterSet::ParseStringValue(char *StringVal)
{
	unsigned int MaxIndex = (unsigned int)strlen(StringVal);
	unsigned int i;

	for (i = 1; i < MaxIndex; i++)
		if (StringVal[i] == '"')
			break;
	StringVal[i] = '\0';
	return &StringVal[1];
}

#define ARRAY_GROW_SIZE 10

int *CParameterSet::ParseIntVectorValue(char *StringVal, int *ArrayLength)
{
	char *cur;
	int *array = NULL, *old_array;
	int num_left = 0;
	int count;
	int cur_int;

	cur = StringVal;
	count = 0;
	while (true)
	{
		while ((!is_valid_integer_char(*cur)) && (*cur != '\0'))
			cur++;

		if (*cur == '\0')
		{
			*ArrayLength = count;
			return array;
		}

		cur_int = strtol(cur, &cur, 10);
		if (num_left == 0)
		{
			if (array == NULL)
				array = new int [ARRAY_GROW_SIZE];
			else
			{
				old_array = array;
				array = new int [count + ARRAY_GROW_SIZE];
				for (int i = 0; i < count; i++)
					array[i] = old_array[i];
				delete [] old_array;
			}
			num_left = ARRAY_GROW_SIZE;
		}

		array[count] = cur_int;

		count++;
		num_left--;
	}
}

double *CParameterSet::ParseDoubleVectorValue(char *StringVal, int *ArrayLength)
{
	char *cur;
	double *array = NULL, *old_array;
	int num_left = 0;
	int count;
	double cur_double;

	cur = StringVal;
	count = 0;
	while (true)
	{
		while ((!is_valid_double_char(*cur)) && (*cur != '\0'))
			cur++;

		if (*cur == '\0')
		{
			//printf("ParseDoubleVectorValue: Vector value is: [");
			//for (int i = 0; i < count; i++)
			//	printf(" %f", array[i]);
			//printf(" ]\n");
			*ArrayLength = count;
			return array;
		}

		cur_double = strtod(cur, &cur);
		if (num_left == 0)
		{
			if (array == NULL)
				array = new double [ARRAY_GROW_SIZE];
			else
			{
				old_array = array;
				array = new double [count + ARRAY_GROW_SIZE];
				for (int i = 0; i < count; i++)
					array[i] = old_array[i];
				delete [] old_array;
			}
			num_left = ARRAY_GROW_SIZE;
		}

		array[count] = cur_double;

		count++;
		num_left--;
	}
}

char *CParameterSet::FirstField(char *start)
{
	char *tmp1, *tmp2;


	// Skip leading whitespace
	for (tmp1 = start; (isspace(*tmp1)) && (*tmp1 != '\0'); tmp1++)
		;

	if (*tmp1 == '\0')
		return NULL;

	for (tmp2 = tmp1; (!isspace(*tmp2)) && (*tmp2 != '\0'); tmp2++)
		;
	*tmp2 = '\0';
	return tmp1;
}

char *CParameterSet::NextField(char *start)
{
	char *tmp;

	// Skip to the next NUL character
	for (tmp = start; *tmp != '\0'; tmp++)
		;
	tmp++;
	return FirstField(tmp);
}

char *CParameterSet::LastField(char *start)
{
	char *tmp;

	// Skip to the next NUL character
	for (tmp = start; *tmp != '\0'; tmp++)
		;
	tmp++;
	return Trim(tmp);
}

char *CParameterSet::Trim(char *start)
{
	char *tmp1, *tmp2;

	// Skip leading whitespace
	for (tmp1 = start; (isspace(*tmp1)) && (*tmp1 != '\0'); tmp1++)
		;

	if (*tmp1 == '\0')
		return NULL;

	// Skip trailing whitespace
	for (tmp2 = tmp1 + strlen(tmp1) - 1; (isspace(*tmp2)) && (tmp1 != tmp2); tmp2--)
		;
	tmp2++;
	*tmp2 = '\0';

	return tmp1;
}

static bool is_valid_integer_char(char which)
{
	if (((which >= '0') && (which <= '9')) || (which == '+') || (which == '-'))
		return true;
	else
		return false;
}

static bool is_valid_double_char(char which)
{
	if ((is_valid_integer_char(which)) || (which == '.') || (which == 'e') || (which == 'E'))
		return true;
	else
		return false;
}

#endif
