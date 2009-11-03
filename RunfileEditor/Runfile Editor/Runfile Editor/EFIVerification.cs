using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;


namespace RunfileEditor
{

    public static class EFIVerification
    {

        /// <summary>
        /// Send the runfile in to the EFI
        /// Receive the verfication email back from the EFI
        /// </summary>
        /// <param name="Runfile"></param>
        /// <param name="RunfileVerificationErrors"></param>
        /// <returns></returns>
        public static bool GetRunfileEFIError(XmlDocument Runfile, out XmlDocument RunfileVerificationErrors)
        {


            string VErrors;
            RunfileVerificationErrors = null;


            if (EFITestWrapper.EFI.VerifyRunfile(XmlUtilities.Xml2Str(Runfile), out VErrors))
            {
                //some errors
                RunfileVerificationErrors = new XmlDocument();
                RunfileVerificationErrors.LoadXml(VErrors);
                return true;
            }
            else
            {
                //no errors
                RunfileVerificationErrors = null;
                return false;

            }
            //public static bool VerifyRunfile(string RunfileXML, out string VerificationXML)


        }


    }


}
