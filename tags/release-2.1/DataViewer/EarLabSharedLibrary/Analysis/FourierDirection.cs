using System;

namespace EarLab.Analysis
{
	/// <summary>
	/// <p>The direction of the fourier transform.</p>
	/// </summary>
	public enum FourierDirection : int 
	{
		/// <summary>
		/// Forward direction.  Usually in reference to moving from temporal
		/// representation to frequency representation
		/// </summary>
		Forward = 1,
		/// <summary>
		/// Backward direction. Usually in reference to moving from frequency
		/// representation to temporal representation
		/// </summary>
		Backward = -1,
	}
}
