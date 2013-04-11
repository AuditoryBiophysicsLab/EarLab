#ifndef __EARLAB_IO_STREAM_INFO_H_INCLUDED
#define __EARLAB_IO_STREAM_INFO_H_INCLUDED
#include "EarlabDataStream.h"
#include "XMLInfo.h"

class EarlabIOStreamInfo: public XMLInfo
{
public:
	EarlabIOStreamInfo(int Index, const char *Name, EarlabDataStreamType Type, const char *Description, const char *Constraints);
private:
	char *StreamTypeToString(EarlabDataStreamType Type);
};

#endif
