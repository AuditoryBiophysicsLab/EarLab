#ifndef _CARDIO_RUNGE_KUTTA_EQUATION_H_INCLUDED
#define _CARDIO_RUNGE_KUTTA_EQUATION_H_INCLUDED

class CardioRungeKuttaEquation
{
public:
	virtual void GetDerivative(double x, double *y, double *dydx) {}
	virtual double GetInitialY(int CompartmentNumber) {return 0;}
	virtual void SetInitialY(int CompartmentNumber, double InitialY) {}
};

#endif
