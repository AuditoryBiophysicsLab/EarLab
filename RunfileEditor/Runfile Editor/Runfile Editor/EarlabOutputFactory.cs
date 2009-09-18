using System;
using System.Collections.Generic;
using System.Text;

namespace RunfileEditor
{
    /// <summary>
    /// This is an Object Factory that return an EarlabOutput
    /// Grabs the XML node.
    /// </summary>
    public class EarlabOutputFactory
    {
        /// <summary>
        /// The default constructor is not used
        /// </summary>
        public EarlabOutputFactory()
        {
        }

        /// <summary>
        /// The constructor that takes an Earlab Input XML Node 
        /// and produces a EarlabInput Object.  
        /// Later this object is used in spg for the display purposes.
        /// </summary>
        /// <param name="RFOutput"></param>
        /// <param name="XmlOutput"></param>
        /// <returns></returns>
        public static EarlabOutput Make(RunfileOutput RFOutput, ModuleXmlOutput XmlOutput)
        {
            EarlabOutput Dummy = new EarlabOutputInt();
            return Dummy;
        }



    }
}
