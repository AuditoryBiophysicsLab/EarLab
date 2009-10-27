using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;

namespace RunfileEditor
{
        //Error Object
        //Error XML -> Collection of error objects

        //

    /// <summary>
    /// Creates a Verification object from the verification event node
    /// </summary>
    public class VerificationError
    {
        #region Data Members  -- Just a copy of the XML Node
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

        //Item = XmlUtilities.CleanInputXML(String_Item)'
        #region Properties
        //Derivatives as listed:

            public string FullErrorPath
            {

                get
                {
                    //ErrorNode.GetElementsByTagName("Severity");
                    //ErrorNode.SelectSingleNode("Severity").InnerText
                    return XmlUtilities.CleanInputXML(ErrorNode["Path"].InnerText);
                }

            }

            /// <summary>
            /// 0.) Module Name
            /// 1.) This position is discarded as a result of the XML.
            /// 2.) A label for a input, output or parameter.
            /// 3.) The name of the input, output or parameter.
            /// </summary>
            public string[] PathArray
            {

                #region Some Notes
                //<Path>
                    //  LeftPinna.Parameters.Parameter.SampleRate_Hz
                    //</Path>
                    //Path Splitter


                    //Checker
                    //foreach (string item in PathArray)
                    //{
                    //    Console.WriteLine(item);
                //)
                #endregion

                get
                {
                    //string path = ErrorNode.["Path"].InnerText;
                    string[] PathArray = FullErrorPath.Split('.');
                    return PathArray;
                }

            }

            public string ModuleName
            {
                get
                {
                    return PathArray[0];      
                }

            }

            public string IOPName
            {
                get
                {
                    if( PathArray[3] != null)
                    { return PathArray[3].ToString();}

                    else
                    { return null; }
                }


            }

            public string Severity
            {
                get
                {
                    return XmlUtilities.CleanInputXML(ErrorNode["Severity"].InnerText);
                }

            }

            public string Message
            {

                get
                {
                    return XmlUtilities.CleanInputLeaveSpacesXML(ErrorNode["Message"].InnerText);
                }

            }

            //This way we can use a very easy Case statement to handle 
            //Updating the RunfileObject
            public bool input
        {

            get
            {
                if (PathArray[2].ToString().ToLower() == "input")
                { return true; }

                else
                { return false; }
            }


        }

            public bool output
        {

            get
            {
                if (PathArray[2].ToString().ToLower() == "output")
                { return true; }

                else
                { return false; }
            }

        }

            public bool parameter
        {
            get
            {
                if (PathArray[2].ToString().ToLower() == "parameter")
                { return true; }

                else
                { return false; }
            }

        }

            public bool module
        {

            get
            {
                if (PathArray[2].ToString().ToLower() == "module")
                { return true; }

                else
                { return false; }
            }

        }

            public string IOPType
            {


            get
            {
                if (input)
                    return "input";
                if (output)
                    return "output";
                if (parameter)
                    return "parameter";
                else
                    return "error";

            }

            }

        #endregion

        #region Constructors
        public VerificationError(XmlNode VError)
        {
            ErrorNode = VError;

        }

       

        #endregion 
    }


    public static class EFIVerification
    {

        /// <summary>
        /// Send the runfile in to the EFI
        /// Receive the verfication email back from the EFI
        /// </summary>
        /// <param name="Runfile"></param>
        /// <param name="RunfileVerificationErrors"></param>
        /// <returns></returns>
        public static bool GetRunfileEFIError(XmlDocument Runfile, out XmlDocument RunfileVerificationErrors)
        {
            //StringWriter sw = new StringWriter(); 
            //XmlTextWriter xw = new XmlTextWriter(sw); 
            //xmlDoc.WriteTo(xw); 
            //return sw.ToString();
            string VErrors;
            RunfileVerificationErrors = null;


            if (EFITestWrapper.EFI.VerifyRunfile(XmlUtilities.Xml2Str(Runfile), out VErrors))
            {
                //some errors
                RunfileVerificationErrors = new XmlDocument();
                RunfileVerificationErrors.LoadXml(VErrors);
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


    }

   
}