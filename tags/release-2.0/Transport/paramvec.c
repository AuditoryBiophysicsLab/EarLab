
#ifndef XCONCAT2
# define XCONCAT2(a,b) a ## b
#endif

#ifndef CONCAT2
# define CONCAT2(a,b)  XCONCAT2(a, b)
#endif

#ifndef XCONCAT3
# define XCONCAT3(a,b,c) a ## b  ## c
#endif

#ifndef CONCAT3
# define CONCAT3(a,b,c)  XCONCAT3(a, b, c)
#endif

#ifndef PTYPE_int
#define PTYPE_int    PTYPE_INT
#define PTYPE_bool   PTYPE_BOOL
#define PTYPE_float  PTYPE_FLOAT
#define PTYPE_double PTYPE_DOUBLE
#define PTYPE_string PTYPE_STRING
#endif

#define VECTYPE CONCAT2(PTYPE_,VTYPE)            /* Parameter type */
#define VSCAN   CONCAT3(scan_,VTYPE,vec)         /* Vector scan routine name */
#define SSCAN   CONCAT2(scan_,VTYPE)             /* Corresponding scaler scanner */
#define VECVAL  CONCAT2(value.data.vvec.vec,VTYPE) /* Vector selector */
#define SCLVAL  CONCAT2(value.data.v,VTYPE)        /* Scalar selector */

INTERNAL int
VSCAN(char **bpp, PARAM *param) {
	char   *s = *bpp;
	PARAM   param1;
	VTYPE  *vec;
	int     n, nalloc, inc;
	int     rc;

	while (*s && iswhite(*s)) ++s;
	if (*s != '[') return(0);
	++s;

	nalloc = inc = 10;
	vec = (VTYPE *) malloc(nalloc*sizeof(VTYPE));
	n = 0;
	FOREVER {
		while (*s && iswhite(*s)) ++s;
		if (!*s || *s == ']') break;

		rc = SSCAN(&s, &param1);
		if (!rc) {
			printf("vecscan: bad datum at elem %d\n", n);
			return(0);
		}

		if (n+1 == nalloc) {
			inc    *= 2;
			nalloc += inc;
			vec = (VTYPE *)realloc(vec, nalloc*sizeof(VTYPE));
			if (!vec) return(0);
		}
		vec[n++] = param1.SCLVAL;
	}
	param->VECVAL      = vec;
	param->value.count = n;
	param->value.type  = VECTYPE;

	*bpp = s;
	return(1);
}

#undef VECTYPE
#undef VSCAN
#undef SSCAN
#undef VECVAL
#undef SCLVAL

