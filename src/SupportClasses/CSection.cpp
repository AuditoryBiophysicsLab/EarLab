#include <iostream>
#include "CSection.h"
#include "CParameter.h"

using namespace std;

CSection::CSection(char *SectionName) : CParameterTree(SectionName)
{
}

CParameter *CSection::FindParameter(const char *ParameterName)
{
	return (CParameter *)GetChild()->Find(ParameterName);
}

CParameter *CSection::GetFirstParameter(void) const
{
	return (CParameter *)GetChild();
}

CSection *CSection::GetNext(void) const
{
	return (CSection *)CParameterTree::GetNext();
}

CSection *CSection::Find(const char *SectionName)
{
	return (CSection *)CParameterTree::Find(SectionName);
}

void CSection::AddParameter(char *ParameterName, CParameterValue &Value)
{
	CParameter *CurParameter;

	CurParameter = new CParameter(ParameterName, Value);
	AddChildAtEnd(CurParameter);
}
