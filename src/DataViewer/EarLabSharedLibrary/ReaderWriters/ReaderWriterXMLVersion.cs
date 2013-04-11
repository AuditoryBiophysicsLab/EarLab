using System;
using System.IO;
using System.Collections;
using System.Xml;
using System.Xml.Schema;
using System.Data;

namespace EarLab.ReaderWriters
{
	/// <summary>
	/// Summary description for ReaderWriterXMLVersion.
	/// </summary>
	public class ReaderWriterXMLVersion
	{
		public static DataSet Read(string fileName)
		{
            Stream schemaStream = GetXMLSchemaStream();
            XmlSchema versionSchema = XmlSchema.Read(schemaStream, new ValidationEventHandler(xmlValidatingReader_ValidationEventHandler));
            versionSchema.TargetNamespace = "http://eardev.bu.edu/software/specifications/schemas/EarLabVersion.xsd";

            XmlReaderSettings xmlReaderSettings = new XmlReaderSettings();
            xmlReaderSettings.Schemas.Add(versionSchema);
            xmlReaderSettings.ValidationType = ValidationType.Schema;
            //xmlReaderSettings.ValidationEventHandler += new ValidationEventHandler(xmlValidatingReader_ValidationEventHandler);

            FileStream fs = File.Open("../../productcataloginstance.xml", FileMode.Open);
            XmlReader xmlReader = XmlReader.Create(fs, xmlReaderSettings);
            fs.Close();

            XmlDocument xdoc = new XmlDocument();
            

			//XmlTextReader xmlTextReader = new XmlTextReader(fileName);
			//XmlValidatingReader xmlValidatingReader = new XmlValidatingReader(xmlTextReader);
			//xmlValidatingReader.ValidationType = ValidationType.Schema;

			//Stream schemaStream = GetXMLSchemaStream();
			//XmlSchema versionSchema = XmlSchema.Read(schemaStream, new System.Xml.Schema.ValidationEventHandler(xmlValidatingReader_ValidationEventHandler));
			//versionSchema.TargetNamespace = "http://eardev.bu.edu/software/specifications/schemas/EarLabVersion.xsd";

			//xmlValidatingReader.Schemas.Add(versionSchema);
			//xmlValidatingReader.ValidationEventHandler += new System.Xml.Schema.ValidationEventHandler(xmlValidatingReader_ValidationEventHandler);

			try
			{
                xdoc.Load(xmlReader);

				//while (xmlValidatingReader.Read()){}

				DataSet xmlDataSet = new DataSet();
				schemaStream.Position = 0;
				xmlDataSet.ReadXmlSchema(schemaStream);
				schemaStream.Close();
				xmlDataSet.ReadXml(fileName, XmlReadMode.IgnoreSchema);

				// we need to establish a relationship between the Module and Version for later use (tricky me!)
				//if (destinationDataSet.Tables["Version"].Columns["ModuleLSID"] == null)
				//xmlDataSet.Tables["Version"].Columns["VersionLSID"].Unique = true;
				//xmlDataSet.Tables["Version"].PrimaryKey = new DataColumn[] {xmlDataSet.Tables["Version"].Columns["VersionLSID"]};

				//destinationDataSet.Tables["Version"].Columns.Add("ModuleLSID", typeof(string));
				//destinationDataSet.Tables["Version"].Rows[0]["ModuleLSID"] = destinationDataSet.Tables["Module"].Rows[0]["ModuleLSID"];
				//xmlDataSet.Relations.Add("ModuleVersion", xmlDataSet.Tables["Module"].Columns["ModuleLSID"], xmlDataSet.Tables["Version"].Columns["ModuleLSID"], true);

				//xmlDataSet.Tables["Version"].Rows[0].SetParentRow(xmlDataSet.Tables["Module"].Rows[0], xmlDataSet.Relations["ModuleVersion"]);

				//xmlDataSet.AcceptChanges();

				//return true;
				
				return xmlDataSet;
			}
			catch //(System.Exception e)
			{
				//System.Diagnostics.Debug.WriteLine(e.Message.ToString());
				return null;
			}
			finally
			{
				schemaStream.Close();
				//xmlTextReader.Close();
			}
		}

		private static Stream GetXMLSchemaStream()
		{
			return System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("EarLab.Schemas.EarLabVersion.xsd");
		}

		public static bool ConvertToXML(string saveDirectory, string zipFilename, DataSet versionDataSet)
		{
			if (versionDataSet.Tables["tblVersion"].Rows[0]["fldExecutableFile"].ToString() != "")
			{
				DataSet xmlDataSet = new DataSet();
				xmlDataSet.ReadXmlSchema(GetXMLSchemaStream());

				DataRow moduleRow = xmlDataSet.Tables["Module"].NewRow();
				moduleRow["ModuleLSID"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldModuleLSID"];
				moduleRow["Name"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldModuleName"];
				moduleRow["Description"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldModuleDescription"];
				xmlDataSet.Tables["Module"].Rows.Add(moduleRow);

				DataRow versionRow = xmlDataSet.Tables["Version"].NewRow();
				versionRow["VersionLSID"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldVersionLSID"];
				versionRow["VersionNumber"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldVersionNumber"];
				versionRow["ReleaseDate"] = DateTime.Parse(versionDataSet.Tables["tblVersion"].Rows[0]["fldReleaseDate"].ToString());
				versionRow["NeuralEntity"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldNeuralEntity"];
				versionRow["Abstract"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldAbstract"];
				versionRow["Description"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldDescription"];
				versionRow["Design"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldDesign"];
				versionRow["Implementation"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldImplementation"];
				versionRow["Interface"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldInterface"];
				versionRow["Evaluation"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldEvaluation"];
				versionRow["Prerequisite"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldPrerequisite"];
				versionRow["Note"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldNote"];
				xmlDataSet.Tables["Version"].Rows.Add(versionRow);

				string[] groupTable = new string[] { "Languages", "OSes", "Compliances", "Histories",
													   "References", "Bugs", "Restrictions" };
				string[] singleTable = new string[] { "Language", "OS", "Compliance", "History",
														"Reference", "Bug", "Restriction" };
				string[] dataTable = new string[] { "tblVersionLanguage", "tblVersionOS", "tblVersionCompliance", "tblHistory",
													  "tblReference", "tblBug", "tblRestriction" };
				string[] fieldTable = new string[] { "fldLanguage", "fldOS", "fldCompliance", "fldItem",
													   "fldItem", "fldItem", "fldItem" };

				for(int i=0; i<groupTable.Length; i++)
				{
					if (versionDataSet.Tables[dataTable[i]].Rows.Count > 0)
					{
						DataRow groupRow = xmlDataSet.Tables[groupTable[i]].NewRow();
						groupRow.SetParentRow(versionRow, xmlDataSet.Relations["Version_" + groupTable[i]]);
						xmlDataSet.Tables[groupTable[i]].Rows.Add(groupRow);
						foreach (DataRow row in versionDataSet.Tables[dataTable[i]].Rows)
						{
							DataRow singleRow = xmlDataSet.Tables[singleTable[i]].NewRow();
							singleRow.SetParentRow(groupRow, xmlDataSet.Relations[groupTable[i] + "_" + singleTable[i]]);
							singleRow[singleTable[i] + "_Column"] = row[fieldTable[i]];
							xmlDataSet.Tables[singleTable[i]].Rows.Add(singleRow);
						}
					}
				}

				if (versionDataSet.Tables["tblClassification"].Rows.Count > 0)
				{
					DataRow classificationsRow = xmlDataSet.Tables["Classifications"].NewRow();
					classificationsRow.SetParentRow(versionRow, xmlDataSet.Relations["Version_Classifications"]);
					xmlDataSet.Tables["Classifications"].Rows.Add(classificationsRow);
					foreach (DataRow row in versionDataSet.Tables["tblClassification"].Rows)
					{
						DataRow classificationRow = xmlDataSet.Tables["Classification"].NewRow();
						classificationRow.SetParentRow(classificationsRow, xmlDataSet.Relations["Classifications_Classification"]);
						classificationRow["Structure"] = row["fldStructure"];
						classificationRow["Substructure"] = row["fldSubstructure"];
						classificationRow["CellType"] = row["fldCellType"];
						xmlDataSet.Tables["Classification"].Rows.Add(classificationRow);
					}
				}

				if (versionDataSet.Tables["tblVersionDataType"].Rows.Count > 0)
				{
					DataRow datatypesRow = xmlDataSet.Tables["DataTypes"].NewRow();
					xmlDataSet.Tables["DataTypes"].Rows.Add(datatypesRow);
					foreach (DataRow row in versionDataSet.Tables["tblVersionDataType"].Rows)
					{
						DataRow datatypeRow = xmlDataSet.Tables["DataType"].NewRow();
						datatypeRow.SetParentRow(datatypesRow, xmlDataSet.Relations["DataTypes_DataType"]);
						datatypeRow["Name"] = row["fldName"];
						datatypeRow["Index"] = row["fldIndex"];
						datatypeRow["Description"] = row["fldDescription"];
						datatypeRow["Direction"] = row["fldDirection"];
						datatypeRow["Type"] = row["fldDataType"];
						xmlDataSet.Tables["DataType"].Rows.Add(datatypeRow);
					}
				}

				if (versionDataSet.Tables["tblParameter"].Rows.Count > 0)
				{
					DataRow parametersRow = xmlDataSet.Tables["Parameters"].NewRow();
					xmlDataSet.Tables["Parameters"].Rows.Add(parametersRow);
					foreach (DataRow row in versionDataSet.Tables["tblParameter"].Rows)
					{
						DataRow parameterRow = xmlDataSet.Tables["Parameter"].NewRow();
						parameterRow.SetParentRow(parametersRow, xmlDataSet.Relations["Parameters_Parameter"]);
						parameterRow["Name"] = row["fldName"];
						parameterRow["Description"] = row["fldDescription"];
						parameterRow["Type"] = row["fldParameterType"];
						parameterRow["Minimum"] = row["fldMinimum"];
						parameterRow["Maximum"] = row["fldMaximum"];
						xmlDataSet.Tables["Parameter"].Rows.Add(parameterRow);
					}
				}

				string xmlName = saveDirectory + "\\" + versionDataSet.Tables["tblVersion"].Rows[0]["fldVersionLSID"].ToString().Replace(':','_') + ".xml";
				xmlDataSet.WriteXml(xmlName, XmlWriteMode.IgnoreSchema);

				string dllName = saveDirectory + "\\" + versionDataSet.Tables["tblVersion"].Rows[0]["fldVersionLSID"].ToString().Replace(':','_') + ".dll";
				File.Copy(versionDataSet.Tables["tblVersion"].Rows[0]["fldExecutableFile"].ToString(), dllName);
				
				EarLab.Utilities.Zip.ZipFiles(new string[] { xmlName, dllName }, saveDirectory + "\\" + zipFilename);

				File.Delete(xmlName);
				File.Delete(dllName);
				
				return true;
			}
			else
				return false;
		}

		public static DataSet ConvertToXML(DataSet versionDataSet)
		{
			if (versionDataSet.Tables["tblVersion"].Rows[0]["fldExecutableFile"].ToString() != "")
			{
				DataSet xmlDataSet = new DataSet();
				xmlDataSet.ReadXmlSchema(GetXMLSchemaStream());

				DataRow moduleRow = xmlDataSet.Tables["Module"].NewRow();
				moduleRow["ModuleLSID"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldModuleLSID"];
				moduleRow["Name"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldModuleName"];
				moduleRow["Description"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldModuleDescription"];
				xmlDataSet.Tables["Module"].Rows.Add(moduleRow);

				DataRow versionRow = xmlDataSet.Tables["Version"].NewRow();
				versionRow["VersionLSID"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldVersionLSID"];
				versionRow["VersionNumber"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldVersionNumber"];
				versionRow["ReleaseDate"] = DateTime.Parse(versionDataSet.Tables["tblVersion"].Rows[0]["fldReleaseDate"].ToString());
				versionRow["NeuralEntity"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldNeuralEntity"];
				versionRow["Abstract"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldAbstract"];
				versionRow["Description"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldDescription"];
				versionRow["Design"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldDesign"];
				versionRow["Implementation"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldImplementation"];
				versionRow["Interface"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldInterface"];
				versionRow["Evaluation"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldEvaluation"];
				versionRow["Prerequisite"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldPrerequisite"];
				versionRow["Note"] = versionDataSet.Tables["tblVersion"].Rows[0]["fldNote"];
				xmlDataSet.Tables["Version"].Rows.Add(versionRow);

				string[] groupTable = new string[] { "Languages", "OSes", "Compliances", "Histories",
													   "References", "Bugs", "Restrictions" };
				string[] singleTable = new string[] { "Language", "OS", "Compliance", "History",
														"Reference", "Bug", "Restriction" };
				string[] dataTable = new string[] { "tblVersionLanguage", "tblVersionOS", "tblVersionCompliance", "tblHistory",
													  "tblReference", "tblBug", "tblRestriction" };
				string[] fieldTable = new string[] { "fldLanguage", "fldOS", "fldCompliance", "fldItem",
													   "fldItem", "fldItem", "fldItem" };

				for(int i=0; i<groupTable.Length; i++)
				{
					if (versionDataSet.Tables[dataTable[i]].Rows.Count > 0)
					{
						DataRow groupRow = xmlDataSet.Tables[groupTable[i]].NewRow();
						groupRow.SetParentRow(versionRow, xmlDataSet.Relations["Version_" + groupTable[i]]);
						xmlDataSet.Tables[groupTable[i]].Rows.Add(groupRow);
						foreach (DataRow row in versionDataSet.Tables[dataTable[i]].Rows)
						{
							DataRow singleRow = xmlDataSet.Tables[singleTable[i]].NewRow();
							singleRow.SetParentRow(groupRow, xmlDataSet.Relations[groupTable[i] + "_" + singleTable[i]]);
							singleRow[singleTable[i] + "_Column"] = row[fieldTable[i]];
							xmlDataSet.Tables[singleTable[i]].Rows.Add(singleRow);
						}
					}
				}

				if (versionDataSet.Tables["tblClassification"].Rows.Count > 0)
				{
					DataRow classificationsRow = xmlDataSet.Tables["Classifications"].NewRow();
					classificationsRow.SetParentRow(versionRow, xmlDataSet.Relations["Version_Classifications"]);
					xmlDataSet.Tables["Classifications"].Rows.Add(classificationsRow);
					foreach (DataRow row in versionDataSet.Tables["tblClassification"].Rows)
					{
						DataRow classificationRow = xmlDataSet.Tables["Classification"].NewRow();
						classificationRow.SetParentRow(classificationsRow, xmlDataSet.Relations["Classifications_Classification"]);
						classificationRow["Structure"] = row["fldStructure"];
						classificationRow["Substructure"] = row["fldSubstructure"];
						classificationRow["CellType"] = row["fldCellType"];
						xmlDataSet.Tables["Classification"].Rows.Add(classificationRow);
					}
				}

				if (versionDataSet.Tables["tblVersionDataType"].Rows.Count > 0)
				{
					DataRow datatypesRow = xmlDataSet.Tables["DataTypes"].NewRow();
					xmlDataSet.Tables["DataTypes"].Rows.Add(datatypesRow);
					foreach (DataRow row in versionDataSet.Tables["tblVersionDataType"].Rows)
					{
						DataRow datatypeRow = xmlDataSet.Tables["DataType"].NewRow();
						datatypeRow.SetParentRow(datatypesRow, xmlDataSet.Relations["DataTypes_DataType"]);
						datatypeRow["Name"] = row["fldName"];
						datatypeRow["Index"] = row["fldIndex"];
						datatypeRow["Description"] = row["fldDescription"];
						datatypeRow["Direction"] = row["fldDirection"];
						datatypeRow["Type"] = row["fldDataType"];
						xmlDataSet.Tables["DataType"].Rows.Add(datatypeRow);
					}
				}

				if (versionDataSet.Tables["tblParameter"].Rows.Count > 0)
				{
					DataRow parametersRow = xmlDataSet.Tables["Parameters"].NewRow();
					xmlDataSet.Tables["Parameters"].Rows.Add(parametersRow);
					foreach (DataRow row in versionDataSet.Tables["tblParameter"].Rows)
					{
						DataRow parameterRow = xmlDataSet.Tables["Parameter"].NewRow();
						parameterRow.SetParentRow(parametersRow, xmlDataSet.Relations["Parameters_Parameter"]);
						parameterRow["Name"] = row["fldName"];
						parameterRow["Description"] = row["fldDescription"];
						parameterRow["Type"] = row["fldParameterType"];
						parameterRow["Minimum"] = row["fldMinimum"];
						parameterRow["Maximum"] = row["fldMaximum"];
						xmlDataSet.Tables["Parameter"].Rows.Add(parameterRow);
					}
				}
				
				return xmlDataSet;
			}
			else
				return null;
		}

		private static void xmlValidatingReader_ValidationEventHandler(object sender, System.Xml.Schema.ValidationEventArgs e)
		{
			throw new Exception(e.Message);
		}
	}
}
