using System;
using System.Collections.Generic;
using System.Text;

namespace RunfileEditor
{

    /// <summary>
    /// This is an Object Factory that takes an EarlabInput node as an input and returns
    /// an EarlabInput Object.  Later this object is used in spg for the display purposes.
    /// </summary>
    public class EarlabInputFactory
    {
        /// <summary>
        /// The default constructor is not used.
        /// </summary>
        public EarlabInputFactory()
        {
        }

        /// <summary>
        /// The constructor that takes an Earlab Input XML Node 
        /// and produces a EarlabInput Object.  
        /// Later this object is used in spg for the display purposes.
        /// </summary>
        /// <param name="RFInput"></param>
        /// <param name="XmlInput"></param>
        /// <returns></returns>
        public static EarlabInput Make(RunfileInput RFInput, ModuleXmlInput XmlInput)
        {
            EarlabInput Dummy = new EarlabInputInt();
            return Dummy;
        }

    }

}
