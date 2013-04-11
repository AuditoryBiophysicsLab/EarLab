#include "Earlab.h"
#include "NthOrderHighpass.h"
#include "EarlabException.h"
#include "memory.h"
#include "math.h"
#include <stdio.h>

NthOrderHighpass::NthOrderHighpass()
{
	memset(this, 0, sizeof(NthOrderHighpass));
}

NthOrderHighpass::~NthOrderHighpass()
{
	if (mFilters != NULL)
		delete [] mFilters;
}

void NthOrderHighpass::SetSampleRate_Hz(double SampleRate_Hz)
{
	mSampleRate_Hz = SampleRate_Hz;
	Init();
}

void NthOrderHighpass::SetCornerFrequency_Hz(double CornerFrequency_Hz)
{
	mCornerFrequency_Hz = CornerFrequency_Hz;
	Init();
}

void NthOrderHighpass::SetTimeConstant_mS(double TimeConstant_mS)
{
	mTimeConstant_mS = TimeConstant_mS;
	Init();
}

void NthOrderHighpass::SetFilterOrder(int FilterOrder)
{
	if (mFilterOrder != 0)
		throw EarlabException("NthOrderHighpass: Filter order has already been set to %d", mFilterOrder);
	mFilterOrder = FilterOrder;
	mFilters = new FirstOrderHighpass[mFilterOrder];
	Init();
}

void NthOrderHighpass::Init(void)
{
	int i;
	if ((mCornerFrequency_Hz != 0) && (mTimeConstant_mS != 0))
		throw EarlabException("NthOrderHighpass: Cannot set both a time constant (%f) and a corner frequency (%f)", mTimeConstant_mS, mCornerFrequency_Hz);
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

double NthOrderHighpass::Filter(double Xn)
{
	int i;

	for (i = 0; i < mFilterOrder; i++)
		Xn = mFilters[i].Filter(Xn);

	return Xn;
}

void NthOrderHighpass::ResetFilterTo(double FilterValue)
{
	int i;

	for (i = 0; i < mFilterOrder; i++)
		mFilters[i].ResetFilterTo(FilterValue);
}
