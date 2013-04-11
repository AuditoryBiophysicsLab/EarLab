using System;
using System.IO;
using System.Diagnostics;
using System.Xml;
using System.Xml.Schema;

namespace EarLab
{
	/// <summary>
	/// Summary description for RunFile.
	/// </summary>
	public class RunFile
	{
		public RunRecord [] Run;

		public RunFile(string RunFileName)
		{
			XmlTextReader Reader = new XmlTextReader(RunFileName);
			XmlDocument Doc = new XmlDocument();
			XmlNodeList Runs;

			Reader.WhitespaceHandling = WhitespaceHandling.None;
			Doc.Load(Reader);
			Runs = Doc.GetElementsByTagName("Run");
			Run = new RunRecord [Runs.Count];
			for (int i = 0; i < Runs.Count; i++)
			{
				Run[i] = new RunRecord(Runs[i]);
				//Run[i].Print();
			}
			Reader.Close();
			//Doc.Save(RunFileName);
		}
	}

	public class RunRecord
	{
		public string Name;
		public string Description;
		public string DiagramFileName;
		public string ParameterFileName;
		public string InputDirectoryPath;
		public string OutputDirectoryPath;
		public string [] SourceFileNames;
		public string [] DestinationFileNames;
		public string [] OutputFileNames;
		public int FrameCount;
		public DateTime LastRunDateTime;
		public static string EarLabPath;

		public RunRecord(XmlNode RunNode)
		{

			if (EarLabPath == null)
				EarLabPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Personal), "EarLab");
			Name = RunNode.Attributes.GetNamedItem("name").InnerText;
			Description = RunNode.Attributes.GetNamedItem("description").InnerText;
			DiagramFileName = Path.Combine(EarLabPath, RunNode["DiagramFileName"].InnerText);
			ParameterFileName = Path.Combine(EarLabPath, RunNode["ParameterFileName"].InnerText);
			InputDirectoryPath = Path.Combine(EarLabPath, RunNode["InputDirectoryPath"].InnerText);
			OutputDirectoryPath = Path.Combine(EarLabPath, RunNode["OutputDirectoryPath"].InnerText);
			FrameCount = int.Parse(RunNode["FrameCount"].InnerText);

			SourceFileNames = new string [RunNode["InputFiles"].ChildNodes.Count];
			DestinationFileNames = new string [RunNode["InputFiles"].ChildNodes.Count];
			for (int i = 0; i < SourceFileNames.Length; i++)
			{
				SourceFileNames[i] = RunNode["InputFiles"].ChildNodes[i]["SourceFileName"].InnerText;
				DestinationFileNames[i] = RunNode["InputFiles"].ChildNodes[i]["DestinationFileName"].InnerText;
			}

			OutputFileNames = new string [RunNode["OutputFiles"].ChildNodes.Count];
			for (int i = 0; i < OutputFileNames.Length; i++)
			{
				OutputFileNames[i] = RunNode["OutputFiles"].ChildNodes[i].InnerText;
			}

			if (RunNode["LastRunDateTime"].InnerText != "Never")
				LastRunDateTime = DateTime.Parse(RunNode["LastRunDateTime"].InnerText);
			RunNode["LastRunDateTime"].InnerText = DateTime.Now.ToString();
		}

		public void Print()
		{
			Debug.WriteLine("Name: " + Name);
			Debug.WriteLine("  Description: " + Description);
			Debug.WriteLine("  Diagram File: " + DiagramFileName);
			Debug.WriteLine("  Parameter File: " + ParameterFileName);
			Debug.WriteLine("  Input directory: " + InputDirectoryPath);
			Debug.WriteLine("  Output directory: " + OutputDirectoryPath);
			Debug.WriteLine("  Frame count: " + FrameCount);
			Debug.WriteLine("  Input Files:");
			for (int i = 0; i < SourceFileNames.Length; i++)
			{
				Debug.WriteLine("   Source File " + i + ": " + SourceFileNames[i]);
				Debug.WriteLine("     Dest File " + i + ": " + DestinationFileNames[i]);
			}
			Debug.WriteLine("  Output Files:");
			for (int i = 0; i < OutputFileNames.Length; i++)
				Debug.WriteLine("   Output File " + i + ": " + OutputFileNames[i]);
		}
	}
}
