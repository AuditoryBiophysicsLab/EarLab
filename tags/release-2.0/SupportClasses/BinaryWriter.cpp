#include "BinaryWriter.h"
#include "EarlabException.h"
#include "memory.h"
#include "string.h"

BinaryWriter::BinaryWriter()
{
	Initialize();
}

BinaryWriter::BinaryWriter(char *baseFilename)
{
	Initialize();
	Open(baseFilename);
}

BinaryWriter::~BinaryWriter()
{
	Close();
}

void BinaryWriter::Initialize(void)
{
	memset(this, 0, sizeof(BinaryWriter));
	Min = 1e300;
	Max = -1e300;
}

void BinaryWriter::Open(char *baseFilename)
{
	if (IsOpen)
		throw EarlabException("BinaryWriter: File is already open, cannot reopen");
	if (baseFilename == NULL)
		throw EarlabException("BinaryWriter: baseFilename cannot be NULL");
	if (strlen(baseFilename) == 0)
		throw EarlabException("BinaryWriter: baseFilename cannot be an empty string");

	BaseFilename = new char [strlen(baseFilename) + 1];
	strcpy(BaseFilename, baseFilename);
	IsOpen = true;
}

void BinaryWriter::Close(void)
{
	if (BinaryFP != NULL)
		fclose(BinaryFP);
	if (MetadataFP != NULL)
		fclose(MetadataFP);

	if (BaseFilename != NULL)
		delete [] BaseFilename;

	if (BinaryFilename != NULL)
		delete [] BinaryFilename;

	if (MetadataFilename != NULL)
		delete [] MetadataFilename;

	Initialize();
}

void BinaryWriter::Write(const float *buf, unsigned long item_count)
{
	unsigned long i;

	if (!IsOpen)
		throw EarlabException("BinaryWriter: Attempted to write binary data to unopened BinaryWriter object");
	if (BinaryFP == NULL)
		OpenBinaryFile();
	for (i = 0; i < item_count; i++)
	{
		Min = Min < buf[i] ? Min : buf[i];
		Max = Max > buf[i] ? Max : buf[i];
	}
	fwrite(buf, sizeof(float), item_count, BinaryFP);
}

void BinaryWriter::CheckMinMax(const float *buf, unsigned long item_count)
{
	unsigned long i;

	for (i = 0; i < item_count; i++)
	{
		Min = Min < buf[i] ? Min : buf[i];
		Max = Max > buf[i] ? Max : buf[i];
	}
}

void BinaryWriter::WriteMetadata(char *tag, char *value)
{
	if (!IsOpen)
		throw EarlabException("BinaryWriter: Attempted to write ASCII metadata (%s) to unopened BinaryWriter object", tag);
	if (MetadataFP == NULL)
		OpenMetadataFile();
	fprintf(MetadataFP, "string\t%s\t%s\n", tag, value);
}

void BinaryWriter::WriteMetadata(char *tag, int value)
{
	if (!IsOpen)
		throw EarlabException("BinaryWriter: Attempted to write integer metadata (%s) to unopened BinaryWriter object", tag);
	if (MetadataFP == NULL)
		OpenMetadataFile();
	fprintf(MetadataFP, "int\t%s\t%d\n", tag, value);
}

void BinaryWriter::WriteMetadata(char *tag, float value)
{
	if (!IsOpen)
		throw EarlabException("BinaryWriter: Attempted to write float precision metadata (%s) to unopened BinaryWriter object", tag);
	if (MetadataFP == NULL)
		OpenMetadataFile();
	fprintf(MetadataFP, "float\t%s\t%f\n", tag, value);
}

void BinaryWriter::WriteMetadata(char *tag, double value)
{
	if (!IsOpen)
		throw EarlabException("BinaryWriter: Attempted to write single precision metadata (%s) to unopened BinaryWriter object", tag);
	if (MetadataFP == NULL)
		OpenMetadataFile();
	fprintf(MetadataFP, "float\t%s\t%f\n", tag, (float)value);
}

void BinaryWriter::WriteMetadata(char *tag, bool value)
{
	if (!IsOpen)
		throw EarlabException("BinaryWriter: Attempted to write boolean metadata (%s) to unopened BinaryWriter object", tag);
	if (MetadataFP == NULL)
		OpenMetadataFile();
	fprintf(MetadataFP, "bool\t%s\t%s\n", tag, value ? "true" : "false");
}

void BinaryWriter::OpenBinaryFile(void)
{
	if (BinaryFP != NULL)
		return;
	if (BinaryFilename == NULL)
	{
		BinaryFilename = new char [strlen(BaseFilename) + 8];	// 8 is to allow space for ".binary\0"
		sprintf(BinaryFilename, "%s.binary", BaseFilename);
		BinaryFP = fopen(BinaryFilename, "wb");
	}
	else
		BinaryFP = fopen(BinaryFilename, "ab");
	if (BinaryFP == NULL)
		throw EarlabException("BinaryWriter: Error opening binary file %s", BinaryFilename);
}

void BinaryWriter::CloseBinaryFile(void)
{
	if (BinaryFP != NULL)
		fclose(BinaryFP);
	BinaryFP = NULL;
}

void BinaryWriter::WriteMinMaxMetadata(void)
{
	WriteMetadata("Minimum", Min);
	WriteMetadata("Maximum", Max);
}

void BinaryWriter::OpenMetadataFile(void)
{
	if (MetadataFP != NULL)
		return;
	if (MetadataFilename == NULL)
	{
		MetadataFilename = new char [strlen(BaseFilename) + 10];	// 10 is to allow space for ".metadata\0"
		sprintf(MetadataFilename, "%s.metadata", BaseFilename);
		MetadataFP = fopen(MetadataFilename, "w");
	}
	else	
		MetadataFP = fopen(MetadataFilename, "a");
	if (MetadataFP == NULL)
		throw EarlabException("MetadataWriter: Error opening metadata file %s", MetadataFilename);
}

void BinaryWriter::CloseMetadataFile(void)
{
	if (MetadataFP != NULL)
		fclose(MetadataFP);
	MetadataFP = NULL;
}
