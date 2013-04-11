#include "Earlab.h"
#include "EarlabException.h"
#include "WaveFile.h"
#include <iostream>
#include <fstream>
#include <memory.h>
#include <stdlib.h>


WaveFile::WaveFile()
{
}

WaveFile::WaveFile(char *WaveFileName)
{
	Open(WaveFileName);
}

void WaveFile::Open(char *WaveFileName)
{
	theFile.open(WaveFileName, ios::in);

	ReadWaveHeader(theFile);
}

void WaveFile::SaveAs(char *WaveFileName)
{
}

unsigned short WaveFile::GetChannelCount(void)
{
	return fmt.GetNumChannels();
}

void WaveFile::SetChannelCount(unsigned short ChannelCount)
{
	fmt.SetNumChannels(ChannelCount);
}

unsigned long WaveFile::GetSampleRate_Hz(void)
{
	return fmt.GetSampleRate_Hz();
}

void WaveFile::SetSampleRate_Hz(unsigned long SampleRate_Hz)
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

unsigned long WaveFile::GetBytesPerSecond(void)
{
	return fmt.GetBytesPerSecond();
}

unsigned short WaveFile::GetBlockAlignment_Bytes(void)
{
	return fmt.GetBlockAlignment();
}

void WaveFile::ReadWaveHeader(fstream &theFile)
{
	riff.Read(theFile);
	fmt.Read(theFile);
	data.Read(theFile);
}

void WaveFile::WriteWaveHeader(fstream &theFile)
{
}

void WaveFile::ReadNextSample(fstream &theFile)
{
}

void WaveFile::WriteNextSample(fstream &theFile)
{
}

Chunk::Chunk()
{
	ChunkSize = 0;
	memset(ChunkID, 0, sizeof(ChunkID));
}

Chunk::Chunk(fstream &InputFile)
{
	Read(InputFile);
}

void Chunk::Read(fstream &InputFile)
{
	InputFile >> ChunkID >> ChunkSize;
}

void Chunk::Write(fstream &OutputFile)
{
}

char *Chunk::GetChunkID(void)
{
	return ChunkID;
}

void Chunk::SetChunkID(char *NewChunkID)
{
	strncpy(ChunkID, NewChunkID, 4);
}

RiffChunk::RiffChunk()
{
	memset(FormatID, 0, sizeof(FormatID));
}

RiffChunk::RiffChunk(fstream &InputFile)
{
	Read(InputFile);
}

char *RiffChunk::GetFormatID(void)
{
	return FormatID;
}

void RiffChunk::SetFormatID(char *NewFormatID)
{
	strncpy(FormatID, NewFormatID, 4);
}

void RiffChunk::Read(fstream &InputFile)
{
	Chunk::Read(InputFile);
	InputFile >> FormatID;
}

void RiffChunk::Write(fstream &OutputFile)
{
}

FormatChunk::FormatChunk()
{
	AudioFormat = 1;
	NumChannels = 0;
	SampleRate_Hz = 0;
	ByteRate_BPS = 0;
	BlockAlign = 0;
	BitsPerSample = 0;
}

FormatChunk::FormatChunk(fstream &InputFile)
{
	Read(InputFile);
}

void FormatChunk::Read(fstream &InputFile)
{
	InputFile >> AudioFormat >> NumChannels >> SampleRate_Hz >> ByteRate_BPS >> BlockAlign >> BitsPerSample;
}

void FormatChunk::Write(fstream &OutputFile)
{
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
