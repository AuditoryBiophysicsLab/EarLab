using System;
using System.Text;
using VisualHint.SmartPropertyGrid;
using System.Windows.Forms;
using System.Globalization;
using System.ComponentModel;
using System.Drawing;
using System.Resources;
using System.Reflection;
using System.Collections.Generic;
using System.Xml;

#region Module XML Notes
/// <summary>
    //<Module>
    //<ModuleInformation>
    //  <ExecutableName>DataSource</ExecutableName>
    //</ModuleInformation>
    //<Inputs></Inputs>
    //<Outputs></Outputs>
    //<Parameters>
    //  <Parameter>
    //    <Name>SampleRate</Name>
    //    <Type>Double</Type>
    //    <Default>0</Default>
    //    <Minimum>1</Minimum>
    //    <Maximum>1000000</Maximum>
    //    <Units>Hz</Units>
    //    <Description>Sample rate, in Hertz</Description>
    //  </Parameter>
    //</Module>

        // public RunfileModuleDescriptor this[string ModuleName]
        //{
        //    get
        //    {
        //        //Search for the Matching moduleName
        //        foreach (RunfileModuleDescriptor currentModule in theRunfileModuleDescriptors)
        //        {
        //            if (currentModule.ModuleInformation.ExecutableName == ModuleName)
        //                return currentModule;
        //        }
        //        throw new IndexOutOfRangeException("The requested module executable \"" + ModuleName + "\" was not found in the Runfile");
        //    }
        //}
 

// (I-O-P) 
// public EarlabParameter this[string ModuleName, string ParamName] 
// or
// public EarlabInput this[string InputName]
// public EarlabOutput this[string OutputName]
// maybe data structure has all modules?


/// </summary>
/// 
#endregion

namespace RunfileEditor
{


    //parses module xml into something useful
    public class ModuleXML
    {
        #region Data Members
        //1.) (!) Meta info
        public ModuleXmlInformation theModuleXmlInformation;
        //2.) (?) Inputs
        public List<ModuleXmlInput> theModuleXmlInputs;
        //3.) (?) Outputs
        public List<ModuleXmlOutput> theModuleXmlOutputs;      
        //4.) (!) Parameters
        public List<ModuleXmlParameter> theModuleXmlParameters = new List<ModuleXmlParameter>();
        #endregion 

        //which one is more useful? index on directory at large
        //or index at each bit.
        //Indexed by Parameter Type, Parameter Name
        /// <summary>
        /// 
        /// </summary>
        /// <param name="ParameterName"></param>
        /// <param name="ParameterType"></param>
        /// <returns></returns>
        #region Data Properties
        public ModuleXmlParameter this[string ParameterName, string ParameterType]
        {


            get
            {
                foreach (ModuleXmlParameter MXLParameter in theModuleXmlParameters)
                {
                    //Check to see if the name matches
                    if ( MXLParameter.ParameterName.ToLower() == ParameterName.ToLower() )
                    {
                        //if the name matches, check to see if type matches
                        if (MXLParameter.ParameterType.ToLower() == ParameterType.ToLower())
                        {
                            //if ParameterName && ParameterType == true return it.
                            return MXLParameter;
                        }
                    }
                }

                //Info!!! Bug here
                // The requested parameter "intarrayparam, integer[]" was not found in the Module XML
                throw new IndexOutOfRangeException("The requested parameter \"" + ParameterName + " , " + ParameterType + "\" was not found in the Module XML");     
            }
        }

        #endregion

        #region Constructors
        //blank constructor
        public ModuleXML()
        {
            //
            // TODO: Add constructor logic here
            //
        }

        //Ok this comes in from the EFI -- 
        //EFI says here's the document you want.
        public ModuleXML(XmlDocument ModuleXML)
        {

           //1.) Handle Meta info
            //(xml statement)
            //theModuleXmlInformation = new ModuleXmlInformation(ModuleXML["ModuleInformation"]);
            XmlNodeList XList = ModuleXML.GetElementsByTagName("ModuleInformation");
            theModuleXmlInformation = new ModuleXmlInformation(XList[0]);


           //2.) Handle I
            //(xml statement) 
            XmlNodeList InList = ModuleXML.GetElementsByTagName("Input");
            foreach(XmlNode Input in InList)
            {
                theModuleXmlInputs.Add(new ModuleXmlInput(Input));
            }

            //2.) Handle O
            //(xml statement)
            XmlNodeList OutList = ModuleXML.GetElementsByTagName("Output");
            foreach (XmlNode Output in OutList)
            {
                theModuleXmlOutputs.Add(new ModuleXmlOutput(Output));
            }

            //2.) Handle P
            //(xml statement) //weird error! on second iteration???
            XmlNodeList PList = ModuleXML.GetElementsByTagName("Parameter");
            foreach (XmlNode Parameter in PList)
            {
                 theModuleXmlParameters.Add(new ModuleXmlParameter(Parameter));
            }



        }
        #endregion


    }
        //(!) Works just like ModuleInformation.
        // end duplicaton or?

    public class ModuleXmlInformation
        {
        /*
        <ModuleInformation>
          <ExecutableName>DataSource</ExecutableName>
        </ModuleInformation
        */

            //works just like RunfileInformation

            #region Data Members
            //public Data
            public string ModuleXMLExecutablename;
            #endregion

            #region Data Properties

            public ModuleXmlInformation(XmlNode ModuleInfo)
            {
                //(xml statement) 
                ModuleXMLExecutablename = ModuleInfo["ExecutableName"].InnerText;
            }
            #endregion

        }

        //(?) needs info
    public class ModuleXmlInput
        {


            //public data
            public ModuleXmlInput(XmlNode ModuleInput)
            {

            }

        }

        //(?) needs info
    public class ModuleXmlOutput
        {
            //public data

            //!!!
            public ModuleXmlOutput(XmlNode ModuleOutput)
            {

            }

        }

        //(!)works!
    public class ModuleXmlParameter
        {
            #region ModuleXMLParameter Notes
            //NEW NOTE!!!
            //Store the value Node
            //And say if it is an array or not!

            /*
           <Parameter>
            <Name>SampleRate</Name>
            <Type>Double</Type>
            <Default>0</Default>
            <Minimum>1</Minimum>
            <Maximum>1000000</Maximum>
            <Units>Hz</Units>
            <Description>Sample rate, in Hertz</Description>
          </Parameter>   
             */

            /* 
              integer int from Runfile
              <Parameter>
                   <Name>ChannelNumber</Name>
                   <Type>Integer</Type>
                   <Value>0</Value>
              </Parameter>
            */
            #endregion

            #region Data Members
                //Public data
                public string ParameterName;
                public string ParameterType;
                public string ParameterUnits;
                public string ParameterDescription;
                public bool isArray;

                //Public Xml Node Data
                public XmlNode ParameterDefaultValue;
                public XmlNode ParameterMinimumValue;
                public XmlNode ParameterMaximumValue;
            #endregion

            #region Constructors
            //--------------------------------->
            public ModuleXmlParameter()
            {
            }

            public ModuleXmlParameter(XmlNode ModuleParameterXml)
            {
                //xml node to vars

                //string Values;
                //(xml statement) 
                    ParameterUnits = ModuleParameterXml["Units"].InnerText;
                    ParameterName = ModuleParameterXml["Name"].InnerText;
                    ParameterType = ModuleParameterXml["Type"].InnerText;
                    ParameterDescription = ModuleParameterXml["Description"].InnerText;

                    //basically we need a quick function to strip out all the white space
                    ParameterType = ParameterType.Replace(" ", "");
                    ParameterName = ParameterName.Replace(" ", "");
                    //Do we need to standardize how the ParameterType ends up stored? as the real or Temp?
                    //I like the idea of standardizing the thing. (?)
                    
                    //Also, since we are storing this stuff as a string this is an 
                    //Might want to write a static class to help with
                    //1.) Array types etc, takes a string in, returns a bool value?
                    if( ParameterType.ToLower() == "integer[]" || ParameterType.ToLower() == "double[]" || ParameterType.ToLower() == "string[]")
                    { isArray = true;
                    XmlNodeList TempValues = ModuleParameterXml["Minimum"].GetElementsByTagName("Value");
                    ParameterMinimumValue = TempValues[0];

                    TempValues = ModuleParameterXml["Maximum"].GetElementsByTagName("Value");
                    ParameterMaximumValue = TempValues[0];

                    TempValues = ModuleParameterXml["Default"].GetElementsByTagName("Value");
                    ParameterDefaultValue = TempValues[0];
                    }
                    else
                    {isArray = false; 
                    
                    ParameterMinimumValue = ModuleParameterXml["Minimum"];
                    ParameterMaximumValue = ModuleParameterXml["Maximum"];
                    ParameterDefaultValue = ModuleParameterXml["Default"];                
                    
                    
                    }
                    
                    //add parameter value
                    //(xml statement) 










                }
            #endregion


        }
}


