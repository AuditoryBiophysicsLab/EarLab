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


    #region Event Framework 1 of 3
    //public class ValueChangedEventArgs<T> : EventArgs
    //{
    //    public readonly T LastValue;
    //    public readonly T NewValue;

    //    public ValueChangedEventArgs(T lastValue, T newValue)
    //    {
    //        LastValue = lastValue;
    //        NewValue = newValue;

    //    }

    //}

    //public static class OnEventUpdatingStuff
    //{

    //    public static void parameter_ValueChanged(object sender, ValueChangedEventArgs<int> e)
    //    {
    //        if (e.NewValue > e.LastValue)
    //            Console.WriteLine("New Value Greater than old value, Run EFI and Repaint!");

    //        else
    //            Console.WriteLine("New value less than or equal to old value, Nothing is going on");

    //    }

    //    public static void parameter_ValueChanged2(object sender, ValueChangedEventArgs<int> e)
    //    {
    //        if ((e.NewValue - e.LastValue) > 10)
    //            Console.WriteLine("Difference is greater than 10, Run EFI and Repaint!");



    //    }

    //}

    //message
    public class DataChangedEventArgs : EventArgs
    {

        //This Event takes no data.
        public DataChangedEventArgs()
        {
        }

    }


    public class ValueChangedEventArgs : EventArgs
    {

        //This Event takes no data.
        public ValueChangedEventArgs()
        {
        }

    }



    #endregion 





    /// <summary>
    /// This is an abstract class that serves as the base class for all data the classes 
    /// in the Earlab [RunfileProject] that store data from the XML Runfile
    /// </summary>
    /// 
    public abstract class EarlabSession
    {
        #region Protected and Private Data Members

            //This is a "dirty bit" that says if the item has been updated.
            protected bool mHasChanged = false;
            //Add on the Child items back into the top.    
            protected List<EarlabSession> mChildren = new List<EarlabSession>();


            //[Unused]
            //Carry the Node for "has changed" and Writing back.
            private XmlNode mOriginalNode;

            //Verification Errors -- will change these to private
            private string mSeverity ="";
            private string mMessage  ="";

        #endregion

        #region Data Properties and Event Handlers


        //====Event Related Methods ===================================================///
             
            //[Changed Value]
            public event EventHandler ValueChanged;

            protected virtual void OnValueChanged()
            {
                if (ValueChanged != null)
                    ValueChanged(this, new EventArgs());
            }
            //[Changed Value]

            
            //[Changed Message]
            public event EventHandler DataChanged;
            
            protected virtual void OnDataChanged()
            {
                if (DataChanged != null)
                    DataChanged(this, new EventArgs());
            }
            //[end Changed Message]



        //====Virtual Implementations ==================================================///
        public virtual string Message
        {
            set 
            { 
                    mMessage = value;

            }

            get 
            {
                    return mMessage; 
            
            }

        }


        public virtual string Severity
        {

            set { 
                mSeverity = value; 
            }

            get { 
                return mSeverity; 
            }

        }


        //====Abstract implementations ================================================///

        public abstract bool HasChanged
        {
            get;
            set;
        }

        public abstract List<EarlabSession> Children
        {
            get;
            set;
        }

        public abstract XmlNode OriginalNode
        {
            get;
            set;
        }

        #endregion
    }
    
    public class EarlabModuleInformation
    {
        #region Private Data Members
        private string mExecutableName;

        private string mInstanceName;

        #endregion

        #region Properties
        public string ExecutableName
        {

            get { return mExecutableName; }
            set { mExecutableName = value; }
        }
        public string InstanceName
        {
            get { return mInstanceName; }
            set { mInstanceName = value; }
        }



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

    public class EarlabModule : EarlabSession
    {
        #region Data Members

            //1.) Module Information:
            public EarlabModuleInformation theEarlabModuleInformation;

            //Public Data -- this is actual data, not strings
            //So we have public collections from factories.

            //2.) Inputs
            //List<EarlabInput> EarlabInputs = new List<EarlabInput>();
            
            //3.) Outputs
            //List<EarlabOutput> EarlabOutputs = new List<EarlabOutput>();

            //4.) Parameters
            public List<EarlabParameter> EarlabParameters = new List<EarlabParameter>();
            public List<EarlabInput> EarlabInputs = new List<EarlabInput>();
            public List<EarlabOutput> EarlabOutputs = new List<EarlabOutput>();

        #endregion





        ////[Display Event]
        //public ModuleGrid DisplayGrid; //testing something

        //private void SomethingChanged(object Sender, EventArgs e)
        //{
        //    DisplayGrid.Initialize(this);//[Display Event]
        //}


        #region Properties
        //1.) implement the abstract property HasChanged
        public override bool HasChanged
            {
                //sets the HasChanged value
                set { 
                    
                    mHasChanged = value; 
                
                
                }

                //get the mHasChanged
                get
                {
                    if (mHasChanged)
                    {
                        mHasChanged = true;
                        return mHasChanged;

                    }
                    //goes through each of the children to see
                    //if they have changed
                    for (int i = 0; i < Children.Count; i++)
                        if (Children[i].HasChanged)
                        {
                            return true;
                        }
                        
                    mHasChanged = false;
                    return mHasChanged;

                }//end get
            }

        //2.) store all valid IOP children to mChildren.
        public override List<EarlabSession> Children
        {
            //This works b/c we know each module has I-O-Ps, we just don't know how many.
            get
            {

                return mChildren;


            }

            set
            {   ////add inputs
                //foreach (EarlabSession parameter in EarlabInputs)
                //{
                //    mChildren.Add(parameter);

                //}

                ////add outputs
                //foreach (EarlabSession parameter in EarlabOutputs)
                //{
                //    mChildren.Add(parameter);

                //}

                //add parameters


                foreach (EarlabSession parameter in EarlabParameters)
                {
                    mChildren.Add(parameter);
                }

            }

            //I don't think set actually ever gets used.
            //but it has the same logic as get, save the return.
            //set
            //{
            //    throw new Exception("The method or operation is not implemented.");
            //}
        }

        //3.) store original node
        public override XmlNode OriginalNode
        {
            get
            {
                throw new Exception("The method or operation is not implemented.");
            }
            set
            {
                throw new Exception("The method or operation is not implemented.");
            }
        }

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

                        //[change] I need to store the node

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
                           EarlabParameter newParam = EarlabParameterFactory.Make(Param, theModuleInfo[Param.ParameterName.ToLower(), Param.ParameterType.ToLower()]);
                        //[Event Handler]
                        //newParam.DataChanged += new EventHandler(SomethingChanged);

                           
                         //3.)(?) Parameters need to fix!
                        EarlabParameters.Add(newParam);

                       }


                   }
        #endregion

     }

    #region IOP Items
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
            private string pName;
            private string pType;
            private string pUnits;
            private string pDescription;
        #endregion

        #region Data Properties for that are all Strings

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
                public override List<EarlabSession> Children
                {
                    //A parameter doesn't have children.
                    get
                    {
                        return mChildren;
                    }
                    set
                    {
                        mChildren = null;
                    }
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
                protected T pOValue; //this is the temporary solution to handle "haschanged"
        #endregion

        #region Data Properties


            #region Event Framework 2 of 3
        //public event EventHandler<ValueChangedEventArgs<T>> ValueChanged;

                //protected abstract void OnValueChanged(ValueChangedEventArgs<T> e);
                //{
                //    if (PriceChanged != null) PriceChanged(this, e);
                //}


                public abstract T PValue
                {
                    get;

                    set;
                   

                }
                //end event framework
                #endregion


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

            //[Remove] -- constructor, factory
            public T POValue
            {
                get { return pOValue; }
                set { pOValue = value; }

            }


        #endregion

        #region Override Properties
        //1.) implement the abstract property HasChanged b/c of the template you have to do it at the last level.
        public override bool HasChanged
        {
            //sets the HasChanged value
            set
            {
                mHasChanged = value;

            }

            //get the mHasChanged
            get
            {
                //if (pValue == pOValue)
                //{
                //    mHasChanged = false;

                //}
                //else
                //{
                //    mHasChanged = true;

                //}

                throw new Exception("The method or operation is not implemented.");
            }//end get
        }

            //2.) store all valid IOP children to mChildren.


        //3.) store original node
        public override XmlNode OriginalNode
        {
            get
            {
                throw new Exception("The method or operation is not implemented.");
            }
            set
            {
                throw new Exception("The method or operation is not implemented.");
            }
        }

        #endregion

        // Must ad pOValue to all these parameters
        public EarlabParameter(string pName, string pType, string pUnits, string pDescription, T pDefault, T pMin, T pMax, T pValue, T pOValue)
            : base(pName, pType, pUnits, pDescription)
        {
            this.pDefault = pDefault;
            this.pMin = pMin;
            this.pMax = pMax;
            this.pValue = pValue;
            this.pOValue = pOValue;
        }


    }
    // End the Generic Set

    public class EarlabParameterInteger : EarlabParameter<int>
    {
        #region Override Properties
            public override bool HasChanged
            {
                //sets the HasChanged value
                set
                {
                    mHasChanged = value;

                }

                //get the mHasChanged
                get
                {
                  
                    
                    return mHasChanged;

                }//end get
            }


        public override int PValue
        {
            get
            {
                return pValue;

            }
            set
            {
                if (pValue == value)
                    return;
                else
                {
                    
                    pValue = value;
                    OnDataChanged();
                }

        }
        #endregion

            }
  

        //Default constructor should never be used
        //Because it would mean that 
        public EarlabParameterInteger(string pName, string pType, string pUnits, string pDescription, int newDefault, int newMin, int newMax, int newValue, int oldValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue, oldValue) //( string newName, string newType, string newUnits, string newDescriptions)
        {
        }

    }

    public class EarlabParameterDouble : EarlabParameter<double>
    {
        #region Override Properties
        public override bool HasChanged
        {
            //sets the HasChanged value
            set
            {
                mHasChanged = value;

            }

            //get the mHasChanged
            get
            {
                if (PValue == POValue)
                {
                    mHasChanged = false;

                }
                else
                {
                    mHasChanged = true;

                }

                return mHasChanged;

            }//end get
        }
        
        public override double PValue
        {
            get
            {
                return pValue;

            }
            set
            {
                if (pValue == value)
                    return;
                else
                {
                    
                    pValue = value;
                    OnDataChanged();
                }

            }


        }
        #endregion


        public EarlabParameterDouble(string pName, string pType, string pUnits, string pDescription, double newDefault, double newMin, double newMax, double newValue, double oldValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue, oldValue)//( string newName, string newType, string newUnits, string newDescriptions)
        {
        }

    }

    public class EarlabParameterBoolean : EarlabParameter<bool>
    {
        #region Override Properties
        public override bool HasChanged
        {
            //sets the HasChanged value
            set
            {
                mHasChanged = value;

            }

            //get the mHasChanged
            get
            {
                if (PValue == POValue)
                {
                    mHasChanged = false;

                }
                else
                {
                    mHasChanged = true;

                }

                return mHasChanged;

            }//end get
        }


        public override bool PValue
        {
            get
            {
                return pValue;

            }
            set
            {
                if (pValue == value)
                    return;
                else
                {
                    
                    pValue = value;
                    OnDataChanged();
                }

            }


        }
        #endregion


        public EarlabParameterBoolean(string pName, string pType, string pUnits, string pDescription, bool newDefault, bool newMin, bool newMax, bool newValue, bool oldValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue, oldValue)//( string newName, string newType, string newUnits, string newDescriptions)
        {
        }

    }

    public class EarlabParameterString : EarlabParameter<string>
    {
        #region Override Properties
        public override bool HasChanged
        {
            //sets the HasChanged value
            set
            {
                mHasChanged = value;

            }

            //get the mHasChanged
            get
            {
                if (PValue == POValue)
                {
                    mHasChanged = false;

                }
                else
                {
                    mHasChanged = true;

                }

                return mHasChanged;

            }//end get
        }



        public override string PValue
        {
            get
            {
                return pValue;

            }
            set
            {
                if (pValue == value)
                    return;
                else
                {
                    
                    pValue = value;
                    OnDataChanged();
                }

            }
        #endregion

        }

        public EarlabParameterString(string pName, string pType, string pUnits, string pDescription, string newDefault, string newMin, string newMax, string newValue, string oldValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue, oldValue)//( string newName, string newType, string newUnits, string newDescriptions)
        {
        }

    }


    public class EarlabParameterIntegerArray : EarlabParameter<int[]>
    {
        #region Override Properties
        public override bool HasChanged
        {
            //sets the HasChanged value
            set
            {
                mHasChanged = value;

            }

            //get the mHasChanged
            get
            {
                if( ArrayCheck.ArrayEquality(PValue, POValue) )
                {
                    mHasChanged = false;


                }
                else
                {
                    mHasChanged = true;

                }

                return mHasChanged;

            }//end get
        }


        public override int[] PValue
        {
            get
            {
                return pValue;

            }
            set
            {
                if (pValue == value)
                    return;
                else
                {
                   
                    pValue = value;
                    OnDataChanged();
                }

            }
        #endregion

        }


        //protected override void OnValueChanged(ValueChangedEventArgs<int[]> e)
        //{
        //    throw new Exception("The method or operation is not implemented.");
        //}
        public EarlabParameterIntegerArray(string pName, string pType, string pUnits, string pDescription, int[] newDefault, int[] newMin, int[] newMax, int[] newValue, int[] oldValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue, oldValue)//( string newName, string newType, string newUnits, string newDescriptions)
        {
        }

    }

    //HasChanged has to be different here because these guys are arrays.
    public class EarlabParameterDoubleArray : EarlabParameter<double[]>
    {
        #region Override Properties
        public override bool HasChanged
        {
            //sets the HasChanged value
            set
            {
                mHasChanged = value;

            }

            //get the mHasChanged
            get
            {
                if ( ArrayCheck.ArrayEquality( PValue, POValue) )
                {
                    mHasChanged = false;


                }
                else
                {
                    mHasChanged = true;

                }

                return mHasChanged;

            }//end get
        }


        public override double[] PValue
        {
            get
            {
                return pValue;

            }
            set
            {
                if (pValue == value)
                    return;

                else
                {
                    
                    pValue = value;
                    OnDataChanged();
                }

            }
        #endregion

        }

        public EarlabParameterDoubleArray(string pName, string pType, string pUnits, string pDescription, double[] newDefault, double[] newMin, double[] newMax, double[] newValue, double[] oldValue)
            : base(pName, pType, pUnits, pDescription, newDefault, newMin, newMax, newValue, oldValue)//( string newName, string newType, string newUnits, string newDescriptions)
        {
        }
    }


    #region Inputs and Outputs
        ///////////Outputs
    /* 
     * EarlabOutput
     *      EarlabOutput<T>
     *          EarlabOutput: (???)
     * 
     */

        public abstract class EarlabOutput : EarlabSession
        {
            public override bool HasChanged
            {
                get
                {
                    throw new Exception("The method or operation is not implemented.");
                }
                set
                {
                    throw new Exception("The method or operation is not implemented.");
                }
            }

            public override List<EarlabSession> Children
            {
                get
                {
                    throw new Exception("The method or operation is not implemented.");
                }
                set
                {
                    throw new Exception("The method or operation is not implemented.");
                }

            }

            public override XmlNode OriginalNode
            {
                get
                {
                    throw new Exception("The method or operation is not implemented.");
                }
                set
                {
                    throw new Exception("The method or operation is not implemented.");
                }
            }





        }

        public abstract class EarlabOutput<T> : EarlabOutput
        {




        }

        public class EarlabOutputInt : EarlabOutput<int>
        {
            

        }

      

    ///////////Inputs
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

        //req classes
        public class EarlabInputInt : EarlabInput<int>
        {
            public override bool HasChanged
            {
                get
                {
                    throw new Exception("The method or operation is not implemented.");
                }
                set
                {
                    throw new Exception("The method or operation is not implemented.");
                }
            }

            public override List<EarlabSession> Children
            {
                get
                {
                    throw new Exception("The method or operation is not implemented.");
                }
                set
                {
                    throw new Exception("The method or operation is not implemented.");
                }

            }

            public override XmlNode OriginalNode
            {
                get
                {
                    throw new Exception("The method or operation is not implemented.");
                }
                set
                {
                    throw new Exception("The method or operation is not implemented.");
                }
            }



        }

    #endregion

    #endregion

}

