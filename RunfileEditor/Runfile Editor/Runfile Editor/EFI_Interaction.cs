using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;
using System.Windows.Forms;
using EFITestWrapper;



namespace RunfileEditor
{

     ///<summary>
     ///This is a static class that simulates interaction with the EFI
     ///</summary>
        public static class EFIInteraction
        {
            //We will need directory information for all
            //[File Location]
            public static string ModuleXmlPath;
            public static string ModuleXmlPathStart = "C:\\..Projects\\Runfile Editor\\Runfile Editor\\XML Docs\\";
            public static string ModuleSchemaXmlPath = "C:\\..Projects\\Runfile Editor\\Runfile Editor\\XML Docs\\moduleInformation.xsd";

            //Some static vars to handle the lists of stuff.
            //ModuleXML -- a string of all modules that are good
            public static List<XmlDocument> mModuleXMLs;
            public static List<string> mMissing;
            public static List<XmlDocument> ModuleErrorXmls;
          
                //XmlDocument doc = new XmlDocument();
                //doc.Load(//string//);

                /// <summary>
                /// 
                /// </summary>
                /// <param name="sFileName"></param>
                /// <returns></returns>
                private static bool checkFileExistence(string sFileName)
                    {


                           //change the string path to the proper info
                           sFileName = ModuleXmlPathStart.ToString() + sFileName.ToString() + ".xml"; 
                            
                            try
                                {
                                //need to check directory
                                    if (File.Exists( sFileName ) == true)
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
                public static bool GetModules(List<string> UniqueModulesNeeded, out List<string> ModulesMissing, out List<XmlDocument> mModuleXmls)
                        {

                                bool cflag = true;
                                mMissing = new List<string>();
                                mModuleXmls = new List<string>(); 
                                string ModuleXmlString1; //Module in string format

                                foreach (string moduleName in UniqueModulesNeeded)
                                {
                                    //if false add the name.
                                    if( ! checkModule (moduleName) )
                                        {

                                            //if it is missing then add to the missing collection of strings
                                            mMissing.Add(moduleName);
                                            cflag = false;

                                        }
                                    else 
                                     {
                                        //the module xml is there
                                         EFITestWrapper.EFI.GetModuleXML(moduleName, ModuleXmlString1);

                                        // if it is good we add it to the string of good Module XML.       
                                        mModuleXmls.Add( );

                                        
                                     }


                                }

                                //instead of cflag could also check ModulesMissing.
                                ModulesMissing = mMissing;
                                mModuleXmls;
                                return cflag;


                        }  
                /// <summary>
                /// Check to see if the module is available.
                /// </summary>
                /// <param name="moduleName"></param>
                /// <returns></returns>
               

                private static bool checkModule(string moduleName)
                        {
                            //set the proper path
                            ModuleXmlPath = ModuleXmlPathStart.ToString() + moduleName.ToString() + ".xml";

                            //check if doc is there? 
                            if ( checkFileExistence(ModuleXmlPath) )

                                    {
                                        //check if it is schema valid?
                                        if ( ModuleXmlSchemaValidate(ModuleXmlPath, moduleName) )
                                            {
                                                    //if schema is okay, return the 
                                            
                                                    return true;
                                        
                                        
                                        
                                             }

                                        else
                                            {return false;}

                                    }


                            else
                                {return false;}

                        }
                /// <summary>
                /// 
                /// </summary>
                /// <param name="ModuleXmlPath"></param>
                /// <param name="moduleName"></param>
                /// <returns></returns>
                private static bool ModuleXmlSchemaValidate(string ModuleXmlPath, string moduleName)
                {




 	                throw new Exception("The method or operation is not implemented.");
                }

                //get Modules in a collection of XML
                //get assumes module xml is ok
                //private static XmlDocument getModuleXml(string moduleName)
                //{
                //    //call EFI and ask for the ModuleXml



                //}



 }//class



}//namespace
