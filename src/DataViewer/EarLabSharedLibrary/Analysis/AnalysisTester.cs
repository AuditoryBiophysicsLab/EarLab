using System;
using System.IO;
using EarLab.Analysis;
namespace FFT_Test
{
	/// <summary>
	/// Summary description for AnalysisTester.
	/// </summary>
	/// 
	class AnalysisTester
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
			AnalysisTester app = new AnalysisTester();
		}

		public AnalysisTester()
		{
#if true
			double [] Signal;
			double [,] DataSet = null;
			double [,] OutputDataSet = null;
			const int XSize = 500;
			const int YSize = 50;
			double Min, Max, MinX, MaxX, MinY, MaxY;
			string ColorbarName, XAxisName, YAxisName, Title;


			//InputData = AnalysisTools.Read("input.txt");
			for (int x = 0; x < XSize; x++)
			{
				Signal = AddSignal(AddSignal(GenerateSine(YSize, 1, 1, YSize), GenerateSine(YSize, 2, 1, YSize)), GenerateSine(YSize, 3, 1, YSize));
				if (DataSet == null)
					DataSet = new double [XSize, YSize];
				for (int y = 0; y < Signal.Length; y++)
					DataSet[x, y] = Signal[y];
			}
			//Write("output.txt", AnalysisTools.CepstrumCoefficients(Signal));
			OutputDataSet = EarLab.Analysis.AnalysisTools.Analyze(3, DataSet, 100000.0, 0,
				out  Min,  out Max, out ColorbarName, 
				out MinX, out MaxX, out XAxisName, 
				out MinY, out MaxY, out YAxisName, 
				out Title);
			System.Diagnostics.Debug.WriteLine("Output size: {" + OutputDataSet.GetLength(0) + ", " + OutputDataSet.GetLength(1) + "}");
			System.Diagnostics.Debug.WriteLine("      Min: " + Min  + "  Max: " + Max  + " ColorbarName: \"" + ColorbarName + "\"");
			System.Diagnostics.Debug.WriteLine("     MinX: " + MinX + " MaxX: " + MaxX + "    XAxisName: \"" + XAxisName + "\"");
			System.Diagnostics.Debug.WriteLine("     MinY: " + MinY + " MaxY: " + MaxY + "    YAxisName: \"" + YAxisName + "\"");
			System.Diagnostics.Debug.WriteLine("     Title: \"" + Title + "\"");
			Write("output.txt", OutputDataSet);
#else
			double [] InputData = AddSignal(GenerateSine(100000, 1000.0, 1.0, 100000.0), GenerateGaussianNoise(100000, 1.0));
			//for (int i = 0; i < InputData.Length; i++)
			//	InputData[i] += 1;
			Write("input.txt", InputData);
			double [] OutputData = AnalysisTools.PeriodHistogram(InputData, 100);
			Write("output.txt", OutputData);
#endif
		}

		private static double [] AddSignal(double [] Signal1, double [] Signal2)
		{
			double [] ReturnValue = new double[Math.Max(Signal1.Length, Signal2.Length)];
			int LoopCount = Math.Min(Signal1.Length, Signal2.Length);
			if (Signal1.Length > Signal2.Length)
				for (int i = 0; i < Signal1.Length; i++)
					ReturnValue[i] = Signal1[i];
			else
				for (int i = 0; i < Signal2.Length; i++)
					ReturnValue[i] = Signal2[i];
			for (int i = 0; i < LoopCount; i++)
				ReturnValue[i] = Signal1[i] + Signal2[i];
			return ReturnValue;
		}

		double [] GenerateSine(int nPoints, double Freq_Hz, double Amplitude, double SampleRate_Hz)
		{
			double [] Output = new double[nPoints];
			double Tau = 1.0 / SampleRate_Hz;
			double theta = 2.0 * Math.PI * Freq_Hz * Tau;

			for (int i = 0; i < nPoints; i++)
				Output[i] = Math.Sin(theta * i) * Amplitude;

			return Output;
		}

		double [] GenerateGaussianNoise(int nPoints, double Amplitude)
		{
			double [] Output = new double[nPoints];
			Random newRandom = new Random();
			const int CentralLimitFactor = 8;
			double RescaleFactor = Math.Sqrt((double)CentralLimitFactor);

			for (int i = 0; i < nPoints; i++)
			{
				Output[i] = 0;
				for (int j = 0; j < CentralLimitFactor; j++)
					Output[i] += (newRandom.NextDouble() - 0.5) * 3.4567;
				Output[i] /= RescaleFactor;
				Output[i] *= Amplitude;
			}

			return Output;
		}

		double [] Read(string Filename)
		{
			int LineCount = 0;
			double [] Output;

			// Create an instance of StreamReader to read from a file.
			// The using statement also closes the StreamReader.
			using (StreamReader sr = new StreamReader(Filename))
			{
				String line;
				// Read and count lines from the file until the end of the file is reached.
				while (sr.ReadLine() != null) 
					LineCount++;

				Output = new double [LineCount];
				LineCount = 0;
				// Seek back to the beginning of the file
				sr.BaseStream.Seek(0, SeekOrigin.Begin);
				while ((line = sr.ReadLine()) != null) 
				{
					Output[LineCount] = double.Parse(line);
					LineCount++;
				}
			}
			return Output;
		}

		void Write(string Filename, double [] Data)
		{
			// Create an instance of StreamWriter to write text to a file.
			// The using statement also closes the StreamWriter.
			using (StreamWriter sw = new StreamWriter(Filename)) 
			{
				for (int i = 0; i < Data.Length; i++)
					sw.WriteLine(Data[i]);
			}
		}
		void Write(string Filename, double [,] Data)
		{
			// Create an instance of StreamWriter to write text to a file.
			// The using statement also closes the StreamWriter.
			using (StreamWriter sw = new StreamWriter(Filename)) 
			{
				for (int i = 0; i < Data.GetLength(0); i++)
					for (int j = 0; j < Data.GetLength(1); j++)
					sw.WriteLine(Data[i, j]);
			}
		}
	}

}
