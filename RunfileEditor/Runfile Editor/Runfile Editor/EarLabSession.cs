using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using VisualHint.SmartPropertyGrid;


namespace RunfileEditor
{

    #region EarLabSession Notes
    /*EarLabSession is the base class
 * EarLabModule
 * 
 * EarLabParameter
 *      EarLabParameter<T>
 *          EarLabParameter: (int,dbl, bool, string, int[], dbl[], bool[], string[])
 * 
 * EarLabInput
 *      EarLabInput<T>
 *          EarLabInput: (???)
 * 
 * EarLabOutput
 *      EarLabOutput<T>
 *          EarLabOutput: (???)
 * 
 * 
 * 
 */
    #endregion

    public abstract class EarLabSession
    {
        #region Data Members
        //this is a "dirty bit" that says if the item has been updated.
        private bool mHasChanged = false;

        private List<EarLabSession> mChildren = new List<EarLabSession>();

        #endregion

        #region Data Properties
        public bool HasChanged
        {
            //sets the HasChanged value
            set { mHasChanged = value; }

            //get the mHasChanged
            get
            {
                if (mHasChanged)
                    return true;

                //goes through each of the children to see
                //if they have changed
                for (int i = 0; i < mChildren.Count; i++)
                    if (mChildren[i].HasChanged)
                        return true;

                return false;
            }//end get
        }//end get set methods
        #endregion
    }

    public class EarLabModule : EarLabSession
    {
        #region Data Members
        //Public Data -- this is actual data, not strings
        //so we have public collections from factories.

        //1.) Inputs
        //List<EarLabInput> EarLabInputs = new List<EarLabInput>();
        
        //2.) Outputs
        //List<EarLabOutput> EarLabOutputs = new List<EarLabOutput>();

        //3.) Parameters
        public EarLabModuleInformation theEarLabModuleInformation;

        public List<EarLabParameter> EarlabParameters = new List<EarLabParameter>();
        public List<EarLabInput> EarlabInputs = new List<EarLabInput>();
        public List<EarLabOutput> EarlabOutputs = new List<EarLabOutput>();

        #endregion


        #region Constructors
        public EarLabModule()
        {
        }
        

       //--------------------->
        public EarLabModule(RunFileModule StringModule, ModuleXML theModuleInfo)
        {
       // I + O + P 
        //call factory up, factory adds to base collection
        //
        // Does it make more sense to do all these things in one while loop?
        // This way can isolate errors...


            theEarLabModuleInformation = new EarLabModuleInformation(StringModule.ModuleInformation);


            foreach (RunFileInput rInput in StringModule.theRunFileInputs)
                 {
                    //1.) Inputs
                    //EarLabInputs(EarLabInputFactory.Make(rInput, theModuleInfo[rInput.InputName, rInput.InputType.ToLower()]);

                 }

            foreach (RunFileOutput rOutput in StringModule.theRunFileOutputs)
                {
                        //2.) Outputs
                        //EarLabOutputs(EarLabOutputFactory.Make(rOutput, theModuleInfo[rOutput.OutputName, rOutput.OutputType]);

                }
           
            foreach (RunFileParameter Param in StringModule.RunFileParameters)
               {
                 //3.)(?) Parameters need to fix!
                 EarlabParameters.Add( EarLabParameterFactory.Make(Param, theModuleInfo[Param.ParameterName.ToLower(), Param.ParameterType.ToLower()]) );

               }


           }
        #endregion


     }

    public class EarLabModuleInformation
    {
        #region Data Members
        public string ExecutableName;

        public string InstanceName;

        #endregion

        #region Constructors
        public EarLabModuleInformation()
        {
        }

        public EarLabModuleInformation(RunFileModuleInformation ModuleInfo )
        {
            ExecutableName = ModuleInfo.ExecutableName;

            InstanceName = ModuleInfo.InstanceName;

        }
        #endregion


    }

    #region EarLab Parameter NOtes
    //
    /*
     * I+O+P
     * 
     * public abstract class EarLabParameter : EarLabSession
     * public abstract class EarLabParameter<T>: EarLabParameter
     * public class EarLabParameterInteger : EarLabParameter<int>
     * public class EarLabParameterDouble : EarLabParameter<double>
     * public class EarLabParameterBoolean : EarLabParameter<bool>
     * ????????????? No idea how to add these
     * Integer Array
     * Double Array
     */

    //----------------------------------------------->
    //Inputs


    //


    //----------------------------------------------->
    //Outputs


    //
    /*
     * 
     *Don't have info to do the Inputs and Outputs 
     * 
     * 
     * 
     * 
    */
    //----------------------------------------------->
    //Parameters
    /*
     * I+O+P
     * 
     * public abstract class EarLabParameter
     * public abstract class EarLabParameter<T>: EarLabParameter
     * public class EarLabParameterInteger : EarLabParameter<int>
     * public class EarLabParameterDouble : EarLabParameter<double>
     * public class EarLabParameterBoolean : EarLabParameter<bool>
     * ????????????? No idea how to add these
     * Integer Array
     * Double Array
     */
    //
    #endregion

    public abstract class EarLabParameter : EarLabSession
    {
        // use a generic to make paramters
        //Always String variables

        #region Data members
        protected string pName;
        protected string pType;
        protected string pUnits;
        protected string pDescription;
        #endregion

        #region Data Proeprties

        public string PName
        {
            get { return pName; }
            set { pName = value; }
        }



        public string PType
        {
            get { return pType; }
            set { pType = value; }
        }



        public string PUnits
        {
            get { return pUnits; }
            set { pUnits = value; }
        }

        public string PDescription
        {
            get { return pDescription; }
            set { pDescription = value; }

        }
        #endregion

        #region Constructors
        public EarLabParameter(string pName, string pType, string pUnits, string pDescription)
        {
            this.pName = pName;
            this.pType = pType;
            this.pUnits = pUnits;
            this.pDescription = pDescription;
        }
        #endregion

    } // End the Abstract Parameter

    public abstract class EarLabParameter<T> : EarLabParameter
    {

        // The abstract class has all the String Parts of the parameter


        // The Generic has all the variable parts of the parameter

        #region Data Members
        protected T pDefault;
        protected T pMin;
        protected T pMax;
        protected T pValue;
        #endregion

        #region Data Properties
        public T PDefault
        {
            get { return pDefault; }
            set { pDefault = value; }
        }



        public T PMin
        {
            get { return pMin; }
            set { pMin = value; }

        }


        public T PMax
        {
            get { return pMax; }
            set { pMax = value; }

        }



        public T PValue
        {
            get { return pValue; }
            set { pValue = value; }

        }
        #endregion



        public EarLabParameter(string pName, string pType, string pUnits, string pDescription, T pDefault, T pMin, T pMax, T pValue)
            : base(pName, pType, pUnits, pDescription)
        {
            this.pDefault = pDefault;
            this.pMin = pMin;
            this.pMax = pMax;
            this.pValue = pValue;
        }


    }
    // End the Generic Set

    public class EarLabParameterInteger : EarLabParameter<int>
    {

        //Default constructor should never be used
        //Because it would mean that 
        public EarLabParameterInteger(string pName, string pType, string pUnits, string pDescription, int newDefault, int newMin, int newMax, int newValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue) //( string newName, string newType, string newUnits, string newDescriptions)
        {
        }

    }
    //end of the Integer Generic Parameter

    public class EarLabParameterDouble : EarLabParameter<double>
    {
        public EarLabParameterDouble(string pName, string pType, string pUnits, string pDescription, double newDefault, double newMin, double newMax, double newValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue)//( string newName, string newType, string newUnits, string newDescriptions)
        {
        }

    }

    public class EarLabParameterBoolean : EarLabParameter<bool>
    {    //(ParameterName, ParameterType, ParameterUnits, ParameterDescription, boolDefault, boolMin, boolMax, boolValue);
        public EarLabParameterBoolean(string pName, string pType, string pUnits, string pDescription, bool newDefault, bool newMin, bool newMax, bool newValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue)//( string newName, string newType, string newUnits, string newDescriptions)
        {
        }

    }

    public class EarLabParameterString : EarLabParameter<string>
    {
        public EarLabParameterString(string pName, string pType, string pUnits, string pDescription, string newDefault, string newMin, string newMax, string newValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue)//( string newName, string newType, string newUnits, string newDescriptions)
        {
        }

    }

//EarLabParameters Arrays


    public class EarLabParameterIntegerArray : EarLabParameter<int[]>
    {
        public EarLabParameterIntegerArray(string pName, string pType, string pUnits, string pDescription, int[] newDefault, int[] newMin, int[] newMax, int[] newValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue)//( string newName, string newType, string newUnits, string newDescriptions)
        {
        }

    }

    //double and float?
    public class EarLabParameterDoubleArray : EarLabParameter<double[]>
    {
        public EarLabParameterDoubleArray(string pName, string pType, string pUnits, string pDescription, double[] newDefault, double[] newMin, double[] newMax, double[] newValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue)//( string newName, string newType, string newUnits, string newDescriptions)
        {
        }
    }

    public class EarLabParameterBooleanArray : EarLabParameter<bool[]>
    {
        public EarLabParameterBooleanArray(string pName, string pType, string pUnits, string pDescription, bool[] newDefault, bool[] newMin, bool[] newMax, bool[] newValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue)//( string newName, string newType, string newUnits, string newDescriptions)
        {
        }
    }
    
    public class EarLabParameterStringArray : EarLabParameter<string[]>
    {
        public EarLabParameterStringArray(string pName, string pType, string pUnits, string pDescription, string[] newDefault, string[] newMin, string[] newMax, string[] newValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue)//( string newName, string newType, string newUnits, string newDescriptions)
        {
        }
    }

/////////////Outputs
/* 
 * EarLabOutput
 *      EarLabOutput<T>
 *          EarLabOutput: (???)
 * 
 */
    public abstract class EarLabOutput : EarLabSession
    {





    }

    public abstract class EarLabOutput<T> : EarLabOutput
    {




    }

    public class EarLabOutputInt : EarLabOutput<int>
    {
    }

  

/////////////Inputs
/*
 * EarLabInput
 *      EarLabInput<T>
 *          EarLabInput: (???)
 */
    public abstract class EarLabInput : EarLabSession
    {





    }

    public abstract class EarLabInput<T> : EarLabInput
    {




    }

    public class EarLabInputInt : EarLabInput<int>
    {
    }



}





