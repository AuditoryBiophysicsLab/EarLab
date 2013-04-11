#include "Earlab.h"
#include "EarlabException.h"
#include "RungeKuttaSolver.h"
#include "RungeKuttaEquation.h"
#include "math.h"
#include "memory.h"
#include "stdlib.h"

#define NR_END 1
#define MAXSTP 10000
#define TINY 1.0e-30
#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
#define FMAX(a,b) ((a) > (b) ? (a) : (b))

RungeKuttaSolver::RungeKuttaSolver()
{
	// DJA: There is a very fine line between genius and insanity!
	a2=0.2;
	a3=0.3;
	a4=0.6;
	a5=1.0;
	a6=0.875;

	b21=0.2;
	b31=3.0/40.0;
	b32=9.0/40.0;
	b41=0.3;
	b42=-0.9;
	b43=1.2;
	b51 = -11.0/54.0;
	b52=2.5;
	b53 = -70.0/27.0;
	b54=35.0/27.0;
	b61=1631.0/55296.0;
	b62=175.0/512.0;
	b63=575.0/13824.0;
	b64=44275.0/110592.0;
	b65=253.0/4096.0;

	c1=37.0/378.0;
	c3=250.0/621.0;
	c4=125.0/594.0;
	c6=512.0/1771.0;

	dc5 = -277.0/14336.0;

	xp=new_vector(1,200);
	yp=new_matrix(1,10,1,200);
}

void RungeKuttaSolver::ode_rk(int nvar, double x1, double x2, double eps, double h1, double hmin, int *nok, int *nbad, RungeKuttaEquation *Equations[])
{
	int nstp,i;
	double xsav, x, hnext, hdid, h;	  // DJA: h is the current time step value
	double *yscal,*y,*dydx;

	yscal=new_vector(1,nvar);
	y=new_vector(1,nvar);
	dydx=new_vector(1,nvar);
	x=x1;
	h=SIGN(h1,x2-x1);
	*nok = (*nbad) = kount = 0;
	for (i = 1; i <= nvar ; i++) 
		y[i] = Equations[i]->InitialY;
	if (kmax > 0) xsav=x-dxsav*2.0;
	for (nstp=1;nstp<=MAXSTP;nstp++)
	{
		// (*derivs)(x,y,dydx); DJA: This is the old call which we've replaced with the following two lines
		for (i = 1; i <= nvar ; i++)
			dydx[i] = Equations[i]->GetDerivative(y[i], y[5]);

		for (i=1;i<=nvar;i++)
			yscal[i]=fabs(y[i])+fabs(dydx[i]*h)+TINY;

		if (kmax > 0 && kount < kmax-1 && fabs(x-xsav) > fabs(dxsav))
		{
			xp[++kount]=x;
			for (i=1;i<=nvar;i++) yp[i][kount]=y[i];
			xsav=x;
		}

		if ((x+h-x2)*(x+h-x1) > 0.0) 
			h=x2-x;

		rkqs(y,dydx,nvar,&x,h,eps,yscal,&hdid,&hnext,Equations);

		if (hdid == h) 
			++(*nok);
		else 
			++(*nbad);

		if ((x-x2)*(x2-x1) >= 0.0)
		{
			for (i=1;i<=nvar;i++) 
				Equations[i]->InitialY = y[i];

			if (kmax)
			{
				xp[++kount]=x;
				for (i=1;i<=nvar;i++) 
					yp[i][kount]=y[i];
			}
			delete_vector(dydx,1,nvar);
			delete_vector(y,1,nvar);
			delete_vector(yscal,1,nvar);
			return;
		}
		//  printf("hnext %f hmin %f\n",hnext,hmin);
		if (fabs(hnext) <= hmin)
		{
			nrerror("Step size too small in odeint");
		}
		h=hnext;
	}
	nrerror("Too many steps in routine odeint");
}

#define SAFETY 0.9
#define PGROW -0.2
#define PSHRNK -0.25
#define ERRCON 1.89e-4

void RungeKuttaSolver::rkqs(double y[], double dydx[], int n, double *x, double htry, double eps, double yscal[], double *hdid, double *hnext, RungeKuttaEquation *Equations[])
{
	int i;
	double errmax,h,xnew,*yerr,*ytemp;

	yerr=new_vector(1,n);
	ytemp=new_vector(1,n);
	h=htry;
	for (;;)
	{
		rkck(y,dydx,n,*x,h,ytemp,yerr,Equations);
		errmax=0.0;
		for (i=1;i<=n;i++) 
			errmax=FMAX(errmax,fabs(yerr[i]/yscal[i]));
		errmax /= eps;
		if (errmax > 1.0)
		{
			h=SAFETY*h*pow(errmax,PSHRNK);
			if (h < 0.1*h)
				h *= 0.1;

			xnew=(*x)+h;
			if (xnew == *x)	
				nrerror("stepsize underflow in rkqs");
			continue;
		}
		else
		{
			if (errmax > ERRCON) 
				*hnext=SAFETY*h*pow(errmax,PGROW);
			else 
				*hnext=5.0*h;
			*x += (*hdid=h);
			for (i=1;i<=n;i++) 
				y[i]=ytemp[i];
			break;
		}
	}
	delete_vector(ytemp,1,n);
	delete_vector(yerr,1,n);
}
#undef SAFETY
#undef PGROW
#undef PSHRNK
#undef ERRCON

void RungeKuttaSolver::rkck(double y[], double dydx[], int n, double x, double h, double yout[], double yerr[], RungeKuttaEquation *Equations[])
{
	int i;
	double dc1=c1-2825.0/27648.0;
	double dc3=c3-18575.0/48384.0;
	double dc4=c4-13525.0/55296.0;
	double dc6=c6-0.25;
	double *ak2,*ak3,*ak4,*ak5,*ak6,*ytemp;

	ak2=new_vector(1,n);
	ak3=new_vector(1,n);
	ak4=new_vector(1,n);
	ak5=new_vector(1,n);
	ak6=new_vector(1,n);
	ytemp=new_vector(1,n);

	for (i = 1; i <= n; i++)
		ytemp[i]=y[i]+b21*h*dydx[i];
	// (*derivs)(x+a2*h,ytemp,ak2); DJA: This is the old call which we've replaced with the following two lines
	for (i = 1; i <= n; i++)
		ak2[i] = Equations[i]->GetDerivative(ytemp[i], y[5]);
	
	for (i = 1; i <= n; i++)
		ytemp[i]=y[i]+h*(b31*dydx[i]+b32*ak2[i]);
	// (*derivs)(x+a3*h,ytemp,ak3); DJA: This is the old call which we've replaced with the following two lines	
	for (i = 1; i <= n; i++)
		ak3[i] = Equations[i]->GetDerivative(ytemp[i], y[5]);

	for (i = 1; i <= n; i++)
		ytemp[i]=y[i]+h*(b41*dydx[i]+b42*ak2[i]+b43*ak3[i]);
	// (*derivs)(x+a4*h,ytemp,ak4); DJA: This is the old call which we've replaced with the following two lines	
	for (i = 1; i <= n; i++)
		ak4[i] = Equations[i]->GetDerivative(ytemp[i], y[5]);

	for (i = 1; i <= n; i++)
		ytemp[i]=y[i]+h*(b51*dydx[i]+b52*ak2[i]+b53*ak3[i]+b54*ak4[i]);
	// (*derivs)(x+a5*h,ytemp,ak5); DJA: This is the old call which we've replaced with the following two lines	
	for (i = 1; i <= n; i++)
		ak5[i] = Equations[i]->GetDerivative(ytemp[i], y[5]);

	for (i = 1; i <= n; i++)
		ytemp[i]=y[i]+h*(b61*dydx[i]+b62*ak2[i]+b63*ak3[i]+b64*ak4[i]+b65*ak5[i]);
	//(*derivs)(x+a6*h,ytemp,ak6); DJA: This is the old call which we've replaced with the following two lines	
	for (i = 1; i <= n; i++)
		ak6[i] = Equations[i]->GetDerivative(ytemp[i], y[5]);

	for (i = 1; i <= n; i++)
		yout[i]=y[i]+h*(c1*dydx[i]+c3*ak3[i]+c4*ak4[i]+c6*ak6[i]);
	for (i = 1; i <= n; i++)
		yerr[i]=h*(dc1*dydx[i]+dc3*ak3[i]+dc4*ak4[i]+dc5*ak5[i]+dc6*ak6[i]);

	delete_vector(ytemp,1,n);
	delete_vector(ak6,1,n);
	delete_vector(ak5,1,n);
	delete_vector(ak4,1,n);
	delete_vector(ak3,1,n);
	delete_vector(ak2,1,n);
}

void RungeKuttaSolver::nrerror(char *error_text)
{
	throw EarlabException("RungeKuttaSolver: Numerical Recipies error: %s", error_text);
}

double *RungeKuttaSolver::new_vector(long nl, long nh)
{
	double *v;

	v=(double *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(double)));
	if (!v)	nrerror("allocation failure in vector()");
	return v-nl+NR_END;
}

double **RungeKuttaSolver::new_matrix(long nrl, long nrh, long ncl, long nch)
{
	long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	double **m;

	/* allocate pointers to rows */
	m=(double **) malloc((size_t)((nrow+NR_END)*sizeof(double*)));
	if (!m)
		nrerror("allocation failure 1 in matrix()");
	m += NR_END;
	m -= nrl;

	/* allocate rows and set pointers to them */
	m[nrl]=(double *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(double)));
	if (!m[nrl])
		nrerror("allocation failure 2 in matrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for (i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}

void RungeKuttaSolver::delete_vector(double *v, long nl, long nh)
{
	free((void *)(v+nl-NR_END));
}

void RungeKuttaSolver::delete_matrix(double **m, long nrl, long nrh, long ncl, long nch)
{
	free((void *) (m[nrl]+ncl-NR_END));
	free((void *) (m+nrl-NR_END));
}
