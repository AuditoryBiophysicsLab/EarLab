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
        /// <summary>
        /// Xml to string conversion file
        /// </summary>
        /// <param name="strFile"></param>
        /// <returns></returns>
        static string Xml2Str(string strFile)
        {
            // Load the xml file into XmlDocument object.
            XmlDocument xmlDoc = new XmlDocument();
            try
            {
                xmlDoc.Load(strFile);
            }
            catch (XmlException e)
            {
                //change to error log
                //[error log]
                Console.WriteLine(e.Message);
            }
            // Now create StringWriter object to get data from xml document.

            Xml2str(xmlDoc);
            //StringWriter sw = new StringWriter();
            //XmlTextWriter xw = new XmlTextWriter(sw);
            //xmlDoc.WriteTo(xw);
            //return sw.ToString();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="xmlDoc"></param>
        /// <returns></returns>
        static string Xml2str(XmlDocument xmlDoc)
        {

            // Load the xml file into XmlDocument object.
            //XmlDocument xmlDoc = new XmlDocument();
            //try
            //{
            //    xmlDoc.Load(strFile);
            //}
            //catch (XmlException e)
            //{
            //    Console.WriteLine(e.Message);
            //}
            // Now create StringWriter object to get data from xml document.
            StringWriter sw = new StringWriter();
            XmlTextWriter xw = new XmlTextWriter(sw);

            try
            {
                xmlDoc.WriteTo(xw);
            }
            catch (XmlException e)
            {

                //[error log]
                Console.WriteLine(e.Message);

            }

            return sw.ToString();
        }

    }
}
