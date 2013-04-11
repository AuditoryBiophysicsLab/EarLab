#include "FPException.h"
#include <float.h>
#include <stdio.h>

#ifndef NOTWINDOWS
#ifndef _WIN64
void UnmaskFPStackException(void)
{
	__asm   FNINIT;
	_clear87();
	_control87(_EM_INVALID, _MCW_EM);
}

void DisplayFPStatusBits(char *tag)
{
	unsigned int status;

	status = _statusfp();
	if (status != 0)
	{
		if (tag != NULL)
			printf("%s ", tag);
		printf("FPU Status: %08.8X ( ", status);
		if (status & _SW_DENORMAL)
			printf("DENORMAL ");
		if (status & _SW_INVALID)
			printf("INVALID ");
		if (status & _SW_ZERODIVIDE)
			printf("DIV0 ");
		if (status & _SW_OVERFLOW)
			printf("OVERFLOW ");
		if (status & _SW_UNDERFLOW)
			printf("UNDERFLOW ");
		if (status & _SW_INEXACT)
			printf("INEXACT ");
		printf(")\n");
	}
}
#endif
#endif
