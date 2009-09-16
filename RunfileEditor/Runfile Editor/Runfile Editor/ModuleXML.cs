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
    /// <summary>
    /// This produces a directory of the modules given back
    /// from the EFI
    /// </summary>
    public class ModuleDirectory
    {

        #region Data members
        //list of module xml data
        //
        List<ModuleXML> ModuleXmlList = new List<ModuleXML>();
        #endregion 

        //the specific thing casted that goes to the factory
        // xmldata and string data => earparam
        // or

        #region Data Properties
        public ModuleXML this[string ModuleName]
        {

            get
            {
                //ModuleXML theData = null;
                string TempModuleName = ModuleName;

                //change some stuff about
                TempModuleName = TempModuleName.Replace(" ", "");
                TempModuleName = TempModuleName.ToLower();


                //send the module back
                foreach (ModuleXML theModuleXML in ModuleXmlList)
                {
                    if (theModuleXML.theModuleXmlInformation.ModuleXMLExecutablename.ToLower() == ModuleName.ToLower())
                        return theModuleXML;
                }
                //(Error Log)
                throw new IndexOutOfRangeException("The requested module executable \"" + ModuleName + "\" was not found in the Module Directory");
            
            }

        }


        //Kind of annoying that you have to cast it, but better than making three versions.

        #region Eliminated String Index Method
   /*
        public EfiXmlData this[string ModuleName, string IOPName, string IOPType]
        {

            get
            {
                ModuleXML tempModuleXML = null;
                string TempModuleName = ModuleName;

                //change some stuff about
                //TempModuleName = TempModuleName.Replace(" ", "");
                //TempModuleName = TempModuleName.toLower();


                //send the module back
                foreach (ModuleXML theModuleXML in ModuleXmlList)
                {
                    if (theModuleXML.theModuleXmlInformation.ModuleXMLExecutablename.ToLower() == ModuleName.ToLower())
                       tempModuleXML = theModuleXML;
                }
                throw new IndexOutOfRangeException("The requested module executable \"" + ModuleName + "\" was not found in the Module Directory");



                switch (IOPType.ToLower())
                {
                    case "parameter":
                        foreach (ModuleXmlParameter MXParameter in tempModuleXML.theModuleXmlParameters)
                        {
                            if (MXParameter.ParameterType.ToLower() == IOPType.ToLower())
                                return MXParameter;   
                        }
                        return null; //blah blah

                    case "output":
                        
                        foreach (ModuleXmlInput MXInput in tempModuleXML.theModuleXmlInputs)
                        {
                            if (MXInput.InputType.ToLower() == IOPType.ToLower())
                                return MXInput;
                        }
                         
                       // return null; //blah blah

                   case "input":
                        
                        foreach (ModuleXmlOutput MXOutput in tempModuleXML.theModuleXmlOutputs)
                        {
                            if (MXOutput.OutputType.ToLower() == IOPType.ToLower())
                                return MXOutput;
                        }
                        
                    return null; //blah blah

                    //has to be some error here
                }
         return null; // blah blah 

          } 

       }
*/            
         #endregion

        //
        // basically i can use this to return whatever is necessary
        // either the Module or the specific thing.
        // but it will have to be cast first to be used.
        #endregion

        #region Constructors
        //Okay here is a constructor!
        //
        //Takes in Module XML string list
        //For each of these lists might do the following
        //1.) Interact with the EFI
        //2.) Just load them out of the directory for now.
        public ModuleDirectory(string[] UniqueEarlabModuleXMLs)
        {
            string LoadXmlItem;
            //string ModuleXMLDocument;
            //Sends the list of all the Modules we want.
            //theDescriptorList
            //Create the proper path as a 
            //C:\Documents and Settings\Jon\My Documents\Visual Studio 2005\Projects\Earlab-SPG-Tester-JLW\WindowsApplication4\XML Docs\DataSource.xml'.
            //C:\Documents and Settings\Jon\My Documents\Visual Studio 2005\Projects\Runfile Editor\Runfile Editor\XML Docs
            //1.) Initize and Load XML Document

            int count = 0;
            string ModuleXmlPathStart;
            string ModuleXmlPath;
            ModuleXmlPathStart = "C:\\Documents and Settings\\Jon\\My Documents\\Visual Studio 2005\\Projects\\Runfile Editor\\Runfile Editor\\XML Docs\\";    //+ LoadXmlItem.ToString() + ".xml";



            //replace in foreach
            while (count < UniqueEarlabModuleXMLs.Length )
            {
                //1.) Get Name
                XmlDocument ModuleXMLDocument = new XmlDocument();
                LoadXmlItem = UniqueEarlabModuleXMLs[count];

                ////Create the proper path as a 
                ////C:\Documents and Settings\Jon\My Documents\Visual Studio 2005\Projects\Earlab-SPG-Tester-JLW\WindowsApplication4\XML Docs\DataSource.xml'.

                ModuleXmlPath = ModuleXmlPathStart.ToString() + LoadXmlItem.ToString() + ".xml";


                //Add try -- fail error
                        //Load it from ModuleXML directory for now

                //(Problem is here now 3/31)
                        ModuleXMLDocument.Load(ModuleXmlPath);
                

                    //2.) Load Document from Module Directory
                    //ModuleXMLDocument.Load(LoadXmlItem);

                    //2.) Add the Name
                     ModuleXmlList.Add(new ModuleXML(ModuleXMLDocument));

                ///----if not there, then put in as "null"

                count++;
            }
        }

        #endregion


    }

    //We return the data... 
    public class EfiXmlData
    {
        //Maybe some information from the EFI
        //(?)


    }

    //parses module xml into something useful
    public class ModuleXML: EfiXmlData
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
        public class ModuleXmlInformation : EfiXmlData
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
        public class ModuleXmlInput : EfiXmlData
        {


            //public data
            public ModuleXmlInput(XmlNode ModuleInput)
            {

            }

        }

        //(?) needs info
        public class ModuleXmlOutput : EfiXmlData
        {
            //public data

            //!!!
            public ModuleXmlOutput(XmlNode ModuleOutput)
            {

            }

        }

        //(!)works!
        public class ModuleXmlParameter : EfiXmlData
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


