using System;

namespace EarLab.Analysis
{
	public enum AnalysisType : int
	{
		FourierTransform	= 0,
		Synchronization		= 1,
		AutoCorrelation		= 2,
		SpatialFrequency	= 3,
		PeriodHistogram		= 4,
	}
}
