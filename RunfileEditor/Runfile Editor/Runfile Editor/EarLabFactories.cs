using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

/// <summary>
/// This File holds all the object factory classes that create 
/// </summary>
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



    //Done and working!
    /// <summary>
    /// This is an Object Factory that return an EarlabParameter
    /// Grabs XML node.
    /// </summary>
    public class EarlabParameterFactory
    {

        /// <summary>
        /// The default constructor is not used.
        /// </summary>
        public EarlabParameterFactory()
        {
        }


        /// <summary>
        /// It needs information from:
        ///  XML Runfile Data in string form
        ///  AND
        ///  XML Module which gives instructions on creating the EarlabParameter.
        /// Do we really need the 
        /// </summary>
        /// <param name="RFParameter"></param>
        /// <param name="XmlParameter"></param>
        /// <returns></returns>
        public static EarlabParameter Make(RunfileParameter RFParameter, ModuleXmlParameter XmlParameter)
        {
            #region Notes on Earlab Notes

            //Each Parameter has the following items inside:
            //Runfile:       Value
            //Module File:   Max, Min, Default, Units, Description

            //[2Write]Going to have to add an error-check routine here on the RFParameter routine.
            //Would I want a generic rountine to parse?
            //Yes, b/c this will be reused on all numbers 
            //And also need a routine to handle arrays and error checking
            //It will parse, and if OK send, if bad it will flag as false
            //and send default values back given info from Module XM


            /*
                   from module Xml
                  <Parameter>
                   <Name>SampleRate</Name>
                   <Type>Double</Type>
                   <Default>0</Default>
                   <Minimum>1</Minimum>
                   <Maximum>1000000</Maximum>
                   <Units>Hz</Units>
                   <Description>Sample rate, in Hertz</Description>
                 </Parameter>   

                      integer int from Runfile
                      <Parameter>
                           <Name>SampleRate</Name>
                           <Type>Double</Type>
                           <Value>1</Value>
                      </Parameter>
           */

            //Name and Type have to match.
            //Logic on how to deal with something outside the range
            // ----- Parameter from factory 
            // ModuleXML(Default, Min, Max, Units, Description)
            // RunfileXML(Value)
            #endregion


            #region Data Members
                string ParameterName;
                string ParameterType;
                string ParameterUnits;
                string ParameterDescription;

                //Private Actual values single values
                //Private Actual values Arrays
                //string[] stringArrayDefault, stringArrayMin, stringArrayMax, stringArrayValue;
                //bool[] boolArrayDefault, boolArrayMin, boolArrayMax, boolArrayValue;


                ParameterName = XmlParameter.ParameterName;
                ParameterType = XmlParameter.ParameterType;
                ParameterUnits = XmlParameter.ParameterUnits;
                ParameterDescription = XmlParameter.ParameterDescription;
            #endregion

        
            switch (ParameterType.ToLower())
            {
                case "integer":
                case "int":

                    int intDefault, intMin, intMax, intValue;

                    if (

                        !int.TryParse(XmlParameter.ParameterDefaultValue.InnerText, out intDefault) ||
                        !int.TryParse(XmlParameter.ParameterMinimumValue.InnerText, out intMin) ||
                        !int.TryParse(XmlParameter.ParameterMaximumValue.InnerText, out intMax) ||
                        !int.TryParse(RFParameter.ParameterValue.InnerText, out intValue)
                        )
                        return null;

                    else
                        return new EarlabParameterInteger(ParameterName, ParameterType, ParameterUnits, ParameterDescription,
                            intDefault, intMin, intMax, intValue);




                case "double":
                case "dbl":

                    double dblDefault, dblMin, dblMax, dblValue;

                    if (
                        !double.TryParse(XmlParameter.ParameterDefaultValue.InnerText, out dblDefault) ||
                        !double.TryParse(XmlParameter.ParameterMinimumValue.InnerText, out dblMin) ||
                        !double.TryParse(XmlParameter.ParameterMaximumValue.InnerText, out dblMax) ||
                        !double.TryParse(RFParameter.ParameterValue.InnerText, out dblValue)
                       ) return null;
                    return new EarlabParameterDouble(ParameterName, ParameterType, ParameterUnits, ParameterDescription,
                                                dblDefault, dblMin, dblMax, dblValue);

                case "boolean":
                case "bool":

                    //bool has problems
                    bool boolDefault, boolMin, boolMax, boolValue;
                    if (
                       !bool.TryParse(XmlParameter.ParameterDefaultValue.InnerText, out boolDefault) ||
                       !bool.TryParse(XmlParameter.ParameterMinimumValue.InnerText, out boolMin) ||
                       !bool.TryParse(XmlParameter.ParameterMaximumValue.InnerText, out boolMax) ||
                       !bool.TryParse(RFParameter.ParameterValue.InnerText, out boolValue)
                       ) return null;

                    return new EarlabParameterBoolean(ParameterName, ParameterType, ParameterUnits, ParameterDescription,
                                                boolDefault, boolMin, boolMax, boolValue);
                //I'll have to add some error checking here
                // an array of bool vars, and a method that checks
                // if any of the bools are false?

                case "string":
                case "str":

                    string stringDefault, stringMin, stringMax, stringValue;

                    stringDefault = XmlParameter.ParameterDefaultValue.InnerText;
                    stringMin = XmlParameter.ParameterMinimumValue.InnerText;
                    stringMax = XmlParameter.ParameterMaximumValue.InnerText;
                    stringValue = RFParameter.ParameterValue.InnerText;
                    //I'll have to add some error checking here
                    // an array of bool vars, and a method that checks
                    // if any of the bools are false?
                    return new EarlabParameterString(ParameterName, ParameterType, ParameterUnits, ParameterDescription,
                                            stringDefault, stringMin, stringMax, stringValue);


                //Arrays -- wrote some nifty parsing and checking functions.

                //Integer Array forms
                case "integer[]":
                case "int[]":
                case "integerarray":
                case "int_array":
                case "integer_array":
                case "int_array[]":
                case "integerarray[]":
                case "integer_array[]":

                    int[] intArrayDefault, intArrayMin, intArrayMax, intArrayValue;

                    if (
                       !ArrayCheck.intArrayReturn(XmlParameter.ParameterDefaultValue, out intArrayDefault) ||
                       !ArrayCheck.intArrayReturn(XmlParameter.ParameterMinimumValue, out intArrayMin) ||
                       !ArrayCheck.intArrayReturn(XmlParameter.ParameterMaximumValue, out intArrayMax) ||
                       !ArrayCheck.intArrayReturn(RFParameter.ParameterValue, out intArrayValue)
                        ) { return null; }

                    return new EarlabParameterIntegerArray(ParameterName, ParameterType, ParameterUnits, ParameterDescription,
                                                intArrayDefault, intArrayMin, intArrayMax, intArrayValue);


                //Double Array and Float why preserve
                case "double[]":
                case "dbl[]":
                case "doublearray[]":
                case "double_array[]":
                case "dbl_array[]":
                case "doublearray":
                case "double_array":

                //Float Array
                case "fl_array[]":
                case "float_array[]":
                case "floatarray[]":
                case "floatarray":
                case "float_array":
                case "float[]":
                case "fl[]":
                    double[] dblArrayDefault, dblArrayMin, dblArrayMax, dblArrayValue;
                    if (
                        !ArrayCheck.dblArrayReturn(XmlParameter.ParameterDefaultValue, out dblArrayDefault) ||
                        !ArrayCheck.dblArrayReturn(XmlParameter.ParameterMinimumValue, out dblArrayMin) ||
                        !ArrayCheck.dblArrayReturn(XmlParameter.ParameterMaximumValue, out dblArrayMax) ||
                        !ArrayCheck.dblArrayReturn(RFParameter.ParameterValue, out dblArrayValue)
                         )
                    { return null; }

                    return new EarlabParameterDoubleArray(ParameterName, ParameterType, ParameterUnits, ParameterDescription,
                                                dblArrayDefault, dblArrayMin, dblArrayMax, dblArrayValue);

                //string Array is sort of a problem -- how do you check a string?


            }

            throw new System.NotSupportedException("The parameter type " + RFParameter.ParameterType + " is not recognized.");
        }

    }









}












