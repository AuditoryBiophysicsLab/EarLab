#ifndef CParameter_H_INCLUDED
#define CParameter_H_INCLUDED
#include "CParameterValue.h"
#include "CParameter.h"
#include "CParameterTree.h"

class CParameter : public CParameterTree
{
public:
	CParameter(char *ParameterName, CParameterValue &Value);
	~CParameter();
	CParameterValue &Value(void);
	CParameter *GetNext(void) const;
protected:
	CParameterValue mValue;
};
#endif
