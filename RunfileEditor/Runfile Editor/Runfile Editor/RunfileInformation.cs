using System;
using System.Text;
using System.Globalization;
using System.Resources;
using System.Reflection;
using System.Collections.Generic;
using System.Xml;
using System.IO;

namespace RunfileEditor
{

    public class RunfileInformation
    {
        //----------------------------------------------------------->
        //
        //
        public XmlNode RunfileInformationNode;
        public string RunfileInformationAuthor;
        public string RunfileInformationAbstract;
        public string RunfileInformationEditDate;
        public string RunfileInformationImageLocation;

        public RunfileInformation()
        {
        }


        public RunfileInformation(XmlNode theRunfileInfoNode, XmlDocument Runfile)
        {
            //private data

            //code
            //0.) Set Node
            RunfileInformationNode = theRunfileInfoNode;

            //1.) Get Info
            //(xml statement) 
            RunfileInformationAuthor = RunfileInformationNode["Author"].InnerText;
            RunfileInformationAbstract = RunfileInformationNode["Abstract"].InnerText;
            RunfileInformationImageLocation = RunfileInformationNode["ImageLocation"].InnerText;
            //RunfileInformationImageLocation = Path.Combine(Path.GetFileNameWithoutExtension(Runfile.) + RunfileInformationImageLocation);
            RunfileInformationEditDate = RunfileInformationNode["EditDate"].InnerText;

            
        }
    }


}
