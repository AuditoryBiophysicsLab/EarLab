#ifndef __COMPLEX_H_INCLUDED
#define __COMPLEX_H_INCLUDED

template <class T> class Complex
{
public:
	Complex(T RealVal, T ImagVal);
	Complex();
	T Magnitude();
	Complex<T> operator+(Complex &rhs);
	Complex<T> operator-(Complex &rhs);
	Complex<T> operator*(Complex &rhs);
	Complex<T> operator*(T rhs);
	Complex<T> operator/(Complex &rhs);
	Complex<T> operator/(T rhs);
	Complex<T> Conjugate();
	Complex<T> SquareRoot();
	Complex<T> eToTheI(T theta);

	char *ToString(char *StringBuf);

	T Real;
	T Imag;
};

typedef Complex<float> FloatComplex;
typedef Complex<double> DoubleComplex;
#endif
