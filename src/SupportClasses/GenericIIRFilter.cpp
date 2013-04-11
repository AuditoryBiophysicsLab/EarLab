#include "GenericIIRFilter.h"
#include <stdio.h>
#include <string.h>
#include "EarlabException.h"

GenericIIRFilter::GenericIIRFilter(int FeedforwardFilterOrder, double *FeedforwardCoefficients, int FeedbackFilterOrder, double *FeedbackCoefficients)
{
	int i;
	memset(this, 0, sizeof(GenericIIRFilter));
	if ((FeedforwardFilterOrder <= 0) || (FeedbackFilterOrder <= 0))
		throw EarlabException("GenericIIRFilter: Can't have a zero-order filter");
	mFeedforwardOrder = FeedforwardFilterOrder + 1;
	mFeedbackOrder = FeedbackFilterOrder;
	mFeedforwardCoefficents = new double[mFeedforwardOrder];
	mPastInputs = new double[mFeedforwardOrder];
	mFeedbackCoefficients = new double[mFeedbackOrder];
	mPastOutputs = new double[mFeedbackOrder];

	for (i = 0; i < mFeedforwardOrder; i++)
		mFeedforwardCoefficents[i] = FeedforwardCoefficients[i];
	for (i = 0; i < mFeedbackOrder; i++)
		mFeedbackCoefficients[i] = FeedbackCoefficients[i];
	memset(mPastInputs, 0, sizeof(double) * mFeedforwardOrder);
	memset(mPastOutputs, 0, sizeof(double) * mFeedbackOrder);
}

GenericIIRFilter::~GenericIIRFilter()
{
	if (mFeedforwardCoefficents != NULL)
		delete [] mFeedforwardCoefficents;
	if (mFeedbackCoefficients != NULL)
		delete [] mFeedbackCoefficients;
	if (mPastInputs != NULL)
		delete [] mPastInputs;
	if (mPastOutputs != NULL)
		delete [] mPastOutputs;
}

double GenericIIRFilter::Filter(double Sample)
{
	int i;
	double Result, FeedforwardTotal, FeedbackTotal;

	// Move each prior input sample one step back in time
	for (i = 0; i < mFeedforwardOrder; i++)
		mPastInputs[mFeedforwardOrder - i - 1] = mPastInputs[mFeedforwardOrder - i - 2];
	// Put the current input sample at the beginning of the list
	mPastInputs[0] = Sample;

	// Sum up the feedforward part of the filter equation
	FeedforwardTotal = 0;
	for (i = 0; i < mFeedforwardOrder; i++)
		FeedforwardTotal += mFeedforwardCoefficents[i] * mPastInputs[i];

	// Sum up the feedback part of the filter equation
	FeedbackTotal = 0;
	for (i = 0; i < mFeedforwardOrder; i++)
		FeedbackTotal += mFeedbackCoefficients[i] * mPastOutputs[i];

	// The result is the difference of the feedforward and feedback terms
	Result = FeedforwardTotal - FeedbackTotal;

	// Move each prior output sample one step back in time
	for (i = 0; i < mFeedbackOrder; i++)
		mPastOutputs[mFeedbackOrder - i - 1] = mPastOutputs[mFeedbackOrder - i - 2];
	// Put the current output sample at the beginning of the list
	mPastOutputs[0] = Result;

	return Result;
}
