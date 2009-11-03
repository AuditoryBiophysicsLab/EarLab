using System;
using System.Text;
using System.Globalization;
using System.Resources;
using System.Reflection;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.Xml;
using System.IO;


namespace RunfileEditor
{
    /// <summary>
    /// A collection of static methods to opperate on XML and String files
    /// </summary>
    public static class XmlUtilities
    {
         ///<summary>
         ///Xml to string conversion.
         ///</summary>
         ///<param name="strFile"></param>
         ///<returns></returns>
        public static string Xml2Str(string strFile)
        {
            // Load the xml file into XmlDocument object.
            XmlDocument xmlDoc = new XmlDocument();
            try
            {
                xmlDoc.Load(strFile);
            }
            catch (XmlException e)
            {
                Console.WriteLine(e.Message);
            }
            // Now create StringWriter object to get data from xml document.
            return Xml2Str(xmlDoc);
        }

        /// <summary>
        /// Xml Document to string conversion method.
        /// </summary>
        /// <param name="xmlDoc"></param>
        /// <returns></returns>
        public static string Xml2Str(XmlDocument xmlDoc)
        {
            StringWriter sw = new StringWriter();
            XmlTextWriter xw = new XmlTextWriter(sw);
            xmlDoc.WriteTo(xw);
            return sw.ToString();
        }

        /// <summary>
        /// A static method that cleans an xml string of any spaces
        /// Use with caution, the spaces maybe needed as in the messages.
        /// </summary>
        /// <param name="strIn"></param>
        /// <returns></returns>
        public static string CleanInputXML(string strIn)
        {
            strIn = CleanInputLeaveSpacesXML(strIn);
            //remove spaces
            strIn = Regex.Replace(strIn, " ",  "");
            return strIn;
        }

        /// <summary>
        /// A static method that cleans \r and \n characters from an xml string
        /// </summary>
        /// <param name="strIn"></param>
        /// <returns></returns>
        public static string CleanInputLeaveSpacesXML(string strIn)
        {
            // Replace invalid characters with empty strings.
            //return Regex.Replace(strIn, @"[^\w\.@-]", "");
            // "\r" 
            // "\n"
            // " "
            // replace them all with ""
            strIn = Regex.Replace(strIn, "\r", "");
            strIn = Regex.Replace(strIn, "\n", "");
            return strIn;
        }

    }
}
