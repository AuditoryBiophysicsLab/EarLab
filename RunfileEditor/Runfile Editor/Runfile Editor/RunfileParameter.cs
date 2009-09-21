using System;
using System.Collections.Generic;
using System.Text;

namespace RunfileEditor
{
    public class RunfileParameter
    {

        //Store the Node location/value rather than the string data.
        //

        /*
         * double -- double -- dbl
        <Parameter>
            <Name>SampleRate</Name>
            <Type>Double</Type>
            <Value>44100</Value>
        </Parameter>
            
         * boolean - bool 
       <Parameter>
            <Name>InputIsWaveFile</Name>
            <Type>Boolean</Type>
            <Value>true</Value>
       </Parameter>
         * 
       integer int 
       <Parameter>
            <Name>ChannelNumber</Name>
            <Type>Integer</Type>
            <Value>0</Value>
       </Parameter>
         * 
     </Parameter>   
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

        //Data Memembers
        public bool isArray;
        public string ParameterName;
        public string ParameterType;

        //Just a public node, which comprises Array or Single value
        public XmlNode ParameterValue;

        //Constructor
        public RunfileParameter(XmlNode ParameterNode)
        {
            //string TempValue;

            //(xml statement) 
            ParameterName = ParameterNode["Name"].InnerText;
            ParameterType = ParameterNode["Type"].InnerText;

            //basically we need a quick function to strip out all the white space
            ParameterType = ParameterType.Replace(" ", "");
            //Do we need to standardize how the ParameterType ends up stored? as the real or Temp?
            //I like the idea of standardizing the thing. (?)

            //Also, since we are storing this stuff as a string this is an 
            //if control structure as opposed to a switch,
            //(?) Reuse for parsing the data.
            if (ParameterType.ToLower() == "integer[]" || ParameterType.ToLower() == "double[]" || ParameterType.ToLower() == "string[]"
                || ParameterType.ToLower() == "bool[]" || ParameterType.ToLower() == "float[]")
            {
                //This node holds values
                //(xml statement) 
                ParameterValue = ParameterNode["Value"];
                isArray = true;
            }

            else
            {
                //This node holds a single value
                //(xml statement) 
                ParameterValue = ParameterNode["Value"];
                isArray = false;
            }
        }

    }
}
