using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using VisualHint.SmartPropertyGrid;


namespace RunfileEditor
{

    #region EarlabSession Notes
    ////EarlabSession is the base class
    ////EarlabModule

    ////EarlabParameter
    ////     EarlabParameter<T>
    ////        EarlabParameter: (int,dbl, bool, string, int[], dbl[], bool[], string[])

    ////EarlabInput
    ////     EarlabInput<T>
    ////          EarlabInput: (???)
     
    ////EarlabOutput
    ////     EarlabOutput<T>
    ////          EarlabOutput: (???)
    #endregion

    /// <summary>
    /// This is an abstract class that serves as the base class for all data the classes 
    /// in the Earlab [RunfileProject] that store data from the XML Runfile
    /// </summary>
    /// 

    public abstract class EarlabSession
    {
        #region Data Members

            //This is a "dirty bit" that says if the item has been updated.
            private bool mHasChanged = false;

            //Add on the Child items back into the top.    
            private List<EarlabSession> mChildren = new List<EarlabSession>();

            //4.) Verification Errors
            public string Severity;
            public string Message;

        #endregion

        #region Data Properties
            public bool HasChanged
            {
                //sets the HasChanged value
                set {mHasChanged = value;}

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

    public class EarlabModule : EarlabSession
    {
        #region Data Members
            //Public Data -- this is actual data, not strings
            //So we have public collections from factories.

            //1.) Inputs
            //List<EarlabInput> EarlabInputs = new List<EarlabInput>();
            
            //2.) Outputs
            //List<EarlabOutput> EarlabOutputs = new List<EarlabOutput>();

            //3.) Parameters
            //List<EarlabOutput> EarlabOutputs = new List<EarlabOutput>();
            public EarlabModuleInformation theEarlabModuleInformation;
            public List<EarlabParameter> EarlabParameters = new List<EarlabParameter>();
            public List<EarlabInput> EarlabInputs = new List<EarlabInput>();
            public List<EarlabOutput> EarlabOutputs = new List<EarlabOutput>();

            //4.) Verification Errors
            //public string Severity;
            //public string Message;

        #endregion


        #region Constructors
                public EarlabModule()
                {
                }
                

               /// <summary>
               /// The constructor for EarlabModule.
               /// 
               /// </summary>
               /// <param name="StringModule"></param>
               /// <param name="theModuleInfo"></param>
                public EarlabModule(RunfileModule StringModule, ModuleXML theModuleInfo)
                {
                        // I + O + P 
                        //call factory up, factory adds to base collection
                        //
                        // Does it make more sense to do all these things in one while loop?
                        // This way can isolate errors...


                    theEarlabModuleInformation = new EarlabModuleInformation(StringModule.ModuleInformation);


                    foreach (RunfileInput rInput in StringModule.theRunfileInputs)
                         {
                            //1.) Inputs
                            //EarlabInputs(EarlabInputFactory.Make(rInput, theModuleInfo[rInput.InputName, rInput.InputType.ToLower()]);

                         }

                    foreach (RunfileOutput rOutput in StringModule.theRunfileOutputs)
                        {
                                //2.) Outputs
                                //EarlabOutputs(EarlabOutputFactory.Make(rOutput, theModuleInfo[rOutput.OutputName, rOutput.OutputType]);

                        }
                   
                    foreach (RunfileParameter Param in StringModule.RunfileParameters)
                       {
                         //3.)(?) Parameters need to fix!
                         EarlabParameters.Add( EarlabParameterFactory.Make(Param, theModuleInfo[Param.ParameterName.ToLower(), Param.ParameterType.ToLower()]) );

                       }


                   }
        #endregion


     }

    public class EarlabModuleInformation
    {
        #region Data Members
                public string ExecutableName;

                public string InstanceName;

        #endregion

        #region Constructors
            public EarlabModuleInformation()
            {
            }

            public EarlabModuleInformation(RunfileModuleInformation ModuleInfo )
            {
                ExecutableName = ModuleInfo.ExecutableName;

                InstanceName = ModuleInfo.InstanceName;

            }
        #endregion


    }

    #region Earlab Parameter Notes
            //
            /*
             * I+O+P
             * 
             * public abstract class EarlabParameter : EarlabSession
             * public abstract class EarlabParameter<T>: EarlabParameter
             * public class EarlabParameterInteger : EarlabParameter<int>
             * public class EarlabParameterDouble : EarlabParameter<double>
             * public class EarlabParameterBoolean : EarlabParameter<bool>
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
             * public abstract class EarlabParameter
             * public abstract class EarlabParameter<T>: EarlabParameter
             * public class EarlabParameterInteger : EarlabParameter<int>
             * public class EarlabParameterDouble : EarlabParameter<double>
             * public class EarlabParameterBoolean : EarlabParameter<bool>
             * ????????????? No idea how to add these
             * Integer Array
             * Double Array
             */
            //
    #endregion

    /// <summary>
    /// An abstract class that inherits form the the EarlabSession class.
    /// All parameters are derived from this class.
    /// </summary>
    public abstract class EarlabParameter : EarlabSession
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
                public EarlabParameter(string pName, string pType, string pUnits, string pDescription)
                {
                    this.pName = pName;
                    this.pType = pType;
                    this.pUnits = pUnits;
                    this.pDescription = pDescription;
                }
        #endregion

    } // End the Abstract Parameter

    public abstract class EarlabParameter<T> : EarlabParameter
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



        public EarlabParameter(string pName, string pType, string pUnits, string pDescription, T pDefault, T pMin, T pMax, T pValue)
            : base(pName, pType, pUnits, pDescription)
        {
            this.pDefault = pDefault;
            this.pMin = pMin;
            this.pMax = pMax;
            this.pValue = pValue;
        }


    }
    // End the Generic Set

    public class EarlabParameterInteger : EarlabParameter<int>
    {

        //Default constructor should never be used
        //Because it would mean that 
        public EarlabParameterInteger(string pName, string pType, string pUnits, string pDescription, int newDefault, int newMin, int newMax, int newValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue) //( string newName, string newType, string newUnits, string newDescriptions)
        {
        }

    }
    //end of the Integer Generic Parameter

    public class EarlabParameterDouble : EarlabParameter<double>
    {
        public EarlabParameterDouble(string pName, string pType, string pUnits, string pDescription, double newDefault, double newMin, double newMax, double newValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue)//( string newName, string newType, string newUnits, string newDescriptions)
        {
        }

    }

    public class EarlabParameterBoolean : EarlabParameter<bool>
    {    //(ParameterName, ParameterType, ParameterUnits, ParameterDescription, boolDefault, boolMin, boolMax, boolValue);
        public EarlabParameterBoolean(string pName, string pType, string pUnits, string pDescription, bool newDefault, bool newMin, bool newMax, bool newValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue)//( string newName, string newType, string newUnits, string newDescriptions)
        {
        }

    }

    public class EarlabParameterString : EarlabParameter<string>
    {
        public EarlabParameterString(string pName, string pType, string pUnits, string pDescription, string newDefault, string newMin, string newMax, string newValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue)//( string newName, string newType, string newUnits, string newDescriptions)
        {
        }

    }

//EarlabParameters Arrays


    public class EarlabParameterIntegerArray : EarlabParameter<int[]>
    {
        public EarlabParameterIntegerArray(string pName, string pType, string pUnits, string pDescription, int[] newDefault, int[] newMin, int[] newMax, int[] newValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue)//( string newName, string newType, string newUnits, string newDescriptions)
        {
        }

    }

    //double and float?
    public class EarlabParameterDoubleArray : EarlabParameter<double[]>
    {
        public EarlabParameterDoubleArray(string pName, string pType, string pUnits, string pDescription, double[] newDefault, double[] newMin, double[] newMax, double[] newValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue)//( string newName, string newType, string newUnits, string newDescriptions)
        {
        }
    }

    public class EarlabParameterBooleanArray : EarlabParameter<bool[]>
    {
        public EarlabParameterBooleanArray(string pName, string pType, string pUnits, string pDescription, bool[] newDefault, bool[] newMin, bool[] newMax, bool[] newValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue)//( string newName, string newType, string newUnits, string newDescriptions)
        {
        }
    }
    
    public class EarlabParameterStringArray : EarlabParameter<string[]>
    {
        public EarlabParameterStringArray(string pName, string pType, string pUnits, string pDescription, string[] newDefault, string[] newMin, string[] newMax, string[] newValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue)//( string newName, string newType, string newUnits, string newDescriptions)
        {
        }
    }

/////////////Outputs
/* 
 * EarlabOutput
 *      EarlabOutput<T>
 *          EarlabOutput: (???)
 * 
 */
    public abstract class EarlabOutput : EarlabSession
    {





    }

    public abstract class EarlabOutput<T> : EarlabOutput
    {




    }

    public class EarlabOutputInt : EarlabOutput<int>
    {
    }

  

/////////////Inputs
/*
 * EarlabInput
 *      EarlabInput<T>
 *          EarlabInput: (???)
 */
    public abstract class EarlabInput : EarlabSession
    {





    }

    public abstract class EarlabInput<T> : EarlabInput
    {




    }

    public class EarlabInputInt : EarlabInput<int>
    {
    }



}





