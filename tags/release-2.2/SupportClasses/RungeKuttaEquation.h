#ifndef _RUNGE_KUTTA_EQUATION_H_INCLUDED
#define _RUNGE_KUTTA_EQUATION_H_INCLUDED

class RungeKuttaEquation
{
public:
	virtual double GetDerivative(double IntermediateY, double InstantaneousVoltage_mV) = 0;
	
	double IntermediateY;				// Intermediate value used by Runge-Kutta differential equation solver
	double InitialY;					// Initial Y value for each time step
};

#endif
