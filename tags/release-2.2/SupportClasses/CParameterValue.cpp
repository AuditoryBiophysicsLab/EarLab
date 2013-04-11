#include "iostream"
#include <string.h>
#include <memory.h>
#include "stdio.h"
#include "CParameterValue.h"
#include "EarlabException.h"

using namespace std;

CParameterValue::CParameterValue(const bool BooleanVal)
{
	Init();
	mType = Boolean;
	mValue.BooleanVal = BooleanVal;
}

CParameterValue::CParameterValue(const int IntegerVal)
{
	Init();
	mType = Integer;
	mValue.IntegerVal = IntegerVal;
}

CParameterValue::CParameterValue(const double DoubleVal)
{
	Init();
	mType = Double;
	mValue.DoubleVal = DoubleVal;
}

CParameterValue::CParameterValue(const char *StringVal)
{
	Init();
	mType = String;
	mValue.StringVal = new char [strlen(StringVal) + 1];
	strcpy(mValue.StringVal, StringVal);
}

CParameterValue::CParameterValue(int *IntVectorVal, const int ArrayLength)
{
	Init();
	mType = IntVector;
	mArrayLength = ArrayLength;
	mValue.IntVectorVal = IntVectorVal;
}

CParameterValue::CParameterValue(double *DoubleVectorVal, const int ArrayLength)
{
	Init();
	mType = DoubleVector;
	mArrayLength = ArrayLength;
	mValue.DoubleVectorVal = DoubleVectorVal;
}

ParameterStatus CParameterValue::GetValue(bool &RetVal)
{
	if (mType != Boolean)
		throw EarlabException("Requested parameter's data type is not boolean.");
	else
	{
		RetVal = mValue.BooleanVal;
		return Success;
	}
}

ParameterStatus CParameterValue::GetValue(int &RetVal)
{
	if (mType != Integer)
		throw EarlabException("Requested parameter's data type is not integer.");
	else
	{
		RetVal = mValue.IntegerVal;
		return Success;
	}
}

ParameterStatus CParameterValue::GetValue(double &RetVal)
{
	if (mType != Double)
		throw EarlabException("Requested parameter's data type is not double.");
	else
	{
		RetVal = mValue.DoubleVal;
		return Success;
	}
}

ParameterStatus CParameterValue::GetValue(char *RetVal, int RetValLen)
{
	if (mType != String)
		throw EarlabException("Requested parameter's data type is not string.");
	else
	{
		strncpy(RetVal, mValue.StringVal, RetValLen);
		return Success;
	}
}

ParameterStatus CParameterValue::GetValue(int *RetVal, int &ArrayLen)
{
	if (mType != IntVector)
		throw EarlabException("Requested parameter's data type is not IntVector.");
	else
	{
		int i;

		for (i = 0; (i < ArrayLen) && (i < mArrayLength); i++)
			RetVal[i] = mValue.IntVectorVal[i];
		ArrayLen = i;
		if (mArrayLength > ArrayLen)
			return BufferOverflowSuccess;
		else
			return Success;
	}
}

ParameterStatus CParameterValue::GetValue(double *RetVal, int &ArrayLen)
{
	if (mType != DoubleVector)
		throw EarlabException("Requested parameter's data type is not DoubleVector.");
	else
	{
		int i;

		for (i = 0; (i < ArrayLen) && (i < mArrayLength); i++)
			RetVal[i] = mValue.IntVectorVal[i];
		ArrayLen = i;
		if (mArrayLength > ArrayLen)
			return BufferOverflowSuccess;
		else
			return Success;
	}
}

int CParameterValue::GetArraySize(void) const
{
	switch (mType)
	{
	case String:
		return (int)strlen(mValue.StringVal);
	case IntVector:
	case DoubleVector:
		return mArrayLength;
	case Uninitialized:
		return -1;
	default:
		return 1;
	}
}

int CParameterValue::GetStringLength(void) const
{
	switch (mType)
	{
	case String:
		return (int)strlen(mValue.StringVal);
	default:
		return -1;
	}
}

const char *CParameterValue::TypeToString(void) const
{
	switch (mType)
	{
	case Boolean:
		return "Bool";
	case Integer:
		return "Int";
	case Double:
		return "Double";
	case String:
		return "String";
	case IntVector:
		return "IntVector";
	case DoubleVector:
		return "DoubleVector";
	case Uninitialized:
		return "Type not set";
	default:
		return "Illegal type";
	}
}

const char *CParameterValue::ValueToString(void)
{
	char TempValueString[30];
	int i;

 	if (mValueString != NULL)
		delete [] mValueString;
	mValueString = NULL;
	switch (mType)
	{
	case Boolean:
		if (mValue.BooleanVal)
			return "true";
		else
			return "false";
	case Integer:
		mValueString = new char [15];
		sprintf(mValueString, "%d", mValue.IntegerVal);
		return mValueString;
	case Double:
		mValueString = new char [25];
		sprintf(mValueString, "%f", mValue.DoubleVal);
		return mValueString;
	case String:
		return mValue.StringVal;
	case IntVector:
		mValueString = new char [16 * mArrayLength];
		memset(mValueString, 0, 16 * mArrayLength);
		strcpy(mValueString, "[ ");
		for (i = 0; i < mArrayLength; i++)
		{
			sprintf(TempValueString, "%d ", mValue.IntVectorVal[i]);
			strcat(mValueString, TempValueString);
		}
		strcat(mValueString, "]");
		return mValueString;
	case DoubleVector:
		mValueString = new char [26 * mArrayLength];
		memset(mValueString, 0, 26 * mArrayLength);
		strcpy(mValueString, "[ ");
		for (i = 0; i < mArrayLength; i++)
		{
			sprintf(TempValueString, "%f ", mValue.DoubleVectorVal[i]);
			strcat(mValueString, TempValueString);
		}
		strcat(mValueString, "]");
		return mValueString;
	case Uninitialized:
		return "Type not set";
	default:
		return "Illegal type";
	}
}

CParameterValue::CParameterValue(CParameterValue &Value)
{
	Init();
	mType = Value.mType;
	mArrayLength = Value.mArrayLength;
	switch (mType)
	{
	case String:
		mValue.StringVal = new char [strlen(Value.mValue.StringVal) + 1];
		strcpy(mValue.StringVal, Value.mValue.StringVal);
		break;
	case IntVector:
		mValue.IntVectorVal = new int [Value.mArrayLength];
		memcpy(mValue.IntVectorVal, Value.mValue.IntVectorVal, mArrayLength * sizeof(int));;
		break;
	case DoubleVector:
		mValue.DoubleVectorVal = new double [Value.mArrayLength];
		memcpy(mValue.DoubleVectorVal, Value.mValue.DoubleVectorVal, mArrayLength * sizeof(double));;
		break;
	default:
		mValue = Value.mValue;
		break;
	}
}

CParameterValue::~CParameterValue()
{
	if (mValueString != NULL)
		delete [] mValueString;
	mValueString = NULL;

	switch (mType)
	{
	case String:
		if (mValue.StringVal != NULL)
			delete [] mValue.StringVal;
		break;
	case IntVector:
		if (mValue.IntVectorVal != NULL)
			delete [] mValue.IntVectorVal;
		break;
	case DoubleVector:
		if (mValue.DoubleVectorVal != NULL)
			delete [] mValue.DoubleVectorVal;
		break;
	default:
		break;
	}
	memset(&mValue, 0, sizeof(mValue));
}

void CParameterValue::Init(void)
{
	mValue.DoubleVal = 0.0;
	mArrayLength = 0;
	mValueString = NULL;
	mType = Uninitialized;
}

const ParameterTypeT &CParameterValue::ParameterType(void) const
{
	return mType;
}
