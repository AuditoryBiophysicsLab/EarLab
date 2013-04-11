using System;
using System.Data;
using System.IO;

namespace EarLab.ReaderWriters
{
	/// <summary>
	/// Summary description for ReaderWriterSpikeASCII.
	/// </summary>
	public class ReaderWriterSpikeASCII
	{
		public static DataTable FileRead(string fileName)
		{
			string line;

			DataTable myDataTable = new DataTable("Spikes");
			myDataTable.Columns.Add("CellID", typeof(int));
			myDataTable.Columns.Add("Spike", typeof(double));

			StreamReader myStreamReader = new StreamReader(fileName, System.Text.Encoding.ASCII);
			while ((line = myStreamReader.ReadLine()) != null)
			{
				if (line.Split(' ').Length != 2)
					break;

				myDataTable.Rows.Add(new object[] { int.Parse(line.Split(' ')[0]), double.Parse(line.Split(' ')[1]) });
			}
			myStreamReader.Close();

			return myDataTable;
		}
	}
}
