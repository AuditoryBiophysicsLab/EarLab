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
            while (count < UniqueEarlabModuleXMLs.Length)
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
}
