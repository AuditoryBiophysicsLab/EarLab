#include "FPException.h"

void UnmaskFPStackException(void)
{
	__asm	FNINIT;
	_clear87();
	_control87(_EM_INVALID, _MCW_EM);
}