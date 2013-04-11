#ifndef _CPARAMETERSPACE_H_INCLUDED
#define _CPARAMETERSPACE_H_INCLUDED

#include <string.h>
#include "ParameterStatus.h"

class CParameterSpace
{
public:
	CParameterSpace();
	CParameterSpace(const  char *FileName);
	CParameterSpace(const char *FileName, const char *Section);
	~CParameterSpace();
	void ReadParameterFile(const char *FileName, const char *Section);
	const char *GetFileName(void);

	/* Bool */
	ParameterStatus GetParameter(const char *SectionName,
				     const char *ParameterName,
				     bool &RetVal, const bool DefVal);

	/* Int */
	ParameterStatus GetParameter(const char *SectionName,
				     const char *ParameterName,
				     int &RetVal, const int DefVal);

	/* Float */
	ParameterStatus GetParameter(const char *SectionName,
				     const char *ParameterName,
				     float &RetVal, const float DefVal);

	/* Double */
	ParameterStatus GetParameter(const char *SectionName,
				     const char *ParameterName,
				     double &RetVal, double DefVal);

	/* String */
	ParameterStatus GetParameter(const char *SectionName,
				     const char *ParameterName,
				     char *RetVal, const char *DefVal);

	/* StringN */
	ParameterStatus GetParameter(const char *SectionName,
				     const char *ParameterName,
				     char *RetVal, unsigned int RetValLen, const char *DefVal);

	/* Int vector */
	ParameterStatus GetParameter(const char *SectionName,
				     const char *ParameterName,
				     int *RetVal,    int &RetValLen, const int    *DefVal, const int DefValLen);

	/* Float vector */
	ParameterStatus GetParameter(const char *SectionName,
				     const char *ParameterName,
				     float *RetVal,  int &RetValLen, const float  *DefVal, const int DefValLen);

	/* Double vector */
	ParameterStatus GetParameter(const char *SectionName,
				     const char *ParameterName,
				     double *RetVal, int &RetValLen, const double *DefVal, const int DefValLen);


private:
	char *mFileName;
	char *mSection;
	char *mData;
};
#endif

















