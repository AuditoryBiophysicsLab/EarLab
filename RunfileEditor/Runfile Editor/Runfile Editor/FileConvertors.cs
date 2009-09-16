using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;



namespace RunfileEditor
{


    /// <summary>
    /// This is a static class that does file conversions.
    /// From stream to string etc.
    /// 
    /// [Use a static class as a unit of organization for methods not associated with particular objects.
    /// Also, a static class can make your implementation simpler and faster because 
    /// you do not have to create an object in order to call its methods. 
    /// It is useful to organize the methods inside the class in a meaningful way, 
    /// such as the methods of the Math class in the System namespace.]
    /// 
    /// </summary>
    static class FileConvertors
    {


        public static String XmlToString(string strFile)
        {
            // Load the xml file into XmlDocument object.
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(strFile);

            //try
            //{
            //    xmlDoc.Load(strFile);
            //}
            //catch (XmlException ex)
            //{
            //    MessageBox.Show(ex.Message);
            //    err = ex.StackTrace;

            //    //lastly make the recording to the errorlog
            //    //Going to have to make a "catch flag" to log errors
            //    {
            //        //!!/write to error pop-up
            //        errorLog currentLog = new errorLog(err);
            //        currentLog.PathName = Application.StartupPath;
            //        currentLog.WriteErrorLog();

            //        RunfileEditor.frmChild child = new RunfileEditor.frmChild(this);
            //        //Show the form
            //        child.Show(); //--> pops it from start.
            //        child.BringToFront();//brings it to front

            //        child.txtErrorMsgs.Text = currentLog.ReadErrorLog();
            //        //Error Logging works --- need to work on windows type display.
            //        //textBox1.Text = currentLog.ReadErrorLog();

            //        //errorBox errorForm = new errorBox();
            //        //errorForm.ShowDialog();
            //        //!!/write to error pop-up
            //    }

            //}

            // Now create StringWriter object to get data from xml document.
            //later my replace with xmltostream, stream to string for symmetry
            StringWriter sw = new StringWriter();
            XmlTextWriter xw = new XmlTextWriter(sw);

            xmlDoc.WriteTo(xw);
            return sw.ToString();
        }

        //??
        public static Stream StringToStream(string XmlString)
        {
            //String -> Stream

            // Create a new instance of an ASCII encoding to convert the XML string to a byte array
            System.Text.ASCIIEncoding encoding = new System.Text.ASCIIEncoding();

            // Using the previously-created ASCII encoding, convert the XML string to a byte array
            byte[] myBuffer = encoding.GetBytes(XmlString);

            // Convert the byte array into a stream, so we can use it to initialize an XML document
            MemoryStream stream = new MemoryStream(myBuffer);

            return stream;

        }

        public static string StreamToString(Stream XmlStream)
        {
            // convert stream to string
            StreamReader reader = new StreamReader(XmlStream);
            string textXml = reader.ReadToEnd();
            return textXml;

        }

        public static XmlDocument StreamToXml(Stream XmlStream)
        {
            XmlDocument XmlDoc = new XmlDocument();
            XmlDoc.Load(XmlStream);
            return XmlDoc;

        }

        public static XmlDocument StringToXml(string strXml)
        {
            XmlDocument XmlDoc = new XmlDocument();
            // String --> Stream --> Xml
            //XmlDoc.Load(fXml);

            XmlDoc.Load(StringToStream( strXml));

            return XmlDoc;
        }

        public static string fXmltoStream(string strFile)
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(strFile);
            StringWriter sw = new StringWriter();
            XmlTextWriter xw = new XmlTextWriter(sw);

            xmlDoc.WriteTo(xw);
            string stringxml = sw.ToString();

            return stringxml;
        }

        public static Stream XmltoStream(XmlDocument xmlDoc)
        {
            StringWriter sw = new StringWriter();
            XmlTextWriter xw = new XmlTextWriter(sw);

            xmlDoc.WriteTo(xw);
            string stringxml = xw.ToString();

            Stream stream = StringToStream(stringxml);

            return stream;
        }










    }
}
