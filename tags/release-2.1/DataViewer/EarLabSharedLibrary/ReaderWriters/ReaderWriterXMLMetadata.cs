using System;
using System.IO;
using System.Collections;
using System.Xml;
using System.Xml.Schema;
using System.Data;

namespace EarLab.ReaderWriters
{
	/// <summary>
	/// Summary description for XMLMetadataReaderWriter.
	/// </summary>
	public class ReaderWriterXMLMetadata
	{
		public enum EarLabBinaryFileType
		{
			/// <summary>
			/// File is an EarLab Time file.
			/// </summary>
			Time,
			/// <summary>
			/// File is an EarLab Frequency-Time file.
			/// </summary>
			FrequencyTime,
			/// <summary>
			/// File is an EarLab Lag-Frequency-Time file.
			/// </summary>
			LagFrequencyTime,
			/// <summary>
			/// File is an unknown file type.
			/// </summary>
			Unknown
		}

		public static DataSet Read(string fileName)
		{
			// read into xml metadata file, and find the SchemaVersion tag in Source node
			string xmlVersionString = "";
			XmlDocument xmlDocument = new XmlDocument();
			xmlDocument.Load(fileName);
			if (xmlDocument.GetElementsByTagName("SchemaVersion").Count == 1)
				xmlVersionString = xmlDocument.GetElementsByTagName("SchemaVersion")[0].InnerXml;
			xmlDocument = null;

			// verify that the SchemaVersion tag was found, and that it is a valid number value
			double xmlVersionNumber;
			if (xmlVersionString == "" || !double.TryParse(xmlVersionString, System.Globalization.NumberStyles.Number, null, out xmlVersionNumber))
				throw new System.Exception("Source Version tag not found in Metadata file.  Please\nverify your file against Metadata XML Schema.");

			// get internal Metadata schema, for verifying the xml Metadata file against
			Stream schemaStream = GetXMLSchemaStream();
			XmlSchema metadataSchema = XmlSchema.Read(schemaStream, new System.Xml.Schema.ValidationEventHandler(xmlValidatingReader_ValidationEventHandler));
			metadataSchema.TargetNamespace = "http://earlab.bu.edu/software/specifications/schemas/EarLabMetadata.xsd";

			// make sure that the internal schema is equal to or newer in version than the Metadata file's source
			if (double.Parse(metadataSchema.Version) != xmlVersionNumber)
				throw new System.Exception("Metadata file version (" + xmlVersionNumber.ToString() + ") does not match internal application Schema version (" + metadataSchema.Version + ").\nPlease use a different version of the application to open file.");

			// get the Metadata file for reading and verification with the internal Metadata schema
			XmlTextReader xmlTextReader = new XmlTextReader(fileName);
			XmlValidatingReader xmlValidatingReader = new XmlValidatingReader(xmlTextReader);
			xmlValidatingReader.ValidationType = ValidationType.Schema;
			xmlValidatingReader.Schemas.Add(metadataSchema);
			xmlValidatingReader.ValidationEventHandler += new System.Xml.Schema.ValidationEventHandler(xmlValidatingReader_ValidationEventHandler);

			try
			{
				while (xmlValidatingReader.Read()){}

				DataSet xmlDataSet = new DataSet();
				schemaStream.Position = 0;
				xmlDataSet.ReadXmlSchema(schemaStream);
				schemaStream.Close();
				xmlDataSet.ReadXml(fileName);

				return xmlDataSet;
			}
			catch (Exception e)
			{
				throw new System.Exception(e.Message.ToString());
			}
			finally
			{
				schemaStream.Close();
				xmlTextReader.Close();
			}
		}

		private static Stream GetXMLSchemaStream()
		{
			return System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("EarLab.Schemas.EarLabMetadata.xsd");
		}

/*		public static void ConvertToXML(string inputFile, string xmlFile)
		{
			EarLabBinaryFileType fileType=EarLabBinaryFileType.Unknown;
			double sampleRate=0, minimumSample=0, maximumSample=0, lagStep=0, lagStart=0, lagEnd=0;
			long frequencySize=0, timeSize=0, lagSize=0, liveSize=0;

			DataSet xmlDataSet = new DataSet();
			xmlDataSet.ReadXmlSchema(GetXMLSchemaStream());

			object[] metadataArray = ReaderWriterASCIIMetadata.Read(inputFile);

			foreach (object metadata in metadataArray)			{				switch (((ReaderWriterASCIIMetadata.MetaDataLine)metadata).Name)				{					case "File_Type":						fileType = (EarLabBinaryFileType)((ReaderWriterASCIIMetadata.MetaDataLine)metadata).Value;						break;					case "Frequency_Dimension_Size":						frequencySize = (long)((ReaderWriterASCIIMetadata.MetaDataLine)metadata).Value;						break;					case "Sample_Rate":					case "Sample_Rate_Hz":						sampleRate = (double)((ReaderWriterASCIIMetadata.MetaDataLine)metadata).Value;						break;					case "Maximum":						maximumSample = (double)((ReaderWriterASCIIMetadata.MetaDataLine)metadata).Value;						break;					case "Minimum":						minimumSample = (double)((ReaderWriterASCIIMetadata.MetaDataLine)metadata).Value;						break;					case "Number_Of_Samples":						timeSize = (long)((ReaderWriterASCIIMetadata.MetaDataLine)metadata).Value;						break;					case "Lag_Dimension_Size":						lagSize = (long)((ReaderWriterASCIIMetadata.MetaDataLine)metadata).Value;						break;					case "Lag_Time_Step_uS":						lagStep = (double)((ReaderWriterASCIIMetadata.MetaDataLine)metadata).Value;						break;					case "Min_Lag_uS":						lagStart = (double)((ReaderWriterASCIIMetadata.MetaDataLine)metadata).Value;						break;					case "Max_Lag_uS":						lagEnd = (double)((ReaderWriterASCIIMetadata.MetaDataLine)metadata).Value;						break;					case "Frame_Length_Samples":						liveSize = (long)((ReaderWriterASCIIMetadata.MetaDataLine)metadata).Value;						break;					default:						break;				}			}
			
			DataRow parametersRow = xmlDataSet.Tables["Parameters"].NewRow();
			
			parametersRow["MinValue"] = minimumSample;
			parametersRow["MaxValue"] = maximumSample;
			parametersRow["SampleRate_Hz"] = sampleRate;

			if (fileType == EarLabBinaryFileType.Time)
				parametersRow["FileType"] = "Time";
			else if (fileType == EarLabBinaryFileType.FrequencyTime)
				parametersRow["FileType"] = "FrequencyTime";
			else
			{
				parametersRow["FileType"] = "LagFrequencyTime";
				parametersRow["LagMin_uS"] = lagStart;
				parametersRow["LagMax_uS"] = lagEnd;
				parametersRow["LagStep_uS"] = lagStep;
			}

			if (liveSize != 0)
				parametersRow["FrameSize"] = liveSize;
			xmlDataSet.Tables["Parameters"].Rows.Add(parametersRow);

			DataRow dimensionsRow = xmlDataSet.Tables["Dimensions"].NewRow();
			xmlDataSet.Tables["Dimensions"].Rows.Add(dimensionsRow);

			if (fileType != EarLabBinaryFileType.Time)
			{
				DataRow frequencyRow = xmlDataSet.Tables["Dimension"].NewRow();
				frequencyRow.SetParentRow(dimensionsRow, xmlDataSet.Relations["Dimensions_Dimension"]);
				frequencyRow["Name"] = "Frequency";
				frequencyRow["Value"] = frequencySize;
				xmlDataSet.Tables["Dimension"].Rows.Add(frequencyRow);
			}

			DataRow timeRow = xmlDataSet.Tables["Dimension"].NewRow();
			timeRow.SetParentRow(dimensionsRow, xmlDataSet.Relations["Dimensions_Dimension"]);
			timeRow["Name"] = "Time";
			timeRow["Value"] = timeSize;
			xmlDataSet.Tables["Dimension"].Rows.Add(timeRow);

			if (fileType == EarLabBinaryFileType.LagFrequencyTime)
			{
				DataRow lagRow = xmlDataSet.Tables["Dimension"].NewRow();
				lagRow.SetParentRow(dimensionsRow, xmlDataSet.Relations["Dimensions_Dimension"]);
				lagRow["Name"] = "Lag";
				lagRow["Value"] = lagSize;
				xmlDataSet.Tables["Dimension"].Rows.Add(lagRow);
			}

			xmlDataSet.WriteXml(xmlFile, XmlWriteMode.IgnoreSchema);
		}*/

		private static void xmlValidatingReader_ValidationEventHandler(object sender, System.Xml.Schema.ValidationEventArgs e)
		{
			throw new Exception(e.Message);
		}

		public static EarLabBinaryFileType GetBinaryFileType(string fileName)
		{
			DataSet xmlDataSet = ReaderWriterXMLMetadata.Read(fileName);

			if (xmlDataSet == null)
				return EarLabBinaryFileType.Unknown;

			if (xmlDataSet.Tables["Parameters"].Rows[0]["FileType"].ToString() == "Time")
				return EarLabBinaryFileType.Time;
			if (xmlDataSet.Tables["Parameters"].Rows[0]["FileType"].ToString() == "FrequencyTime")
				return EarLabBinaryFileType.FrequencyTime;
			else
				return EarLabBinaryFileType.LagFrequencyTime;
		}
	}
}
