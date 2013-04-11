#ifndef __COMPLEX_H_INCLUDED
#define __COMPLEX_H_INCLUDED

template <class T> class Complex
{
public:
	Complex(T RealVal, T ImagVal)
	{
		Real = RealVal;
		Imag = ImagVal;
	};
	
	Complex()
	{
		Real = 0;
		Imag = 0;
	};

	T Magnitude()
	{
		T intermediate;

		if (Real >= Imag)
		{
			intermediate = Imag / Real;
			return fabs(Real) * sqrt(1 + (intermediate * intermediate));
		}
		else
		{
			intermediate = Real / Imag;
			return fabs(Imag) * sqrt(1 + (intermediate * intermediate));
		}
	};

	Complex<T> operator+(Complex &rhs)
	{
		return Complex(Real + rhs.Real, Imag + rhs.Imag);
	};

	Complex<T> operator-(Complex &rhs)
	{
		return Complex(Real - rhs.Real, Imag - rhs.Imag);
	};

	Complex<T> operator*(Complex &rhs)
	{
		return Complex((Real * rhs.Real) - (Imag * rhs.Imag), (Imag * rhs.Real) + (Real * rhs.Imag));
	};

	Complex<T> operator*(T rhs)
	{
		return Complex(Real * rhs, Imag * rhs);
	};

	Complex<T> operator/(Complex &rhs)
	{
		T intermediate;
		T denominator;
		T res_real, res_imag;

		if (fabs(rhs.Real) >= fabs(rhs.Imag))
		{
			intermediate = rhs.Imag / rhs.Real;
			denominator = rhs.Real + (rhs.Imag * intermediate);
			res_real = (Real + (Imag * intermediate)) / denominator;
			res_imag = (Imag - (Real * intermediate)) / denominator;
			return Complex(res_real, res_imag);
		}
		else
		{
			intermediate = rhs.Real / rhs.Imag;
			denominator = (rhs.Real * intermediate) + rhs.Imag;
			res_real = ((Real * intermediate) + Imag) / denominator;
			res_imag = ((Imag * intermediate) - Real) / denominator;
			return Complex(res_real, res_imag);
		}
	};

	Complex<T> operator/(T rhs)
	{
		return Complex(Real / rhs, Imag / rhs);
	};

	Complex<T> Conjugate()
	{
		return Complex(Real, Imag * -1);
	};

	Complex<T> SquareRoot()
	{
		T w, c, d, x;

		if ((Real == 0) && (Imag == 0))
			return Complex(0, 0);

		c = Real;
		d = Imag;
		if (c >= d)
		{
			x = d / c;
			w = sqrt(fabs(c)) * sqrt((1 + sqrt(1 + (x * x))) / 2);
		}
		else
		{
			x = c / d;
			w = sqrt(fabs(d)) * sqrt((fabs(x) + sqrt(1 + (x * x))) / 2);
		}
		if (c >= 0)
			return Complex(w, (d / (2 * w)));
		else
		{
			if (d >= 0)
				return Complex(fabs(d) / (2 * w), w);
			else
				return Complex(fabs(d) / (2 * w), -1 * w);
		}
	};

	Complex<T> eToTheI(T theta)
	{
		return Complex(cos(theta), sin(theta));
	};

	char *ToString(char *StringBuf)
	{
		if (Imag >= 0)
			sprintf(StringBuf, "(%f + %fi)", Real, Imag);
		else
			sprintf(StringBuf, "(%f - %fi)", Real, Imag);

		return StringBuf;
	};

	T Real;
	T Imag;
};

typedef Complex<float> FloatComplex;
typedef Complex<double> DoubleComplex;
#endif
