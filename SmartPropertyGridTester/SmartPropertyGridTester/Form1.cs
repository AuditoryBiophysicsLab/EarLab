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
            mParamList.Add(EarlabParameterFactory.CreateParameter("Float", "SampleRate_Hz", "Sample rate, in Hertz", "0", "44100.00"));
            //mParamList.Add(new EarlabFloat("SampleRate_Hz", "Sample rate, in Hertz", 0, 44100.00));
            mParamList.Add(EarlabParameterFactory.CreateParameter("String", "InputFileName", "Name of input file to be read", "", "InputFile.dat"));
            mParamList.Add(EarlabParameterFactory.CreateParameter("Boolean", "InputIsWaveFile", "True if the input file is in .WAV format", "false", "false"));
            mParamList.Add(EarlabParameterFactory.CreateParameter("Integer", "ChannelNumber", "Channel number of the input, for WAV files only", "0", "0"));
            //mParamList.Add(EarlabParameterFactory.CreateParameter(("IntArrayParam", "This is an integer array", null, new int[] { 1, 2, 3, 4, 5, 6 }));
            //mParamList.Add(EarlabParameterFactory.CreateParameter(("FloatArrayParam", "This is an double array", null, new double[] { 1.2, 2.3, 3.4, 4.5, 5.6, Math.PI, Math.E }));
            mParamList[3].IsError = true;
            mParamList[3].ErrorText = "This is some error text!";
            propertyGrid.ParameterList = mParamList;
        }

        private void btnDone_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void btnDisplay_Click(object sender, EventArgs e)
        {
            string display = "";
            foreach (EarlabParameter param in mParamList)
                display += param.ToString() + "\r\n";
            MessageBox.Show(display, "Parameter display");
        }
    }
}