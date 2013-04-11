#ifndef __EARLAB_H_INCLUDED
#define __EARLAB_H_INCLUDED

const int EarlabMaxIOStreamCount = 100;					// Maximum number of inputs or outputs for each module
const int EarlabMaxIOStreamDimensions = 10;				// Maximum number of dimensions for each input or output
const int EarlabMaxParametersPerModule = 100;			// Maximum number of parameters per module

const double PI = 3.1415926535897932384626433832795;	// We all know what this is, don't we?
const double TWOPI = PI * 2;
const double SQRT2 = 1.4142135623730950488016887242097;	// Square root of two
const double E = 2.71828182845904523536028747135266;	// Root of the natual logarithm, of course

const float fPI = (float)PI;							// Float versions of above constants
const float fTWOPI = (float)TWOPI;
const float fSQRT2 = (float)SQRT2;
const float fE = (float)E;
                      
const int MaxEarlabModuleCount = 50;					// Maximum number of modules permitted in a simulation in the Desktop Earlab environment

#ifdef _WIN32
#undef  _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#undef  _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 1
#pragma warning(disable : 4996)
#endif

#endif
