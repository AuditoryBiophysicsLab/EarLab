using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace Earlab
{
    public class ModuleDescriptor
    {
        public readonly ModuleInformation ModuleInformation;
        public readonly ParameterDefinition[] ParameterDefinitions;

        public ModuleDescriptor(XmlNode moduleElement)
        {
            int CurParam = 0;

            ModuleInformation = new ModuleInformation(moduleElement["ModuleInformation"]);
            ParameterDefinitions = new ParameterDefinition[moduleElement["Parameters"].ChildNodes.Count];
            foreach (XmlNode curParam in moduleElement["Parameters"].ChildNodes)
            {
                ParameterDefinitions[CurParam++] = new ParameterDefinition(curParam);
            }
        }
    }

    public class ModuleInformation
    {
        public readonly string ExecutableName;

        public ModuleInformation(XmlElement infoElement)
        {
            ExecutableName = infoElement["ExecutableName"].Value;
        }
    }

    public class ParameterDefinition
    {
        public readonly string Name;
        public readonly string DataType;
        public readonly string Default;
        public readonly string Minimum;
        public readonly string Maximum;
        public readonly string Description;
        public readonly string Units;
        public ParameterDefinition(XmlNode paramNode)
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

    public class ModuleInputDefinition
    {
    }

    public class ModuleOutputDefinition
    {
    }

    public static class ModuleXMLParser
    {
        public static ModuleDescriptor[] LoadModuleDescription(string ModuleXML)
        {
            int ModuleCount;
            int CurModule = 0;
            ModuleDescriptor[] modules;

            XmlDocument myDoc = new XmlDocument();
            myDoc.LoadXml(ModuleXML);
            ModuleCount = myDoc["Modules"].ChildNodes.Count;

            if (ModuleCount < 1)
                return null;
            modules = new ModuleDescriptor[ModuleCount];

            foreach (XmlNode curModule in myDoc["Modules"].ChildNodes)
            {
                modules[CurModule++] = new ModuleDescriptor(curModule);
            }
            return modules;
        }
    }
}
