using System;
using System.Collections.Generic;
using System.Text;

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


        public RunfileInformation(XmlNode theRunfileInfoNode)
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
            RunfileInformationEditDate = RunfileInformationNode["EditDate"].InnerText;
        }
    }


}
