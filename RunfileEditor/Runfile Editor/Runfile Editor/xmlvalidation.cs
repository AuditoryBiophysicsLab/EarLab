using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Xml;
using System.Xml.Schema;

namespace RunfileEditor
{
    public class ConsoleValidator
    {
        public static void ValidateXml(string xmlFilename, string schemaFilename)
        {
            XmlTextReader r = new XmlTextReader(xmlFilename);
            XmlValidatingReader validator = new XmlValidatingReader(r);
            validator.ValidationType = ValidationType.Schema;

            XmlSchemaCollection schemas = new XmlSchemaCollection();
            schemas.Add(null, schemaFilename);
            validator.Schemas.Add(schemas);

            validator.ValidationEventHandler += new ValidationEventHandler(ValidationEventHandler);

            try
            {
                while (validator.Read())
                { }
            }
            catch (XmlException err)
            {
                Console.WriteLine(err.Message);
            }
            finally
            {
                validator.Close();
            }
        }

        private static void ValidationEventHandler(object sender, ValidationEventArgs args)
        {
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
//    Console.WriteLine("Validating your.xml.");
//    ValidateXml("your.xml", "your.xsd");
//}http://www.java2s.com/Code/CSharp/XML/ValidateanXMLDocumentAgainstaSchema.htm
