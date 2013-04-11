#ifndef CSection_H_INCLUDED
#define CSection_H_INCLUDED
#include "CParameterTree.h"
#include "CParameter.h"

class CSection : public CParameterTree
{
public:
	CSection(char *SectionName);
	CParameter *GetFirstParameter(void) const;
	CParameter *FindParameter(const char *ParameterName);
	CSection *GetNext(void) const;
	CSection *Find(const char *SectionName);
	void AddParameter(char *ParameterName, CParameterValue &Value);
};
#endif
