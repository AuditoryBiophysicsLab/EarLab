#ifndef CParameterFile_H_INCLUDED
#define CParameterFile_H_INCLUDED
#include "CParameterSet.h"

class CParameterFile : public CParameterSet
{
public:
	CParameterFile();
	CParameterFile(char *FileName);
	~CParameterFile();
	void ReadParameterFile(char *FileName);
	const char *GetFileName(void);
private:
	char *mFileName;
};
#endif
