using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using VisualHint.SmartPropertyGrid;

namespace SmartPropertyGridTester
{
    public partial class Form1 : Form
    {
        private List<EarlabParameter> mParamList = new List<EarlabParameter>();
        public Form1()
        {
            InitializeComponent();

            propertyGrid.CommentsVisibility = true;
            propertyGrid.CommentsHeight = 100;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            int[] zeros = {0, 0, 0, 0, 0};
            int[] ones = {1, 1, 1, 1, 1, 1};
            double[] dArray = { 1.2, 3.4, 5.6, Math.PI, Math.E };
            EarlabArray<int> zeroArray = new EarlabArray<int>(zeros);
            EarlabArray<int> oneArray = new EarlabArray<int>(ones);
            EarlabArray<double> doubleArray = new EarlabArray<double>(dArray);

            mParamList.Add(new EarlabString("ExperimentName", "Name of the experiment", "Nothing", "myExperiment"));
            mParamList.Add(new EarlabBoolean("EnableTestMode", "Test mode enabled", false, false));
            mParamList.Add(new EarlabIntArray("IntArrayParam", "This is an integer array", zeroArray, oneArray));
            mParamList.Add(new EarlabFloatArray("FloatArrayParam", "This is an float array", doubleArray, doubleArray));
            propertyGrid.ParameterList = mParamList;
        }

        private void DisplayParameters(EarlabParameter myParam)
        {
        }

        private void btnDone_Click(object sender, EventArgs e)
        {
            //MessageBox.Show("String: \"" + myParam.Value + "\"", "Property display");
            //DisplayParameters();
        }
    }

    public enum EarlabDataType
    {
        Integer, Float, String, IntArray, FloatArray, InputFile, OutputFile
    }

    public class FooEarlabParameter<DataType>
    {
        protected string mName;
        protected EarlabDataType mType;
        protected DataType mData;
        protected DataType mDefaultValue;
        protected string mDescription;

        public FooEarlabParameter(string Name, EarlabDataType Type, DataType Data, string Description)
        {
            mName = Name;
            mType = Type;
            mData = Data;
            mDefaultValue = Data;
            mDescription = Description;
        }

        public string Name { get { return mName; } }
        public EarlabDataType Type { get { return mType; } }
        public string Description { get { return mDescription; } }
        public DataType Default
        {
            get { return mDefaultValue; }
            set { mDefaultValue = value; }
        }

        public DataType Value
        {
            get { return mData; }
            set { mData = value; }
        }

        public bool ValueHasChanged
        {
            get
            {
                if (mDefaultValue.Equals(mData))
                    return true;
                return false;
            }
        }
    }


}