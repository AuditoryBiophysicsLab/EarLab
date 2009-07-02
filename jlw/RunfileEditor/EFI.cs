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
        public static string GetModuleXML(string ModuleExeName)
        {
            string resourceName = "EFITestWrapper.ModuleXML." + ModuleExeName + ".xml";
            ManifestResourceInfo info = _assembly.GetManifestResourceInfo(resourceName);
            
            if (info == null)
                return null;
            using (StreamReader _textStreamReader = new StreamReader(_assembly.GetManifestResourceStream(resourceName)))
            {
                return _textStreamReader.ReadToEnd();
            }
        }

        public static string VerifyRunfile(string RunfileXML)
        {
            string resourceName = "EFITestWrapper.VerificationErrorsXML.VerificationErrors.xml";
            ManifestResourceInfo info = _assembly.GetManifestResourceInfo(resourceName);

            if (info == null)
                return null;
            using (StreamReader _textStreamReader = new StreamReader(_assembly.GetManifestResourceStream(resourceName)))
            {
                return _textStreamReader.ReadToEnd();
            }
        }
    }
}
