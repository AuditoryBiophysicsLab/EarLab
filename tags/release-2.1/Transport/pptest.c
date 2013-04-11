#define STRING(x) #x
#define XSTRING(x) STRING(x)

#define MODULENAME XSTRING(MODULE)

#define XCONCAT(a,b) a ## b
#define CONCAT(a,b) XCONCAT(a, b)

#define AdvanceModule CONCAT(Advance_, MODULE)
#define StartModule   CONCAT(Start_, MODULE)

#include <stdio.h>
#include <daffie/event.h>
main(int argc, char *argv) { printf("%d\n", sizeof(EVENT)); }

