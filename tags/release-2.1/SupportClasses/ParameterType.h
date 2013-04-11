#ifndef ParameterType_H_INCLUDED
#define ParameterType_H_INCLUDED
typedef enum
{
	Uninitialized	=  0,
	Boolean			=  1,
	Integer			=  2,
	Double			=  3,
	String			=  4,
	IntVector		=  5,
	DoubleVector	=  6,
	Comment			= 10,
	Section			= 11,
} ParameterTypeT;
#endif
