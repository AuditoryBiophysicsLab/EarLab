#ifndef _CL_H
# define _CL_H

#ifdef CONTROL_LAYER
# define Global EXPORT
#else
# define Global IMPORT
#endif

#define MAX_MODULES   100
#define MAX_ARGS      8
#define MAX_DIM       EFI_MAXDIM

#define NSAMPLE   0
#define NCHANNEL  1
#define NDELAY    2
#define NREP      3

Global int  define_module(char *name, char *module, int ninput,  int indim[],  int *intype, int noutput, int outdim[], int *outtype);
Global int  lookup_module(char *name);
Global int  lookup_module_client(int clientid);
Global void wire_module(int source, int output, int sink, int input);
Global int  config_module_input (int id, int arg, int type, int ndim, int rank[]);
Global int  config_module_output(int id, int arg, int type, int ndim, int rank[]);
Global int  set_parameters_module(int id, char *file, char *section);

#undef Global

#endif
