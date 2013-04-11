#include "CParameter.h"

CParameter::CParameter(char *ParameterName, CParameterValue &Value) : CParameterTree(ParameterName), mValue(Value)
{
}

CParameter *CParameter::GetNext(void) const
{
	return (CParameter *)CParameterTree::GetNext();
}

CParameter::~CParameter()
{
}

CParameterValue &CParameter::Value(void)
{
	return mValue;
}

