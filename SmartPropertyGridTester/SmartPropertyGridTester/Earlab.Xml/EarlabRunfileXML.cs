using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;

namespace Earlab.Xml
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
