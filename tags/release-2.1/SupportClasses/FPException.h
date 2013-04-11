#include "float.h"

void UnmaskFPStackException(void);
void DisplayFPStatusBits(char *tag);

#define IS_ALMOST_DENORMAL(f) (fabs(f) < 3.e-34)
