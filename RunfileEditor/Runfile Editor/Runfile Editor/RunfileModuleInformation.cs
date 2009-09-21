using System;
using System.Text;
using VisualHint.SmartPropertyGrid;
using System.Globalization;
using System.Resources;
using System.Reflection;
using System.Collections.Generic;
using System.Xml;

namespace RunfileEditor
{
    public class RunfileModuleInformation
    {
        //data memebers
        public string InstanceName;
        public string ExecutableName;

        //code
        public RunfileModuleInformation()
        {
        }
        //
        public RunfileModuleInformation(XmlNode ModuleInformationNode)
        {
            //(xml statement) 
            InstanceName = ModuleInformationNode["InstanceName"].InnerText;
            ExecutableName = ModuleInformationNode["ExecutableName"].InnerText;
        }

    }
}
