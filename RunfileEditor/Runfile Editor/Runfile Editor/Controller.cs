using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;


namespace RunfileEditor
{
    static class  Controller
    {

        //Runfile Object to EFI and Back



        //design is problematic for updates!
        public static void EFI_Run(EarlabRunfile RunfileObject)
        {

                if (RunfileObject != null)
                {

                    XmlDocument RunfileVerificationErrors;

                    //need to give doc a title?

                    EFIVerification.GetRunfileEFIError(RunfileObject.RunfileXMLCreate(), out RunfileVerificationErrors);

                    // 
                    //Process errors
                    RunfileObject.AllEarlabObjectUpdate(RunfileVerificationErrors);

                    //Display Errors on "Summary" GUI
                    //if no errors, create the Desktop Earlab launch
                    //button_create_if_no_errors();
                }


        }



    }
}
