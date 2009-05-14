using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;



namespace RunfileEditor
{
    /// <summary>
    /// This class contains custom Array Checks
    /// on Node to int Arrays 
    /// and
    /// on Node to dbl Arrays
    /// if it is okay, you receive a True and the Array
    /// if it is bad, you receive a False
    /// wrote these array returning functions like the "tryparse" for error checking.
    /// if parsing doesn't work a null array is returned.
    /// This doesn't work
    /// 
    /// </summary>
    /// 
    public class ArrayCheck
    {




        /// <summary>
        /// This is a custom method that accepts an int XML array Node
        /// and returns a bool (true/false), and the Array if true.
        /// </summary>
        /// <param name="ParameterNode"></param>
        /// <param name="intArray"></param>
        /// <returns></returns>
        public static bool intArrayReturn(XmlNode ParameterNode, out int[] intArray)
        {

            List<int> ParameterValues = new List<int>();
            int singleInteger;


            //(xml statement)
            XmlNodeList arrayValues = ParameterNode.ChildNodes;

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

            intArray = ParameterValues.ToArray();
            return true;
        }

        //wrote these array returning functions like the "tryparse" for error checking.
        //if parsing doesn't work a null array is returned.
        // doesnt work

        /// <summary>
        /// This is a custom method that accepts an int XML array Node
        /// and returns a bool (true/false), and the Array if true.
        /// </summary>
        /// <param name="ParameterNode"></param>
        /// <param name="intArray"></param>
        /// <returns></returns>
        public static bool dblArrayReturn(XmlNode ParameterNode, out double[] dblArray)
        {

            List<double> ParameterValues = new List<double>();
            //haha.
            double singleDouble;


            ///INFO!!!! XmlNodeList ePList = ModuleNode["Parameters"].ChildNodes;
            /*
       <Parameter>
        <Name>IntArrayParam</Name>
        <Type>Integer[]</Type>
        <Value>
          <Element>1</Element>
          <Element>2</Element>
          <Element>3</Element>
          <Element>4</Element>
          <Element>5</Element>
          <Element>6</Element>
        </Value>
      </Parameter>
             */

            //Break
            //XmlNodeList arrayItems = ParameterNode["Value"].GetElementsByTagName("Element");
            XmlNodeList arrayValues = ParameterNode.ChildNodes;

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

            dblArray = ParameterValues.ToArray();
            return true;
        }

    }



}
