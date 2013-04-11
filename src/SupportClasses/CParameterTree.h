#ifndef CParameterTree_H_INCLUDED
#define CParameterTree_H_INCLUDED

class CParameterTree
{
public:
	CParameterTree();
	CParameterTree(char *Name);
	~CParameterTree();
	const char *GetName(void) const;
	CParameterTree *GetNext(void) const;
	CParameterTree *GetChild(void) const;
	void AddAtEnd(CParameterTree *newObj);
	void AddChildAtEnd(CParameterTree *newChild);
	CParameterTree *Find(const char *Name);
private:
	char mName[256];
	CParameterTree *mNext;
	CParameterTree *mChild;
};
#endif
