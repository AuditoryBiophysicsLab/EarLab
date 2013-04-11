#include <iostream>
#include "CParameterTree.h"

using namespace std;

CParameterTree::CParameterTree()
{
	mNext = NULL;
	mChild = NULL;
}

CParameterTree::CParameterTree(char *Name)
{
	mName = new char [strlen(Name) + 1];
	strcpy(mName, Name);

	mNext = NULL;
	mChild = NULL;
}

CParameterTree::~CParameterTree()
{
	if (mName != NULL)
		delete [] mName;

	if (mNext != NULL)
		delete mNext;

	if (mChild != NULL)
		delete mChild;
}

const char *CParameterTree::GetName(void) const
{
	return mName;
}

CParameterTree *CParameterTree::Find(const char *Name)
{
	CParameterTree *cur;

	for (cur = this; cur != NULL; cur = cur->mNext)
	{
		if (strcmp(Name, cur->mName) == 0)
			return cur;
	}
	return NULL;
}

void CParameterTree::AddAtEnd(CParameterTree *newObj)
{
	CParameterTree *cur;

	if (mNext == NULL)
		mNext = newObj;
	else
	{
		for (cur = this; cur->mNext != NULL; cur = cur->mNext);
		cur->mNext = newObj;
	}
}

void CParameterTree::AddChildAtEnd(CParameterTree *newChild)
{
	CParameterTree *cur;

	if (mChild == NULL)
		mChild = newChild;
	else
	{
		for (cur = mChild; cur->mNext != NULL; cur = cur->mNext);
		cur->mNext = newChild;
	}
}
CParameterTree *CParameterTree::GetNext(void) const
{
	return mNext;
}

CParameterTree *CParameterTree::GetChild(void) const
{
	return mChild;
}
