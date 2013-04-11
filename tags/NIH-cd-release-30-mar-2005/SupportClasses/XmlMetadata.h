#pragma once
#include <vector>
#include <stdio.h>
#include "Earlab.h"

using namespace std;

class XmlMetadataEntry
{
public:
	XmlMetadataEntry();
	XmlMetadataEntry(const char *Name, const char  *Value);
	XmlMetadataEntry(const char *Name, const int    Value);
	XmlMetadataEntry(const char *Name, const double Value);
	~XmlMetadataEntry();
	virtual void Write(FILE *fp) = 0;
protected:
	void Init(const char *Name, const char *Value);
	char *Type;
	char *Name;
	char *Value;
};

class XmlParameter : XmlMetadataEntry
{
public:
	XmlParameter(const char *Name, const char  *Value) : XmlMetadataEntry(Name, Value) {};
	XmlParameter(const char *Name, const int    Value) : XmlMetadataEntry(Name, Value) {};
	XmlParameter(const char *Name, const float  Value) : XmlMetadataEntry(Name, (double)Value) {};
	XmlParameter(const char *Name, const double Value) : XmlMetadataEntry(Name, Value) {};
	virtual void Write(FILE *fp);
};

class XmlSource : XmlMetadataEntry
{
public:
	XmlSource(const char  *InstanceName, const char *ModuleLSID);
	~XmlSource();

	virtual void Write(FILE *fp);
protected:
	char *InstanceName, *VersionLSID;
};

class XmlDimension: XmlMetadataEntry
{
public:
	XmlDimension(const char *Name, const int Size, const double AxisStartValue, const double AxisEndValue);
	XmlDimension(const char *Name, const int Size, const double *AxisValues);
	XmlDimension(const char *Name, const int Size, const char *AxisValuesFileName);
	~XmlDimension();
	void SetIndex(int Index) {this->Index = Index;}
	virtual void Write(FILE *fp);
protected:
	int Index, Size;
	double AxisStartValue, AxisEndValue;
	double *AxisValues;
};

class XmlMetadataFile
{
public:
	XmlMetadataFile(const char *Filename);
	~XmlMetadataFile();
	void Add(XmlParameter *NewParameter);
	void Add(XmlDimension *NewDimension);
	void SetSource(XmlSource *NewSource);
	void Write(void);
	void UpdateMinMax(double Sample);
	void SetSampleRate_Hz(double SampleRate_Hz) {this->SampleRate_Hz = SampleRate_Hz;}
	void AddSample(void) {Samples++;}
	void AddSample(int NumSamples) {Samples += NumSamples;}
	int GetSampleCount(void) {return Samples;}
private:
	char *Filename;
	FILE *fp;
	vector <XmlParameter *> Parameters;
	vector <XmlDimension *> Dimensions;
	XmlSource *Source;
	unsigned long Samples;
	double Min, Max;
	double SampleRate_Hz;
};

