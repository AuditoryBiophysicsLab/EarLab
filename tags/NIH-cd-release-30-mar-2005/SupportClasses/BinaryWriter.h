#ifndef _BINARY_WRITER_H_INCLUDED
#define _BINARY_WRITER_H_INCLUDED

#include "stdio.h"

class BinaryWriter
{
public:
	BinaryWriter();
	BinaryWriter(char *baseFilename);
	~BinaryWriter();
	void Open(char *baseFilename);
	void Close(void);
	void CheckMinMax(const float *buf, unsigned long item_count);
	void Write(const float *buf, unsigned long item_count);
	void WriteMetadata(char *tag, char *value);
	void WriteMetadata(char *tag, int value);
	void WriteMetadata(char *tag, float value);
	void WriteMetadata(char *tag, double value);
	void WriteMetadata(char *tag, bool value);
	void WriteMinMaxMetadata(void);
	void CloseBinaryFile(void);
	void CloseMetadataFile(void);
private:
	void Initialize(void);
	void OpenBinaryFile(void);
	void OpenMetadataFile(void);
	bool IsOpen;
	FILE *BinaryFP, *MetadataFP;
	char *BaseFilename;
	char *BinaryFilename;
	char *MetadataFilename;
	double Min, Max;
};

#endif
