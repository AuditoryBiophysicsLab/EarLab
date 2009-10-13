using System;
using System.Text;
using VisualHint.SmartPropertyGrid;
using System.Globalization;
using System.Resources;
using System.Reflection;
using System.Collections.Generic;
using System.Xml;
using System.IO;


namespace RunfileEditor
{

    /// <summary>
    /// This class creates the RunfileObject that contains all the XML information in a class structure.
    /// This also has a directory accessor that allows you to return any object:
    /// 1.) Module
    /// 2.) I-O-P
    /// as a 
    /// </summary>
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
        //These are the errors from the EFI processing the Runfile
        public XmlDocument RunfileVerificationErrors;
        //
        //Still a bit unsure about this, but this is the Runfile xml Node
        //This is the Modules Node that contains all the individual child modules
        public XmlNode RunfileModulesNode;
        //
        //
        //The title of the document file of the XML in string form
        //This is not needed
        //public string RunfileXMLString;
        //
        /// <summary>
        /// The module directory is ...
        /// These are the unique EarlabModule titles that are needed from the EFI
        /// </summary>
        public ModuleDirectory ModuleDirectory;
        //
        //
        private bool changed1 = false;

        //Runfile Data Structures
        //
        /// <summary>
        /// Run File Information -- contains informaiton about the model
        /// </summary>
        public RunfileInformation RunfileInformation;
        //
        /// <summary>
        /// This is the list of Modules contained in the Runfile
        /// </summary>
        public List<RunfileModule> RunfileModules = new List<RunfileModule>();
        /// <summary>
        /// Modules carry two data structures: (XML data as strings),(XML data as the appropriate value)
        /// </summary>
        public List<EarlabModule> EarlabModules = new List<EarlabModule>();
        #endregion

        #region Properties

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
        /// This is a property that accesses the RunfileModule Collection
        /// And gives back a string array of all the unique Module XML titles in the Runfile.
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

        //unneeded
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

        /// <summary>
        /// This allows you to access the Earlabsession object you want by using the path.
        /// 1.) Module
        /// 2.) I-O-P
        /// 3.) Thus you have to cast the object when you want to use it.
        /// </summary>
        /// <param name="VError"></param>
        /// <returns></returns>
        public EarlabSession this[VerificationError VError]
        {
            //problem if module error and parameter error.
            //how to you return the problem twice.
            //no problem, b/c there will be two diffferetn paths
            //VError.ModuleName
            //VError.parameter
            //VError.IOPName

            get
            {
                

                   //string searchString = VError.IOPName;
                   //module
                   if (VError.IOPName == null )
                   {
                       foreach (EarlabModule currentModule in EarlabModules)
                       {
                           if (currentModule.theEarlabModuleInformation.ExecutableName.ToLower() == VError.ModuleName)
                               return currentModule;
                       }

                   }


                    //this is a input-output-parameter
                    //VError.IOPType.ToLower()
                   else
                   {
                       
                       //go through each module
                       foreach (EarlabModule currentModule in EarlabModules)
                       {
                           //We have the right module now
                           if (VError.ModuleName == currentModule.theEarlabModuleInformation.ExecutableName.ToLower())
                           {
                               
                               //Now we need the input output or parameter
                               switch (VError.IOPType.ToLower())
                               {

                                   case "parameter":  
                                           //code pattern for finding the IOP
                                           foreach (EarlabParameter parameter in currentModule.EarlabParameters)
                                           {
                                               if (parameter.PName == VError.IOPName)
                                                   return parameter;

                                               //we have some error
                                               else
                                                   return null;
                                           }
                                           break;

                                   case "input":
                                       
                                           //foreach (EarlabInput input in currentModule.EarlabInputs)
                                           //{
                                           //    if (input.PName == VError.IOPName)
                                           //        TheVar = input;
                                           return null;

                                           //}
                                       
                                       

                                   case "output":
                                       
                                           //foreach (EarlabOutput output in currentModule.EarlabOutputs)
                                           //{
                                           //    if (output.PName == VError.IOPName)
                                           //        TheVar = output;
                                           return null;
                                           //}
                                       
                                   default:
                                        throw new System.NotSupportedException("The Error type " + VError.IOPType + " is not recognized.");

                               }//end switch
                           }//end if

                           //module name is not recognized what would this mean?
                           //else
                           //throw new System.NotSupportedException("The Error type " + VError.ModuleName + " is not recognized.");

                       }//end foreach
                       //this is the same problem as above -- module name not recognized, no reason to have the code twice.
                     throw new System.NotSupportedException("The Error type " + VError.ModuleName + " is not recognized.");
                   }//end else

             throw new System.NotSupportedException("The Full Verification Error type with path" + VError.FullErrorPath + "is not recognized.");
            }
        }

        #endregion
        
        #region Constructors
        //-------------------------------------------------------------------->
        //Constructors

        /// <summary>
        /// Here is the default constructor, it is unused.
        /// </summary>
        public EarlabRunfile()
        {
        }

        /// <summary>
        /// Here is the constructor that is used.
        /// This constructor takes a valid Runfile as input, and then creates the RunfileObject.
        /// The RunfileObject is a collection of objects that comprise the information in the XML Runfile Document
        /// The constructor relies on the intialization method
        /// </summary>
        /// <param name="Runfile"></param>
        public EarlabRunfile(XmlDocument Runfile)
        {
            Initialize(Runfile);
        } 
        #endregion Constructors

        #region This is the Controller Section of the EarlabRunfileObject

        /// <summary>
        /// [Move] This needs to be moved to a static controller class
        /// Takes in XML Document produces Model Object
        /// </summary>
        /// <param name="Runfile"></param>
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
                /// Converts XML Node Module into the collection of ModuleInfo and IOP 
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

        /// <summary>
        /// Send the runfile in to the EFI
        /// Receive the verfication email back from the EFI
        /// </summary>
        /// <param name="Runfile"></param>
        /// <param name="RunfileVerificationErrors"></param>
        /// <returns></returns>
        public bool GetRunfileEFIError(XmlDocument Runfile, out XmlDocument RunfileVerificationErrors)
        {
            //StringWriter sw = new StringWriter(); 
            //XmlTextWriter xw = new XmlTextWriter(sw); 
            //xmlDoc.WriteTo(xw); 
            //return sw.ToString();
            string VErrors;
            RunfileVerificationErrors = null;
        

            if (EFITestWrapper.EFI.VerifyRunfile( XmlUtilities.Xml2Str(Runfile), out VErrors))
            {
                //some errors
                
                RunfileVerificationErrors.Load( new StringReader(VErrors) );
                return true;
            }
            else
            {
                //no errors
                RunfileVerificationErrors = null;
                return false;

            }
            //public static bool VerifyRunfile(string RunfileXML, out string VerificationXML)


        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="VError"></param>
        /// <returns></returns>
        public bool RunfileObjectErrorUpdate(VerificationError VError)
        {

                   //string searchString = VError.IOPName;
                   //module
                   if (VError.IOPName == null )
                   {
                       foreach (EarlabModule currentModule in EarlabModules)
                       {
                           if (currentModule.theEarlabModuleInformation.ExecutableName.ToLower() == VError.ModuleName)
                           {
                               //update module with verror info
                               currentModule.Message = VError.Message;
                               currentModule.Severity = VError.Severity;
                               return true;
                           }
                       }

                   }


                    //this is a input-output-parameter
                    //VError.IOPType.ToLower()
                   else
                   {
                       
                       //go through each module
                       foreach (EarlabModule currentModule in EarlabModules)
                       {
                           //We have the right module now
                           if (VError.ModuleName == currentModule.theEarlabModuleInformation.ExecutableName.ToLower())
                           {
                               
                               //Now we need the input output or parameter
                               switch (VError.IOPType.ToLower())
                               {

                                   case "parameter":  
                                           //code pattern for finding the IOP
                                           foreach (EarlabParameter parameter in currentModule.EarlabParameters)
                                           {
                                               if (parameter.PName == VError.IOPName)
                                               {
                                                   //update parameter with verror info
                                                   parameter.Message = VError.Message;
                                                   parameter.Severity = VError.Severity;
                                                   return true;
                                               }
                                               else
                                                   return false;
                                           }
                                           break;

                                   case "input":
                                       
                                           //foreach (EarlabInput input in currentModule.EarlabInputs)
                                           //{
                                           //    if (input.PName == VError.IOPName)
                                           //        TheVar = input;
                                           return true;

                                           //}
                                       
                                       

                                   case "output":
                                       
                                           //foreach (EarlabOutput output in currentModule.EarlabOutputs)
                                           //{
                                           //    if (output.PName == VError.IOPName)
                                           //        TheVar = output;
                                           return true;
                                           //}
                                       
                                   default:
                                        throw new System.NotSupportedException("The Error type " + VError.IOPType + " is not recognized.");

                               }//end switch
                           }//end if

                           //module name is not recognized what would this mean?
                           //else
                           //throw new System.NotSupportedException("The Error type " + VError.ModuleName + " is not recognized.");

                       }//end foreach
                       //this is the same problem as above -- module name not recognized, no reason to have the code twice.
                     throw new System.NotSupportedException("The Error type " + VError.ModuleName + " is not recognized.");
                   }//end else

             throw new System.NotSupportedException("The Full Verification Error type with path" + VError.FullErrorPath + "is not recognized.");
            }

        /// <summary>
        /// Creates a Runfile from the RunfileObject
        /// </summary>
        /// <returns></returns>
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

        #endregion

    }


}
