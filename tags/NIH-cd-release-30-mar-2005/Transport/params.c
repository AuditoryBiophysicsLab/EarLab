/*
 * Earlab module parameter functions
 */

#include <stdio.h>
#include <local/std.h>

#include "params.h"

void WriteParameterFile(FILE *f);
int  WriteParameterFileName(char *filename);
void PrintParameterFile(void);
void SetParameters(void);

INTERNAL int Debug    = 0;
INTERNAL int HashInit = 0;

INTERNAL int  ParseParameter(char *section, char *buf);
INTERNAL int  vec_select(PARAM_VALUE *pval, int i, PARAM_VALUE *elem);
INTERNAL void param_spec(char *spec, PARAM *param);
INTERNAL void print_param(PARAM *param);
INTERNAL void sprint_param(char *s, PARAM *param);
INTERNAL void sprint_pvalue(char *s, PARAM_VALUE *pval);
INTERNAL void print_scalarvalue(PARAM_VALUE *pval);
INTERNAL void fprint_scalarvalue(FILE *f, PARAM_VALUE *pval);
INTERNAL void sprint_scalarvalue(char *s, PARAM_VALUE *pval);

INTERNAL PARAM *hashnext(void);

#ifdef TEST
main(int argc, char *argv[]) {
	int   i, rc;
	PARAM param, *pp, **plist;
	PARAM_VALUE pval;
	int   x;
	char  str[20];

	if (argc < 2) {
		printf("usage: %s filename\n", argv[0]);
		exit(1);
	}

	printf("reading parameter file %s\n", argv[1]);
	rc = ReadParameterFile(argv[1]);

	rc = GetSectionParameter("Right_AuditoryNerve", "SampleRate_Hz", &param);
	if (rc) print_param(&param), printf("\n");
	else    printf("Right_AuditoryNerve.SampleRate_Hz not found\n");

	SetPval(&pval, PTYPE_INT, (PVAL)5);
	SetParameterValue("test.foo", &pval);

	rc = GetSectionParameterInt("test", "foo", &x);
	if (rc) printf("test.foo = %d\n", x);
	else    printf("test.foo not found\n");

	rc = GetSectionParameterString("test", "foo", str);
	if (rc) printf("test.foo = \"%s\"\n", str);
	else    printf("test.foo not found\n");

	printf("---------------- hashed parameters ---------------- \n");
	rc = hashselect(NULL);
	printf("Number of parameters = %d\n", rc);

	SetParameters();
}
#endif


/*
 * PARAM Type lookup table
 */
static struct {
	char *name; int type;
} PtypeTable[] = {
  { "NONE",          PTYPE_ERROR},
  { "int",           PTYPE_INT},
  { "bool",          PTYPE_BOOL},
  { "float",         PTYPE_FLOAT},
  { "double",        PTYPE_DOUBLE},
  { "string",        PTYPE_STRING},
  { "IntVector",     PTYPE_INTVEC},
  { "BoolVector",    PTYPE_BOOLVEC},
  { "FloatVector",   PTYPE_FLOATVEC},
  { "DoubleVector",  PTYPE_DOUBLEVEC},
  { "StringVector",  PTYPE_STRINGVEC},
  { NULL, 0},
};

/*
 * Module variables
 */
static int   PtypeSize[PTYPE_MAX+1];    /* Size in bytes of each type */
static char *PnameTable[PTYPE_MAX+1];   /* Type names (as strings)    */

INTERNAL void splitname(char *s, char *section, char *name);
INTERNAL void trimnl(char *s);

INTERNAL int scan_int(char **bpp,       PARAM *param);
INTERNAL int scan_bool(char **bpp,      PARAM *param);
INTERNAL int scan_float(char **bpp,     PARAM *param);
INTERNAL int scan_double(char **bpp,    PARAM *param);
INTERNAL int scan_string(char **bpp,    PARAM *param);
INTERNAL int scan_intvec(char **bpp,    PARAM *param);
INTERNAL int scan_boolvec(char **bpp,   PARAM *param);
INTERNAL int scan_floatvec(char **bpp,  PARAM *param);
INTERNAL int scan_doublevec(char **bpp, PARAM *param);
INTERNAL int scan_stringvec(char **bpp, PARAM *param);

#define SPECSIZE 64*1024

/*********************************** HASHER **********************************/
#define SCOPE     INTERNAL
#define TYPE      PARAM
#define KEY(r)    (r).name
#define KEYTYPE   char *
#define EQ(k1,k2) (strcmp(k1,k2)==0)
#define HASH(key) hash_key(key)
#define HASHSIZE  256

INTERNAL
hash_key(s)                       /* Make a hash key */
 FAST char *s;
 {
    FAST int key;

    key = 0;
    while (*s) key = key + (*s++);
    return(key & (HASHSIZE-1));
}

#include <local/generic/hash.c>

#undef SCOPE
#undef TYPE
#undef KEY
#undef KEYTYPE
#undef EQ
#undef HASH
#undef HASHSIZE
/*********************************** HASHER **********************************/

/*********************************** SORTER **********************************/
#define SCOPE     INTERNAL
#define TYPE      PARAM *
#define KEY(r)    ((*r)->name)
#define KEYTYPE   char *
#define INORDER(k1,k2) (strcmp(k1,k2)<=0)

#include <local/generic/qsort.c>

#undef SCOPE
#undef TYPE
#undef KEY
#undef KEYTYPE
#undef INORDER
/*********************************** SORTER **********************************/


ReadParameterFile(char *file) {
	FILE *f;
	char buf[4000], *bp, section[200], *s;
	int  lineno = 0, errs = 0;
	int  notdone, gotbuf;
	int  i, rc;

	/* Initialize hash tables if not already done */
	if (!HashInit) {
		hashinit();
		HashInit = 1;

		PnameTable[PTYPE_ERROR]     = "*error*";
		PnameTable[PTYPE_INT]       = "int";
		PnameTable[PTYPE_BOOL]      = "bool";
		PnameTable[PTYPE_FLOAT]     = "float";
		PnameTable[PTYPE_DOUBLE]    = "double";
		PnameTable[PTYPE_STRING]    = "string";
		PnameTable[PTYPE_INTVEC]    = "intvec";
		PnameTable[PTYPE_BOOLVEC]   = "boolvec";
		PnameTable[PTYPE_FLOATVEC]  = "floatvec";
		PnameTable[PTYPE_DOUBLEVEC] = "doublevec";
		PnameTable[PTYPE_STRINGVEC] = "stringvec";
		PnameTable[PTYPE_STRUCT]    = "struct";

		PtypeSize[PTYPE_ERROR]      = 4;
		PtypeSize[PTYPE_INT]        = 4;
		PtypeSize[PTYPE_BOOL]       = 4;
		PtypeSize[PTYPE_FLOAT]      = 4;
		PtypeSize[PTYPE_DOUBLE]     = 8;
		PtypeSize[PTYPE_STRING]     = 0;
		PtypeSize[PTYPE_INTVEC]     = 4;
		PtypeSize[PTYPE_BOOLVEC]    = 4;
		PtypeSize[PTYPE_FLOATVEC]   = 4;
		PtypeSize[PTYPE_DOUBLEVEC]  = 8;
		PtypeSize[PTYPE_STRINGVEC]  = 0;
		PtypeSize[PTYPE_STRUCT]     = 0;
	}

	if (!(f = fopen(file, "r"))) {
		fprintf(stderr,
			"ReadParameterFile: couldn't open file \"%s\"\n",
			file);
		return(0);
	}

	gotbuf  = 0;
	notdone = 1;
	while (notdone) {
		/* Read a new buffer if we don't have one */
		if (!gotbuf) {
			if (!fgets(buf, sizeof(buf), f)) break;
			trimnl(buf);

			++lineno;
			bp = buf;
		}

		/* Skip white space */
		while (*bp && iswhite(*bp)) ++bp;

		if (Debug > 1)
		   printf("processing line %d%s: '%s'\n",
			   lineno,
			   substr("//",bp)?" (comment)":"",
			   bp),fflush(stdout);

		gotbuf = 0;

		/* Skip blank lines and comments */
		if (*bp == '\0' || substr("//", bp)) continue;

		/* Check to make sure we are starting a section */
		if (*bp != '[') {
			if (Debug)
			   printf("rpf: ignoring line '%s'\n", bp);
			continue;
		}

		/* Grab and stash section name */
		s = section;
		while (*++bp && *bp != ']') *s++ = *bp;
		*s = '\0';

		if (Debug) printf("Beginning section %s\n", section), fflush(stdout);

		/* Now read and process section parameters */
		while (notdone = (fgets(buf, sizeof(buf), f) != NULL)) {
			trimnl(buf);

			++lineno;

			bp = buf;
			while (*bp && iswhite(*bp)) ++bp;

			/* Skip blank lines and comments */
			if (!*bp || substr("//", bp)) continue;

			/* Check for beginning of next section */
			if (*bp == '[') {
				gotbuf = 1;
				break;
			}

			rc = ParseParameter(section, bp);
			if (!rc) {
				printf("rpf: error at line %d: '%s'\n", lineno, buf);
				++errs;
			}
		}
		if (Debug) printf("Ending section %s\n", section);
	}
}

INTERNAL int
ParseParameter(char *section, char *buf) {
	char *bp = buf, *s;
	char  type[100], name[400], value[4000];
	int   typeid;
	PARAM param;
	int   i, ok;

	/* Skip white skip and ignore comments */
	while (*bp && iswhite(*bp)) ++bp;
	if (*bp == '\0' || substr("//", bp)) return(1);

	/*****************/
	/* VARIABLE TYPE */
	/*****************/
	s = type;
	while (*bp && !iswhite(*bp)) *s++ = *bp++;
	*s = '\0';

	/* Look up type id */
	typeid = PTYPE_ERROR;
	for (i = 0; PtypeTable[i].name; ++i)
	  if (strimatch(PtypeTable[i].name, type)) {
		typeid = PtypeTable[i].type;
		break;
	  }
	if (typeid == PTYPE_ERROR) {
		printf("rpf: bad data type \"%s\"\n", type);
		return(0);
	}

	/*****************/
	/* VARIABLE NAME */
	/*****************/

	/* Skip white space */
	while (*bp && iswhite(*bp)) ++bp;

	/* Check for end-of-line, raise error */
	if (!*bp || substr("//", bp)) {
		printf("rpf: Variable name missing\n");
		return(0);
	}

	/* Extract name */
	s = name;
	while (*bp && !iswhite(*bp)) *s++ = *bp++;
	*s = '\0';

	/******************/
	/* VARIABLE VALUE */
	/******************/

	while (*bp && iswhite(*bp)) ++bp;

	/* Check for end-of-line, raise error */
	if (!*bp || substr("//", bp)) {
		printf("rpf: Variable value missing\n");
		return(0);
	}

	switch (typeid) {
		case PTYPE_INT:
			ok = scan_int(&bp, &param);
			break;

		case PTYPE_BOOL:
			ok = scan_bool(&bp, &param);
			break;

		case PTYPE_FLOAT:
			ok = scan_float(&bp, &param);
			break;

		case PTYPE_DOUBLE:
			ok = scan_double(&bp, &param);
			break;

		case PTYPE_STRING:
			ok = scan_string(&bp, &param);
			break;

		case PTYPE_INTVEC:
			ok = scan_intvec(&bp, &param);
			break;

		case PTYPE_BOOLVEC:
			ok = scan_boolvec(&bp, &param);
			break;

		case PTYPE_FLOATVEC:
			ok = scan_floatvec(&bp, &param);
			break;

		case PTYPE_DOUBLEVEC:
			ok = scan_doublevec(&bp, &param);
			break;

		case PTYPE_STRINGVEC:
			ok = scan_stringvec(&bp, &param);
			break;

		default:
			printf("rfp: invalid type id %d\n", typeid);
			exit(1);
	}

	if (!ok) {
		printf("rpf: illegal data value type for \"%s\"\n", type);
		return(0);
	}

	param.name = (char *) malloc(strlen(section)+strlen(name)+2);
	if (!param.name) {
		printf("rpf: couldn't allocate name space for \"%s.%s\"\n",
			section, name);
		return(0);
	}
	sprintf(param.name, "%s.%s", section, name);

	if (Debug) {
		printf("Adding variable: ");
		print_param(&param);
		printf("\n");
	}
	hashmod(param);
	return(1);
}

INTERNAL int
scan_string(char **bpp, PARAM *param) {
	char *s = *bpp;
	char buf[4000], *t;
	int  n;

	while (*s && iswhite(*s)) ++s;
	if (!*s) return(0);

	n = 0;
	t = buf;
	if (*s == '"') ++s;
	while (*s && *s != '"' && ++n) *t++ = *s++;
	*t = '\0';

	s = (char *)malloc(n+1);
	if (!s) {
		printf("scan_str: couldn't allocate string space for \"%s\"\n", t);
		return(0);
	}
	strcpy(s, buf);

	param->value.data.vstring  = s;
	param->value.count         = PTYPE_SCALAR;
	param->value.type          = PTYPE_STRING;

	*bpp = s;
	return(1);
}

INTERNAL int
scan_int(char **bpp, PARAM *param) {
	char *s = *bpp;
	int v, sign = 1;

	if (*s == '+') ++s;
	else if (*s == '-') { sign = -1; ++s; }

	if (!isdigit(*s)) return(0);
	v = 0;
	while (*s && isdigit(*s)) v = 10*v + *s++-'0';

	param->value.data.vint = v * sign;
	param->value.count     = PTYPE_SCALAR;
	param->value.type      = PTYPE_INT;

	*bpp = s;
	return(1);
}

INTERNAL int
scan_bool(char **bpp, PARAM *param) {
	char *s = *bpp;
	int v;

	if (isdigit(*s)) {
	   if (*s == '0') v = 0;
	   else           v = 1;
	}
	else if (strimatch("f*", s)) v = 0;
	else if (strimatch("t*", s)) v = 1;
	else {
		fprintf(stderr, "scan_bool: ***ERROR*** invalid value %s\n", s);
		return(0);
	}

	param->value.data.vint = v;
	param->value.count     = PTYPE_SCALAR;
	param->value.type      = PTYPE_BOOL;

	*bpp = s;
	return(1);
}

INTERNAL int
scan_float(char **bpp, PARAM *param) {
	char *s = *bpp;
	char   *start = s;
	double v;

	/* Verify we have something that looks like a valid double */
	if (*s == '+' || *s == '-' || *s == '.') ++s;
	if (!isdigit(*s)) return(0);

	sscanf(start, " %lf", &v);

	param->value.data.vfloat = v;
	param->value.count       = PTYPE_SCALAR;
	param->value.type        = PTYPE_FLOAT;

	*bpp = s;
	return(1);
}

INTERNAL int
scan_double(char **bpp, PARAM *param) {
	char   *s     = *bpp;
	char   *start = s;
	double v;

	/* Verify we have something that looks like a valid double */
	if (*s == '+' || *s == '-' || *s == '.') ++s;
	if (!isdigit(*s)) return(0);

	sscanf(start, " %lf", &v);

	param->value.data.vdouble = v;
	param->value.count        = PTYPE_SCALAR;
	param->value.type         = PTYPE_DOUBLE;

	*bpp = s;
	return(1);
}

#define XCONCAT2(a,b) a ## b
#define CONCAT2(a,b)  XCONCAT2(a, b)
#define XCONCAT3(a,b,c) a ## b  ## c
#define CONCAT3(a,b,c)  XCONCAT3(a, b, c)

/***************************************************************************/
/* INT vector scan                                                         */
/*                                                                         */
#define VTYPE   int

#include "paramvec.c"

#undef VTYPE

/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/* BOOL vector scan                                                        */
/*                                                                         */
typedef int bool;
#define VTYPE   bool

#include "paramvec.c"

#undef VTYPE

/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/* FLOAT vector scan                                                       */
/*                                                                         */
#define VTYPE   float

#include "paramvec.c"

#undef VTYPE
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/* DOUBLE vector scan                                                      */
/*                                                                         */
#define VTYPE   double

#include "paramvec.c"

#undef VTYPE
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/* STRING vector scan                                                      */
/*                                                                         */
typedef char *string;

#define VTYPE   string

#include "paramvec.c"

#undef VTYPE
/*                                                                         */
/***************************************************************************/

INTERNAL void
trimnl(char *s) {
	if (!s || !*s) return;

	while (*s) ++s;
	if (*--s == '\n') *s = '\0';
}

INTERNAL void
splitname(char *s, char *section, char *name) {
	if (!s || !*s) return;

	while (*s && *s != '.') *section++ = *s++;
	*section = '\0';

	if (*s == '.') ++s;
	while (*s) *name++ = *s++;
	*name = '\0';
}

int
WriteParameterFileName(char *filename) {
	FILE *f;

	f = fopen(filename, "w");
	if (!f) return(0);
	WriteParameterFile(f);
	fclose(f);
}

void
PrintParameterFile(void) {
	WriteParameterFile(stdout);
}

void
WriteParameterFile(FILE *f) {
	int     i, n, len;
	PARAM **plist, *pp;
	char    buf[SPECSIZE];
	char    section[500], last[500], name[500];

	n = hashselect(NULL);

	plist = (PARAM **)malloc(n*sizeof(PARAM *));
	i = 0;
	while (pp = hashnext()) {
		plist[i++] = pp;
	}
	qsort(&plist[0], &plist[i-1]);
	strcpy(last, "*NONE*");

	for (i = 0; i < n; ++i) {
		pp = plist[i];
		splitname(pp->name, section, name);
		if (!streq(section, last)) {
			fprintf(f, "[%s]\n", section);
			strcpy(last, section);
		}
		fprintf(f, "%s\t%s", PtypeTable[pp->value.type].name, name);
		len = strlen(name);
		while (len++ < 23) fprintf(f, " ");
		fprintf(f, " ");
		sprint_pvalue(buf, &pp->value);
		fprintf(f, "%s\n", buf);
	}
}

void
SetParameters(void) {
	int     i, n, len;
	PARAM **plist, *pp;
	char    buf[SPECSIZE];

	n = hashselect(NULL);

	plist = (PARAM **)malloc(n*sizeof(PARAM *));
	i = 0;
	while (pp = hashnext()) {
		plist[i++] = pp;
	}
	qsort(&plist[0], &plist[i-1]);

	for (i = 0; i < n; ++i) {
		pp = plist[i];

		sprint_param(buf, pp);
		printf("%s\n", buf);
	}
}

INTERNAL int
vec_select(PARAM_VALUE *pval, int i, PARAM_VALUE *elem) {
	if (i < 0 || i >= pval->count) return(0);

	switch (pval->type) {
	 case PTYPE_INT:
		elem->data.vint = pval->data.vvec.vecint[i];
		break;
	 case PTYPE_BOOL:
		elem->data.vint = pval->data.vvec.vecint[i];
		break;
	 case PTYPE_STRING:
		elem->data.vstring = pval->data.vvec.vecstring[i];
		break;
	 case PTYPE_FLOAT:
		elem->data.vfloat = pval->data.vvec.vecfloat[i];
		break;
	 case PTYPE_DOUBLE:
		elem->data.vdouble = pval->data.vvec.vecdouble[i];
		break;
	 default:
		return(0);
	}
	return(1);
}

INTERNAL void
sprint_param(char *s, PARAM *param) {
	char buf[SPECSIZE];

	sprintf(s, "%s %s ", PtypeTable[param->value.type].name, param->name);
	sprint_pvalue(buf, &param->value);
	strcat(s, buf);
}

INTERNAL void
sprint_pvalue(char *s, PARAM_VALUE *pval) {
	int         i;
	PARAM_VALUE elem;
	char        buf[SPECSIZE];

	if (pval->count == 0) {
		sprint_scalarvalue(s, pval);
		return;
	}

	elem = *pval;
	elem.count = 0;

	strcpy(s, "[");
	buf[0] = ' ';
	for (i = 0; i < pval->count; ++i) {
		vec_select(pval, i, &elem);
		sprint_scalarvalue(buf+1, &elem);
		strcat(s, buf);
	}
	strcat(s, " ]");
}

INTERNAL void
print_param(PARAM *param) {
	int         i;
	PARAM_VALUE elem;

	printf("%s (%s", param->name, PnameTable[param->value.type]);

	if (param->value.count) printf(" [%d]", param->value.count);

	printf(") = ");

	if (param->value.count == 0) print_scalarvalue(&param->value);
	else {
		elem = param->value;
		elem.count = 0;

		printf("[");
		for (i = 0; i < param->value.count; ++i) {
			printf(" ");
			vec_select(&param->value, i, &elem);
			print_scalarvalue(&elem);
		}
		printf(" ]");
	}
}


INTERNAL void
print_scalarvalue(PARAM_VALUE *pval) {
	fprint_scalarvalue(stdout, pval);
}

INTERNAL void
fprint_scalarvalue(FILE *f, PARAM_VALUE *pval) {
	char buf[1000];

	sprint_scalarvalue(buf, pval);
	fprintf(f, "%s", buf);
}

INTERNAL void
sprint_scalarvalue(char *s, PARAM_VALUE *pval) {
	switch (pval->type) {
	 case PTYPE_ERROR:
		sprintf(s, "*****"); break;
	 case PTYPE_INT:
		sprintf(s, "%d", pval->data.vint);
		break;
	 case PTYPE_BOOL:
		sprintf(s, "%s", pval->data.vint?"true":"false");
		break;
	 case PTYPE_STRING:
		sprintf(s, "\"%s\"", pval->data.vstring);
		break;
	 case PTYPE_FLOAT:
		sprintf(s, "%f", pval->data.vfloat);
		break;
	 case PTYPE_DOUBLE:
		sprintf(s, "%f", pval->data.vdouble);
		break;
	 default:
		printf("unknown or nonscalar  type %d", pval->type);
	}
}

ReadParameters(char *file, char *section) {

}

/*
 * Set a PVAL to a given type/value
 */
SetPval(PARAM_VALUE *pval, int type, PVAL val) {
	switch (type) {
	   case PTYPE_INT:
	   case PTYPE_BOOL:
	   case PTYPE_FLOAT:
	   case PTYPE_DOUBLE:
		pval->type  = type;
		pval->count = 0;
		pval->data  = val;
		break;

	   case PTYPE_STRING:
		pval->type  = type;
		pval->count = 0;
		pval->data.vstring = (char *)malloc(strlen(val.vstring)+1);
		if (pval->data.vstring == NULL) {
			fprintf(stderr,
				"SetPval: couldn't allocate string storage for pval %s\n",
				 val.vstring);
			return(0);
		}
		strcpy(pval->data.vstring, val.vstring);
		break;

	   case PTYPE_INTVEC:
	   case PTYPE_BOOLVEC:
	   case PTYPE_FLOATVEC:
	   case PTYPE_DOUBLEVEC:
	   case PTYPE_STRINGVEC:
		fprintf(stderr,
			"SetPval: don't do vectors yet\n");
		return(0);
		break;

	   default:
		fprintf(stderr,
			"SetPval: didn't recognize type %d\n", type);
			return(0);
	}
	return(1);

}

/*
 * Get PVAL of  a given type/value
 */
GetPval(PARAM_VALUE *pval, int type, PVAL *val) {
	switch (type) {
	   case PTYPE_INT:
	   case PTYPE_BOOL:
	   case PTYPE_FLOAT:
	   case PTYPE_DOUBLE:
		memcpy(val, &pval->data, PtypeSize[type]);
		break;

	   case PTYPE_STRING:
		strcpy(val->vstring, pval->data.vstring);
		break;

	   case PTYPE_INTVEC:
	   case PTYPE_BOOLVEC:
	   case PTYPE_FLOATVEC:
	   case PTYPE_DOUBLEVEC:
		memcpy((void *)val->vvec.vecint, (void *)pval->data.vvec.vecint,
		       pval->count*PtypeSize[type]);
		break;

	   case PTYPE_STRINGVEC:

	   default:
		fprintf(stderr,
			"GetPval: didn't recognize type %d\n", type);
			return(0);
	}
	return(1);

}

GetParameterInt(char *name, int *retval) {
	int         rc;
	PARAM_VALUE pval;

	rc = GetParmeterValue(name, &pval);

	if (!rc) return(0);
	if (pval.count > 1) return(0);
	switch (pval.type) {
	  case PTYPE_INT:
	  case PTYPE_BOOL:
		*retval = pval.data.vint;
		return(1);

	  case PTYPE_FLOAT:
		*retval = (int)pval.data.vfloat;
		return(1);

	  case PTYPE_DOUBLE:
		*retval = (int)pval.data.vdouble;
		return(1);

	  case PTYPE_STRING:
		rc = sscanf(" %d", pval.data.vstring, retval);
		return(rc);

	  default:
		fprintf(stderr, "GetParameterX: invalid param type %d\n", pval.type);
		return(0);
	}
}

GetParameterBool(char *name, int *retval) {
	int         rc;
	int         x;
	PARAM_VALUE pval;

	rc = GetParmeterValue(name, &pval);

	if (!rc) return(0);
	if (pval.count > 1) return(0);
	switch (pval.type) {
	  case PTYPE_INT:
	  case PTYPE_BOOL:
		*retval = (pval.data.vint != 0);
		return(1);

	  case PTYPE_FLOAT:
		*retval = ((int)pval.data.vfloat != 0);
		return(1);

	  case PTYPE_DOUBLE:
		*retval = ((int)pval.data.vdouble != 0);
		return(1);

	  case PTYPE_STRING:
		rc = sscanf(" %d", pval.data.vstring, &x);
		if (!rc) return(0);
		*retval = (x != 0);
		return(1);

	  default:
		fprintf(stderr, "GetParameterX: invalid param type %d\n", pval.type);
		return(0);
	}
}

GetParameterFloat(char *name, float *retval) {
	int         rc;
	PARAM_VALUE pval;

	rc = GetParmeterValue(name, &pval);

	if (!rc) return(0);
	if (pval.count > 1) return(0);
	switch (pval.type) {
	  case PTYPE_INT:
		*retval = (float)pval.data.vint;   return(1);
	  case PTYPE_BOOL:
		*retval = (float)pval.data.vbool; return(1);
	  case PTYPE_FLOAT:
		*retval = pval.data.vfloat;        return(1);
	  case PTYPE_DOUBLE:
		*retval = pval.data.vdouble;       return(1);
	  case PTYPE_STRING:
		rc = sscanf(" %f", pval.data.vstring, retval);
		return(rc);

	  default:
		fprintf(stderr, "GetParameterX: invalid param type %d\n", pval.type);
		return(0);
	}
}

GetParameterDouble(char *name, double *retval) {
	int         rc;
	PARAM_VALUE pval;

	rc = GetParmeterValue(name, &pval);

	if (!rc) return(0);
	if (pval.count > 1) return(0);
	switch (pval.type) {
	  case PTYPE_INT:
		*retval = (float)pval.data.vint;   return(1);
	  case PTYPE_BOOL:
		*retval = (float)pval.data.vbool;  return(1);
	  case PTYPE_FLOAT:
		*retval = pval.data.vfloat;        return(1);
	  case PTYPE_DOUBLE:
		*retval = pval.data.vdouble;       return(1);
	  case PTYPE_STRING:
		rc = sscanf(" %lf", pval.data.vstring, retval);
		return(rc);

	  default:
		fprintf(stderr, "GetParameterX: invalid param type %d\n", pval.type);
		return(0);
	}
}

GetParameterString(char *name, char *retval) {
	int         rc;
	PARAM_VALUE pval;

	rc = GetParmeterValue(name, &pval);

	if (!rc) return(0);
	if (pval.count > 1) return(0);
	switch (pval.type) {
	  case PTYPE_INT:
		sprintf(retval, "%d", pval.data.vint);       return(1);
	  case PTYPE_BOOL:
		sprintf(retval, "%d", pval.data.vbool);      return(1);
	  case PTYPE_FLOAT:
		sprintf(retval, "%f", pval.data.vfloat);     return(1);
	  case PTYPE_DOUBLE:
		sprintf(retval, "%lf", pval.data.vdouble);   return(1);
	  case PTYPE_STRING:
		strcpy(retval, pval.data.vstring);           return(1);
	  default:
		fprintf(stderr, "GetParameterX: invalid param type %d\n", pval.type);
		return(0);
	}
}

GetSectionParameterInt(char *section, char *varname, int *retval) {
	char  *np, name[2000];

	if (section && *section) {
		 sprintf(name, "%s.%s", section, varname);
		 np = name;
	}
	else np = varname;

	return(GetParameterInt(np, retval));
}

GetSectionParameterBool(char *section, char *varname, int *retval) {
	char  *np, name[2000];

	if (section && *section) {
		 sprintf(name, "%s.%s", section, varname);
		 np = name;
	}
	else np = varname;

	return(GetParameterBool(np, retval));
}

GetSectionParameterFloat(char *section, char *varname, float *retval) {
	char  *np, name[2000];

	if (section && *section) {
		 sprintf(name, "%s.%s", section, varname);
		 np = name;
	}
	else np = varname;

	return(GetParameterFloat(np, retval));
}

GetSectionParameterDouble(char *section, char *varname, double *retval) {
	char  *np, name[2000];

	if (section && *section) {
		 sprintf(name, "%s.%s", section, varname);
		 np = name;
	}
	else np = varname;

	return(GetParameterDouble(np, retval));
}

GetSectionParameterString(char *section, char *varname, char *retval) {
	char  *np, name[2000];

	if (section && *section) {
		 sprintf(name, "%s.%s", section, varname);
		 np = name;
	}
	else np = varname;

	return(GetParameterString(np, retval));
}

GetParameter(PARAM *param) {
	return(hashget(param));
}

GetParmeterValue(char *name, PARAM_VALUE *rval) {
	PARAM  param;

	param.name = name;

	if (!hashget(&param)) return(0);

	*rval = param.value;

	return(1);
}

GetSectionParameter(char *section, char *varname, PARAM *param) {
	char  *np, name[2000], *tp;


	if (section && *section) {
		 sprintf(name, "%s.%s", section, varname);
		 np = name;
	}
	else np = varname;

	tp = param->name;
	param->name = np;

	if (GetParameter(param)) return(1);

	param->name = tp;
	return(0);
}

GetSectionParameterValue(char *section, char *varname, PARAM_VALUE *rval) {
	char  *np, name[2000];

	if (section && *section) {
		 sprintf(name, "%s.%s", section, varname);
		 np = name;
	}
	else np = varname;

	return(GetParmeterValue(np, rval));
}

SetParameter(PARAM *param) {
	PARAM  tmp;

	tmp.name = param->name;
	if (hashget(&tmp)) {
		tmp.value = param->value;
		if (!hashrep(tmp)) {
			fprintf(stderr,
				"SetParameter: hashrep failed on found variable %s\n",
				tmp.name);
			return(0);
		}
		printf("SetParameter: resetting variable %s\n", tmp.name);
		return(1);
	}
	tmp = *param;
	tmp.name = (char *)malloc(strlen(param->name)+1);
	if (!tmp.name) {
		fprintf(stderr,
			"SetParameter: couldn't allocate name space for %s\n",
			 param->name);
		return(0);
	}
	strcpy(tmp.name, param->name);

	if (!hashadd(tmp)) {
		fprintf(stderr,
			"SetParameter: hashadd failed on found variable %s\n",
			tmp.name);
		return(0);
	}
	printf("SetParameter: adding variable %s\n", tmp.name);
	return(1);
}

SetParameterValue(char *name, PARAM_VALUE *pval) {
	PARAM  tmp;

	tmp.name  = name;
	tmp.value = *pval;

	return(SetParameter(&tmp));
}

SetSectionParameter(char *section, char *varname, PARAM *param) {
	PARAM  tmp;
	char  *np, name[2000];

	if (section && *section) {
		sprintf(name, "%s.%s", section, varname);
		np = name;
	}
	else np = varname;

	tmp.name = np;
	if (hashget(&tmp)) {
		tmp.value = param->value;
		if (!hashrep(tmp)) return(0);

		return(1);
	}
	tmp = *param;
	tmp.name = (char *)malloc(strlen(np)+1);
	if (!tmp.name) {
		fprintf(stderr,
			"SetParameter: couldn't allocate name space for %s.%s\n",
			section, name);
		return(0);
	}

	if (!hashadd(tmp)) return(0);

	return(1);
}

SetSectionParameterValue(char *section, char *varname, PARAM_VALUE *pval) {
	PARAM  tmp;
	char  *np, name[2000];

	if (section && *section) {
		sprintf(name, "%s.%s", section, varname);
		np = name;
	}
	else np = varname;

	tmp.name = np;
	if (hashget(&tmp)) {
		tmp.value = *pval;
		if (!hashrep(tmp)) return(0);

		return(1);
	}
	tmp.value = *pval;
	tmp.name  = (char *)malloc(strlen(np)+1);
	if (!tmp.name) {
		fprintf(stderr,
			"SetParameter: couldn't allocate name space for %s.%s\n",
			section, name);
		return(0);
	}

	if (!hashadd(tmp)) return(0);

	return(1);
}


