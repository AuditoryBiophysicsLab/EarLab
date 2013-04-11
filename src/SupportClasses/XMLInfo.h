#ifndef __XML_INFO_H_INCLUDED
#define __XML_INFO_H_INCLUDED
#include "Earlab.h"
#include "tinyxml.h"

class XMLInfo
{
public:
	XMLInfo(char *NodeName);
	~XMLInfo();

	TiXmlElement *XmlElement() {return mElement;}

	void AddElement(const char *ElementName, const char *ElementValue, const int count);
	void AddElement(const char *ElementName, const char *ElementValue);
	void AddElement(const char *ElementName, const int ElementValue);
	void AddElement(const char *ElementName, const double ElementValue);
	void AddElement(const char *ElementName, const bool ElementValue);
	void AddElement(const char *ElementName, const int *ElementValue, const int count);
	void AddElement(const char *ElementName, const double *ElementValue, const int count);
	void AddElement(TiXmlElement *NewElement);

	bool CleanupXmlElement;
private:
	TiXmlElement *mElement;

	char *ToString(int value);
	char *ToString(double value);
	char *ToString(bool value);
};

#endif
