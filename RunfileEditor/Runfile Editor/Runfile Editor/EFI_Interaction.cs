using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;
using System.Windows.Forms;



namespace RunfileEditor
{

    /// <summary>
    /// This is a static class that simulates interaction with the EFI
    /// 
    /// </summary>
static class EFIInteraction
    {
        //WE will need directory information for all
        public static string ModuleXmlPath;
        public static string ModuleXmlPathStart = "C:\\..Projects\\Runfile Editor\\Runfile Editor\\XML Docs\\";
        public static string ModuleSchemaXmlPath = "C:\\..Projects\\Runfile Editor\\Runfile Editor\\XML Docs\\moduleInformation.xsd";

        //Some static vars to handle the lists of stuff.
                        //ModuleXML -- a string of all modules that are good
        public static List<XmlDocument> ModuleXMLs;
        public static List<string> mMissing;
        public static List<XmlDocument> mModuleXmls;


        /// <summary>
        /// file exist method
        /// </summary>
        /// <param name="sFileName"></param>
        /// <returns></returns>
        private static bool checkFileExistence(string sFileName)
        {
            try
                {
                    if (File.Exists(sFileName) == true)
                         {
                                return true;
                         }
                                              
                    else
                         { 
                            return false; 
                         }
                }

            catch (Exception ex)
                {   
                    //Ok we'll have to log this error
                    //Console.WriteLine(ex.Message);
                    MessageBox.Show(ex.Message);
                    return false;
                }
        }//end check file existence

/// <summary>
/// 
/// </summary>
/// <param name="UniqueModulesNeeded"></param>
/// <param name="ModulesMissing"></param>
/// <param name="ModuleXML"></param>
/// <returns></returns>
        public static bool checkModules(List<string> UniqueModulesNeeded, out List<string> ModulesMissing, out List<XmlDocument> ModuleXmls)
        {

                bool cflag = true;
                mMissing = new List<string>();
                mModuleXmls = new List<XmlDocument>(); 
                foreach (string moduleName in UniqueModulesNeeded)
                {
                    //if false add the name.
                    if( ! checkModule (moduleName) )
                        {

                            //wtf??
                            mMissing.Add(moduleName);
                            cflag = false;

                        }
                    else 
                     {
                        // if it is good we add it to the string of good Module XML.
                        //mModuleXml.Add( //document from call);


                     }


                }

                //instead of cflag could also check ModulesMissing.
                ModulesMissing = mMissing;
                ModuleXmls = mModuleXmls;
                return cflag;

         
        }  
         
/// <summary>
/// 
/// </summary>
/// <param name="moduleName"></param>
/// <returns></returns>
private static bool checkModule(string moduleName)
        {
            //look in directory.
            ModuleXmlPath = ModuleXmlPathStart.ToString() + moduleName.ToString() + ".xml";

            //check if doc is there? 
            if ( checkFileExistence(ModuleXmlPath) )
                    
                    {
                        //check if it is schema valid?

                        if ( ModuleXmlSchemaValidate(ModuleXmlPath, moduleName) )
                            {return true;}
                        
                        else
                            {return false;}

                    }


            else
                {return false;}

        }


public static bool ModuleXmlSchemaValidate(string strXMLDoc, string moduleName)
{
//1st is an XMLDoc

//2nd is schema




bool flag = false;
return flag;
}


//This is pseudo code 

public static EFInteraction(     )
{

//(a)static routine to take the whole xml return collection of event objects.
//1.) Take event xml
//2.) Store full xml into a collection of event nodes.

	foreach(xmlnode xNode in xmlnodelist)
	    { 
		//get path
		//split path
		//get status
		//get msg

				//start looking for the proper object to update
				foreach(module RunfileModules in Runfileobject.modules)
					{
						if (path[index++] == module.name)
								{
									if( path[index++] != "null")
									
										{
												switch
												
													(Each I-O-P 
												
												
														switch
																here update object
																	// 1.) warning <- xml.warning;
																	// 2.) msg	   <- xml.msg.
														default
															ex
												default
													ex
								
										}
										
									else
										{
										 //there is a problem with the module
										
										
										
										}	


                        }


                }

    }

//3.)getModuleXmls
//        Sends back the xmls for all modules in a collection of Module XML docs
        
//        signature like tryParse True = there, False = not there.

//a.) CheckModules
//b.) getModuleXmls
//    i.) getModuleXml


//public static bool getModuleXmls(string array ModuleNames,out  List<moduleXml> ModuleXmls)

//        a.) I have a special accessor so this is the format I want a List back.
//        b.) Pseudo code I have these two already in program

            

//private static bool getModuleXml(string ModuleName,out  XmlDocument ModuleXml)

//    if we get a false we have to end the program
//In the RunfileObject
//    bool moduleThere?





//3.) Chk Errors
//        EFI checks each module.

//public static bool checkRunfile(RunfileXml theRunfileXml, out  XmlDocument errorRunfile)

//1.) Are all modules there?
//2.) if ok check Runfile
//3.) check each module
//        public static bool checkModule( EarlabModuleObject theModule, moduleXml theModuleXml, out XmlNode ModuleErrorXml)
//4.)Check inputs
        
//5.) Check outputs

//6.) Check params
 
//New XML -- a schema is not really needed, since it cannot be edited.

//1.) RunfileErrorXml
//2.) ModuleErrorXml



//1.) RunfileErrorXml









//4.) Send EFI
//        checkRunfile
//        send.

//public static bool processRunfile(RunfileXml RunfileXml, out  XmlDocument errorEFI)


//A.) Module Error XML?
//B.) process errorEFI XML ?












    }//class











}//namespace
