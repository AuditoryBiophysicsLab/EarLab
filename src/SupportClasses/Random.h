#ifndef __RANDOM_H_INCLUDED
#define __RANDOM_H_INCLUDED

#include "time.h"

class Random
{
public:
	Random();
	inline void SetSeed(time_t NewSeed) {seed = NewSeed;}

	inline double ran1(void) {return uniform();}
	double uniform(void);
	double gaussian(void);

private:
	static time_t seed;
	static int RandomCount;

};
#endif
