using System;
using System.Data;
using System.IO;

namespace EarLab.ReaderWriters
{
	/// <summary>
	/// Summary description for ReaderWriterSurfaceBinary.
	/// </summary>
	public class ReaderWriterSurfaceBinary
	{
		public static DataTable FileRead(string fileName)
		{
			DataTable myDataTable = new DataTable("2DData");
			myDataTable.Columns.Add("Sample", typeof(int));
			myDataTable.Columns.Add("Channel", typeof(int));
			myDataTable.Columns.Add("Value", typeof(float));

			BinaryReader myBinaryReader = new BinaryReader(new FileStream(fileName, FileMode.Open, FileAccess.Read));


			for (int i=0;i<1000*64;i++)
				myBinaryReader.ReadSingle();
			
			for (int i=0;i<100;i++)
				for (int j=0;j<64;j++)
					myDataTable.Rows.Add(new object[] { i, j, myBinaryReader.ReadSingle() });

			myBinaryReader.Close();

			return myDataTable;
		}
	}
}
