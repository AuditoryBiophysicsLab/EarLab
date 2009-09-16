using System;
using System.Text;
using VisualHint.SmartPropertyGrid;
using System.Globalization;
using System.Resources;
using System.Reflection;
using System.Collections.Generic;
using System.Xml;

#region Notes on File
//These classes take a Runfile in XML and convert them to a Runfile Object containing strings.
//1.) RunfileXMLtoString
//2.) RunfileString
//3.) Runfile

//-------------------------------------------------------------------->
/*
Runfile XML to String Object 
1.) Meta Data
2.) Module
3.) Input
4.) Output
5.) Parameter (info as strings)
*/
#endregion

namespace RunfileEditor
{
    #region Notes on EarlabRunfile Class
    // Runfile Data Structure
    //      ----> (a class RunfileInformation with data properties) a single RunfileInformation
    //      ----> (a class that is the list of Module Descriptors) used to create data params
    //      ----> (a class EarlabModule w/ string I-O-P and data I-O-P) a list of EarlabModules
    //      ----> (a Module Count, uses RF ModuleDescriptors) a count of the # of Modules
    //      ---------------->(Module: MetaData, list of Inputs, list of Outputs, list of Params)
    //
    //
    //This is the RunfileInformation
    //Contains the metadata about the Runfile Model that is used

    #endregion

    public class EarlabRunfile
    {

        #region Data Members
        //--------------------------------------------------------------------->
        // Data Memembers & Property Methods of Runfile
        //
        //XML Vars
        //There is the actual XmlDocument that has been validated
        public XmlDocument RunfileXML;
        //
        //
        //Still a bit unsure about this, but this is the Runfile xml Node
        public XmlNode RunfileModulesNode;
        //
        //
        //The title of the document file of the XML in string form
        public string RunfileXMLString;
        //
        //Has the Runfile changed?
        private bool changed1 = false;
        //
        //These are the unique EarlabModule titles that are needed from the EFI
        //(?) Something back from EFI
        public ModuleDirectory ModuleDirectory;
        //
        //
        //Runfile Data Structures
        //
        //Run File Information -- contains informaiton about the model
        public RunfileInformation RunfileInformation;
        //
        //This is the list of Modules contained in the Runfile
        public List<RunfileModule> RunfileModules = new List<RunfileModule>();
        //
        //Modules carry two data structures: (XML data as strings),(XML data as the appropriate value)
        public List<EarlabModule> EarlabModules = new List<EarlabModule>();
        //
        #endregion

        #region Constructors
        //-------------------------------------------------------------------->
        //Constructors

        public EarlabRunfile()
        {
        }

        public EarlabRunfile(XmlDocument Runfile)
        {
            Initialize(Runfile);
        }

        public void Initialize(XmlDocument Runfile)
        {

            //code --- calls to other small classes
            //1.) Create Run File Information using RunfileInformation class
            //XmlNode, RunfileInformation
            //the indexing method returns a xmlnode list
            //perhaps a better way to do this.                            
            XmlNodeList XList = Runfile.GetElementsByTagName("RunfileInformation");
            RunfileInformation = new RunfileInformation(XList[0]);

            //2.) Run File Modules that are used
            //RunfileDescriptors -> ModuleData
            //(?)I'm not sure these "string data modules are needed"
            //(xml statement) --Additional information: Object reference not set to an instance of an object.
            //Reformat the xml statement!!*****
            XmlNodeList MList = Runfile.GetElementsByTagName("Module");
            foreach (XmlNode theModule in MList)
            {
                RunfileModules.Add(new RunfileModule(theModule));
            }
            //-----------------------
            //3.) Hey EFI give me those Module XMLs i want.
            //(?) For now use the contructor on ModuleDirectory
            //ModuleDirectory interacts with the EFI and gets what is necessary
            ModuleDirectory = new ModuleDirectory(UniqueEarlabModuleXMLs);

            //4.) Run File -- Earlab Modules using EarlabModule and loop
            //RunfileDescriptors + ModuleData => Factories -------> SPGs in GUI
            //The logical separation in this is a bit dicey.....
            //(?) Not sure rather to have this here, or put it somewhere else

            foreach (RunfileModule sModule in RunfileModules)
            {
                //Label the proper Module Name
                string ModuleName = sModule.ModuleInformation.ExecutableName.ToLower();

                //Send that RunfileModule and the ModuleXML Module
                //(xml statement) 
                EarlabModules.Add(new EarlabModule(sModule, ModuleDirectory[ModuleName]));
            }
        }
        #endregion Constructors

        #region Properties


        //public XmlDocument RunfileXml
        //   {
        //       set
        //       {
        //           XmlDocument myDocument = new XmlDocument();
        //           myDocument.LoadXml(value);
        //           this.XmlDocument = myDocument;
        //       }

        //       get
        //       {
        //           //XmlDocument myDocument = this.XmlDocument;
        //           //MemoryStream output = new MemoryStream();
        //           //XmlTextWriter myWriter = new XmlTextWriter(output, Encoding.UTF8);
        //           //myWriter.Formatting = Formatting.Indented;
        //           //myWriter.WriteProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
        //           //myWriter.WriteStartElement("Root");
        //           ////myWriter.Close();
        //           //StreamReader converter = new StreamReader(output);
        //           //string text = converter.ReadToEnd();
        //           return RunfileXML;
        //       }

        //   }

        //public XmlDocument XmlDocument
        //     {
        //         set { Initialize(value); }
        //         get { return EarlabRunfileCreate(); }
        //     }


        public int ModuleCount { get { return RunfileModules.Count; } }

        // need to write the set
        public bool HasChanged
        {
            get { return changed1; }

            set
            {
                //check all parts of Runfile 
                //in a generic fashion



            }

        }

        //info
        ///<summary>
        /// This is some stuff
        /// 
        ///
        ///</summary>
        public string[] UniqueEarlabModuleXMLs
        {
            //compare all theRunfileModuleDescriptors against each other.
            //if it is not listed keep it, otherwise don't add it.

            get
            {
                //1st shot doest not work, you can't add to a collection that's in a foreach loop.
                //Internal data memebers
                List<string> Uniques = new List<string>();


                //sorted in order
                foreach (RunfileModule RfMd in RunfileModules)
                {
                    bool foundit = false;

                    foreach (string s in Uniques)
                    {
                        if (s == RfMd.ModuleInformation.ExecutableName)
                        {
                            foundit = true;
                            break;
                        }
                    }
                    if (!foundit)
                        Uniques.Add(RfMd.ModuleInformation.ExecutableName);
                }

                return Uniques.ToArray();
            }
        }


        public RunfileModule this[int ModuleIndex]
        {
            get
            {
                if ((ModuleIndex < 0) || (ModuleIndex >= RunfileModules.Count))
                    throw new IndexOutOfRangeException("The requested module at index " + ModuleIndex + " was not found in the Runfile");
                return RunfileModules[ModuleIndex];
            }
        }
        //

        public RunfileModule this[string ModuleName]
        {
            get
            {
                //(**)
                //Search for the Matching moduleName
                foreach (RunfileModule currentModule in RunfileModules)
                {
                    if (currentModule.ModuleInformation.ExecutableName.ToLower() == ModuleName)
                        return currentModule;
                }
                throw new IndexOutOfRangeException("The requested module executable \"" + ModuleName + "\" was not found in the Runfile");
            }
        }
        //
        #endregion

        #region Misc. Notes
        //---------------------------------------------------------------------->
        // Methods

        //Updates the Runfile an XML Doc, Powers the Constructor that takes the Runfile Xml Document as well.
        //Has a data in string component and a data in data component
        //public void RunfileUpdateXML(XmlDocument theRunfile)



        /*
               //Updates the Runfile from a String Doc, Powers the similar Constructor
               public void RunfileUpdateString(String theRunfile)
               {
                   //private data

                   //code

               }
       */
        //------------------->


        //------------------------------------------------------------------------------------------->
        // More Smaller Classes that belong to the Runfile System is dependent on.
        /*
         * RunfileInformation
         * RunfileModuleDescriptor
  

         * 
        */
        //Takes information from Runfile Information node and works it
        /*
           <RunfileInFormation>
                Author
         *      Abstract
         *      Date
         *      Image Location
          </RunfileInFormation>
        */
        //--------------------------------------------------------------------------//
        //This section deals with writing a Runfile
        //Cases to use
        //1.) Write New Document
        //2.) Rewrite old Document
        //---------------//

        //


        //Basically creates a new XML Document to save it.





        /*
         * 
         * 
         */
        #endregion

        //Need to divide this guy up 

        public XmlDocument RunfileXMLCreate()
        {
            XmlDocument NewRunfile = new XmlDocument();

            //1.) Top Header
            //<?xml version="1.0" encoding="utf-8" ?>
            NewRunfile.AppendChild(NewRunfile.CreateXmlDeclaration("1.0", "utf-8", ""));

            //2.) Routine to Write Runfile header
            XmlNode root = NewRunfile.CreateElement("Runfile");

            NewRunfile.AppendChild(root);

            #region  RunfileInformation XML Node
            //2.) Start modules -- open module tag
            /*
                    <Runfile>
                        <RunfileInformation>
                          <Author> Blah Smith </Author>
                          <Abstract> afldlkdklasdjsad </Abstract>
                          <EditDate> 1/1/09 </EditDate>
                          <ImageLocation> /image/image.jpg </ImageLocation>
                        </RunfileInformation>
                    <Modules>
             */
            #endregion

            #region RunfileInformation Code

            XmlNode eRunfileInformation = NewRunfile.CreateElement("RunfileInformation");
            root.AppendChild(eRunfileInformation);

            //Author ---
            XmlNode eRFIAuthor = NewRunfile.CreateElement("Author");
            eRFIAuthor.InnerText = this.RunfileInformation.RunfileInformationAuthor.ToString();

            XmlNode eRFIAbstract = NewRunfile.CreateElement("Abstract");
            eRFIAbstract.InnerText = this.RunfileInformation.RunfileInformationAbstract.ToString();

            XmlNode eRFIEditDate = NewRunfile.CreateElement("EditDate");
            eRFIEditDate.InnerText = this.RunfileInformation.RunfileInformationEditDate.ToString();

            XmlNode eRFIImageLocation = NewRunfile.CreateElement("ImageLocation");
            eRFIImageLocation.InnerText = this.RunfileInformation.RunfileInformationImageLocation.ToString();

            eRunfileInformation.AppendChild(eRFIAuthor);
            eRunfileInformation.AppendChild(eRFIAbstract);
            eRunfileInformation.AppendChild(eRFIEditDate);
            eRunfileInformation.AppendChild(eRFIImageLocation);
            ///-----------------------------------------------------------------------------------------------------|
            #endregion

            #region ModuleInformation Node Notes
            //3.) Write modules
            //foreach write module
            //module write method

            //method to write module information

            //method to write module I-O-P

            /* 
                     <ModuleInformation>
                      <InstanceName>Left_Pinna</InstanceName>
                      <ExecutableName>DataSource</ExecutableName>
                    </ModuleInformation>
             */
            #endregion

            XmlNode ModulesRoot = NewRunfile.CreateElement("Modules");
            root.AppendChild(ModulesRoot);


            foreach (EarlabModule Module in this.EarlabModules)
            {

                XmlNode ModuleRoot = NewRunfile.CreateElement("Module");

                #region ModuleInformation Write Method
                //Module Info
                XmlNode eModuleInfo = NewRunfile.CreateElement("ModuleInformation");
                ModuleRoot.AppendChild(eModuleInfo);

                XmlNode eMName = NewRunfile.CreateElement("InstanceName");
                eMName.InnerText = Module.theEarlabModuleInformation.InstanceName.ToString();
                eModuleInfo.AppendChild(eMName);

                XmlNode eName = NewRunfile.CreateElement("ExecutableName");
                eName.InnerText = Module.theEarlabModuleInformation.ExecutableName.ToString();
                eModuleInfo.AppendChild(eName);
                // Mod. end
                #endregion

                #region Inputs && Outputs Methods
                XmlNode eInputs = NewRunfile.CreateElement("Inputs");

                foreach (EarlabInput elIn in Module.EarlabInputs)
                {



                }
                ModuleRoot.AppendChild(eInputs);

                XmlNode eOutputs = NewRunfile.CreateElement("Outputs");
                foreach (EarlabOutput elOut in Module.EarlabOutputs)
                {


                }
                ModuleRoot.AppendChild(eOutputs);
                #endregion


                //Sample parameter -- we have to cast it to get value
                XmlNode eParams = NewRunfile.CreateElement("Parameters");

                foreach (EarlabParameter elParam in Module.EarlabParameters)
                {
                    //XmlNode thing!! damn it :|
                    //problem with the node that is returned.
                    //XmlNode TempNode = NewRunfile.CreateElement("Parameter");

                    XmlNode ParamRoot = NewRunfile.CreateElement("Parameter");


                    XmlNode ePName = NewRunfile.CreateElement("Name");
                    ePName.InnerText = elParam.PName.ToString();
                    ParamRoot.AppendChild(ePName);

                    XmlNode ePType = NewRunfile.CreateElement("Type");
                    ePType.InnerText = elParam.PType.ToString();
                    ParamRoot.AppendChild(ePType);

                    //////////////////////////////////////////////////// Value is hard part/////

                    string TestType = ePType.InnerXml.ToString();
                    TestType = TestType.ToLower();

                    XmlNode ePValue = NewRunfile.CreateElement("Value");
                    //ParamRoot.AppendChild(ePValue);

                    switch (TestType)
                    {

                        case "integer":
                        case "int":


                            ePValue.InnerText = ((EarlabParameterInteger)elParam).PValue.ToString();
                            ParamRoot.AppendChild(ePValue);
                            break;


                        case "float":
                        case "double":
                        case "dbl":
                        case "fl":

                            ePValue.InnerText = ((EarlabParameterDouble)elParam).PValue.ToString();
                            ParamRoot.AppendChild(ePValue);

                            break;


                        case "str":
                        case "string":

                            ePValue.InnerText = ((EarlabParameterString)elParam).PValue.ToString();
                            ParamRoot.AppendChild(ePValue);
                            break;



                        case "bool":
                        case "boolean":

                            ePValue.InnerText = ((EarlabParameterBoolean)elParam).PValue.ToString();
                            ParamRoot.AppendChild(ePValue);
                            break;


                        case "integer[]":
                        case "int[]":
                            //only for ease of use
                            EarlabParameterIntegerArray IntParam = ((EarlabParameterIntegerArray)elParam);


                            //to covert the array to a node of element tags that contain values
                            //trying to figure out how to encapsulate this into a method.
                            for (int counter = 0; counter < IntParam.PValue.Length; counter++)
                            {
                                XmlNode Element1 = NewRunfile.CreateElement("Element");
                                Element1.InnerText = IntParam.PValue[counter].ToString();
                                ePValue.AppendChild(Element1);
                            }

                            ParamRoot.AppendChild(ePValue);
                            break;

                        case "double[]":
                        case "float[]":
                        case "dbl[]":
                        case "fl[]":
                            //only for ease of use
                            EarlabParameterDoubleArray DblParam = ((EarlabParameterDoubleArray)elParam);


                            //to covert the array to a node of element tags that contain values
                            //trying to figure out how to encapsulate this into a method.
                            for (int counter = 0; counter < DblParam.PValue.Length; counter++)
                            {
                                XmlNode Element1 = NewRunfile.CreateElement("Element");
                                Element1.InnerText = DblParam.PValue[counter].ToString();
                                ePValue.AppendChild(Element1);
                            }

                            ParamRoot.AppendChild(ePValue);
                            break;


                        default:
                            //fragged 
                            break;
                    }
                    eParams.AppendChild(ParamRoot);

                }//end the massive foreach loop -- 
                ModuleRoot.AppendChild(eParams);


                //Added all the I-O-Ps
                ModulesRoot.AppendChild(ModuleRoot);
            }



            //5.) Check File against schema


            //6.) Send file back
            return NewRunfile;
        }

    }

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

    #region ModuleInformation Notes
    //Takes in the ModuleInformation Node on a module and gives a data structure
    /*
          <ModuleInformation>
            <InstanceName>Left_Pinna</InstanceName>
            <ExecutableName>DataSource</ExecutableName>
          </ModuleInformation>
        
     */
    #endregion

    #region RunfileModule Notes
    /*
    RunfileModule  -- contains all data from XML in Object form, in the string format.
    
     * RunfileModuleInformation -- contains module info
     * RunfileInputs
     * RunfileOutputs
     * RunfileParameters
 
    */
    #endregion

    #region Data Classes for RunfileModule

    //public class RunfileModuleDescriptor
    //{
    //    //Data members
    //    public RunfileModuleInformation ModuleInformation;
    //    public List<RunfileParameter> RunfileParameters = new List<RunfileParameter>();

    //    public RunfileModuleDescriptor(XmlNode ModuleNode)
    //    {
    //        //1.) Get Module Information using RunfileModuleInformation class
    //        //(xml statement) 
    //        ModuleInformation = new RunfileModuleInformation(ModuleNode["ModuleInformation"]);

    //        //2.) Get Run File Parameters using foreach and RunfileParameter class
    //        //(xml statement) //doesn't work!
    //        foreach (XmlNode ParameterNode in ModuleNode["Parameters"].ChildNodes)
    //        {
    //            //1.) Create a Runfile Parameter and add it to list
    //            RunfileParameters.Add(new RunfileParameter(ParameterNode));

    //        }
    //    }
    //}

    public class RunfileModule
    {
        #region Data Members
        public XmlNode theRunfileModuleNode;

        public RunfileModuleInformation ModuleInformation;

        public List<RunfileInput> theRunfileInputs = new List<RunfileInput>();

        public List<RunfileOutput> theRunfileOutputs = new List<RunfileOutput>();

        public List<RunfileParameter> RunfileParameters = new List<RunfileParameter>();
        #endregion

        #region Constructors

        public RunfileModule()
        {
        }

        public RunfileModule(XmlNode ModuleNode)
        {
            theRunfileModuleNode = ModuleNode;

            /*
            1 get rf module info -- 
             * use constructor (grab proper node) and store in in the public data member
             * 
             * 2 get I-O-Ps these are just foreach loops.
             * 
             * e.g. get P --- send parameters node.
                    add at end of each loop
             * 

            */
            //1.)constuctor to R F M I
            //theRunfileModuleInformation = new RunfileModuleInformation(ModuleNode["ModuleInformation"]);

            //Xpath info ...
            //(xml statement) 

            ModuleInformation = new RunfileModuleInformation(ModuleNode["ModuleInformation"]);


            //2.) I - O - P
            //Need to add error protection in front of each of these
            //(?)Possibly rewrite as a generic class? Is it worth it?

            //Inputs
            //(xml statement) 
            //must be fixed
            foreach (XmlNode eInput in ModuleNode["Inputs"].ChildNodes)
            {
                theRunfileInputs.Add(new RunfileInput(eInput));
            }

            //Outputs
            //(xml statement) 
            //must be fixed
            foreach (XmlNode eOutput in ModuleNode["Outputs"].ChildNodes)
            {
                theRunfileOutputs.Add(new RunfileOutput(eOutput));
            }

            // DOM to grab the parameter in the parameters node
            //(xml statement) 

            //
            //XmlNodeList ePList = ModuleNode["Parameters"].ChildNodes;
            foreach (XmlNode eParameter in ModuleNode["Parameters"].ChildNodes)
            {
                RunfileParameters.Add(new RunfileParameter(eParameter));
            }
        }

        #endregion
    }

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

    public class RunfileInput
    {

        public RunfileInput(XmlNode Input)
        {
        }



    }

    public class RunfileOutput
    {
        public RunfileOutput(XmlNode Output)
        {
        }
    }

    public class RunfileParameter
    {

        //Store the Node location/value rather than the string data.
        //

        /*
         * double -- double -- dbl
        <Parameter>
            <Name>SampleRate</Name>
            <Type>Double</Type>
            <Value>44100</Value>
        </Parameter>
            
         * boolean - bool 
       <Parameter>
            <Name>InputIsWaveFile</Name>
            <Type>Boolean</Type>
            <Value>true</Value>
       </Parameter>
         * 
       integer int 
       <Parameter>
            <Name>ChannelNumber</Name>
            <Type>Integer</Type>
            <Value>0</Value>
       </Parameter>
         * 
     </Parameter>   
        <Name>IntArrayParam</Name>
        <Type>Integer[]</Type>
        <Value>
         <Element>1</Element>
         <Element>2</Element>
         <Element>3</Element>
         <Element>4</Element>
         <Element>5</Element>
         <Element>6</Element>
       </Value>
      </Parameter>
       */

        //Data Memembers
        public bool isArray;
        public string ParameterName;
        public string ParameterType;

        //Just a public node, which comprises Array or Single value
        public XmlNode ParameterValue;

        //Constructor
        public RunfileParameter(XmlNode ParameterNode)
        {
            //string TempValue;

            //(xml statement) 
            ParameterName = ParameterNode["Name"].InnerText;
            ParameterType = ParameterNode["Type"].InnerText;

            //basically we need a quick function to strip out all the white space
            ParameterType = ParameterType.Replace(" ", "");
            //Do we need to standardize how the ParameterType ends up stored? as the real or Temp?
            //I like the idea of standardizing the thing. (?)

            //Also, since we are storing this stuff as a string this is an 
            //if control structure as opposed to a switch,
            //(?) Reuse for parsing the data.
            if (ParameterType.ToLower() == "integer[]" || ParameterType.ToLower() == "double[]" || ParameterType.ToLower() == "string[]"
                || ParameterType.ToLower() == "bool[]" || ParameterType.ToLower() == "float[]")
            {
                //This node holds values
                //(xml statement) 
                ParameterValue = ParameterNode["Value"];
                isArray = true;
            }

            else
            {
                //This node holds a single value
                //(xml statement) 
                ParameterValue = ParameterNode["Value"];
                isArray = false;
            }
        }

    }

    #endregion Data Classes for Runfile

}



