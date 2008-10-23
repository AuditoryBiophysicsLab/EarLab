using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace SmartPropertyGridTester
{
    public class EarlabModuleXML
    {
        public readonly EarlabModuleXMLInfo ModuleInfo;
        readonly List<EarlabModuleXMLParameter> mParameters;

        public EarlabModuleXML(XmlElement moduleElement)
        {
            ModuleInfo = new EarlabModuleXMLInfo(moduleElement["ModuleInformation"]);
            foreach (XmlNode curParam in moduleElement.FirstChild)
            {
            }
        }

        public List<EarlabModuleXMLParameter> Parameters { get { return mParameters; } }
    }

    public class EarlabModuleXMLInfo
    {
        public readonly string ExecutableName;

        public EarlabModuleXMLInfo(XmlElement infoElement)
        {
            ExecutableName = infoElement["ExecutableName"].Value;
        }
    }

    class EarlabModuleXMLParameter
    {
        public readonly string Name;
        public readonly string DataType;
        public readonly string Default;
        public readonly string Minimum;
        public readonly string Maximum;
        public readonly string Description;
        public readonly string Units;
        public EarlabModuleXMLParameter(XmlNode paramNode)
        {
            Name = paramNode["Name"].Value;
            DataType = paramNode["Type"].Value;
            Default = paramNode["Default"].Value;
            Minimum = paramNode["Minimum"].Value;
            Maximum = paramNode["Maximum"].Value;
            Description = paramNode["Description"].Value;
            Units = paramNode["Units"].Value;
        }
    }

    public class EarlabModuleXMLInput
    {
    }

    public class EarlabModuleXMLOutput
    {
    }
}
