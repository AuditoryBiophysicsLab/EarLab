using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace RunfileEditor
{
        //Error Object
        //Error XML -> Collection of error objects

        //


    public class VerificationError
    {
        #region Data Members
        private XmlNode ErrorNode;
        //<VerificationEvent>
        //  <Severity>
        //    Warning
        //  </Severity>
        //  <Path>
        //    LeftMiddleEar
        //  </Path>
        //  <Message>
        //    Module does not support ModuleXML Interface
        //  </Message>
        //</VerificationEvent>
        #endregion

        #region Properties

        public string FullErrorPath
        {

            get
            {
                //ErrorNode.GetElementsByTagName("Severity");
                //ErrorNode.SelectSingleNode("Severity").InnerText
                return ErrorNode["Severity"].InnerText;
            }

        }




        public string ModuleName
        {
            get
            {
               
                
            }

        }

        public string IOPName
        {
            get
            {

            }


        }


        public string Severity
        {
            get
            {
            }

        }

        public string Message
        {

            get
            {

            }

        }

        public bool parameter
        {
            get
            {
                
            }

        }


        public bool input
        {

            get
            {

            }


        }

        public bool output
        {

            get
            {
            }

        }



        public bool module
        {

            get
            {
            }

        }

        #endregion


        #region Constructors

        public VerificationError(XmlNode VError)
        {
            ErrorNode = VError;

        }
        
        #endregion 


        #region methods

        //the split path
        private string[] path;
 









        #endregion



    }



    //class ErrorDirectory
    //{
    //    //        #region Data Members

    //    //        //
    //    //        //
    //    //        //list of Verification Error xml data
    //    //        //
    //    //        List<VErrorDataXML> ErrorXmlList = new List<VErrorDataXml>();
    //    //        List<ErrorObject> ErrorObjectCollection = new List<ErrorObject>();


    //    //         ///<summary>
    //    //         ///Takes in the Runfile errors in xml
    //    //         ///1.) Parses the errors
    //    //         ///2.) Updates the Runfile Object with the errors
    //    //         ///</summary>
    //    //         ///<param name="RunfileVerificationErrors"></param>
    //    //         ///<returns></returns>
    //    //        public bool RunfileObjectUpdate(XmlDocument RunfileVerificationErrors)
    //    //        {
    //    //                          //<RunfileVerificationResults>
    //    //                          //<VerificationEvent>
    //    //                          //  <Severity>
    //    //                          //    Warning
    //    //                          //  </Severity>
    //    //                          //  <Path>
    //    //                          //    LeftMiddleEar
    //    //                          //  </Path>
    //    //                          //  <Message>
    //    //                          //    Module does not support ModuleXML Interface
    //    //                          //  </Message>
    //    //                          //</VerificationEvent>
    //    //                         //</RunfileVerificationResults>

    //    //                XmlNodeList VerificationEventList = RunfileVerificationErrors.GetElementsByTagName("VerificationEvent");
    //    //                foreach (XmlNode VerificationEvent in VerificationEventList)
    //    //                {
    //    //                    /// Converts XML Node Module into the collection of ModuleInfo and IOP 

    //    //                        //<Path>
    //    //                        //LeftPinna.Parameters.Parameter.SampleRate_Hz
    //    //                        //</Path>
    //    //                    //1.) Store Path
    //    //                        //Module.Name
    //    //                        //Module.Parameter.SampleRate_Hz


    //    //                    //2.) Find I-O-P


    //    //                    //3.) 



    //    //                }
    //    //        }

    //    //        private bool VerificationEventParser(XmlNode VerificationNode)
    //    //        {




    //    //        }

    //    //        [EFITestWrapper Hook to update the RunfileObject]

    //    //        public void EFIUpdate(XmlDocument VerificationErrors)

    //    //        Need to divide this guy up
    //    //        Takes in the Model Object produces XML


    //    //    }
    //    //}
    //}
}