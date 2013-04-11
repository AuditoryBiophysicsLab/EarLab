#include <iostream>
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "Earlab.h"
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"
#include "EarlabDataStream.h"
#include "tinyxml.h"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

void Usage(char *Program);
void UpdateOrCreateOutput(CParameter *CurParameter, TiXmlElement *ParentElement);
void UpdateOrCreateInput(CParameter *CurParameter, TiXmlElement *ParentElement);
void AddParameters(TiXmlElement *CurModuleParameters, CSection *CurParameterSection);
TiXmlElement *FindOutput(TiXmlElement *AllModules, const char *SourceInstanceName, const char *SourceOutputName);

XERCES_CPP_NAMESPACE_USE

// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of char* data to XMLCh data.
// ---------------------------------------------------------------------------
class XStr
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    XStr(const char* const toTranscode)
    {
        // Call the private transcoding method
        fUnicodeForm = XMLString::transcode(toTranscode);
    }

    ~XStr()
    {
        XMLString::release(&fUnicodeForm);
    }


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const XMLCh* unicodeForm() const
    {
        return fUnicodeForm;
    }

private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fUnicodeForm
    //      This is the Unicode XMLCh format of the string.
    // -----------------------------------------------------------------------
    XMLCh*   fUnicodeForm;
};

#define X(str) XStr(str).unicodeForm()

int main(int argc, char *argv[])
{
	int cur_arg = 0;
	char *DiagramFile = NULL;
	char *ParameterFile = NULL;
	char *XmlFile = NULL;

	CParameterFile diagram, parameter;
	CSection *curSection;
	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "UTF-8", "no");
	TiXmlElement *modulesElement = new TiXmlElement("Modules");
	doc.LinkEndChild(decl);
	doc.LinkEndChild(modulesElement);

	for (cur_arg = 1; cur_arg < 100; cur_arg++)
	{
		if (argv[cur_arg] == NULL)
			break;
		if (strcmp("-d", argv[cur_arg]) == 0)
		{
			DiagramFile = argv[++cur_arg];
		}
		else
		{
			if (strcmp("-p", argv[cur_arg]) == 0)
			{
				ParameterFile = argv[++cur_arg];
			}
			else
			{
				if (strcmp("-x", argv[cur_arg]) == 0)
				{
					XmlFile = argv[++cur_arg];
				}
			}
		}
	}
	if ((DiagramFile == NULL) || (ParameterFile == NULL) || (XmlFile == NULL))
	{
		Usage(argv[0]);
	}
	diagram.ReadParameterFile(DiagramFile);
	parameter.ReadParameterFile(ParameterFile);
	curSection = diagram.GetFirstSection();
	while (curSection != NULL)
	{
		TiXmlElement *moduleElement, *inputsElement, *outputsElement, *parametersElement;
		CParameter *curParam;

		moduleElement = inputsElement = outputsElement = parametersElement = NULL;

		if (stricmp(curSection->GetName(), "Model") == 0)
		{
			curSection = diagram.GetNextSection(curSection);
			continue;	// Dump the [Model] section, no longer used
		}

		// Create a new TiXmlElement for the current module
		moduleElement = new TiXmlElement("Module");
		TiXmlElement *instanceName = new TiXmlElement("InstanceName");
		instanceName->LinkEndChild(new TiXmlText(curSection->GetName()));
		moduleElement->LinkEndChild(instanceName);
		// Add the module element to the current Run element
		modulesElement->LinkEndChild(moduleElement);

		// Get the first parameter in the current module section
		curParam = curSection->GetFirstParameter();
		while (curParam != NULL)
		{
			const char *paramName = curParam->GetName();
			if (stricmp(paramName, "ModuleExecutable") == 0)
			{
				char *paramValue = (char *)curParam->Value().ValueToString();
				char *tmp = strtok(paramValue, ".");
				TiXmlElement *executable = new TiXmlElement("ExecutableFile");
				executable->LinkEndChild(new TiXmlText(tmp));
				moduleElement->LinkEndChild(executable);
				//moduleElement->SetAttribute("executable", tmp);
			}
			else if (strnicmp(paramName, "Output", 6) == 0)
			{
				if (outputsElement == NULL)
				{
					outputsElement = new TiXmlElement("Outputs");
					moduleElement->LinkEndChild(outputsElement);
				}
				UpdateOrCreateOutput(curParam, outputsElement);
			}
			else if (strnicmp(paramName, "Input", 5) == 0)
			{
				if (inputsElement == NULL)
				{
					inputsElement = new TiXmlElement("Inputs");
					moduleElement->LinkEndChild(inputsElement);
				}
				UpdateOrCreateInput(curParam, inputsElement);
			}

			curParam = curParam->GetNext();
		}

		// Skip on to the next section
		curSection = diagram.GetNextSection(curSection);
	}

	curSection = parameter.GetFirstSection();
	while (curSection != NULL)
	{
		TiXmlElement *curModule = modulesElement->FirstChildElement();
		while (curModule != NULL)
		{
			
			if (strcmp(curModule->FirstChildElement("InstanceName")->GetText(), curSection->GetName()) == 0)
			{
				TiXmlElement *curParameters = curModule->FirstChildElement("Parameters");
				if (curParameters == NULL)
				{
					curParameters = new TiXmlElement("Parameters");
					curModule->LinkEndChild(curParameters);
				}
				AddParameters(curParameters, curSection);
			}
			curModule = curModule->NextSiblingElement();
		}
		curSection = parameter.GetNextSection(curSection);
	}

	TiXmlElement *curModule = modulesElement->FirstChildElement();
	while (curModule != NULL)
	{
		TiXmlElement *curInputs = curModule->FirstChildElement("Inputs");
		if (curInputs != NULL)
		{
			TiXmlElement *curInput = curInputs->FirstChildElement("Input");
			while (curInput != NULL)
			{
				TiXmlElement *matchingOutput = FindOutput(modulesElement, 
					curInput->FirstChildElement("SourceInstanceName")->GetText(), 
					curInput->FirstChildElement("SourceOutputName")->GetText());
				if (matchingOutput != NULL)
				{
					TiXmlElement *streamType = new TiXmlElement("StreamType");
					TiXmlElement *sourceOutputIndex = new TiXmlElement("SourceOutputIndex");
					streamType->LinkEndChild(new TiXmlText(matchingOutput->FirstChildElement("StreamType")->GetText()));
					sourceOutputIndex->LinkEndChild(new TiXmlText(matchingOutput->FirstChildElement("Index")->GetText()));
					curInput->LinkEndChild(streamType);
					curInput->LinkEndChild(sourceOutputIndex);
				}
				curInput = curInput->NextSiblingElement();
			}
		}
		curModule = curModule->NextSiblingElement();
	}

	doc.SaveFile(XmlFile);
	return 0;
}

TiXmlElement *FindOutput(TiXmlElement *AllModules, const char *SourceInstanceName, const char *SourceOutputName)
{
	TiXmlElement *curModule = AllModules->FirstChildElement();
	while (curModule != NULL)
	{
		if (strcmp(curModule->FirstChildElement("InstanceName")->GetText(), SourceInstanceName) == 0)
		{
			TiXmlElement *curOutputs = curModule->FirstChildElement("Outputs");
			if (curOutputs != NULL)
			{
				TiXmlElement *curOutput = curOutputs->FirstChildElement("Output");
				while (curOutput != NULL)
				{
					if (strcmp(curOutput->FirstChildElement("Name")->GetText(), SourceOutputName) == 0)
						return curOutput;
					curOutput = curOutput->NextSiblingElement();
				}
			}
		}
		curModule = curModule->NextSiblingElement();
	}
	return NULL;
}

void AddParameters(TiXmlElement *CurModuleParameters, CSection *CurParameterSection)
{
	CParameter *curParam;

	curParam = CurParameterSection->GetFirstParameter();
	while (curParam != NULL)
	{
		TiXmlElement *newParameter = new TiXmlElement("Parameter");
		TiXmlElement *name = new TiXmlElement("Name");
		TiXmlElement *dataType = new TiXmlElement("DataType");
		TiXmlElement *value = new TiXmlElement("Value");
		CurModuleParameters->LinkEndChild(newParameter);
		newParameter->LinkEndChild(name);
		newParameter->LinkEndChild(dataType);
		newParameter->LinkEndChild(value);
		name->LinkEndChild(new TiXmlText(curParam->GetName()));
		dataType->LinkEndChild(new TiXmlText(curParam->Value().TypeToString()));
		value->LinkEndChild(new TiXmlText(curParam->Value().ValueToString()));
		curParam = curParam->GetNext();
	}
}

void UpdateOrCreateOutput(CParameter *CurParameter, TiXmlElement *ParentElement)
{
	char *dummy = strtok((char *)CurParameter->GetName(), ".");
	char *name = strtok(NULL, ".");
	char *streamType = strtok(NULL, ".");
	TiXmlElement *curElement = ParentElement->FirstChildElement("Output");
	const char *curName;

	while (curElement != NULL)
	{
		curName = curElement->FirstChildElement("Name")->GetText();
		if ((curName != NULL) && (stricmp(curName, name) == 0))
			break;
		curElement = curElement->NextSiblingElement();
	}
	if (curElement == NULL)
	{
		curElement = new TiXmlElement("Output");
		TiXmlElement *nameElement = new TiXmlElement("Name");
		TiXmlElement *indexElement = new TiXmlElement("Index");
		curElement->LinkEndChild(nameElement);
		curElement->LinkEndChild(indexElement);
		nameElement->LinkEndChild(new TiXmlText(name));
		indexElement->LinkEndChild(new TiXmlText(name));
		ParentElement->LinkEndChild(curElement);
	}
	if (streamType != NULL)
	{
		TiXmlElement *curStreamType =  curElement->FirstChildElement("StreamType");
		curStreamType = new TiXmlElement("StreamType");
		curElement->LinkEndChild(curStreamType);
		curStreamType->LinkEndChild(new TiXmlText(CurParameter->Value().ValueToString()));
	}
}

void UpdateOrCreateInput(CParameter *CurParameter, TiXmlElement *ParentElement)
{
	char *dummy = strtok((char *)CurParameter->GetName(), ".");
	char *name = strtok(NULL, ".");
	TiXmlElement *curElement = ParentElement->FirstChildElement("Input");
	const char *curName;

	while (curElement != NULL)
	{
		curName = curElement->Attribute("name");
		if ((curName != NULL) && (stricmp(curName, name) == 0))
			break;
		curElement = curElement->NextSiblingElement();
	}
	if (curElement == NULL)
	{
		curElement = new TiXmlElement("Input");
		TiXmlElement *nameElement = new TiXmlElement("Name");
		TiXmlElement *indexElement = new TiXmlElement("Index");
		curElement->LinkEndChild(nameElement);
		curElement->LinkEndChild(indexElement);
		nameElement->LinkEndChild(new TiXmlText(name));
		indexElement->LinkEndChild(new TiXmlText(name));
		ParentElement->LinkEndChild(curElement);
	}
	char *sourceInstance = strtok((char *)CurParameter->Value().ValueToString(), ".");
	char *sourceOutput = strtok(NULL, ".");
	TiXmlElement *sourceInstanceElement = new TiXmlElement("SourceInstanceName");
	TiXmlElement *sourceOutputElement = new TiXmlElement("SourceOutputName");
	sourceInstanceElement->LinkEndChild(new TiXmlText(sourceInstance));
	sourceOutputElement->LinkEndChild(new TiXmlText(sourceOutput));
	curElement->LinkEndChild(sourceInstanceElement);
	curElement->LinkEndChild(sourceOutputElement);
}

void Usage(char *Program)
{
	cerr << "Usage: " << Program << " -d <DiagramFile> -p <ParameterFile> -x <XmlFile>" << endl;
	cerr << endl;
	cerr << "Where: <DiagramFile>   is a file that describes an Earlab model" << endl;
	cerr << "       <ParameterFile> is a parameter file that contains parameter definitions" << endl;
	cerr << "                       for all modules specified by the <DiagramFile>." << endl;
	cerr << "       <XmlFile>       is the name of an XML file that will be created" << endl;
	cerr << "                       as this program's output, that will contain all the" << endl;
	cerr << "                       information in the diagram and parameter files." << endl;
	cerr << "                       This file will be usable in future versions of EarLab." << endl;
	cerr << endl;
	exit(1);
}
