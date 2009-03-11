using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;

namespace Earlab
{
    public class EarlabRunfile
    {
        XmlDocument XmlDocument;
        public List<RunfileModuleDescriptor> RunfileModuleDescriptors = new List<RunfileModuleDescriptor>();

        public EarlabRunfile()
        {
        }

        public EarlabRunfile(string RunfileXML)
        {
            Xml = RunfileXML;
        }

        public EarlabRunfile(TextReader XMLStream)
        {
            string RunfileXML = XMLStream.ReadToEnd();
            Xml = RunfileXML;
        }

        public string[] UniqueModuleExecutableNames
        {
            get
            {
                List<string> results = new List<string>();
                bool foundMatch;

                foreach (RunfileModuleDescriptor curModule in RunfileModuleDescriptors)
                {
                    string curModuleName = curModule.ModuleInformation.ExecutableName;

                    if (results.Count == 0)
                        results.Add(curModuleName);
                    else
                    {
                        foundMatch = false;
                        foreach (string UniqueName in results)
                        {
                            if (UniqueName.ToLower() == curModuleName.ToLower())
                            {
                                foundMatch = true;
                                break;
                            }
                        }
                        if (!foundMatch)
                            results.Add(curModuleName);
                    }
                }
                return results.ToArray();
            }
        }

        public RunfileModuleDescriptor this[string ModuleName]
        {
            get
            {
                foreach (RunfileModuleDescriptor curModule in RunfileModuleDescriptors)
                {
                    if (curModule.ModuleInformation.ExecutableName == ModuleName)
                        return curModule;
                }
                throw new IndexOutOfRangeException("The requested module executable \"" + ModuleName + "\" was not found in the runfile");
            }
        }

        public RunfileModuleDescriptor this[int ModuleIndex]
        {
            get
            {
                if ((ModuleIndex < 0) || (ModuleIndex >= RunfileModuleDescriptors.Count))
                    throw new IndexOutOfRangeException("The requested module at index " + ModuleIndex + " was not found in the runfile");
                return RunfileModuleDescriptors[ModuleIndex];
            }
        }

        public void LoadXml(string XmlFilename)
        {
            StreamReader XMLStream = new StreamReader(XmlFilename);
            string RunfileXML = XMLStream.ReadToEnd();

            Xml = RunfileXML;
        }

        public string Xml
        {
            get
            {
                if (XmlDocument == null)
                    return null;

                StringBuilder sbXml = new StringBuilder();
                XmlWriter xwXml = XmlWriter.Create(sbXml);
                XmlDocument.Save(xwXml);
                return xwXml.ToString();
            }

            set
            {
                XmlDocument = new XmlDocument();
                XmlDocument.LoadXml(value);
                foreach (XmlNode ModuleNode in this.XmlDocument["Modules"].ChildNodes)
                {
                    RunfileModuleDescriptors.Add(new RunfileModuleDescriptor(ModuleNode));
                }
            }
        }
    }

    public class RunfileModuleDescriptor
    {
        private XmlNode mModuleNode;
        public RunfileModuleInformation ModuleInformation;
        public List<RunfileParameter> Parameters = new List<RunfileParameter>();

        public RunfileModuleDescriptor(XmlNode ModuleNode)
        {
            mModuleNode = ModuleNode;
            ModuleInformation = new RunfileModuleInformation(mModuleNode["ModuleInformation"]);
            foreach (XmlNode ParamNode in mModuleNode["Parameters"].ChildNodes)
            {
                Parameters.Add(new RunfileParameter(ParamNode));
            }
        }
    }

    public class RunfileModuleInformation
    {
        private XmlNode mInfoNode;
        private string mInstanceName;
        public readonly string ExecutableName;
        public RunfileModuleInformation(XmlNode InfoNode)
        {
            mInfoNode = InfoNode;
            ExecutableName = InfoNode["ExecutableName"].InnerText;
            mInstanceName = InfoNode["InstanceName"].InnerText;
        }

        public string InstanceName
        {
            get { return mInstanceName; }
            set
            {
                mInstanceName = value;
                mInfoNode["InstanceName"].InnerText = value;
            }
        }
    }

    public class RunfileParameter
    {
        private XmlNode mParamNode;
        public readonly string Name;
        public readonly string DataType;
        public XmlNode ValueNode;
        public bool IsArray;

        public RunfileParameter(XmlNode ParamNode)
        {
            mParamNode = ParamNode;
            Name = ParamNode["Name"].InnerText;
            DataType = ParamNode["Type"].InnerText;
            IsArray = false;
            if (DataType.EndsWith("Array") || DataType.EndsWith("[]"))
                IsArray = true;
            ValueNode = ParamNode["Value"];
        }
    }

    public class RunfileInput
    {
    }

    public class RunfileOutput
    {
    }
}
