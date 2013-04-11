#ifndef __GENERIC_IIR_H_INCLUDED
#define __GENERIC_IIR_H_INCLUDED

class GenericIIRFilter
{
public:
	GenericIIRFilter(int FeedforwardFilterOrder, double *FeedforwardCoefficients, int FeedbackFilterOrder, double *FeedbackCoefficients);
	~GenericIIRFilter();
	double Filter(double Sample);
private:
	int mFeedforwardOrder, mFeedbackOrder;
	double *mFeedforwardCoefficents, *mFeedbackCoefficients;
	double *mPastInputs, *mPastOutputs;
};

#endif
