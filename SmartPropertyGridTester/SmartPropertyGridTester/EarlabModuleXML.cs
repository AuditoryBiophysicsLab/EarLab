using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace SmartPropertyGridTester
{
    public class EarlabModuleDescriptor
    {
        public readonly EarlabModuleInformation EarlabModuleInformation;
        public readonly List<EarlabParameterDefinition> EarlabParameterDefinitions = new List<EarlabParameterDefinition>();

        public EarlabModuleDescriptor(XmlNode moduleElement)
        {
            EarlabModuleInformation = new EarlabModuleInformation(moduleElement["ModuleInformation"]);
            foreach (XmlNode curParam in moduleElement["Parameters"].ChildNodes)
            {
                EarlabParameterDefinitions.Add(new EarlabParameterDefinition(curParam));
            }
        }
    }

    public class EarlabModuleInformation
    {
        public readonly string ExecutableName;

        public EarlabModuleInformation(XmlElement infoElement)
        {
            ExecutableName = infoElement["ExecutableName"].Value;
        }
    }

    public class EarlabParameterDefinition
    {
        public readonly string Name;
        public readonly string DataType;
        public readonly string Default;
        public readonly string Minimum;
        public readonly string Maximum;
        public readonly string Description;
        public readonly string Units;
        public EarlabParameterDefinition(XmlNode paramNode)
        {
            Name = paramNode["Name"].InnerText;
            DataType = paramNode["Type"].InnerText;
            Default = paramNode["Default"].InnerText;
            Minimum = paramNode["Minimum"].InnerText;
            Maximum = paramNode["Maximum"].InnerText;
            Description = paramNode["Description"].InnerText;
            Units = paramNode["Units"].InnerText;
        }
    }

    public class EarlabModuleInputDefinition
    {
    }

    public class EarlabModuleOutputDefinition
    {
    }

    public static class ModuleXMLParser
    {
        public static EarlabModuleDescriptor[] LoadModuleDescription(string ModuleXML)
        {
            int ModuleCount;
            int CurModule = 0;
            EarlabModuleDescriptor[] modules;

            XmlDocument myDoc = new XmlDocument();
            myDoc.LoadXml(ModuleXML);
            ModuleCount = myDoc["Modules"].ChildNodes.Count;

            if (ModuleCount < 1)
                return null;
            modules = new EarlabModuleDescriptor[ModuleCount];

            foreach (XmlNode curModule in myDoc["Modules"].ChildNodes)
            {
                modules[CurModule++] = new EarlabModuleDescriptor(curModule);
            }
            return modules;
        }
    }
}
