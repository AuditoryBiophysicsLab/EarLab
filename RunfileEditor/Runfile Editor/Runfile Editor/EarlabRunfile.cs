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

        /// <summary>
        /// The module directory is ...
        /// These are the unique EarlabModule titles that are needed from the EFI
        /// </summary>
        private ModuleDirectory ModuleDirectory;
        //
        //
        //
        /// <summary>
        /// Has the Runfile Object Changed
        /// </summary>
        private bool RHasChanged = false;

        /// <summary>
        /// This collection contains all the children 
        /// </summary>
        private List<EarlabSession> mChildren = new List<EarlabSession>(); 
       
        /// <summary>
        /// This is the list of Modules contained in the Runfile
        /// </summary>
        private List<RunfileModule> RunfileModules = new List<RunfileModule>();        
        
        //
        //Runfile Data Structures that are public
        //
        /// <summary>
        /// Run File Information -- contains informaiton about the model
        /// This can be reconfigured to public-private
        /// </summary>
        public RunfileInformation RunfileInformation;
        /// <summary>
        /// Modules carry two data structures: (XML data as strings),(XML data as the appropriate value)
        /// </summary>
        public List<EarlabModule> EarlabModules = new List<EarlabModule>();

        //errors in an accessable format
        public List<VerificationError> VErrorCollection = new List<VerificationError>();
        

        #endregion

        #region Properties

        //Basically I can do the event on change of a Module
        //Repaint the Tabs.
        public event EventHandler DataChanged;

        protected virtual void OnDataChanged()
        {
            if (DataChanged != null)
                DataChanged(this, new EventArgs());
        }



        public int ModuleCount 
        { 
            
            get 
            { 
                return RunfileModules.Count; 
            
            } 
        
        
        
        }
        //Module -- IOP -- Module IOP etc.. alternative is Module, Module, Module, IOP, IOP, IOP or some such.
        public List<EarlabSession> Children
        {


            get
            {
                //mChildren = null;

                foreach (EarlabModule EarlabM in EarlabModules)
                {
                    mChildren.Add(EarlabM);

                    foreach (EarlabSession EarlabS in EarlabM.Children)
                    {
                        mChildren.Add(EarlabS);

                    }
                }

                return mChildren;

            }





        }

        // need to write the set
        public bool HasChanged
        {
            get 
            {
                foreach (EarlabModule etModule in EarlabModules)
                {
                    if (etModule.HasChanged == true)
                        RHasChanged = true;
                    else
                        RHasChanged = false;
                }
                
                return RHasChanged; 
            
            }

            //set
            //{
            //    ////check all parts of Runfile 
            //    ////in a generic fashion

            //    //foreach (EarlabModule Module in EarlabModules)
            //    //{
            //    //    if (Module.HasChanged == true)
            //    //        changed1 = true;
            //    //}


            //}

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
            RunfileInformation = new RunfileInformation(XList[0], Runfile);

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


        //Verrors section
        public void AllEarlabObjectUpdate(XmlDocument Verrors) 
        {
            VErrorCollection = ErrorListCreator(Verrors);

            foreach (VerificationError VerrorObject in VErrorCollection)
            {
                OneObjectErrorUpdate(VerrorObject);
                //some error here then
            }

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


        /// <summary>
        /// 
        /// </summary>
        /// <param name="RunfileVerificationErrors"></param>
        /// <returns></returns>
        private List<VerificationError> ErrorListCreator(XmlDocument RunfileVerificationErrors)
        {
            //clear the old errors
            List<VerificationError> mVErrorCollection = new List<VerificationError>();
            XmlNodeList VList = RunfileVerificationErrors.GetElementsByTagName("VerificationEvent");

            foreach (XmlNode errorNode in VList)
            {
                /// Converts XML Node Module into the collection of ModuleInfo and IOP 
                mVErrorCollection.Add( new VerificationError(errorNode) );
            }

            return mVErrorCollection;
        }



        ////design is problematic for updates!
        public void EFI_Run()
        {
            XmlDocument RunfileVerificationErrors;

            //need to give doc a title?

            EFIVerification.GetRunfileEFIError(this.RunfileXMLCreate(), out RunfileVerificationErrors);

            // 
            //Process errors
            this.AllEarlabObjectUpdate(RunfileVerificationErrors);

            //Display Errors on "Summary" GUI
            //if no errors, create the Desktop Earlab launch
            //button_create_if_no_errors();
        }

        #endregion


        /// <summary>
        /// 
        /// </summary>
        /// <param name="VError"></param>
        /// <returns></returns>
        private void OneObjectErrorUpdate(VerificationError VError)
        {
            //Problem with the design ---
            //Once it HasChanged been updated we need to exit the loop.
            //Each error is only going to be there 1 time.
            //Once we find it we need out!!

            //string searchString = VError.IOPName;
            //module
            // if null then it is a module, else it is an IOP
            if (VError.IOPName == null)
            {
                #region Module Level Error Method
                foreach (EarlabModule currentModule in EarlabModules)
                {
                    //This is NOT working b/c it is not referencing the right Object!!!
                    if (currentModule.theEarlabModuleInformation.ExecutableName.ToLower() == VError.ModuleName.ToLower())
                    {
                        //update module with verror info
                        currentModule.Message = VError.Message;
                        currentModule.Severity = VError.Severity;
                        return;
                    }
                }
                #endregion
            }

             //this is a input-output-parameter
            //VError.IOPType.ToLower()
            else if (VError.IOPType.ToLower() == "parameter" || VError.IOPType.ToLower() == "input" || VError.IOPType.ToLower() == "output")
            {
                #region IOP Level Error Method
                //go through each module
                foreach (EarlabModule currentModule in EarlabModules)
                {
                    //We have the right module now
                    if (VError.ModuleName.ToLower() == currentModule.theEarlabModuleInformation.ExecutableName.ToLower())
                    {

                        //Now we need the input output or parameter
                        switch (VError.IOPType.ToLower())
                        {

                            case "parameter":
                                //code pattern for finding the IOP
                                foreach (EarlabParameter parameter in currentModule.EarlabParameters)
                                {
                                    if (parameter.PName.ToLower() == VError.IOPName.ToLower())
                                    {
                                        //update parameter with verror info
                                        parameter.Message = VError.Message;
                                        parameter.Severity = VError.Severity;
                                        //return true;
                                        return;
                                    }

                                }
                                //This means that the error sent back from the EFI is a parameter, but not named properly.
                                throw new System.NotSupportedException("The Parameter Name " + VError.IOPName + " is not recognized.");
                                //break;

                            case "input":

                                //foreach (EarlabInput input in currentModule.EarlabInputs)
                                //{
                                //    if (input.PName == VError.IOPName)
                                //        TheVar = input;
                                //return;
                                break;


                            //}



                            case "output":

                                //foreach (EarlabOutput output in currentModule.EarlabOutputs)
                                //{
                                //    if (output.PName == VError.IOPName)
                                //        TheVar = output;
                                //return true;
                                //}
                                break;

                            default:
                                throw new System.NotSupportedException("The Error type " + VError.IOPType + " is not recognized.");
                                

                        }//end switch
                    }//end if

                    //module name is not recognized what would this mean?
                    //else
                    //throw new System.NotSupportedException("The Error type " + VError.ModuleName + " is not recognized.");

                }//end foreach
                #endregion
                //this is the same problem as above -- module name not recognized, no reason to have the code twice.
                //throw new System.NotSupportedException("The Error type " + VError.ModuleName + " is not recognized.");

            }//end else if

            //catch -- all we have soem error and it doesn't correspond to a module or module's IOP.
            else
            {
                throw new System.NotSupportedException("The Full Verification Error type with path " + VError.FullErrorPath + " is not recognized.");
            }
        }





    }


}
