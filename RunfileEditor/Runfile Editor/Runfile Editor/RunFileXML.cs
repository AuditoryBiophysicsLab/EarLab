using System;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using VisualHint.SmartPropertyGrid;
using System.Globalization;
using System.Resources;
using System.Reflection;
using System.Collections.Generic;
using System.Xml;
using System.IO;

#region Notes on File
//These classes take a RunFile in XML and convert them to a RunFile Object containing strings.
//1.) RunFileXMLtoString
//2.) RunFileString
//3.) RunFile

//-------------------------------------------------------------------->
/*
RunFile XML to String Object 
1.) Meta Data
2.) Module
3.) Input
4.) Output
5.) Parameter (info as strings)
*/
#endregion

namespace RunfileEditor
{
    #region Notes on EarLabRunFile Class
    // RunFile Data Structure
    //      ----> (a class RunFileInformation with data properties) a single RunFileInformation
    //      ----> (a class that is the list of Module Descriptors) used to create data params
    //      ----> (a class EarLabModule w/ string I-O-P and data I-O-P) a list of EarLabModules
    //      ----> (a Module Count, uses RF ModuleDescriptors) a count of the # of Modules
    //      ---------------->(Module: MetaData, list of Inputs, list of Outputs, list of Params)
    //
    //
    //This is the RunFileInformation
    //Contains the metadata about the RunFile Model that is used

    /*
     * to do
     * schema validation method 
     * 
     * 
    */
    #endregion

    public class EarLabRunFile
    {

        #region Data Members
        //--------------------------------------------------------------------->
        // Data Memembers & Property Methods of RunFile
        //
        //XML Vars
            //There is the actual XmlDocument that has been validated
            public XmlDocument runFileXML;
            //
            //
            //Still a bit unsure about this, but this is the RunFile xml Node
            public XmlNode RunFileModulesNode;
        //
        //
        //The title of the document file of the XML in string form
        public string RunFileXMLString;
        //
        //Has the RunFile changed?
        private bool changed1 = false;
        //
        //These are the unique EarLabModule titles that are needed from the EFI
        //(?) Something back from EFI
        public ModuleDirectory ModuleDirectory;
        //
        //
        //RunFile Data Structures
        //
            //Run File Information -- contains informaiton about the model
            public RunFileInformation RunFileInformation;
            //
            //This is the list of Modules contained in the RunFile
            public List<RunFileModule> RunFileModules = new List<RunFileModule>();
            //
            //Modules carry two data structures: (XML data as strings),(XML data as the appropriate value)
            public List<EarLabModule> EarLabModules = new List<EarLabModule>();
            //
        #endregion

        #region Constructors
        //-------------------------------------------------------------------->
        //Constructors

        public EarLabRunFile()
        {
        }
        
        public EarLabRunFile(XmlDocument RunFile)
        {
            Initialize(RunFile);
        }

            public void Initialize(XmlDocument RunFile)
            {

                //code --- calls to other small classes
                //1.) Create Run File Information using RunFileInformation class
                //XmlNode, RunFileInformation
                //the indexing method returns a xmlnode list
                //perhaps a better way to do this.                            
                XmlNodeList XList = RunFile.GetElementsByTagName("RunFileInformation");
                    RunFileInformation = new RunFileInformation(XList[0]);
              
                //2.) Run File Modules that are used
                //RunFileDescriptors -> ModuleData
                //(?)I'm not sure these "string data modules are needed"
                //(xml statement) --Additional information: Object reference not set to an instance of an object.
                //Reformat the xml statement!!*****
                XmlNodeList MList = RunFile.GetElementsByTagName("Module");
                    foreach (XmlNode theModule in MList)
                    {
                        RunFileModules.Add(new RunFileModule(theModule));
                    }
                //-----------------------
                //3.) Hey EFI give me those Module XMLs i want.
                //(?) For now use the contructor on ModuleDirectory
                //ModuleDirectory interacts with the EFI and gets what is necessary
                ModuleDirectory = new ModuleDirectory(UniqueEarLabModuleXMLs);

                //4.) Run File -- EarLab Modules using EarLabModule and loop
                //RunFileDescriptors + ModuleData => Factories -------> SPGs in GUI
                //The logical separation in this is a bit dicey.....
                //(?) Not sure rather to have this here, or put it somewhere else

                foreach (RunFileModule sModule in RunFileModules)
                    {
                        //Label the proper Module Name
                        string ModuleName = sModule.ModuleInformation.ExecutableName.ToLower();

                        //Send that RunFileModule and the ModuleXML Module
                        //(xml statement) 
                        EarLabModules.Add(new EarLabModule(sModule, ModuleDirectory[ModuleName]));
                    }
            }
        #endregion Constructors

        #region Properties


         //public XmlDocument RunFileXml
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
         //           return runFileXML;
         //       }

         //   }

       //public XmlDocument XmlDocument
       //     {
       //         set { Initialize(value); }
       //         get { return EarLabRunFileCreate(); }
       //     }


        public int ModuleCount { get { return RunFileModules.Count; }}

        // need to write the set
        public bool HasChanged
        {
            get { return changed1; }

            set
            {
                //check all parts of RunFile 
                //in a generic fashion

                  

            }

        }

        //info
        ///<summary>
        /// This is some stuff
        /// 
        ///
        ///</summary>
        public string[] UniqueEarLabModuleXMLs
        {
            //compare all theRunFileModuleDescriptors against each other.
            //if it is not listed keep it, otherwise don't add it.

          get
           {
               //1st shot doest not work, you can't add to a collection that's in a foreach loop.
               //Internal data memebers
               List<string> Uniques = new List<string>();


               //sorted in order
               foreach (RunFileModule RfMd in RunFileModules)
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


        public RunFileModule this[int ModuleIndex]
        {
            get
            {
                if ((ModuleIndex < 0) || (ModuleIndex >= RunFileModules.Count))
                    throw new IndexOutOfRangeException("The requested module at index " + ModuleIndex + " was not found in the runfile");
                return RunFileModules[ModuleIndex];
            }
        }
        //

        public RunFileModule this[string ModuleName]
        {
            get
            {
                //(**)
                //Search for the Matching moduleName
                foreach (RunFileModule currentModule in RunFileModules)
                {
                    if (currentModule.ModuleInformation.ExecutableName.ToLower() == ModuleName)
                        return currentModule;
                }
                throw new IndexOutOfRangeException("The requested module executable \"" + ModuleName + "\" was not found in the runfile");
            }
        }
        //
        #endregion

        #region Misc. Notes
        //---------------------------------------------------------------------->
        // Methods

        //Updates the RunFile an XML Doc, Powers the Constructor that takes the RunFile Xml Document as well.
        //Has a data in string component and a data in data component
        //public void RunFileUpdateXML(XmlDocument theRunFile)



        /*
               //Updates the RunFile from a String Doc, Powers the similar Constructor
               public void RunFileUpdateString(String theRunFile)
               {
                   //private data

                   //code

               }
       */
        //------------------->


        //------------------------------------------------------------------------------------------->
        // More Smaller Classes that belong to the RunFile System is dependent on.
        /*
         * RunFileInformation
         * RunFileModuleDescriptor
  

         * 
        */
        //Takes information from RunFile Information node and works it
        /*
           <RunFileInFormation>
                Author
         *      Abstract
         *      Date
         *      Image Location
          </RunFileInFormation>
        */
        //--------------------------------------------------------------------------//
        //This section deals with writing a RunFile
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

        public XmlDocument RunFileXMLCreate()
        {
            XmlDocument NewRunFile = new XmlDocument();
           
            //1.) Top Header
            //<?xml version="1.0" encoding="utf-8" ?>
            NewRunFile.AppendChild(NewRunFile.CreateXmlDeclaration("1.0", "utf-8", ""));

            //2.) Routine to Write Runfile header
            XmlNode root = NewRunFile.CreateElement("RunFile");

            NewRunFile.AppendChild(root);

            #region  RunFileInformation XML Node 
            //2.) Start modules -- open module tag
            /*
                    <RunFile>
                        <RunFileInformation>
                          <Author> Blah Smith </Author>
                          <Abstract> afldlkdklasdjsad </Abstract>
                          <EditDate> 1/1/09 </EditDate>
                          <ImageLocation> /image/image.jpg </ImageLocation>
                        </RunFileInformation>
                    <Modules>
             */
            #endregion

            #region RunFileInformation Code

            XmlNode eRunFileInformation = NewRunFile.CreateElement("RunFileInformation");
            root.AppendChild(eRunFileInformation);

            //Author ---
            XmlNode eRFIAuthor = NewRunFile.CreateElement("Author");
            eRFIAuthor.InnerText = this.RunFileInformation.RunFileInformationAuthor.ToString();

            XmlNode eRFIAbstract = NewRunFile.CreateElement("Abstract");
            eRFIAbstract.InnerText = this.RunFileInformation.RunFileInformationAbstract.ToString();

            XmlNode eRFIEditDate = NewRunFile.CreateElement("EditDate");
            eRFIEditDate.InnerText = this.RunFileInformation.RunFileInformationEditDate.ToString();

            XmlNode eRFIImageLocation = NewRunFile.CreateElement("ImageLocation");
            eRFIImageLocation.InnerText = this.RunFileInformation.RunFileInformationImageLocation.ToString();

            eRunFileInformation.AppendChild(eRFIAuthor);
            eRunFileInformation.AppendChild(eRFIAbstract);
            eRunFileInformation.AppendChild(eRFIEditDate);
            eRunFileInformation.AppendChild(eRFIImageLocation);
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

            XmlNode ModulesRoot = NewRunFile.CreateElement("Modules");
            root.AppendChild(ModulesRoot);


            foreach (EarLabModule Module in this.EarLabModules)
            {

             XmlNode ModuleRoot = NewRunFile.CreateElement("Module");

                   #region ModuleInformation Write Method 
                //Module Info
                    XmlNode eModuleInfo = NewRunFile.CreateElement("ModuleInformation");
                    ModuleRoot.AppendChild(eModuleInfo);

                    XmlNode eMName = NewRunFile.CreateElement("InstanceName");
                    eMName.InnerText = Module.theEarLabModuleInformation.InstanceName.ToString();
                    eModuleInfo.AppendChild(eMName);

                    XmlNode eName = NewRunFile.CreateElement("ExecutableName");
                    eName.InnerText = Module.theEarLabModuleInformation.ExecutableName.ToString();
                    eModuleInfo.AppendChild(eName);
                    // Mod. end
                #endregion

                   #region Inputs && Outputs Methods
                    XmlNode eInputs = NewRunFile.CreateElement("Inputs");

                    foreach (EarLabInput elIn in Module.EarLabInputs)
                    {



                    }
                ModuleRoot.AppendChild(eInputs);

                XmlNode eOutputs = NewRunFile.CreateElement("Outputs");
                    foreach (EarLabOutput elOut in Module.EarLabOutputs)
                    {


                    }
                ModuleRoot.AppendChild(eOutputs);
                    #endregion


                //Sample parameter -- we have to cast it to get value
                XmlNode eParams = NewRunFile.CreateElement("Parameters");

                foreach (EarLabParameter elParam in Module.EarLabParameters)
                {
                    //XmlNode thing!! damn it :|
                    //problem with the node that is returned.
                    //XmlNode TempNode = NewRunFile.CreateElement("Parameter");

                        XmlNode ParamRoot = NewRunFile.CreateElement("Parameter");


                            XmlNode ePName = NewRunFile.CreateElement("Name");
                            ePName.InnerText = elParam.PName.ToString();
                            ParamRoot.AppendChild(ePName);

                            XmlNode ePType = NewRunFile.CreateElement("Type");
                            ePType.InnerText = elParam.PType.ToString();
                            ParamRoot.AppendChild(ePType);

                    //////////////////////////////////////////////////// Value is hard part/////

                            string TestType = ePType.InnerXml.ToString();
                            TestType = TestType.ToLower();
                            
                            XmlNode ePValue = NewRunFile.CreateElement("Value");
                            //ParamRoot.AppendChild(ePValue);

                                    switch (TestType)
                                    {

                                        case "integer":
                                        case "int":

                                            
                                            ePValue.InnerText = ((EarLabParameterInteger)elParam).PValue.ToString();
                                            ParamRoot.AppendChild(ePValue);
                                            break;


                                        case "float":
                                        case "double":
                                        case "dbl":
                                        case "fl":
                                            
                                            ePValue.InnerText = ((EarLabParameterDouble)elParam).PValue.ToString();
                                            ParamRoot.AppendChild(ePValue);

                                            break;


                                        case "str":
                                        case "string":
                                            
                                            ePValue.InnerText = ((EarLabParameterString)elParam).PValue.ToString();
                                            ParamRoot.AppendChild(ePValue);
                                            break;



                                        case "bool":
                                        case "boolean":
                                           
                                            ePValue.InnerText = ((EarLabParameterBoolean)elParam).PValue.ToString();
                                            ParamRoot.AppendChild(ePValue);
                                            break;


                                        case "integer[]":
                                        case "int[]":
                                            //only for ease of use
                                            EarLabParameterIntegerArray IntParam = ((EarLabParameterIntegerArray)elParam);
                                            

                                            //to covert the array to a node of element tags that contain values
                                            //trying to figure out how to encapsulate this into a method.
                                            for (int counter = 0; counter < IntParam.PValue.Length; counter++)
                                            {
                                                XmlNode Element1 = NewRunFile.CreateElement("Element");
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
                                            EarLabParameterDoubleArray DblParam = ((EarLabParameterDoubleArray)elParam);
                                            

                                            //to covert the array to a node of element tags that contain values
                                            //trying to figure out how to encapsulate this into a method.
                                            for (int counter = 0; counter < DblParam.PValue.Length; counter++)
                                            {
                                                XmlNode Element1 = NewRunFile.CreateElement("Element");
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
            return NewRunFile;
            }

    }

    public class RunFileInformation
    {
        //----------------------------------------------------------->
        //
        //
        public XmlNode RunFileInformationNode;
        public string RunFileInformationAuthor;
        public string RunFileInformationAbstract;
        public string RunFileInformationEditDate;
        public string RunFileInformationImageLocation;
        public string RunFileModelImageLocation;
        public Image RunFileModelImage;

        public RunFileInformation()
        {
        }
        
        //public Bitmap ( string filename ); 

        public RunFileInformation(XmlNode theRunFileInfoNode)
        {
            //private data

            //code
            //0.) Set Node
            RunFileInformationNode = theRunFileInfoNode;

            //1.) Get Info
            //(xml statement) 
            RunFileInformationAuthor = RunFileInformationNode["Author"].InnerText;
            RunFileInformationAbstract = RunFileInformationNode["Abstract"].InnerText;
            RunFileInformationImageLocation = RunFileInformationNode["ImageLocation"].InnerText;
            RunFileInformationEditDate = RunFileInformationNode["EditDate"].InnerText;
            RunFileModelImageLocation = RunFileInformationNode["ImageLocation"].InnerText;

            //ImageLocation
            //Adding the information to handle the image
            //Documents and Settings\\Jon\\My Documents\\Visual Studio 2005\\Projects\\Runfile Editor\\Runfile Editor\\XML Docs\\image\\modelImage.jpg 
            RunFileModelImage = new Bitmap(RunFileModelImageLocation);




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

    #region RunFileModule Notes
    /*
    RunFileModule  -- contains all data from XML in Object form, in the string format.
    
     * RunFileModuleInformation -- contains module info
     * RunFileInputs
     * RunFileOutputs
     * RunFileParameters
 
    */
    #endregion

    #region Data Classes for RunFileModule

    //public class RunfileModuleDescriptor
    //{
    //    //Data members
    //    public RunFileModuleInformation ModuleInformation;
    //    public List<RunFileParameter> RunFileParameters = new List<RunFileParameter>();

    //    public RunfileModuleDescriptor(XmlNode ModuleNode)
    //    {
    //        //1.) Get Module Information using RunFileModuleInformation class
    //        //(xml statement) 
    //        ModuleInformation = new RunFileModuleInformation(ModuleNode["ModuleInformation"]);

    //        //2.) Get Run File Parameters using foreach and RunFileParameter class
    //        //(xml statement) //doesn't work!
    //        foreach (XmlNode ParameterNode in ModuleNode["Parameters"].ChildNodes)
    //        {
    //            //1.) Create a RunFile Parameter and add it to list
    //            RunFileParameters.Add(new RunFileParameter(ParameterNode));

    //        }
    //    }
    //}

    public class RunFileModule
    {
        #region Data Members
        public XmlNode theRunFileModuleNode;

        public RunFileModuleInformation ModuleInformation;

        public List<RunFileInput> theRunFileInputs = new List<RunFileInput>();

        public List<RunFileOutput> theRunFileOutputs = new List<RunFileOutput>();

        public List<RunFileParameter> RunFileParameters = new List<RunFileParameter>();
        #endregion

        #region Constructors

        public RunFileModule()
        {
        }

        public RunFileModule(XmlNode ModuleNode)
        {
            theRunFileModuleNode = ModuleNode;

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
            //theRunFileModuleInformation = new RunFileModuleInformation(ModuleNode["ModuleInformation"]);

            //Xpath info ...
            //(xml statement) 

            ModuleInformation = new RunFileModuleInformation(ModuleNode["ModuleInformation"]);


            //2.) I - O - P
            //Need to add error protection in front of each of these
            //(?)Possibly rewrite as a generic class? Is it worth it?

            //Inputs
            //(xml statement) 
            //must be fixed
            foreach (XmlNode eInput in ModuleNode["Inputs"].ChildNodes)
            {
                theRunFileInputs.Add(new RunFileInput(eInput));
            }

            //Outputs
            //(xml statement) 
            //must be fixed
            foreach (XmlNode eOutput in ModuleNode["Outputs"].ChildNodes)
            {
                theRunFileOutputs.Add(new RunFileOutput(eOutput));
            }

            // DOM to grab the parameter in the parameters node
            //(xml statement) 

            //
            //XmlNodeList ePList = ModuleNode["Parameters"].ChildNodes;
            foreach (XmlNode eParameter in ModuleNode["Parameters"].ChildNodes)
            {
                RunFileParameters.Add(new RunFileParameter(eParameter));
            }
        }

        #endregion
    }

        public class RunFileModuleInformation
    {
        //data memebers
        public string InstanceName;
        public string ExecutableName;

        //code
        public RunFileModuleInformation()
        {
        }
        //
        public RunFileModuleInformation(XmlNode ModuleInformationNode)
        {
            //(xml statement) 
            InstanceName = ModuleInformationNode["InstanceName"].InnerText;
            ExecutableName = ModuleInformationNode["ExecutableName"].InnerText;
        }

    }

        public class RunFileInput
    {

        public RunFileInput(XmlNode Input)
        {
        }



    }

        public class RunFileOutput
    {
        public RunFileOutput(XmlNode Output)
        {
        }
    }

        public class RunFileParameter
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
        public RunFileParameter(XmlNode ParameterNode)
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

    #endregion Data Classes for RunFile

}



