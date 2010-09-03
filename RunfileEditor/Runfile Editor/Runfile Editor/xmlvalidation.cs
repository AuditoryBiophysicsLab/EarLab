using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Xml;
using System.Xml.Schema;

namespace RunfileEditor
{

public class XmlValidator
    {

        public static int error1 = 0;

         /// <summary>
         /// Validates an Xml file against an xml schema
         /// true = good
         /// false= bad
         /// 
         /// </summary>
         /// <param name="xmlFilename"></param>
         /// <param name="schemaFilename"></param>
         /// <returns></returns>
        public static bool ValidateXml(string xmlFilename, string schemaFilename)
        {
            //string SchemaPath = Path.GetFullPath(schemaFilename);

            // Create the XmlSchemaSet class.
            XmlSchemaSet sc = new XmlSchemaSet();

            // Add the schema to the collection.
            sc.Add(null, schemaFilename);

            // Set the validation settings.
            XmlReaderSettings settings = new XmlReaderSettings();
            settings.ValidationType = ValidationType.Schema;
            settings.Schemas = sc;
            settings.ValidationEventHandler += new ValidationEventHandler(ValidationEventHandler);

            // Create the XmlReader object.
            XmlReader reader = XmlReader.Create(xmlFilename, settings);

            try
            {
                while (reader.Read())
                {

                }//while reqires 
            }
            catch (XmlException err)
            {
                //[ErrorLog]
                //Console.WriteLine(err.Message);
                error1++;
            }
            finally
            {
                reader.Close();
            }


            if (error1 == 0)
                return true;

            else
            {
                return false;
            }
        }
        private static void ValidationEventHandler(object sender, ValidationEventArgs e)
        {
            //Console.WriteLine("Validation Error: {0}", e.Message);
            //This could be used for error reporting.

            if (e.Message != null)
            {
                error1++;
            }
        }


    }


}
