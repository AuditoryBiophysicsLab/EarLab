using System;
using EarLab.ReaderWriters;

namespace EarLab.Analysis
{
	/// <summary>
	/// Summary description for SpikeAnalysis.
	/// </summary>
	public class SpikeAnalysis
	{
		public static double [][]BinSpikes(ReaderWriterBinarySpikes.SpikeItem[] Spikes, double StartTime_Seconds, double EndTime_Seconds, int CellCount, int CellIDBinCount, int TimeBinCount)
		{
			double TimeLength = EndTime_Seconds - StartTime_Seconds;
			//double TimeLength = Spikes[Spikes.Length-1].time - Spikes[0].time;

			double [][] Bins = new double[2][];
			int SpikeCount = Spikes.Length;
			int CellID;
			double SpikeTime_Seconds;
			double CellsPerBin, SecondsPerBin;
			int CellBin, TimeBin;

			Bins[0] = new double[CellIDBinCount];
			Bins[1] = new double[TimeBinCount];

			CellsPerBin = (double)CellCount / (double)CellIDBinCount;
			SecondsPerBin = TimeLength / (double)TimeBinCount;

			for (int CurSpike=0; CurSpike < SpikeCount; CurSpike++)
			{
				CellID = Spikes[CurSpike].cellID;
				SpikeTime_Seconds = Spikes[CurSpike].time;
				
				CellBin = (int)((CellCount-CellID-1) / CellsPerBin);
				TimeBin = Math.Min(TimeBinCount-1, (int)((double)(SpikeTime_Seconds - StartTime_Seconds) / SecondsPerBin));
				
				Bins[0][CellIDBinCount-CellBin-1]++;
				Bins[1][TimeBin]++;
			}

			return Bins;
		}

		public static double [,]PostStimulusAnalysis(ReaderWriterBinarySpikes.SpikeItem[] Spikes, double BinWidth_Seconds, double StartTime_Seconds, double EndTime_Seconds, int Start_CellID, int End_CellID)
		{
			int SpikeCount = Spikes.Length;
			int CellID;
			double SpikeTime_Seconds;
			double AnalysisWindow_Seconds;
			int TimeBinCount, CellBinCount, SelectedSpikeCount;
			double [,] Bins;
			int TimeBin, CellBin;

			SelectedSpikeCount = 0;
			AnalysisWindow_Seconds = EndTime_Seconds - StartTime_Seconds;
			TimeBinCount = (int)(AnalysisWindow_Seconds / BinWidth_Seconds);
			CellBinCount = End_CellID - Start_CellID;
			Bins = new double [TimeBinCount, CellBinCount+1];

			for (int CurSpike=0; CurSpike < SpikeCount; CurSpike++)
			{
				CellID = Spikes[CurSpike].cellID;
				SpikeTime_Seconds = Spikes[CurSpike].time;
				if ((StartTime_Seconds <= SpikeTime_Seconds) && (SpikeTime_Seconds <= EndTime_Seconds) &&
					(Start_CellID <= CellID) && (CellID <= End_CellID))
				{
					SelectedSpikeCount++;
					TimeBin = (int)Math.Min(TimeBinCount-1, Math.Round((SpikeTime_Seconds - StartTime_Seconds) / BinWidth_Seconds, 0));
					CellBin = CellID - Start_CellID;
					Bins[TimeBin, CellBin]++;
				}
			}

			return Bins;
		}

		public static double [,]PeriodAnalysis(ReaderWriterBinarySpikes.SpikeItem[] Spikes, double Period_Seconds, int PeriodBinCount, double StartTime_Seconds, double EndTime_Seconds, int Start_CellID, int End_CellID)
		{
			int SpikeCount = Spikes.Length;
			int CellID;
			double SpikeTime_Seconds;
			double PeriodRatio, TimePerBin_Seconds;
			int CellBinCount, SelectedSpikeCount;
			double [,] Bins;
			int TimeBin, CellBin;

			SelectedSpikeCount = 0;
			CellBinCount = End_CellID - Start_CellID;
			TimePerBin_Seconds = Period_Seconds / PeriodBinCount;
			Bins = new double [PeriodBinCount, CellBinCount+1];

			for (int CurSpike=0; CurSpike < SpikeCount; CurSpike++)
			{
				CellID = Spikes[CurSpike].cellID;
				SpikeTime_Seconds = Spikes[CurSpike].time;
				if ((StartTime_Seconds <= SpikeTime_Seconds) && (SpikeTime_Seconds <= EndTime_Seconds) &&
					(Start_CellID <= CellID) && (CellID <= End_CellID))
				{
					SelectedSpikeCount++;
					PeriodRatio = (SpikeTime_Seconds - StartTime_Seconds) / Period_Seconds;
					PeriodRatio -= Math.Floor(PeriodRatio);
					TimeBin = (int)Math.Min(PeriodBinCount - 1, Math.Round(PeriodRatio * PeriodBinCount));
					CellBin = CellID - Start_CellID;
					Bins[TimeBin, CellBin]++;
				}
			}

			return Bins;
		}

		public static double [,]IntervalAnalysis(ReaderWriterBinarySpikes.SpikeItem[] Spikes, double MaxInterval_Seconds, int IntervalBinCount, double StartTime_Seconds, double EndTime_Seconds, int Start_CellID, int End_CellID)
		{
			int SpikeCount = Spikes.Length;
			int CellID;
			double SpikeTime_Seconds;
			double IntervalTime_Seconds, TimePerBin_Seconds;
			int CellBinCount, IntervalCount;
			double [] LastSpikeTime_Seconds;
			double [,] Bins;
			int TimeBin, CellBin;

			IntervalCount = 0;
			CellBinCount = (End_CellID - Start_CellID) + 1;
			TimePerBin_Seconds = MaxInterval_Seconds / IntervalBinCount;
			LastSpikeTime_Seconds = new double [CellBinCount];
			Bins = new double [IntervalBinCount, CellBinCount];

			// Initialize the last spike time array to all -1, indicating no spike yet recieved for this cell ID
			for (CellBin = 0; CellBin < CellBinCount; CellBin++)
				LastSpikeTime_Seconds[CellBin] = -1.0;

			for (int CurSpike=0; CurSpike < SpikeCount; CurSpike++)
			{
				CellID = Spikes[CurSpike].cellID;
				SpikeTime_Seconds = Spikes[CurSpike].time;
				if ((StartTime_Seconds <= SpikeTime_Seconds) && (SpikeTime_Seconds <= EndTime_Seconds) &&
					(Start_CellID <= CellID) && (CellID <= End_CellID))
				{
					CellBin = CellID - Start_CellID;
					if (LastSpikeTime_Seconds[CellBin] != -1)
					{
						IntervalCount++;
						IntervalTime_Seconds = (SpikeTime_Seconds - LastSpikeTime_Seconds[CellBin]);
						if (IntervalTime_Seconds < MaxInterval_Seconds)
						{
							TimeBin = (int)(IntervalTime_Seconds / TimePerBin_Seconds);
							Bins[TimeBin, CellBin]++;
						}
					}
					LastSpikeTime_Seconds[CellBin] = SpikeTime_Seconds;
				}
			}

			return Bins;
		}
	}
}
