#ifndef _MODEL_H
#define _MODEL_H

#ifdef MODEL
# define Global EXPORT
#else
# define Global IMPORT
#endif

Global void init_model(void);
Global void define_model(void);
Global void wire_model(void);
Global void set_parameters_model(void);

#undef Global

#endif
