#include <iostream>
#include <fstream>
#include "CParameterSpace.h"
#include "ParameterStatus.h"
#include "EarlabException.h"

using namespace std;

typedef const char * String;

extern "C" int CReadParameterFile(String file);
extern "C" int GetSectionParameterBool     (String section, String varname, int   *retval);
extern "C" int GetSectionParameterInt      (String section, String varname, int    *retval);
extern "C" int GetSectionParameterFloat    (String section, String varname, float  *retval);
extern "C" int GetSectionParameterDouble   (String section, String varname, double *retval);
extern "C" int GetSectionParameterString   (String section, String varname, char  *retval);
extern "C" int GetSectionParameterStringN  (String section, String varname, int     n, char   *retval);
extern "C" int GetSectionParameterIntVec   (String section, String varname, int    *n, int    *retval);
extern "C" int GetSectionParameterFloatVec (String section, String varname, int    *n, float  *retval);
extern "C" int GetSectionParameterDoubleVec(String section, String varname, int    *n, double *retval);

CParameterSpace::CParameterSpace()
{
	mFileName = NULL;
}

CParameterSpace::CParameterSpace(const char *FileName)
{
	mFileName = NULL;
	ReadParameterFile(FileName, NULL);
}

CParameterSpace::CParameterSpace(const char *FileName, const char *Section)
{
	mFileName = NULL;
	ReadParameterFile(FileName, Section);
}

CParameterSpace::~CParameterSpace(void)
{
	if (mFileName != NULL)
		delete [] mFileName;
}

void CParameterSpace::ReadParameterFile(const char *FileName, const char *Section)
{

	if (FileName) {
		 if (mFileName != NULL)
			delete [] mFileName;
		 mFileName = new char[strlen(FileName) + 1];
		 if (FileName) strcpy(mFileName, FileName);
		 else          mFileName[0] = '\0';
	}
	if (Section) {
		 if (mSection != NULL)
			delete [] mSection;
		 mSection = new char[strlen(Section) + 1];
		 if (Section) strcpy(mSection, Section);
		 else         mSection[0] = '\0';
	}
	CReadParameterFile(FileName);

	return;
}

/****************
 * GetParameter
 ****************/

/* Bool */
ParameterStatus
CParameterSpace::
GetParameter(const char *SectionName, const char *ParameterName,
	     bool &RetVal, const bool DefVal)
{
	int rc;
	int val;

	rc = GetSectionParameterBool(SectionName?SectionName:mSection,
				       ParameterName,
				       &val);

	RetVal = val?true:false;

	switch (rc) {
	  case 0:
		RetVal = DefVal;
		return(Defaulted);

	  case 1:
		return(Success);

	  default:
		return(Failure);
	}
}

/* Int */
ParameterStatus
CParameterSpace::
GetParameter(const char *SectionName, const char *ParameterName,
	     int &RetVal, const int DefVal)
{
	int rc;

	rc = GetSectionParameterInt(SectionName?SectionName:mSection,
				       ParameterName,
				       &RetVal);

	switch (rc) {
	  case 0:
		RetVal = DefVal;
		return(Defaulted);

	  case 1:
		return(Success);

	  default:
		return(Failure);
	}
}

/* Float */
ParameterStatus
CParameterSpace::
GetParameter(const char *SectionName, const char *ParameterName,
	     float &RetVal, const float DefVal)
{
	int rc;

	rc = GetSectionParameterFloat(SectionName?SectionName:mSection,
				       ParameterName,
				       &RetVal);
	switch (rc) {
	  case 0:
		RetVal = DefVal;
		return(Defaulted);

	  case 1:
		return(Success);

	  default:
		return(Failure);
	}
}

/* Double */
ParameterStatus CParameterSpace::
GetParameter(const char *SectionName, const char *ParameterName,
	     double &RetVal, double DefVal)
{
	int rc;

	rc = GetSectionParameterDouble(SectionName?SectionName:mSection,
				       ParameterName,
				       &RetVal);
	switch (rc) {
	  case 0:
		RetVal = DefVal;
		return(Defaulted);

	  case 1:
		return(Success);

	  default:
		return(Failure);
	}
}

/* String */
ParameterStatus
CParameterSpace::
GetParameter(const char *SectionName, const char *ParameterName,
	     char *RetVal, const char *DefVal)
{
	int rc;

	rc = GetSectionParameterString(SectionName?SectionName:mSection,
				       ParameterName,
				       RetVal);
	switch (rc) {
	  case 0:
		if (!DefVal) return(Failure);
		strcpy(RetVal, DefVal);
		return(Defaulted);

	  case 1:
		return(Success);

	  default:
		return(Failure);
	}
}

/* StringN */
ParameterStatus
CParameterSpace::
GetParameter(const char *SectionName, const char *ParameterName,
	      char *RetVal, unsigned int RetValLen, const char *DefVal)
{
	int rc;

	rc = GetSectionParameterStringN(SectionName?SectionName:mSection,
				       ParameterName,
				       RetValLen, RetVal);
	switch (rc) {
	  case 0:
		if (!DefVal) return(Failure);
		if (RetValLen < strlen(DefVal)) {
			strcpy(RetVal, DefVal);
			return(Defaulted);
		}
		else {
			strncpy(RetVal, DefVal, RetValLen);
			return(BufferOverflowDefaulted);
		}

	  case -1:
		return(BufferOverflowSuccess);

	  case 1:
		return(Success);

	  default:
		return(Failure);
	}
}

/* Int vector */
ParameterStatus
CParameterSpace::
GetParameter(const char *SectionName, const char *ParameterName,
	     int *RetVal, int &RetValLen, const int *DefVal, int DefValLen)
{
	return(Success);
	int rc;

	rc = GetSectionParameterIntVec(SectionName?SectionName:mSection,
				       ParameterName,
				       &RetValLen, RetVal);
	switch (rc) {
	  case 0:
		if (!DefVal) return(Failure);

		memcpy(RetVal, DefVal, DefValLen*sizeof(int));
		return(Defaulted);

	  case 1:
		return(Success);

	  default:
		return(Failure);
	}
}

/* Float vector */
ParameterStatus
CParameterSpace::
GetParameter(const char *SectionName, const char *ParameterName,
		 float *RetVal, int &RetValLen, const float *DefVal, int DefValLen)
{
	int rc;

	rc = GetSectionParameterFloatVec(SectionName?SectionName:mSection,
				       ParameterName,
				       &RetValLen, RetVal);
	switch (rc) {
	  case 0:
		if (!DefVal) return(Failure);

		memcpy(RetVal, DefVal, DefValLen*sizeof(float));
		return(Defaulted);

	  case 1:
		return(Success);

	  default:
		return(Failure);
	}
	return(Success);
}

/* Double vector */
ParameterStatus
CParameterSpace::
GetParameter(const char *SectionName, const char *ParameterName,
		 double *RetVal, int &RetValLen, const double *DefVal, int DefValLen)
{
	int rc;

	rc = GetSectionParameterDoubleVec(SectionName?SectionName:mSection,
				       ParameterName,
				       &RetValLen, RetVal);
	switch (rc) {
	  case 0:
		if (!DefVal) return(Failure);

		memcpy(RetVal, DefVal, DefValLen*sizeof(double));
		return(Defaulted);

	  case 1:
		return(Success);

	  default:
		return(Failure);
	}
	return(Success);
}

