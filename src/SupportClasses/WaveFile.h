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
	WaveSample(int Sample) {SampleBits = 24; SampleData.Sample24 = Sample;}

	void Read(fstream &TheFile);

	int GetData();
private:
	int SampleBits;
	union
	{
		unsigned char Sample8;
		short Sample16;
		int Sample24;
	} SampleData;
};

class Chunk
{
public:
	Chunk();

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

class DataChunk : Chunk
{
public:
	DataChunk();

	virtual void Read(fstream &InputFile);
	virtual void Write(fstream &OutputFile);
private:
};

class WaveBuffer : Chunk
{
public:
	WaveBuffer(int BitsPerSample, int InitialBufferSize_Samples);

	virtual void Read(fstream &InputFile);
	virtual void Write(fstream &OutputFile);

	inline float &operator()(int SampleNumber, int ChannelNumber)
	{
		return mFloatData[(SampleNumber * mNumChannels) + ChannelNumber];
	}
	
	void Normalize(void);
private:
	int &Sample(int SampleNumber, int ChannelNumber);
	int mBitsPerSample;
	int mNumChannels;
	float mMinFloatValue;
	float mMaxFloatValue;
	int mMinSampleValue;
	int mMaxSampleValue;
	int *mSampleData;
	float *mFloatData;
};

class WaveFile
{
public:
	WaveFile();
	WaveFile(char *WaveFileName);
	~WaveFile();

	void Read(char *WaveFileName);
	void Write(char *WaveFileName);

	// Channel count
	unsigned short GetChannelCount(void);
	void SetChannelCount(unsigned short ChannelCount);

	// Sample rate, in Hertz
	unsigned int GetSampleRate_Hz(void);
	void SetSampleRate_Hz(unsigned int SampleRate_Hz);

	// Bits per sample
	unsigned short GetBitsPerSample(void);
	void SetBitsPerSample(unsigned short BitsPerSample);

	// Read-only parameters: Bytes per Second and Block Alignment, in Bytes
	unsigned int GetBytesPerSecond(void);
	unsigned short GetBlockAlignment_Bytes(void);
	
	float &operator()(int SampleNumber, int ChannelNumber);

private:
	void ReadWaveHeader(void);
	void WriteWaveHeader(void);
	void ReadNextSample(void);
	void WriteNextSample(void);

	fstream theFile;

	RiffChunk riff;
	FormatChunk fmt;
	Chunk data;
};

#endif //__WAVE_FILE_H_INCLUDED
