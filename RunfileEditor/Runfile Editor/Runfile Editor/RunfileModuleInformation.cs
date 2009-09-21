using System;
using System.Collections.Generic;
using System.Text;

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
