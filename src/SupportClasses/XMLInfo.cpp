#include "XMLInfo.h"
#include "tinyxml.h"

XMLInfo::XMLInfo(char *NodeName)
{
	mElement = new TiXmlElement(NodeName);
	CleanupXmlElement = false;
}

XMLInfo::~XMLInfo()
{
	if ((mElement != NULL) && (CleanupXmlElement))
		delete mElement;
}

void XMLInfo::AddElement(const char *ElementName, const char *ElementValue, const int ElementCount)
{
	AddElement(ElementName, ElementValue);
}

void XMLInfo::AddElement(const char *ElementName, const char *ElementValue)
{
	TiXmlElement *theElement = new TiXmlElement(ElementName);
	TiXmlText *theValue = new TiXmlText(ElementValue);
	theElement->LinkEndChild(theValue);
	mElement->LinkEndChild(theElement);
}

void XMLInfo::AddElement(const char *ElementName, const int ElementValue)
{
	TiXmlElement *theElement = new TiXmlElement(ElementName);
	TiXmlText *theValue = new TiXmlText(ToString(ElementValue));
	theElement->LinkEndChild(theValue);
	mElement->LinkEndChild(theElement);
}

void XMLInfo::AddElement(const char *ElementName, const double ElementValue)
{
	TiXmlElement *theElement = new TiXmlElement(ElementName);
	TiXmlText *theValue = new TiXmlText(ToString(ElementValue));
	theElement->LinkEndChild(theValue);
	mElement->LinkEndChild(theElement);
}

void XMLInfo::AddElement(const char *ElementName, const bool ElementValue)
{
	TiXmlElement *theElement = new TiXmlElement(ElementName);
	TiXmlText *theValue = new TiXmlText(ToString(ElementValue));
	theElement->LinkEndChild(theValue);
	mElement->LinkEndChild(theElement);
}

void XMLInfo::AddElement(const char *ElementName, const int *Elements, const int ElementCount)
{
	TiXmlElement *theElement = new TiXmlElement(ElementName);
	if (Elements == NULL)
		theElement->LinkEndChild(new TiXmlText("null"));
	else
	{
		TiXmlElement *value;
		for (int i = 0; i < ElementCount; i++)
		{
			value = new TiXmlElement("IntegerArrayValue");
			value->LinkEndChild(new TiXmlText(ToString(Elements[i])));
			theElement->LinkEndChild(value);
		}
	}
	mElement->LinkEndChild(theElement);
}

void XMLInfo::AddElement(const char *ElementName, const double *Elements, const int ElementCount)
{
	TiXmlElement *theElement = new TiXmlElement(ElementName);
	if (Elements == NULL)
		theElement->LinkEndChild(new TiXmlText("null"));
	else
	{
		TiXmlElement *value;
		for (int i = 0; i < ElementCount; i++)
		{
			value = new TiXmlElement("FloatArrayValue");
			value->LinkEndChild(new TiXmlText(ToString(Elements[i])));
			theElement->LinkEndChild(value);
		}
	}
	mElement->LinkEndChild(theElement);
}

void XMLInfo::AddElement(TiXmlElement *NewElement)
{
	mElement->LinkEndChild(NewElement);
}

char *XMLInfo::ToString(int value)
{
	char buf[100];
	char *retval;
	sprintf(buf, "%d", value);
	retval = new char[strlen(buf) + 1];
	strcpy(retval, buf);
	return retval;
}

char *XMLInfo::ToString(double value)
{
	char buf[100];
	char *retval;
	sprintf(buf, "%f", value);
	retval = new char[strlen(buf) + 1];
	strcpy(retval, buf);
	return retval;
}

char *XMLInfo::ToString(bool value)
{
	char *retval;
	if (value)
	{
		retval = new char[5];
		strcpy(retval, "true");
	}
	else
	{
		retval = new char[6];
		strcpy(retval, "false");
	}
	return retval;
}
