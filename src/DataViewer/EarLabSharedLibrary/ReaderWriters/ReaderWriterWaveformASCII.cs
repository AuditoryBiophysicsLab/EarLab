using System;
using System.Data;
using System.IO;

namespace EarLab.ReaderWriters
{
	/// <summary>
	/// Summary description for ReaderWriterWaveformASCII.
	/// </summary>
	public class ReaderWriterWaveformASCII
	{
		public static DataTable FileRead(string fileName)
		{
			string line;

			DataTable myDataTable = new DataTable("Waveform");
			myDataTable.Columns.Add("Time", typeof(double));
			myDataTable.Columns.Add("Value", typeof(double));

			double i = 0;
			StreamReader myStreamReader = new StreamReader(fileName, System.Text.Encoding.ASCII);
			while ((line = myStreamReader.ReadLine()) != null)
			{
				myDataTable.Rows.Add(new object[] { i, double.Parse(line) });
				i += .00001;
			}
			myStreamReader.Close();

			return myDataTable;
		}
	}
}
