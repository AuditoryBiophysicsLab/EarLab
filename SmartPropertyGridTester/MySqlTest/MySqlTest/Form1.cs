using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MySqlTest
{
    public partial class frmDatabaseEditor : Form
    {
        public frmDatabaseEditor()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'esmeDataSet.environmentdataset' table. You can move, or remove it, as needed.
            this.environmentdatasetTableAdapter.Fill(this.esmeDataSet.environmentdataset);
            // TODO: This line of code loads data into the 'esmeDataSet.environmentdatatype' table. You can move, or remove it, as needed.
            this.environmentdatatypeTableAdapter.Fill(this.esmeDataSet.environmentdatatype);

        }

        private void btnEditDataTypeList_Click(object sender, EventArgs e)
        {

        }
    }
}