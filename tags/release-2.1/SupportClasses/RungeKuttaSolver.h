#ifndef _RUNGE_KUTTA_SOLVER_H_INCLUDED
#define _RUNGE_KUTTA_SOLVER_H_INCLUDED

#include "RungeKuttaEquation.h"

class RungeKuttaSolver
{
public:
	RungeKuttaSolver();

	void ode_rk(int nvar, double x1, double x2, double eps, double h1, double hmin, int *nok, int *nbad, RungeKuttaEquation *Equations[]); 

	int kmax,kount;
	double *xp,**yp, dxsav;

private:
	void rkqs(double y[], double dydx[], int n, double *x, double htry, double eps, double yscal[], double *hdid, double *hnext, RungeKuttaEquation *Equations[]);
	void rkck(double y[], double dydx[], int n, double x, double h, double yout[], double yerr[], RungeKuttaEquation *Equations[]);

	void nrerror(char *error_text);
	double *new_vector(long nl, long nh);
	double **new_matrix(long nrl, long nrh, long ncl, long nch);
	void delete_vector(double *v, long nl, long nh);
	void delete_matrix(double **m, long nrl, long nrh, long ncl, long nch);

	// DJA: These are the magic constants used in rkck()
	double a2;
	double a3;
	double a4;
	double a5;
	double a6;
	double b21;
	double b31;
	double b32;
	double b41;
	double b42;
	double b43;
	double b51;
	double b52;
	double b53;
	double b54;
	double b61;
	double b62;
	double b63;
	double b64;
	double b65;
	double c1;
	double c3;
	double c4;
	double c6;
	double dc5;
};

#endif
