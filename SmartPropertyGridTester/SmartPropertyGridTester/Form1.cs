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
            mParamList.Add(new EarlabString("ExperimentName", "Name of the experiment", "Nothing", "myExperiment"));
            mParamList.Add(new EarlabBoolean("EnableTestMode", "Test mode enabled", false, false));
            mParamList.Add(new EarlabIntArray("IntArrayParam", "This is an integer array", null, new int[] { 1, 2, 3, 4, 5, 6 }));
            mParamList.Add(new EarlabFloatArray("FloatArrayParam", "This is an double array", null, new double[] { 1.2, 2.3, 3.4, 4.5, 5.6, Math.PI, Math.E }));
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
            MessageBox.Show(display);
        }
    }
}