#ifndef CParameterFile_H_INCLUDED
#define CParameterFile_H_INCLUDED

/*
 * Use new parameter space version
 */
#ifdef PARAMETER_SPACE
#include "CParameterSpace.h"

// Derive CParamaterFile from CParameterSpace
class CParameterFile : public CParameterSpace
{
  public:
	CParameterFile();
	CParameterFile(const char *FileName);
	~CParameterFile();

  private:
};
#endif

/*
 * Use older parameter file/set version
 */
#else

#include "CParameterSet.h"

class CParameterFile : public CParameterSet
{
public:
	CParameterFile();
	CParameterFile(const char *FileName);
	~CParameterFile();
	void ReadParameterFile(const char *FileName);
	const char *GetFileName(void);
private:
	char *mFileName;
};
#endif
