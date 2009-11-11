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
            int error1 = 0;

            //Forward stream reading access to data
            XmlTextReader r = new XmlTextReader(xmlFilename);

            //deprecated way of checking agaisnt a schema -- update.
            //xmlreader class.
            XmlValidatingReader validator = new XmlValidatingReader(r);
            validator.ValidationType = ValidationType.Schema;

            //XmlReader validator = new XmlReader.Create(

            XmlSchemaCollection schemas = new XmlSchemaCollection();
            schemas.Add(null, schemaFilename);
            validator.Schemas.Add(schemas);

            //on error write message
            validator.ValidationEventHandler += new ValidationEventHandler(ValidationEventHandler);

            try
            {
                while (validator.Read())
                { }//while reqires 
            }
            catch (XmlException err)
            {
                //[ErrorLog]
                Console.WriteLine(err.Message);
                error1++;
            }
            finally
            {
                validator.Close();        
            }


            if (error1 == 0)
                return true;
            
            else
            { 
                return false; 
            }
        }

        private static void ValidationEventHandler(object sender, ValidationEventArgs args)
        {
            //[ErrorLog]
            Console.WriteLine("Validation error: " + args.Message);
        }


    }


}
//Format I want to have:
//try
//{
//XmlDocument doc = ValidatingXmlLoader.Load( xml, schema);
//}
//catch
//{
//Debug.WriteLine( "The validation failed.");
//}
//More Notes
//private static void Main()
//{
//    ValidateXml("your.xml", "your.xsd");

