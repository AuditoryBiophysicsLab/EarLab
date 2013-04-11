using System;
using System.IO;
using System.Collections;
using System.Xml;
using System.Xml.Schema;
using System.Data;

namespace EarLab.ReaderWriters
{
	/// <summary>
	/// Summary description for ReaderWriterASCIIMetadata.
	/// </summary>
	public class ReaderWriterASCIIMetadata
	{
		public class MetaDataLine
		{
			private string dataType, dataName;
			private object dataValue;

			public MetaDataLine(string dataType, string dataName, object dataValue)
			{
				this.dataType = dataType;
				this.dataName = dataName;
				this.dataValue = dataValue;
			}

			public string Type
			{
				get { return this.dataType; }
			}

			public string Name
			{
				get { return this.dataName; }
			}

			public object Value
			{
				get { return this.dataValue; }
			}
		}

		public static object[] Read(string fileName)
		{
			bool fileType = false;
			ArrayList metadataArray = new ArrayList();			StreamReader metadataFileStream = new StreamReader(fileName);			string metadataLine;			string[] metadataSplitLine;			try			{				while ((metadataLine = metadataFileStream.ReadLine()) != null)				{					metadataSplitLine = metadataLine.Split('\t', ' ');					if (metadataSplitLine.Length < 3)						throw new Exception("MetaDataReaderWriter.ReadMetaData(fileName): Incorrect EarLab .metadata parameter file format.");					if (metadataSplitLine[1].Trim() != "File_Type")					{						switch (metadataSplitLine[0].Trim())						{							case "Int":							case "int":								metadataArray.Add(new MetaDataLine(metadataSplitLine[0].Trim(), metadataSplitLine[1].Trim(), long.Parse(metadataSplitLine[2].Trim())));								break;							case "Bool":							case "bool":								metadataArray.Add(new MetaDataLine(metadataSplitLine[0].Trim(), metadataSplitLine[1].Trim(), bool.Parse(metadataSplitLine[2].Trim())));								break;							case "Long":							case "long":								metadataArray.Add(new MetaDataLine(metadataSplitLine[0].Trim(), metadataSplitLine[1].Trim(), long.Parse(metadataSplitLine[2].Trim())));								break;							case "Flaot":							case "float":								metadataArray.Add(new MetaDataLine(metadataSplitLine[0].Trim(), metadataSplitLine[1].Trim(), double.Parse(metadataSplitLine[2].Trim())));								break;							case "Double":							case "double":								metadataArray.Add(new MetaDataLine(metadataSplitLine[0].Trim(), metadataSplitLine[1].Trim(), double.Parse(metadataSplitLine[2].Trim())));								break;							case "String":							case "string":							default:								metadataArray.Add(new MetaDataLine(metadataSplitLine[0].Trim(), metadataSplitLine[1].Trim(), metadataSplitLine[2].Trim()));								break;						}					}					else					{						switch (metadataSplitLine[2].Trim())						{							case "Lag_Frequency_Time":								metadataArray.Add(new MetaDataLine(metadataSplitLine[0].Trim(), metadataSplitLine[1].Trim(), ReaderWriterXMLMetadata.EarLabBinaryFileType.LagFrequencyTime));								break;							case "Frequency_Time":								metadataArray.Add(new MetaDataLine(metadataSplitLine[0].Trim(), metadataSplitLine[1].Trim(), ReaderWriterXMLMetadata.EarLabBinaryFileType.FrequencyTime));								break;							default:								metadataArray.Add(new MetaDataLine(metadataSplitLine[0].Trim(), metadataSplitLine[1].Trim(), ReaderWriterXMLMetadata.EarLabBinaryFileType.Unknown));								break;						}						fileType = true;					}				}			}			catch			{				throw new Exception("ReaderWriterASCIIMetadata.Read(fileName): Incorrect EarLab .metadata parameter file format.");			}			finally			{				metadataFileStream.Close();	// make sure we close this in case we have problems			}

			if (!fileType)
				throw new Exception("ReaderWriterASCIIMetadata.Read(fileName): No file type found in .metadata parameter file.");

			object[] returnArray = new object[metadataArray.Count];
			for (int i=0; i<metadataArray.Count; i++)
				returnArray[i] = metadataArray[i];

			return returnArray;
		}
	}
}
