#ifndef CParameterSet_H_INCLUDED
#define CParameterSet_H_INCLUDED
#include "CSection.h"
#include "ParameterStatus.h"

class CParameterSet
{
public:
	CParameterSet();
	~CParameterSet();
	ParameterStatus GetParameter(const char *SectionName, const char *ParameterName, bool &RetVal, const bool DefVal);
	ParameterStatus GetParameter(const char *SectionName, const char *ParameterName, int &RetVal, const int DefVal);
	ParameterStatus GetParameter(const char *SectionName, const char *ParameterName, double &RetVal, const double DefVal);
	ParameterStatus GetParameter(const char *SectionName, const char *ParameterName, char *RetVal, int RetValLen, const char *DefVal);
	ParameterStatus GetParameter(const char *SectionName, const char *ParameterName, int *RetVal, int &RetValLen, const int *DefVal, const int DefValLen);
	ParameterStatus GetParameter(const char *SectionName, const char *ParameterName, double *RetVal, int &RetValLen, const double *DefVal, const int DefValLen);

	CParameter *GetParameter(const char *SectionName, const char *Name);

	bool ParameterExists(char *SectionName, char *ParameterName);

	void AddParameter(char *SectionName, char *ParameterName, CParameterValue &Value);

	CSection *GetFirstSection(void) const;
	CSection *FindSection(const char *SectionName);
	CSection *AddSection(char *SectionName);
	void ParseLine(char *CurLine);
private:
	ParameterTypeT ParseTypeField(char *StringVal);
	bool   ParseBooleanValue(char *StringVal);
	int    ParseIntegerValue(char *StringVal);
	double ParseDoubleValue(char *StringVal);
	char   *ParseStringValue(char *StringVal);
	int    *ParseIntVectorValue(char *StringVal, int *ArrayLength);
	double *ParseDoubleVectorValue(char *StringVal, int *ArrayLength);
	char *Trim(char *start);
	char *FirstField(char *start);
	char *NextField(char *start);
	char *LastField(char *start);

	char mCurSectionName[80];

	CSection *mFirstSection;
	CSection *mCurSection;
};
#endif
