#ifndef __WAVE_FILE_H_INCLUDED
#define __WAVE_FILE_H_INCLUDED

#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

class WaveSample
{
public:
	WaveSample(unsigned char Sample) {SampleBits = 8; SampleData.Sample8 = Sample;}
	WaveSample(short Sample) {SampleBits = 16; SampleData.Sample16 = Sample;}
	WaveSample(long Sample) {SampleBits = 24; SampleData.Sample24 = Sample;}

	void Read(fstream &TheFile);

	long GetData();
private:
	int SampleBits;
	union
	{
		unsigned char Sample8;
		short Sample16;
		long Sample24;
	} SampleData;
};

class Chunk
{
public:
	Chunk();
	Chunk(fstream &InputFile);

	virtual void Read(fstream &InputFile);
	virtual void Write(fstream &OutputFile);

	char *GetChunkID(void);
	void SetChunkID(char *NewChunkID);

	int ChunkSize;

private:
	char ChunkID[4];
};

class RiffChunk : Chunk
{
public:
	RiffChunk();
	RiffChunk(fstream &InputFile);

	virtual void Read(fstream &InputFile);
	virtual void Write(fstream &OutputFile);

	char *GetFormatID(void);
	void SetFormatID(char *NewFormatID);

private:
	char FormatID[4];	// Must be 'WAVE'
};

class FormatChunk : Chunk
{
public:
	FormatChunk();
	FormatChunk(fstream &InputFile);

	virtual void Read(fstream &InputFile);
	virtual void Write(fstream &OutputFile);

	void SetNumChannels(short NumChannels);
	short GetNumChannels(void);

	void SetSampleRate_Hz(int SampleRate_Hz);
	int GetSampleRate_Hz(void);

	void SetBitsPerSample(short BitsPerSample);
	short GetBitsPerSample(void);

	inline int GetBytesPerSecond(void) {return ByteRate_BPS;}
	inline short GetBlockAlignment(void) {return BlockAlign;}
	inline short GetAudioFormatCode(void) {return AudioFormat;}

private:
	void UpdateFields(void);

	short AudioFormat;		// PCM = 1, other values indicate some form of compression, which is not supported at the present time by these classes
	short NumChannels;		// 1 = Mono, 2 = Stereo, etc.
	int SampleRate_Hz;		// Sample rate in samples per channel per second
	int ByteRate_BPS;		// Data rate in bytes per second == SampleRate_Hz * NumChannels * BitsPerSample/8
	short BlockAlign;		// Number of bytes for one sample across all channels
	short BitsPerSample;	// Number of bits in each sample for each channel
};

class WaveFile
{
public:
	WaveFile();
	WaveFile(char *WaveFileName);
	~WaveFile();

	void Open(char *WaveFileName);
	void SaveAs(char *WaveFileName);

	// Channel count
	unsigned short GetChannelCount(void);
	void SetChannelCount(unsigned short ChannelCount);

	// Sample rate, in Hertz
	unsigned long GetSampleRate_Hz(void);
	void SetSampleRate_Hz(unsigned long SampleRate_Hz);

	// Bits per sample
	unsigned short GetBitsPerSample(void);
	void SetBitsPerSample(unsigned short BitsPerSample);

	// Read-only parameters: Bytes per Second and Block Alignment, in Bytes
	unsigned long GetBytesPerSecond(void);
	unsigned short GetBlockAlignment_Bytes(void);

private:
	void ReadWaveHeader(fstream &theFile);
	void WriteWaveHeader(fstream &theFile);
	void ReadNextSample(fstream &theFile);
	void WriteNextSample(fstream &theFile);

	fstream theFile;

	RiffChunk riff;
	FormatChunk fmt;
	Chunk data;


};

#endif //__WAVE_FILE_H_INCLUDED
