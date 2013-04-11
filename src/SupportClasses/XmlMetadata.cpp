#include "Earlab.h"
#include <string.h>
#include <stdio.h>
#include <memory.h>
#include "XmlMetadata.h"
#include "EarlabException.h"
#include "ModuleServices.h"

XmlMetadataEntry::XmlMetadataEntry()
{
	Name = NULL;
	Value = NULL;
}

XmlMetadataEntry::XmlMetadataEntry(const char *Name, const int Value)
{
	char Temp[50];
	Type = "int";
	sprintf(Temp, "%d", Value);
	Init(Name, Temp);
}

XmlMetadataEntry::XmlMetadataEntry(const char *Name, const double Value)
{
	char Temp[50];
	Type = "double";
	sprintf(Temp, "%f", Value);
	Init(Name, Temp);
}

XmlMetadataEntry::XmlMetadataEntry(const char *Name, const char *Value)
{
	Type = "string";
	Init(Name, Value);
}

void XmlMetadataEntry::Init(const char *Name, const char *Value)
{
	this->Name = new char[strlen(Name) + 1];
	strcpy(this->Name, Name);
	this->Value = new char[strlen(Value) + 1];
	strcpy(this->Value, Value);
}

XmlMetadataEntry::~XmlMetadataEntry()
{
	if (Name != NULL)
		delete [] Name;
	if (Value != NULL)
		delete [] Value;
}

void XmlParameter::Write(FILE *fp)
{
	fprintf(fp, "    <%s>%s</%s>\n", Name, Value, Name);
}

XmlSource::XmlSource(const char *InstanceName, const char *VersionLSID)
{
	this->InstanceName = new char[strlen(InstanceName) + 1];
	strcpy(this->InstanceName, InstanceName);
	this->VersionLSID = new char[strlen(VersionLSID) + 1];
	strcpy(this->VersionLSID, VersionLSID);
}

XmlSource::~XmlSource()
{
	if (InstanceName != NULL)
		delete [] InstanceName;
	if (VersionLSID != NULL)
		delete [] VersionLSID;
}

void XmlSource::Write(FILE *fp)
{
	fprintf(fp, "      <InstanceName>%s</InstanceName>\n", InstanceName);
	fprintf(fp, "      <VersionLSID>%s</VersionLSID>\n", VersionLSID);
    fprintf(fp, "      <SchemaVersion>2.0</SchemaVersion>\n");
}

XmlDimension::XmlDimension(const char *Name, const int Size, const double AxisStartValue, const double AxisEndValue) : XmlMetadataEntry(Name, Size)
{
	this->Size = Size;
	AxisValues = NULL;
	this->AxisStartValue = AxisStartValue;
	this->AxisEndValue = AxisEndValue;
}

XmlDimension::XmlDimension(const char *Name, const int Size, const double *AxisValues) : XmlMetadataEntry(Name, Size)
{
	int i;

	this->Size = Size;
	AxisStartValue = 0.0f;
	AxisEndValue = 0.0f;
	this->AxisValues = new double [Size];
	for (i = 0; i < Size; i++)
		this->AxisValues[i] = AxisValues[i];
}

XmlDimension::XmlDimension(const char *Name, const int Size, const char *AxisValuesFileName) : XmlMetadataEntry(Name, Size)
{
	int i = 0;
	FILE *fp;
	float tmp;

	this->Size = Size;
	if ((AxisValuesFileName == NULL) || (strlen(AxisValuesFileName) == 0))
	{
		AxisValues = NULL;
		AxisStartValue = 1;
		AxisEndValue = Size;
		return;
	}
	fp = OpenOutputFile(AxisValuesFileName, "r");
	if (fp != NULL)
	{
		while (!feof(fp))
		{
			fscanf(fp, "%f", &tmp);
			i++;
		}
		i--;

		if (Size >= i)
		{
			AxisValues = new double [Size];
			memset(AxisValues, 0, sizeof(double) * Size);
		}
		else
		{
			AxisValues = new double [i];
			memset(AxisValues, 0, sizeof(double) * i);
		}
		fseek(fp, 0, SEEK_SET);
		for (int j = 0; j < i; j++)
		{
			fscanf(fp, "%f", &tmp);
			AxisValues[j] = tmp;
		}
		fclose(fp);
	}
	else
		throw EarlabException("XmlDimension: Unable to open specified dimension file: \"%s\"", AxisValuesFileName);
}

XmlDimension::~XmlDimension()
{
	if (AxisValues != NULL)
		delete [] AxisValues;
}

void XmlDimension::Write(FILE *fp)
{
	fprintf(fp, "    <Dimension>\n");
	fprintf(fp, "      <Name>%s</Name>\n", Name);
	fprintf(fp, "      <Size>%d</Size>\n", Size);
	fprintf(fp, "      <Index>%d</Index>\n", Index);
	fprintf(fp, "      <Axis>\n");
	if (AxisValues == NULL)
	{
		fprintf(fp, "        <Auto>\n");
		fprintf(fp, "          <StartValue>%f</StartValue>\n", AxisStartValue);
		fprintf(fp, "          <EndValue>%f</EndValue>\n", AxisEndValue);
		fprintf(fp, "        </Auto>\n");
	}
	else
	{
		fprintf(fp, "        <Array>\n");
		for (int i = 0; i < Size; i++)
			fprintf(fp, "          <Element Index=\"%d\" Value=\"%f\" />\n", i, AxisValues[i]);
		fprintf(fp, "        </Array>\n");
	}
	fprintf(fp, "      </Axis>\n");
	fprintf(fp, "    </Dimension>\n");
}

XmlMetadataFile::XmlMetadataFile(const char *Filename)
{
	this->Filename = new char[strlen(Filename) + 1];
	strcpy(this->Filename, Filename);
	Source = NULL;
	Samples = 0;
	Min = 1e308;
	Max = -1e308;
}

XmlMetadataFile::~XmlMetadataFile()
{
	delete [] Filename;
	Parameters.clear();
	Dimensions.clear();
	if (Source != NULL)
		delete Source;
}

void XmlMetadataFile::Add(XmlParameter *NewParameter)
{
	Parameters.push_back(NewParameter);
}

void XmlMetadataFile::Add(XmlDimension *NewDimension)
{
	Dimensions.push_back(NewDimension);
	NewDimension->SetIndex((int)Dimensions.size());
}

void XmlMetadataFile::SetSource(XmlSource *NewSource)
{
	if (Source != NULL)
		delete Source;
	Source = NewSource;
}

void XmlMetadataFile::Write(void)
{
	unsigned int i;
	XmlDimension Time("Time (ms)", Samples, 0.0, ((double)Samples / SampleRate_Hz) * 1000);
	Time.SetIndex((int)Dimensions.size() + 1);


	fp = OpenOutputFile(Filename, "w");
	fprintf(fp, "<?xml version=\"1.0\" standalone=\"yes\"?>\n");
	fprintf(fp, "<EarLabMetadata xmlns=\"http://earlab.bu.edu/software/specifications/schemas/EarLabMetadata.xsd\">\n");
	fprintf(fp, "  <Parameters>\n");
	fprintf(fp, "    <MinimumValue>%f</MinimumValue>\n", Min);
	fprintf(fp, "    <MaximumValue>%f</MaximumValue>\n", Max);
	for (i = 0; i < Parameters.size(); i++)
		Parameters[i]->Write(fp);
	fprintf(fp, "  </Parameters>\n");
	fprintf(fp, "  <Dimensions>\n");
	for (i = 0; i < Dimensions.size(); i++)
		Dimensions[i]->Write(fp);
	Time.Write(fp);
	fprintf(fp, "  </Dimensions>\n");
	if (Source != NULL)
	{
		fprintf(fp, "  <Source>\n");
		Source->Write(fp);
		fprintf(fp, "  </Source>\n");
	}
	fprintf(fp, "</EarLabMetadata>\n");
	CloseFile(fp);
}

void XmlMetadataFile::UpdateMinMax(double Sample)
{
	Min = Min < Sample ? Min : Sample;
	Max = Max > Sample ? Max : Sample;
}
