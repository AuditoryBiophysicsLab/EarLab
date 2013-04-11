#include "Earlab.h"
#include "EarlabException.h"
#include "WaveFile.h"
#include <iostream>
#include <fstream>
#include <memory.h>
#include <stdlib.h>

using namespace std;

#define WAVEFILE_DEBUG

WaveFile::WaveFile()
{
}

WaveFile::WaveFile(char *WaveFileName)
{
#ifdef WAVEFILE_DEBUG
	cout << "WaveFile(" << WaveFileName << ");" << endl;
#endif
	Read(WaveFileName);
}

WaveFile::~WaveFile()
{
}

void WaveFile::Read(char *WaveFileName)
{
	theFile.open(WaveFileName, ios::in | ios::binary);
	if (!theFile.good())
		throw EarlabException("WaveFile::Read: Error opening \"%s\" for reading", WaveFileName);

	ReadWaveHeader();

	theFile.close();
}

void WaveFile::Write(char *WaveFileName)
{
	theFile.open(WaveFileName, ios::out | ios::binary | ios::trunc);
	if (!theFile.good())
		throw EarlabException("WaveFile::Write: Error opening \"%s\" for writing", WaveFileName);
	
	WriteWaveHeader();

	theFile.close();
}

unsigned short WaveFile::GetChannelCount(void)
{
	return fmt.GetNumChannels();
}

void WaveFile::SetChannelCount(unsigned short ChannelCount)
{
	fmt.SetNumChannels(ChannelCount);
}

unsigned int WaveFile::GetSampleRate_Hz(void)
{
	return fmt.GetSampleRate_Hz();
}

void WaveFile::SetSampleRate_Hz(unsigned int SampleRate_Hz)
{
	fmt.SetSampleRate_Hz(SampleRate_Hz);
}

unsigned short WaveFile::GetBitsPerSample(void)
{
	return fmt.GetBitsPerSample();
}

void WaveFile::SetBitsPerSample(unsigned short BitsPerSample)
{
	fmt.SetBitsPerSample(BitsPerSample);
}

unsigned int WaveFile::GetBytesPerSecond(void)
{
	return fmt.GetBytesPerSecond();
}

unsigned short WaveFile::GetBlockAlignment_Bytes(void)
{
	return fmt.GetBlockAlignment();
}

void WaveFile::ReadWaveHeader(void)
{
#ifdef WAVEFILE_DEBUG
	cout << "Reading \"RIFF\" chunk..." << endl;
#endif
	riff.Read(theFile);
#ifdef WAVEFILE_DEBUG
	cout << "Reading \"fmt \" chunk..." << endl;
#endif
	fmt.Read(theFile);
#ifdef WAVEFILE_DEBUG
	cout << "Reading \"data\" chunk..." << endl;
#endif
	data.Read(theFile);
}

void WaveFile::WriteWaveHeader(void)
{
#ifdef WAVEFILE_DEBUG
	cout << "Writing \"RIFF\" chunk..." << endl;
#endif
	riff.Write(theFile);
#ifdef WAVEFILE_DEBUG
	cout << "Writing \"fmt \" chunk..." << endl;
#endif
	fmt.Write(theFile);
#ifdef WAVEFILE_DEBUG
	cout << "Writing \"data\" chunk..." << endl;
#endif
	data.Write(theFile);
}

void WaveFile::ReadNextSample(void)
{
}

void WaveFile::WriteNextSample(void)
{
}

Chunk::Chunk()
{
	ChunkSize = 0;
	memset(ChunkID, 0, sizeof(ChunkID));
}

void Chunk::Read(fstream &InputFile)
{
	InputFile.read(ChunkID, sizeof(ChunkID));
	InputFile.read((char *)&ChunkSize, sizeof(ChunkSize));
#ifdef WAVEFILE_DEBUG
	cout << "  Chunk ID  : \"" << ChunkID[0] << ChunkID[1] << ChunkID[2] << ChunkID[3] << "\"" << endl;
	cout << "  Chunk Size: " << ChunkSize << endl;
#endif
}

void Chunk::Write(fstream &OutputFile)
{
	OutputFile.write(ChunkID, sizeof(ChunkID));
	OutputFile.write((const char *)&ChunkSize, sizeof(ChunkSize));
}

char *Chunk::GetChunkID(void)
{
	return ChunkID;
}

void Chunk::SetChunkID(char *NewChunkID)
{
	strncpy((char *)ChunkID, NewChunkID, 4);
}

RiffChunk::RiffChunk()
{
	SetChunkID("RIFF");
	strncpy(FormatID, "WAVE", 4);
}

char *RiffChunk::GetFormatID(void)
{
	return FormatID;
}

void RiffChunk::SetFormatID(char *NewFormatID)
{
	strncpy((char *)FormatID, NewFormatID, 4);
}

void RiffChunk::Read(fstream &InputFile)
{
	Chunk::Read(InputFile);
	if (strncmp(GetChunkID(), "RIFF", 4) != 0)
		throw EarlabException("RiffChunk::Read: Invalid wave file format. Chunk ID must be \"RIFF\", not \"%4.4s\"", GetChunkID());
	InputFile.read(FormatID, sizeof(FormatID));
	if (strncmp(FormatID, "WAVE", 4) != 0)
		throw EarlabException("RiffChunk::Read: Invalid wave file format. Format ID must be \"WAVE\", not \"%4.4s\"", FormatID);
#ifdef WAVEFILE_DEBUG
	cout << "    Format ID: \"" << FormatID[0] << FormatID[1] << FormatID[2] << FormatID[3] << "\"" << endl;
#endif
}

void RiffChunk::Write(fstream &OutputFile)
{
	if (strncmp(GetChunkID(), "RIFF", 4) != 0)
		throw EarlabException("RiffChunk::Write: Invalid wave file format. Chunk ID must be \"RIFF\", not \"%4.4s\"", GetChunkID());
	if (strncmp(FormatID, "WAVE", 4) != 0)
		throw EarlabException("RiffChunk::Write: Invalid wave file format. Format ID must be \"WAVE\", not \"%4.4s\"", FormatID);
	Chunk::Write(OutputFile);
	OutputFile.write(FormatID, sizeof(FormatID));
#ifdef WAVEFILE_DEBUG
	cout << "    Format ID: \"" << FormatID[0] << FormatID[1] << FormatID[2] << FormatID[3] << "\"" << endl;
#endif
}

FormatChunk::FormatChunk()
{
	SetChunkID("fmt ");
	AudioFormat = 1;
	NumChannels = 0;
	SampleRate_Hz = 0;
	ByteRate_BPS = 0;
	BlockAlign = 0;
	BitsPerSample = 0;
}

void FormatChunk::Read(fstream &InputFile)
{
	Chunk::Read(InputFile);
	if (strncmp(GetChunkID(), "fmt ", 4) != 0)
		throw EarlabException("FormatChunk::Read: Invalid wave file format. Chunk ID must be \"fmt \", not \"%4.4s\"", GetChunkID());
	InputFile.read((char *)&AudioFormat,		sizeof(AudioFormat));
	if (AudioFormat != 1)
		throw EarlabException("FormatChunk::Read: Invalid wave file format. Audio format must be 1 (Uncompressed PCM), not %d", AudioFormat);
	InputFile.read((char *)&NumChannels,		sizeof(NumChannels));
	InputFile.read((char *)&SampleRate_Hz,		sizeof(SampleRate_Hz));
	InputFile.read((char *)&ByteRate_BPS,		sizeof(ByteRate_BPS));
	InputFile.read((char *)&BlockAlign,			sizeof(BlockAlign));
	InputFile.read((char *)&BitsPerSample,		sizeof(BitsPerSample));
#ifdef WAVEFILE_DEBUG
	cout << "    Audio Format         : " << AudioFormat << endl;
	cout << "    Number of Channels   : " << NumChannels << endl;
	cout << "    Sample Rate (Hz)     : " << SampleRate_Hz << endl;
	cout << "    Byte Rate (Bytes/Sec): " << ByteRate_BPS << endl;
	cout << "    Block Alignment      : " << BlockAlign << endl;
	cout << "    Bits per Sample      : " << BitsPerSample << endl;
#endif
}

void FormatChunk::Write(fstream &OutputFile)
{
	if (strncmp(GetChunkID(), "fmt ", 4) != 0)
		throw EarlabException("FormatChunk::Write: Invalid wave file format. Chunk ID must be \"fmt \", not \"%4.4s\"", GetChunkID());
	if (AudioFormat != 1)
		throw EarlabException("FormatChunk::Write: Invalid wave file format. Audio format must be 1 (Uncompressed PCM), not %d", AudioFormat);
	Chunk::Write(OutputFile);
	OutputFile.write((char *)&AudioFormat,		sizeof(AudioFormat));
	OutputFile.write((char *)&NumChannels,		sizeof(NumChannels));
	OutputFile.write((char *)&SampleRate_Hz,	sizeof(SampleRate_Hz));
	OutputFile.write((char *)&ByteRate_BPS,		sizeof(ByteRate_BPS));
	OutputFile.write((char *)&BlockAlign,		sizeof(BlockAlign));
	OutputFile.write((char *)&BitsPerSample,	sizeof(BitsPerSample));
#ifdef WAVEFILE_DEBUG
	cout << "    Audio Format         : " << AudioFormat << endl;
	cout << "    Number of Channels   : " << NumChannels << endl;
	cout << "    Sample Rate (Hz)     : " << SampleRate_Hz << endl;
	cout << "    Byte Rate (Bytes/Sec): " << ByteRate_BPS << endl;
	cout << "    Block Alignment      : " << BlockAlign << endl;
	cout << "    Bits per Sample      : " << BitsPerSample << endl;
#endif
}

void FormatChunk::SetNumChannels(short NumChannels)
{
	this->NumChannels = NumChannels;
	UpdateFields();
}

short FormatChunk::GetNumChannels(void)
{
	return NumChannels;
}

void FormatChunk::SetSampleRate_Hz(int SampleRate_Hz)
{
	this->SampleRate_Hz = SampleRate_Hz;
	UpdateFields();
}

int FormatChunk::GetSampleRate_Hz(void)
{
	return SampleRate_Hz;
}

void FormatChunk::SetBitsPerSample(short BitsPerSample)
{
	switch (BitsPerSample)
	{
	case 8:
	case 16:
	case 24:
		this->BitsPerSample = BitsPerSample;
		UpdateFields();
		break;
	default:
		throw new EarlabException("WaveFile: Bits per sample must be 8, 16 or 24 ONLY");
		break;
	}
}

short FormatChunk::GetBitsPerSample(void)
{
	return BitsPerSample;
}

void FormatChunk::UpdateFields(void)
{
	BlockAlign = (BitsPerSample / 8) * NumChannels;
	ByteRate_BPS = BlockAlign * SampleRate_Hz;
}
