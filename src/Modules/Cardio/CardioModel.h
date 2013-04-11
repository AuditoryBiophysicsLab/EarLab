#ifndef __CARDIO_MODEL_H_INCLUDED
#define __CARDIO_MODEL_H_INCLUDED
#include "CardioRungeKuttaEquation.h"
#include "CardioCompartment.h"

class CardioModel : public CardioRungeKuttaEquation
{
public:
	CardioModel(int NumCompartments, double SampleRate_Hz);
	~CardioModel();

	virtual void GetDerivative(double x, double *y, double *dydx);
	virtual double GetInitialY(int CompartmentNumber);
	virtual void SetInititalY(int CompartmentNumber, double InitialY);

	void SetVbias(double NewVbias);
	void SetVentricularElastance(double NewVentricularElastance);
	void Step(void);
	double *GetOutputs(void);

	void AddCompartment(CardioCompartment *Compartment);
	void Initialize(void);
private:
	CardioCompartment **mCompartment;
	double *mRequiv1, *mRequiv2;
	double mDeltaT, mV1, mV2;
	int mNumCompartments, mMaxCompartments;
	bool mInitialized;
	CardioRungeKuttaSolver rkSolver;
	double *mOutputArray;	// Array of output values, three per compartment, I1, I2 and BiasedVc in that order.
							// Compartments are presented in numerical order within this array
};

#endif
