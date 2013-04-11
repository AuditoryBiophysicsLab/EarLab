#ifndef _RUNGE_KUTTA_EQUATION_H_INCLUDED
#define _RUNGE_KUTTA_EQUATION_H_INCLUDED

class RungeKuttaEquation
{
public:
	virtual double GetDerivative(double IntermediateY, double InstantaneousVoltage_mV) = 0;
	
	// Intermediate value used by Runge-Kutta differential equation solver
	//virtual double GetIntermediateY(void) {return IntermediateY;}
	//virtual void SetIntermediateY(double IntermediateY) {this->IntermediateY = IntermediateY;}
	
	// Initial Y value for each time step
	//virtual double GetInitialY(void) {return InitialY;}
	//virtual void SetInitialY(double InitialY) {this->InitialY = InitialY;}

	double IntermediateY;				// Intermediate value used by Runge-Kutta differential equation solver
	double InitialY;					// Initial Y value for each time step
};

#endif
