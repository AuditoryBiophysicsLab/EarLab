using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;
using EFITestWrapper;

namespace EFITestWrapperTester
{
    class Program
    {
        static void Main(string[] args)
        {
            string ModuleXML;

            if (EFI.GetModuleXML("DataSource", out ModuleXML))
                Console.WriteLine("DataSource Module XML follows:\n{0}", ModuleXML);
            else
                Console.WriteLine("DataSource Module XML not found!");
            //Console.WriteLine(EFI.GetModuleXML("DataSink"));
            //Console.WriteLine(EFI.VerifyRunfile("stuff"));
        }

        static void OldMain(string[] args)
        {
            // Get the ModulesXML (multiple modules) from the EFI wrapper
            string ModulesXML = GetModulesXML();

            // Create a new instance of an ASCII encoding to convert the XML string to a byte array
            System.Text.ASCIIEncoding encoding = new System.Text.ASCIIEncoding();

            // Using the previously-created ASCII encoding, convert the XML string to a byte array
            byte[] myBuffer = encoding.GetBytes(ModulesXML);

            // Convert the byte array into a stream, so we can use it to initialize an XML document
            MemoryStream stream = new MemoryStream(myBuffer);

            // Create a new, empty XML document
            XmlDocument doc = new XmlDocument();

            // Initialize the XML document from the previously-created stream
            doc.Load(stream);

            // Let's see if the name of a runfile was passed in on the command line
            if (args.Length > 1)
            {
                FileStream runfileStream = new FileStream(args[1], FileMode.Open, FileAccess.ReadWrite);
                XmlDocument runfile = new XmlDocument();
                // Load the XML from the runfile
                runfile.Load(runfileStream);

                if (runfileStream.CanSeek)
                {
                    // Seek back to the beginning of the file
                    runfileStream.Seek(0, SeekOrigin.Begin);
                }

                runfile.Save(runfileStream);
            }
        }

        // This function simulates a call to the EFI Wrapper to get the modules XML
        static string GetModulesXML()
        {
            return "<modules>" +
                   "  <module>" +
                   "    <executableName>" +
                   "      DataSource" +
                   "    </executableName>" +
                   "    <parameters>" +
                   "      <parameter>" +
                   "        InputFileName" +
                   "      </parameter>" +
                   "    </parameters>" +
                   "  </module>"+
                   "</modules>";
        }
    }
}
