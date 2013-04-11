using System;
using System.IO;
using System.Collections;
using System.Xml;
using System.Xml.Schema;
using System.Data;

namespace EarLab.ReaderWriters
{
	/// <summary>
	/// Summary description for ReaderWriterXMLSpikes.
	/// </summary>
	public class ReaderWriterXMLSpikes
	{
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
			metadataSchema.TargetNamespace = "http://earlab.bu.edu/software/specifications/schemas/EarLabSpikes.xsd";

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
			return System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("EarLab.Schemas.EarLabSpikes.xsd");
		}

		private static void xmlValidatingReader_ValidationEventHandler(object sender, System.Xml.Schema.ValidationEventArgs e)
		{
			throw new Exception(e.Message);
		}
	}
}
