#include "Earlab.h"
#include "NthOrderLowpass.h"
#include "EarlabException.h"
#include "memory.h"
#include "math.h"
#include <stdio.h>

NthOrderLowpass::NthOrderLowpass()
{
	memset(this, 0, sizeof(NthOrderLowpass));
}

NthOrderLowpass::~NthOrderLowpass()
{
	if (mFilters != NULL)
		delete [] mFilters;
}

void NthOrderLowpass::SetSampleRate_Hz(double SampleRate_Hz)
{
	mSampleRate_Hz = SampleRate_Hz;
	Init();
}

void NthOrderLowpass::SetCornerFrequency_Hz(double CornerFrequency_Hz)
{
	mCornerFrequency_Hz = CornerFrequency_Hz;
	Init();
}

void NthOrderLowpass::SetTimeConstant_mS(double TimeConstant_mS)
{
	mTimeConstant_mS = TimeConstant_mS;
	Init();
}

void NthOrderLowpass::SetFilterOrder(int FilterOrder)
{
	if (mFilterOrder != 0)
		throw EarlabException("NthOrderLowpass: Filter order has already been set to %d", mFilterOrder);
	mFilterOrder = FilterOrder;
	mFilters = new FirstOrderLowpass[mFilterOrder];
	Init();
}

void NthOrderLowpass::Init(void)
{
	int i;
	if ((mCornerFrequency_Hz != 0) && (mTimeConstant_mS != 0))
		throw EarlabException("NthOrderLowpass: Cannot set both a time constant (%f) and a corner frequency (%f)", mTimeConstant_mS, mCornerFrequency_Hz);
	if (mFilterOrder != 0)
	{
		if ((mSampleRate_Hz != 0) && ((mCornerFrequency_Hz != 0) || (mTimeConstant_mS != 0)))
		{
			for (i = 0; i < mFilterOrder; i++)
			{
				mFilters[i].SetSampleRate_Hz(mSampleRate_Hz);
				if (mCornerFrequency_Hz != 0)
					mFilters[i].SetCornerFrequency_Hz(mCornerFrequency_Hz);
				else
					mFilters[i].SetTimeConstant_mS(mTimeConstant_mS);
			}
			mInited = true;
		}
	}
}

double NthOrderLowpass::Filter(double Xn)
{
	int i;

	for (i = 0; i < mFilterOrder; i++)
		Xn = mFilters[i].Filter(Xn);

	return Xn;
}
