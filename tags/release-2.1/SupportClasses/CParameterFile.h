#ifndef CParameterFile_H_INCLUDED
#define CParameterFile_H_INCLUDED
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
