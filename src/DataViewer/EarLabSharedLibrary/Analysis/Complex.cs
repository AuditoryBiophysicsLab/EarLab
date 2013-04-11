using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace EarLab.Analysis
{

	// Comments? Questions? Bugs? Tell Ben Houston at ben@exocortex.org
	// Version: May 4, 2002

	/// <summary>
	/// <p>A double-precision complex number representation.</p>
	/// </summary>
	[StructLayout(LayoutKind.Sequential)]
	public struct Complex : IComparable, ICloneable 
	{
		//-----------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------

		/// <summary>
		/// The real component of the complex number
		/// </summary>
		public double Re;

		/// <summary>
		/// The imaginary component of the complex number
		/// </summary>
		public double Im;

		//-----------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------

		/// <summary>
		/// Create a complex number from a real and an imaginary component
		/// </summary>
		/// <param name="Real"></param>
		/// <param name="Imaginary"></param>
		public Complex(double Real, double Imaginary) 
		{
			this.Re	= (double) Real;
			this.Im	= (double) Imaginary;
		}

		/// <summary>
		/// Create a complex number based on an existing complex number
		/// </summary>
		/// <param name="c"></param>
		public Complex(Complex c) 
		{
			this.Re	= c.Re;
			this.Im	= c.Im;
		}

#if false
		/// <summary>
		/// Create a complex number from a real and an imaginary component
		/// </summary>
		/// <param name="Real"></param>
		/// <param name="Imaginary"></param>
		/// <returns></returns>
		static public Complex FromRealImaginary(double Real, double Imaginary) 
		{
			Complex c;
			c.Re = (double) Real;
			c.Im = (double) Imaginary;
			return c;
		}
#endif
		/// <summary>
		/// Create a complex number from a modulus (length) and an argument (radian)
		/// </summary>
		/// <param name="Magnitude"></param>
		/// <param name="Phase"></param>
		/// <returns></returns>
		static public Complex FromMagnitudePhase(double Magnitude, double Phase) 
		{
			Complex c;
			c.Re	= (double)(Magnitude * System.Math.Cos(Phase));
			c.Im	= (double)(Magnitude * System.Math.Sin(Phase));
			return c;
		}
		
		//-----------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------

		object ICloneable.Clone() 
		{
			return new Complex(this);
		}
		/// <summary>
		/// Clone the complex number
		/// </summary>
		/// <returns></returns>
		public Complex	Clone() 
		{
			return new Complex(this);
		}
		
		//-----------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------

		/// <summary>
		/// The magnitude (length) of the complex number
		/// </summary>
		/// <returns></returns>
		public double Magnitude
		{
			get
			{
				double	x = this.Re;
				double	y = this.Im;
				return	(double) Math.Sqrt(x*x + y*y);
			}
		}

		/// <summary>
		/// The squared magnitude (length^2) of the complex number
		/// </summary>
		/// <returns></returns>
		public double MagnitudeSquared
		{
			get
			{
				double	x = this.Re;
				double	y = this.Im;
				return	(double) x*x + y*y;
			}
		}

		/// <summary>
		/// The phase, in radians of the complex number
		/// </summary>
		/// <returns></returns>
		public double Phase 
		{
			get
			{
				return (double) Math.Atan2(this.Im, this.Re);
			}
		}

		//-----------------------------------------------------------------------------------

		/// <summary>
		/// Get the conjugate of the complex number
		/// </summary>
		/// <returns></returns>
		public Complex Conjugate
		{
			get
			{
				return new Complex(this.Re, -this.Im);
			}
		}

		//-----------------------------------------------------------------------------------

		/// <summary>
		/// Scale the complex number to 1.
		/// </summary>
		public void Normalize() 
		{
			double	Magnitude = this.Magnitude;

			if (Magnitude == 0) 
				throw new DivideByZeroException( "Can not normalize a complex number that is zero." );

			this.Re	= (double)( this.Re / Magnitude );
			this.Im	= (double)( this.Im / Magnitude );
		}

		//-----------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------

		/// <summary>
		/// Convert from a single precision real number to a complex number
		/// </summary>
		/// <param name="d"></param>
		/// <returns></returns>
		public static explicit operator Complex (double d) 
		{
			return new Complex(d, 0);
		}

		/// <summary>
		/// Convert from a single precision complex to a real number
		/// </summary>
		/// <param name="c"></param>
		/// <returns></returns>
		public static explicit operator double (Complex c) 
		{
			return c.Re;
		}
		
		//-----------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------

		/// <summary>
		/// Are these two complex numbers equivalent?
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		public static bool	operator==(Complex a, Complex b) 
		{
			return	( a.Re == b.Re ) && ( a.Im == b.Im );
		}

		/// <summary>
		/// Are these two complex numbers different?
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		public static bool	operator!=(Complex a, Complex b) 
		{
			return	( a.Re != b.Re ) || ( a.Im != b.Im );
		}

		/// <summary>
		/// Get the hash code of the complex number
		/// </summary>
		/// <returns></returns>
		public override int		GetHashCode() 
		{
			return	( this.Re.GetHashCode() ^ this.Im.GetHashCode() );
		}

		/// <summary>
		/// Is this complex number equivalent to another object?
		/// </summary>
		/// <param name="o"></param>
		/// <returns></returns>
		public override bool	Equals( object o ) 
		{
			if (o is Complex) 
			{
				Complex c = (Complex) o;
				return   ( this == c );
			}
			return	false;
		}

		//-----------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------

		/// <summary>
		/// Compare to other complex numbers or real numbers
		/// </summary>
		/// <param name="o"></param>
		/// <returns></returns>
		public int	CompareTo( object o ) 
		{

			if( o == null ) 
				return 1;  // null sorts before current

			if( o is Complex )
				return	this.Magnitude.CompareTo(((Complex)o).Magnitude);
			
			if( o is double ) 
				return	this.Magnitude.CompareTo( (double)o );

			if( o is float ) 
				return	this.Magnitude.CompareTo( (float)o );

			throw new ArgumentException();
		}

		//-----------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------

		/// <summary>
		/// This operator doesn't do much. :-)
		/// </summary>
		/// <param name="a"></param>
		/// <returns></returns>
		public static Complex operator+(Complex a) 
		{
			return a;
		}

		/// <summary>
		/// Negate the complex number
		/// </summary>
		/// <param name="a"></param>
		/// <returns></returns>
		public static Complex operator-( Complex a ) {
			a.Re	= -a.Re;
			a.Im	= -a.Im;
			return a;
		}

		/// <summary>
		/// Add a complex number to a real
		/// </summary>
		/// <param name="a"></param>
		/// <param name="f"></param>
		/// <returns></returns>
		public static Complex operator+( Complex a, double f ) {
			a.Re	= (double)( a.Re + f );
			return a;
		}

		/// <summary>
		/// Add a real to a complex number
		/// </summary>
		/// <param name="f"></param>
		/// <param name="a"></param>
		/// <returns></returns>
		public static Complex operator+( double f, Complex a ) {
			a.Re	= (double)( a.Re + f );
			return a;
		}

		/// <summary>
		/// Add to complex numbers
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		public static Complex operator+( Complex a, Complex b ) {
			a.Re	= a.Re + b.Re;
			a.Im	= a.Im + b.Im;
			return a;
		}

		/// <summary>
		/// Subtract a real from a complex number
		/// </summary>
		/// <param name="a"></param>
		/// <param name="f"></param>
		/// <returns></returns>
		public static Complex operator-( Complex a, double f ) {
			a.Re	= (double)( a.Re - f );
			return a;
		}

		/// <summary>
		/// Subtract a complex number from a real
		/// </summary>
		/// <param name="f"></param>
		/// <param name="a"></param>
		/// <returns></returns>
		public static Complex operator-( double f, Complex a ) {
			a.Re	= (float)( f - a.Re );
			a.Im	= (float)( 0 - a.Im );
			return a;
		}

		/// <summary>
		/// Subtract two complex numbers
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		public static Complex operator-( Complex a, Complex b ) {
			a.Re	= a.Re - b.Re;
			a.Im	= a.Im - b.Im;
			return a;
		}

		/// <summary>
		/// Multiply a complex number by a real
		/// </summary>
		/// <param name="a"></param>
		/// <param name="f"></param>
		/// <returns></returns>
		public static Complex operator*( Complex a, double f ) {
			a.Re	= (double)( a.Re * f );
			a.Im	= (double)( a.Im * f );
			return a;
		}
		
		/// <summary>
		/// Multiply a real by a complex number
		/// </summary>
		/// <param name="f"></param>
		/// <param name="a"></param>
		/// <returns></returns>
		public static Complex operator*( double f, Complex a ) {
			a.Re	= (double)( a.Re * f );
			a.Im	= (double)( a.Im * f );
			
			return a;
		}
		
		/// <summary>
		/// Multiply two complex numbers together
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		public static Complex operator*( Complex a, Complex b ) 
		{
			// (x + yi)(u + vi) = (xu – yv) + (xv + yu)i. 
			double	x = a.Re, y = a.Im;
			double	u = b.Re, v = b.Im;
			
			a.Re	= (double)( x*u - y*v );
			a.Im	= (double)( x*v + y*u );
			
			return a;
		}

		/// <summary>
		/// Divide a complex number by a real number
		/// </summary>
		/// <param name="a"></param>
		/// <param name="f"></param>
		/// <returns></returns>
		public static Complex operator/( Complex a, double f ) 
		{
			if( f == 0 ) 
				throw new DivideByZeroException();
			
			a.Re	= (double)( a.Re / f );
			a.Im	= (double)( a.Im / f );
			
			return a;
		}
		
		/// <summary>
		/// Divide a complex number by a complex number
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		public static Complex operator/( Complex a, Complex b ) {
			double	x = a.Re,	y = a.Im;
			double	u = b.Re,	v = b.Im;
			double	denom = u*u + v*v;

			if( denom == 0 ) {
				throw new DivideByZeroException();
			}

			a.Re	= (double)( ( x*u + y*v ) / denom );
			a.Im	= (double)( ( y*u - x*v ) / denom );
			
			return a;
		}

		/// <summary>
		/// Parse a complex representation in this fashion: "( %f, %f )"
		/// </summary>
		/// <param name="s"></param>
		/// <returns></returns>
		static public Complex Parse( string s ) {
			throw new NotImplementedException( "Complex Complex.Parse( string s ) is not implemented." );
		}
		
		/// <summary>
		/// Get the string representation
		/// </summary>
		/// <returns></returns>
		public override string ToString() {
			return	String.Format( "( {0}, {1}i )", this.Re, this.Im );
		}

		//-----------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------

		/// <summary>
		/// Determine whether two complex numbers are almost (i.e. within the tolerance) equivalent.
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <param name="tolerance"></param>
		/// <returns></returns>
		static public bool IsEqual( Complex a, Complex b, double tolerance ) {
			return
				( Math.Abs( a.Re - b.Re ) < tolerance ) &&
				( Math.Abs( a.Im - b.Im ) < tolerance );

		}
		
		//----------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------

		/// <summary>
		/// Represents zero
		/// </summary>
		static public Complex	Zero {
			get	{	return	new Complex( 0, 0 );	}
		}

		/// <summary>
		/// Represents the result of sqrt( -1 )
		/// </summary>
		static public Complex	I {
			get {	return	new Complex( 0, 1 );	}
		}

		/// <summary>
		/// Represents the largest possible value of Complex.
		/// </summary>
		static public Complex	MaxValue {
			get {	return	new Complex( double.MaxValue, double.MaxValue );	}
		}

		/// <summary>
		/// Represents the smallest possible value of Complex.
		/// </summary>
		static public Complex	MinValue {
			get {	return	new Complex( double.MinValue, double.MinValue );	}
		}


		//----------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------
	}

}
