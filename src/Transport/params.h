/*
 * Earlab module parameter header
 */
#ifndef _PARAMS_H
#define _PARAMS_H

/*
 * PDATA type IDs
 */
#define PTYPE_ERROR     0x0000
#define PTYPE_INT       0x0001
#define PTYPE_BOOL      0x0002
#define PTYPE_FLOAT     0x0003
#define PTYPE_DOUBLE    0x0004
#define PTYPE_STRING    0x0005
#define PTYPE_VEC       0x0010
#define PTYPE_STRUCT    0x0020
#define PTYPE_INTVEC    (PTYPE_VEC|PTYPE_INT)
#define PTYPE_BOOLVEC   (PTYPE_VEC|PTYPE_BOOL)
#define PTYPE_FLOATVEC  (PTYPE_VEC|PTYPE_FLOAT)
#define PTYPE_DOUBLEVEC (PTYPE_VEC|PTYPE_DOUBLE)
#define PTYPE_STRINGVEC (PTYPE_VEC|PTYPE_STRING)
#define PTYPE_MAX       PTYPE_STRUCT

#define PTYPE_SCALAR    0

typedef union {
	char   **vecstring;
	int    *vecint;
	int    *vecbool;
	float  *vecfloat;
	double *vecdouble;
} PVEC;

typedef union {
	char   *vstring;
	int    vint;
	int    vbool;
	float  vfloat;
	double vdouble;
	PVEC   vvec;
} PVAL;

typedef struct {
	int     type;
	int     count;
	PVAL    data;
} PARAM_DEF;

typedef char *PARAM_NAME;

typedef struct {
	PARAM_NAME name;
	PARAM_DEF  value;
} PARAM;

int ReadParameterFile(char *file);
int ReadParameters(char *file, char *section);
int ParseParameter(char *section, char *buf);

int GetParameter(PARAM *param);
int GetParameterValue(char *name, PARAM_DEF *pdef);
int GetSectionParameter(char *section, char *varname, PARAM *param);
int GetSectionParameterValue(char *section, char *varname, PARAM_DEF *pdef);

int SetParameter(PARAM *param);
int SetParameterValue(char *name, PARAM_DEF *pdef);
int SetSectionParameter(char *section, char *varname, PARAM *param);
int SetSectionParameterValue(char *section, char *varname, PARAM_DEF *pdef);

int GetParameterBool  (char *name, int    *retval);
int GetParameterInt   (char *name, int    *retval);
int GetParameterFloat (char *name, float  *retval);
int GetParameterDouble(char *name, double *retval);
int GetParameterString(char *name, char   *retval);

int GetSectionParameterBool  (char *section, char *varname, int    *retval);
int GetSectionParameterInt   (char *section, char *varname, int    *retval);
int GetSectionParameterFloat (char *section, char *varname, float  *retval);
int GetSectionParameterDouble(char *section, char *varname, double *retval);
int GetSectionParameterString(char *section, char *varname, char   *retval);

int SetPval(PARAM_DEF *pdef, int type, PVAL val);

int SetParameterBool  (char *name, int    val);
int SetParameterInt   (char *name, int    val);
int SetParameterFloat (char *name, float  val);
int SetParameterDouble(char *name, double val);
int SetParameterString(char *name, char  *val);

#endif
