using System;
using System.Text;
using System.Globalization;
using System.Resources;
using System.Reflection;
using System.Collections.Generic;
using System.Xml;
using System.IO;


namespace RunfileEditor
{
    /// <summary>
    /// Summary description for Class1
    /// </summary>
    public static class XmlUtilities
    {
         ///<summary>
         ///Xml to string conversion file
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

        public static string Xml2Str(XmlDocument xmlDoc)
        {
            StringWriter sw = new StringWriter();
            XmlTextWriter xw = new XmlTextWriter(sw);
            xmlDoc.WriteTo(xw);
            return sw.ToString();
        }


        //public static XmlDocument Str2Xml(string XmlStr)
        //{




        //}

    }
}
