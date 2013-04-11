#ifndef CParameterValue_H_INCLUDED
#define CParameterValue_H_INCLUDED

#include "ParameterStatus.h"
#include "ParameterType.h"

class CParameterValue
{
public:
	// One constructor for each type of parameter
	CParameterValue(const bool BooleanVal);
	CParameterValue(const int IntegerVal);
	CParameterValue(const double DoubleVal);
	CParameterValue(const char *StringVal);
	CParameterValue(int *IntVectorVal, const int ArrayLength);
	CParameterValue(double *DoubleVectorVal, const int ArrayLength);

	CParameterValue(CParameterValue &Value);        // Copy constructor

	~CParameterValue();

 //     const ParameterType &ParameterType(void) const;
	const ParameterTypeT &ParameterType(void) const;

	ParameterStatus GetValue(bool &RetVal);
	ParameterStatus GetValue(int &RetVal);
	ParameterStatus GetValue(double &RetVal);
	ParameterStatus GetValue(char *RetVal, int RetValSize);
	ParameterStatus GetValue(int *RetVal, int &ArraySize);
	ParameterStatus GetValue(double *RetVal, int &ArraySize);

	int GetArraySize(void) const;
	int GetStringLength(void) const;

	const char *TypeToString(void) const;
	const char *ValueToString(void);
private:
	void Init(void);
	union
	{
		bool BooleanVal;
		int IntegerVal;
		double DoubleVal;
		char *StringVal;
		int *IntVectorVal;
		double *DoubleVectorVal;
	} mValue;
	int mArrayLength;
	char *mValueString;
	::ParameterTypeT mType;
};
#endif
