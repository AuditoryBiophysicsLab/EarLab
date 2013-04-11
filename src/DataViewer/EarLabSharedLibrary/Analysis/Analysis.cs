using System;
using System.IO;
using EarLab.Analysis;
using EarLab.ReaderWriters;

namespace EarLab.Analysis
{
	struct AnalysisResults
	{
		public double [,] Output;
		public double MinValue, MaxValue, MinX, MaxX, MinY, MaxY;
		public string ColorbarName, XAxisName, YAxisName, Title;
	}

	class AnalysisTools
	{
		public static string [] GetAnalysisMenus()
		{
			string [] ReturnValue = new string [5];
			ReturnValue[0] = "Fourier Transform";
			ReturnValue[1] = "Synchronization";
			ReturnValue[2] = "Auto Correlation";
			ReturnValue[3] = "Spatial Frequency";
			ReturnValue[4] = "Period Histogram";
			return ReturnValue;
		}

		public static string GetAnalysisMenuQuestion(int theAnalysis, out double defaultValue)
		{
			defaultValue = 0;

			AnalysisType whichAnalysis = (AnalysisType)theAnalysis;
			switch (whichAnalysis)
			{
				case AnalysisType.FourierTransform:
					defaultValue = 2000;
					return "Please enter the cutoff frequency, in Hertz";
				case AnalysisType.Synchronization:
					defaultValue = 2000;
					return "Please enter the cutoff frequency, in Hertz";
				case AnalysisType.AutoCorrelation:
					defaultValue = 20;
					return "Please enter the cutoff delay, in milliseconds";
				case AnalysisType.SpatialFrequency:
					return "";
				case AnalysisType.PeriodHistogram:
					defaultValue = 10;
					return "Please enter the period, in milliseconds";
				default:
					return "";
			}
		}
	
		// Waveform analysis
		public static void Analyze(int theAnalysis, double [,] Input, double SampleRate_Hz, double Parameter,
			out AnalysisResults Results)
		{
			Results = new AnalysisResults();
			Results.MinY = 0;
			Results.MaxY = Input.GetLength(1);
			Results.Output = Analyze(theAnalysis, Input, SampleRate_Hz, Parameter, 
				out Results.MinValue,	out Results.MaxValue,	out Results.ColorbarName, 
				out Results.MinX,		out Results.MaxX,		out Results.XAxisName, 
				out Results.MinY,		out Results.MaxY,		out Results.YAxisName,
				out Results.Title);
		}

		// Spike analysis
		public static void Analyze(int theAnalysis, ReaderWriterBinarySpikes.SpikeItem[] Input, double SampleRate_Hz, double Parameter,
			out AnalysisResults Results)
		{
			Results = new AnalysisResults();
			Results.MinY = 0;
			Results.MaxY = Input.GetLength(1);
			//Results.Output = Analyze(theAnalysis, Input, SampleRate_Hz, Parameter, 
			//	out Results.MinValue,	out Results.MaxValue,	out Results.ColorbarName, 
			//	out Results.MinX,		out Results.MaxX,		out Results.XAxisName, 
			//	out Results.MinY,		out Results.MaxY,		out Results.YAxisName,
			//	out Results.Title);
		}

		public static double [,] Analyze(int theAnalysis, double [,] Input, double SampleRate_Hz, double Parameter,
				out double MinValue,	out double MaxValue,	out string ColorbarName,
				out double MinX,		out double MaxX,		out string XAxisName,
				out double MinY,		out double MaxY,		out string YAxisName,
				out string Title)
		{
			int InputXSize, InputYSize;
			int OutputXSize, OutputYSize;
			int Period_Samples;
			double FrequencyStepSize_Hz;
			int CutoffFrequencyIndex;
			double [,] Output = null;
			double [] IntermediateOutput = null;
			double [] IntermediateInput;
			AnalysisType whichAnalysis = (AnalysisType)theAnalysis;

			MinValue = double.MaxValue;
			MaxValue = double.MinValue;
			ColorbarName = "";
			MinX = 0;
			MaxX = Input.GetLength(0);
			MinY = 1;
			MaxY = Input.GetLength(1);
			YAxisName = "";
			XAxisName = "";
			Title = "Analysis";

			InputXSize = Input.GetLength(0);
			InputYSize = Input.GetLength(1);

			if (whichAnalysis == AnalysisType.SpatialFrequency)
			{
				IntermediateInput = new double [InputYSize];
				OutputXSize = Input.GetLength(0);
				ColorbarName = "Amplitude";
				XAxisName = "Sample Number";
				YAxisName = "Spatial Frequency (Cycles)";
				Title = "Spatial Frequency";
				for (int x = 0; x < InputXSize; x++)
				{
					for (int y = 0; y < InputYSize; y++)
						IntermediateInput[y] = Input[x, y];
					IntermediateOutput = SpatialFrequency(IntermediateInput);
					OutputYSize = IntermediateOutput.Length;
					if (Output == null)
						Output = new double [OutputXSize, OutputYSize];
					for (int y = 0; y < OutputYSize; y++)
					{
						MinValue = Math.Min(MinValue, IntermediateOutput[y]);
						MaxValue = Math.Max(MaxValue, IntermediateOutput[y]);
						Output[x, y] = IntermediateOutput[y];
					} // for (int y = 0; y < OutputYSize; y++)
				} // for (int x = 0; x < InputXSize; x++)
				MaxY = Output.GetLength(1);
			} // if (whichAnalysis == AnalysisType.SpatialFrequency)
			else
			{
				IntermediateInput = new double [InputXSize];
				OutputYSize = InputYSize;

				for (int y = 0; y < InputYSize; y++)
				{
					for (int x = 0; x < InputXSize; x++)
						IntermediateInput[x] = Input[x, y];
					switch (whichAnalysis)
					{
						case AnalysisType.FourierTransform:
							IntermediateOutput = RFFT(IntermediateInput);
							FrequencyStepSize_Hz = (SampleRate_Hz / 2.0) / IntermediateOutput.Length;
							CutoffFrequencyIndex = (int)(Parameter / FrequencyStepSize_Hz) + 1;
							ColorbarName = "Amplitude";
							XAxisName = "Frequency (Hz)";
							YAxisName = "Frequency Channel";
							MaxX = SampleRate_Hz / 2.0;
							YAxisName = "Frequency Channel";
							Title = "Fourier Analysis";
							if (CutoffFrequencyIndex < 2)
								CutoffFrequencyIndex = 2;
							if ((CutoffFrequencyIndex > 0) && (CutoffFrequencyIndex < IntermediateOutput.Length))
							{
								// In this case, parameter is used as a cutoff freq for display
								double [] Temp = null;

								Temp = new double [CutoffFrequencyIndex];
								for (int i = 0; i < Temp.Length; i++)
									Temp[i] = IntermediateOutput[i];
								IntermediateOutput = Temp;
								Temp = null;
							} // if ((CutoffFrequencyIndex > 0) && (CutoffFrequencyIndex < IntermediateOutput.Length))
							MaxX = Parameter;
							break;
						case AnalysisType.Synchronization:
							IntermediateOutput = SyncCoefficients(IntermediateInput);
							FrequencyStepSize_Hz = (SampleRate_Hz / 2.0) / IntermediateOutput.Length;
							CutoffFrequencyIndex = (int)(Parameter / FrequencyStepSize_Hz) + 1;
							ColorbarName = "Sync Coefficient";
							XAxisName = "Frequency (Hz)";
							YAxisName = "Frequency Channel";
							Title = "Synchronization";
							if (CutoffFrequencyIndex < 2)
								CutoffFrequencyIndex = 2;
							if ((CutoffFrequencyIndex > 0) && (CutoffFrequencyIndex < IntermediateOutput.Length))
							{
								// In this case, parameter is used as a cutoff freq for display
								double [] Temp = null;

								Temp = new double [CutoffFrequencyIndex];
								for (int i = 0; i < Temp.Length; i++)
									Temp[i] = IntermediateOutput[i];
								IntermediateOutput = Temp;
								Temp = null;
							} // if ((CutoffFrequencyIndex > 0) && (CutoffFrequencyIndex < IntermediateOutput.Length))
							MaxX = Parameter;
							break;
						case AnalysisType.AutoCorrelation:
							Period_Samples = (int)((Parameter / 1000) * SampleRate_Hz);
							IntermediateOutput = Autocorrelation(IntermediateInput);
							ColorbarName = "Energy";
							MaxX = Parameter;
							XAxisName = "Delay (mS)";
							YAxisName = "Frequency Channel";
							Title = "Auto Correlation";
							if ((Period_Samples > 0) && (Period_Samples < IntermediateOutput.Length))
							{
								double [] Temp;
								Temp = new double [Period_Samples];
								for (int i = 0; i < Temp.Length; i++)
									Temp[i] = IntermediateOutput[i];
								IntermediateOutput = Temp;
								Temp = null;
							} // if ((Parameter > 0) && (Parameter < MaxX))
							break;
						case AnalysisType.PeriodHistogram:
							Period_Samples = (int)((Parameter / 1000) * SampleRate_Hz);
							IntermediateOutput = PeriodHistogram(IntermediateInput, Period_Samples);
							ColorbarName = "Amplitude";
							MaxX = Parameter;
							XAxisName = "Time (mS)";
							YAxisName = "Frequency Channel";
							Title = "Period Histogram";
							break;
						default:
							break;
					} // switch (whichAnalysis)
					OutputXSize = IntermediateOutput.Length;
					if (Output == null)
						Output = new double [OutputXSize, OutputYSize];
					for (int x = 0; x < OutputXSize; x++)
					{
						MinValue = Math.Min(MinValue, IntermediateOutput[x]);
						MaxValue = Math.Max(MaxValue, IntermediateOutput[x]);
						Output[x, y] = IntermediateOutput[x];
					} // for (int x = 0; x < OutputXSize; x++)
				} // for (int y = 0; y < InputYSize; y++)
			} // if (whichAnalysis == AnalysisType.SpatialFrequency) else
			return Output;
		} // Analyze()

		public static double [] PeriodHistogram(double [] Signal, int Period_Samples)
		{
			if (Period_Samples < 1)
				Period_Samples = 2;
			if (Signal.Length < Period_Samples)
				Period_Samples = Signal.Length;

			int nPeriods = Signal.Length / Period_Samples;
			double [] ReturnValue = new double[Period_Samples];

			for (int j = 0; j < nPeriods; j++)
				for (int i = 0; i < Period_Samples; i++)
					ReturnValue[i] += Signal[(j * Period_Samples) + i];
			for (int i = 0; i < Period_Samples; i++)
				ReturnValue[i] /= nPeriods;
			return ReturnValue;
		}

		public static double [] RFFT(double [] Signal)
		{
			Complex [] CompSignal;

			CompSignal = ConvertToComplex(HanningWindow(Signal));
			Fourier.FFT(CompSignal, FourierDirection.Forward);
			return NormalizedPositiveFrequencies(CompSignal);
		}

		public static double [] SyncCoefficients(double [] Signal)
		{
			Complex [] CompSignal;
			double Sync;
			double [] ReturnValue;

			CompSignal = ConvertToComplex(HanningWindow(Signal));
			Fourier.FFT(CompSignal, FourierDirection.Forward);
			Sync = CompSignal[0].Magnitude;
			ReturnValue = new double [CompSignal.Length / 2];
			for (int i = 0; i < ReturnValue.Length; i++)
				ReturnValue[i] = CompSignal[i].Magnitude / Sync;
			ReturnValue[0] = 0;
			return ReturnValue;
		}

		public static double [] Autocorrelation(double [] Signal)
		{
			Complex [] InputSignal;
			Complex [] Correlation;

			InputSignal = ConvertToComplex(HanningWindow(Signal));
			Fourier.FFT(InputSignal, FourierDirection.Forward);
			InputSignal[0].Re = 0;
			InputSignal[0].Im = 0;
			Correlation = new Complex[InputSignal.Length];
			for (int i = 0; i < InputSignal.Length; i++)
			{
				InputSignal[i] = InputSignal[i] / InputSignal.Length;
				Correlation[i] = InputSignal[i] * InputSignal[i].Conjugate;
			}
			Fourier.FFT(Correlation, FourierDirection.Backward);
			// Normalize by the value in the zeroth element of the array
			return MagnitudeAndSign(Correlation);
		}

		public static double [] Crosscorrelation(Complex [] SignalConjugate, double [] Signal)
		{
			Complex [] InputSignal;
			Complex [] Correlation;

			InputSignal = ConvertToComplex(HanningWindow(Signal));
			Fourier.FFT(InputSignal, FourierDirection.Forward);
			InputSignal[0].Re = 0;
			InputSignal[0].Im = 0;
			SignalConjugate[0].Re = 0;
			SignalConjugate[0].Im = 0;
			Correlation = new Complex[InputSignal.Length];
			for (int i = 0; i < InputSignal.Length; i++)
			{
				InputSignal[i] = InputSignal[i] / InputSignal.Length;
				Correlation[i] = InputSignal[i] * SignalConjugate[i];
			}
			Fourier.FFT(Correlation, FourierDirection.Backward);
			// Normalize by the product of the standard deviations of the signal
			return MagnitudeAndSign(Correlation);
		}

		private static double [] MagnitudeAndSign(Complex [] Signal)
		{
			double [] ReturnValue;
			ReturnValue = new double [Signal.Length / 2];
			for (int i = 0; i < ReturnValue.Length; i++)
				ReturnValue[i] = Signal[i].Magnitude * Math.Cos(Signal[i].Phase);
			return ReturnValue;
		}

		private static double [] SpatialFrequency(double [] Signal)
		{
			Complex [] CompSignal;

			CompSignal = ConvertToComplex(HanningWindow(Signal));
			Fourier.FFT(CompSignal, FourierDirection.Forward);
			return NormalizedPositiveFrequencies(CompSignal);
		}

		private static Complex [] ConvertToComplex(double [] Signal)
		{
			Complex [] Temp = new Complex[Signal.Length];
			for (int i = 0; i < Signal.Length; i++)
				Temp[i].Re = Signal[i];
			return Temp;
		}

		private static double [] NormalizedPositiveFrequencies(Complex [] Data)
		{
			double [] Output = new double[Data.Length / 2];
			for (int i = 0; i < Output.Length; i++)
				Output[i] = (Data[i].Magnitude / Output.Length) * 2;
			return Output;
		}

		private static double [] HanningWindow(double [] InputData)
		{
			int InputLength, OutputLength;
			int i, n, last1 = 0;
			double [] Window;
			double [] OutputData;
			double theta;

			InputLength = InputData.Length;
			n = InputLength;
			if (n == 0)
				return null;

			// Find the smallest array whose length is a power of two that will contain the data vector.
			// This means that if 'InputData' is 500 points long, we want to put it into a 512-point array
			for (i = 0; i < 32; i++)
			{
				if ((n & 0x00000001) == 0x00000001)	
					last1 = i;	// last1 gets the index of the highest bit set in the size of the array
				n = n >> 1;
			}

			n = InputLength;
			if ((1 << last1) < n)	// If the length of the array is not a power of two
				last1++;			// bump the index up one more bit
			if (last1 > 32)
				throw new ApplicationException("HanningWindow: Maximum array size exceeded");

			OutputLength = 1 << last1;	// Now, OutputLength contains the length of the smallest power-of-two array that will contain the passed-in data
			OutputData = new double [OutputLength];
			Window = new double [OutputLength];
			theta = (2.0 * Math.PI) / OutputLength;
			for (i = 0; i < OutputLength; i++)
			{
				OutputData[i] = 0;
				Window[i] = (1.0 - Math.Cos(theta * i)) / 2.0;
			}

			n = (OutputLength - InputLength) / 2;
			for (i = 0; i < InputLength; i++)
				OutputData[i + n] = InputData[i];
			
			for (i = 0; i < OutputLength; i++)
				OutputData[i] *= Window[i];	

			return OutputData;
		}
	}
}
