#ifndef ParameterStatus_H_INCLUDED
#define ParameterStatus_H_INCLUDED
typedef enum
{
	Failure					= 0x00,
	Success					= 0x01,
	BufferOverflowSuccess	= 0x41,
	Defaulted				= 0x81,
	BufferOverflowDefaulted	= 0xC1,
} ParameterStatus;
#endif
