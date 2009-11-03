using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace RunfileEditor
{

    /// <summary>
    /// Array Check is a class that contains methods to do validations on the Earlab XML specification for an 
    /// array of data. The data can be in the format of an integer, float or double.  
    /// The methods are written to behave in the same fashion that the "tryparse" C# methods work to parse strings
    /// for integer, float or double values.  
    /// In fact, the tryparse method is used to parse the individual element string values.
    ///
    ////<Parameter>
    ////  <Name>IntArrayParam</Name>
    ////  <Type>Integer[]</Type>
    ////  <Value>
    ////    <Element>1</Element>
    ////    <Element>2</Element>
    ////    <Element>3</Element>
    ////    <Element>4</Element>
    ////    <Element>5</Element>
    ////    <Element>6</Element>
    ////  </Value>
    ////</Parameter>
    /// 
    /// If the data contained in the Earlab definied array is valid, you receive a "True" bool variable, the data is converted to an array and returned. 
    /// if the data contained in the Earlab definied array is invalid, you receive a "False" bool variable, and a null array is returned.
    /// </summary>
    public class ArrayCheck
    {
        /// <summary>
        /// If the data contained in the Earlab definied array is valid, you receive a "True" bool variable, the data is converted to an integer array and returned. 
        /// if the data contained in the Earlab definied array is invalid, you receive a "False" bool variable, and a null array is returned.
        /// 
        /// </summary>
        /// <param name="ParameterNode"></param>
        /// The method takes as an input a parameter node containing the following nestled XML Tags: Name, Type, Value and Element. 
        /// Each of the element tags contain a single value as a string.
        /// Inside the element tag, the int.tryparse is used to attempt to parse the string into an integer. 
        /// <param name="intArray"></param>
        /// If the parameter node can be parsed into an array of integers, than an integer array and "True" bool is returned.
        /// This means that all of the individual element tags were parsed by int.tryparse successfully.
        /// 
        /// If the parameter node can not be parsed into an array of integers, than null array and "False" bool is returned.
        /// This means that one of the individual element tags failed to be parsed by the int.tryparse.
        /// <returns>
        /// If the parameter node can be parsed "True" bool is returned, if not "False" bool is returned.
        ///</returns>
        public static bool intArrayReturn(XmlNode ParameterNode, out int[] intArray)
        {
            //Create the array initially as a list collection, b/c there useful tools.  
            List<int> ParameterValues = new List<int>();

            //A single variable integer to store a successful parsed value from an element.
            int singleInteger;

            //XML statement to make a node list of the array values
            XmlNodeList arrayValues = ParameterNode.ChildNodes;

                //This foreach control statement goes through the XML Node List 
                //It grabs each Element in sequential order
                //Parses the element
                    //If the element parses
                        //store the accepted integer value into the ParameterValues
                    //Else
                        //make the array null
                        //retur a bool value of false.
                        foreach (XmlElement arrayPoint in arrayValues)
                        {

                            //add value from point to the next.
                            if (int.TryParse(arrayPoint.InnerText, out singleInteger))
                            {
                                ParameterValues.Add(singleInteger);
                            }
                            else
                            {
                                intArray = null;
                                return false;
                            }
                        }

            //Change the List into an Array with the ToArray Method.
            //This Array will now go out, using the out command.
            intArray = ParameterValues.ToArray();

            //
            return true;
        }

        /// <summary>
        /// If the data contained in the Earlab definied array is valid, you receive a "True" bool variable, the data is converted to an double array and returned. 
        /// if the data contained in the Earlab definied array is invalid, you receive a "False" bool variable, and a null array is returned.
        /// 
        /// </summary>
        /// <param name="ParameterNode"></param>
        /// The method takes as an input a parameter node containing the following nestled XML Tags: Name, Type, Value and Element. 
        /// Each of the element tags contain a single value as a string.
        /// Inside the element tag, the int.tryparse is used to attempt to parse the string into an double. 
        /// <param name="intArray"></param>
        /// If the parameter node can be parsed into an array of doubles, than an double array and "True" bool is returned.
        /// This means that all of the individual element tags were parsed by int.tryparse successfully.
        /// 
        /// If the parameter node can not be parsed into an array of doubles, than null array and "False" bool is returned.
        /// This means that one of the individual element tags failed to be parsed by the int.tryparse.
        /// <returns></returns>
        /// 
        public static bool dblArrayReturn(XmlNode ParameterNode, out double[] dblArray)
        {
            //Create the array initially as a list collection, b/c there useful tools.  
            List<double> ParameterValues = new List<double>();

            //A single variable integer to store a successful parsed value from an element.
            double singleDouble;

            //XML statement to make a node list of the array values
            XmlNodeList arrayValues = ParameterNode.ChildNodes;

                    //This foreach control statement goes through the XML Node List 
                    //It grabs each Element in sequential order
                    //Parses the element
                        //If the element parses
                            //store the accepted integer value into the ParameterValues
                        //Else
                            //make the array null
                            //return a bool value of false.
                    foreach (XmlElement arrayPoint in arrayValues)
                    {
                        //double singleDouble;
                        //add value from point to the next.
                        if (double.TryParse(arrayPoint.InnerText, out singleDouble))
                        {
                            ParameterValues.Add(singleDouble);
                        }
                        else
                        {
                            //ParameterValues.Add(null);
                            dblArray = null;
                            return false;
                        }
                    }
            //Change the List into an Array with the ToArray Method.
            //This Array will now go out, using the out command.
            dblArray = ParameterValues.ToArray();

            //The array is ok, so return true
            return true;
        }

        //public bool ArrayEquality
        public static bool ArrayEquality(double[] Array1, double[] Array2)
        {
            if (Array1.Length != Array2.Length)
                return false;

            for (int i = 0; i < Array2.Length; i++)
            {
                if (Array1[i] != Array2[i])
                    return false;
            }
            return true;

        }


        //overload to handle ints
        public static bool ArrayEquality(int[] Array1, int[] Array2)
        {
            if (Array1.Length != Array2.Length)
                return false;

            for (int i = 0; i < Array2.Length; i++)
            {
                if (Array1[i] != Array2[i])
                    return false;
            }
            return true;

        }




    }



}
