using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Reflection;

namespace EFITestWrapper
{
    public static class EFI
    {
        private static Assembly _assembly;

        static EFI()
        {
            _assembly = Assembly.GetExecutingAssembly();
        }

        // This returns the <Module>...</Module> XML metadata for a given module EXE name
        public static bool GetModuleXML(string ModuleExeName, out string ModuleXML)
        {
            return GetXMLFile(Path.Combine("ModuleXML", ModuleExeName + ".xml"), out ModuleXML);
        }

        public static bool VerifyRunfile(string RunfileXML, out string VerificationXML)
        {
            return GetXMLFile(Path.Combine("VerificationErrorsXML", Path.GetFileNameWithoutExtension(RunfileXML) + "_V.xml"), 
                out VerificationXML);
        }

        private static bool GetXMLFile(string XMLFile, out string XMLContents)
        {
            string curDir = Path.GetDirectoryName(_assembly.Location);
            XMLFile = Path.Combine(curDir, XMLFile);
            XMLContents = null;
            if (!File.Exists(XMLFile))
                return false;
            using (StreamReader _textStreamReader = new StreamReader(XMLFile))
            {
                XMLContents = _textStreamReader.ReadToEnd();
                return true;
            }
        }
    }
}
