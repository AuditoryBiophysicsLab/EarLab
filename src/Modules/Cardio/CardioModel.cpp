#include "Earlab.h"
#include "EarlabException.h"
#include "CardioModel.h"
#include "RungeKuttaEquation.h"
#include "RungeKuttaSolver.h"

CardioModel::CardioModel(int MaxCompartments, double SampleRate_Hz)
{
	mMaxCompartments = MaxCompartments;
	mNumCompartments = 0;
	mDeltaT = 1.0 / SampleRate_Hz;
	mCompartment = new CardioCompartment *[mNumCompartments];
	mRequiv1 = new double [mNumCompartments];
	mRequiv2 = new double [mNumCompartments];
	mOutputArray = new double [2 * (mNumCompartments * 3)];
	mInitialized = false;
}

CardioModel::~CardioModel()
{
	for (int i = 0; i < mNumCompartments; i++)
		delete mCompartment[i];
	delete [] mCompartment;
	delete [] mRequiv1;
	delete [] mRequiv2;
	delete [] mOutputArray;
}

void CardioModel::AddCompartment(CardioCompartment *Compartment)
{
	_ASSERTE( _CrtCheckMemory( ) );
	if (mInitialized)
		throw new EarlabException("CardioModel::AddCompartment: Error: Cannot add a new compartment after Initialize has been called");
	if (mNumCompartments == mMaxCompartments)
		throw new EarlabException("CardioModel::AddCompartment: Error: Cannot add more than MaxCompartments compartments, as passed into the constuctor");
	_ASSERTE( _CrtCheckMemory( ) );

	mCompartment[mNumCompartments] = Compartment;
	_ASSERTE( _CrtCheckMemory( ) );
	mNumCompartments++;
}

void CardioModel::Initialize(void)
{
	mInitialized = true;	// Prevent any more compartments from being added
}

void CardioModel::GetDerivative(double x, double *y, double *dydx)
{
	int i;

	for (int k = 0; k < 2; k++)
	{
		// Compute equivalent resistances for each branch of each compartment
		for (i = 0; i < mNumCompartments; i++)
		{
			mRequiv1[i] = 0;
			mRequiv2[i] = 0;
			for (int j = 0; j < mNumCompartments; j++)
			{
				if (j != i)
				{
					if (mCompartment[j]->Valve1IsOpen())
						mRequiv1[i] += 1.0 / mCompartment[j]->GetR1();
					if (mCompartment[j]->Valve2IsOpen())
						mRequiv2[i] += 1.0 / mCompartment[j]->GetR2();
				}
			}
			mRequiv1[i] = 1.0 / mRequiv1[i];
			mRequiv2[i] = 1.0 / mRequiv2[i];
		}

		// Compute V1 and V2 using the superposition method
		mV1 = mV2 = 0;
		for (i = 0; i < mNumCompartments; i++)
		{
			if (mCompartment[i]->IsVentricle() && mCompartment[i]->Valve1IsOpen())
				mV1 += mCompartment[i]->GetBiasedVc() * (mRequiv1[i] / (mCompartment[i]->GetR1() + mRequiv1[i]));
			if (mCompartment[i]->IsVentricle() && mCompartment[i]->Valve2IsOpen())
				mV2 += mCompartment[i]->GetBiasedVc() * (mRequiv2[i] / (mCompartment[i]->GetR2() + mRequiv2[i]));
		}

		for (i = 0; i < mNumCompartments; i++)
		{
			dydx[i + 1] = mCompartment[i]->GetDerivative(mV1, mV2, y[i + 1]);
		}
	}
}

double CardioModel::GetInitialY(int CompartmentNumber)
{
	return mCompartment[CompartmentNumber - 1]->GetVc();
}

void CardioModel::SetInititalY(int CompartmentNumber, double InitialY)
{
	mCompartment[CompartmentNumber - 1]->SetVc(InitialY);
}

void CardioModel::SetVbias(double NewVbias)
{
	for (int i = 0; i < mNumCompartments; i++)
	{
		if (mCompartment[i]->HasVbias())
			mCompartment[i]->SetVbias(NewVbias);
	}
}

void CardioModel::SetVentricularElastance(double NewVentricularElastance)
{
	for (int i = 0; i < mNumCompartments; i++)
	{
		if (mCompartment[i]->IsVentricle())
			mCompartment[i]->SetElastance(NewVentricularElastance);
	}
}

double *CardioModel::GetOutputs(void)
{
	int index = 0;
	mOutputArray[index++]= mV1;
	mOutputArray[index++]= mV2;
	for (int i = 0; i < mNumCompartments; i++)
	{
		mOutputArray[index++]= mCompartment[i]->GetI1();
		mOutputArray[index++]= mCompartment[i]->GetI2();
		mOutputArray[index++]= mCompartment[i]->GetBiasedVc();
	}
	return mOutputArray;
}

void CardioModel::Step(void)
{
	int nbad, nok;
	double eps = 1.0e-4;
	double hstart = mDeltaT;	/* 'Best guess' for time step */
	double hmin = 1.0e-6; /* Minimum time step */
	double x1, x2;	  // Howitt 12Dec01

	if (!mInitialized)
		throw EarlabException("CardioModel::Step: Error: Must call Initialize() before calling Step() for the first time");

	/* Try  ODEINT here , from Numerical Recipes */
	rkSolver.kmax=100;
	x1 = 1.0; 
	x2 = 1.0 + mDeltaT; 
	rkSolver.dxsav=(x2-x1)/20.0;

	// DJA: Set the present instananeous voltage of the compartment in each of my dependent conductances
	// Note: NumHHChannels is the number of HH Channels in the compartment, plus one for the compartment
	rkSolver.ode_rk(mNumCompartments, x1, x2, eps, hstart, hmin, &nok, &nbad, this);
}
