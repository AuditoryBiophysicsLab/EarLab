using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using MySql.Data.MySqlClient;

namespace MySqlTest
{
    public partial class frmDatabaseEditor : Form
    {
        int curDataTypeID, curDataSetID, CurDataSubsetID;

        public frmDatabaseEditor()
        {
            InitializeComponent();
            //ConnectToDatabase("nikon.bu.edu", "da", "AmySophie17");
            //PopulateDatatype();
        }
#if false
        private void ConnectToDatabase(string Hostname, string Username, string Password)
        {
            string ConnectString = "Database=esme_environment;Data Source=" + Hostname + ";User Id=" + Username + ";Password=" + Password;
            //dbConnection = new MySqlConnection(ConnectString);
        }

        private void PopulateDatatype()
        {
            //datasetBindingSource.
        }
#endif
        private void Form1_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'esme_environmentDataSet.datasubset' table. You can move, or remove it, as needed.
            // this.datasubsetTableAdapter.Fill(this.esme_environmentDataSet.datasubset);
            // TODO: This line of code loads data into the 'esme_environmentDataSet.dataset' table. You can move, or remove it, as needed.
            // this.datasetTableAdapter.Fill(this.esme_environmentDataSet.dataset);
            // TODO: This line of code loads data into the 'esme_environmentDataSet.datatype' table. You can move, or remove it, as needed.
            this.datatypeTableAdapter.Fill(this.esme_environmentDataSet.datatype);
            this.datatypeTableAdapter.ClearBeforeFill = true;
            this.datasetTableAdapter.ClearBeforeFill = true;
            this.datasubsetTableAdapter.ClearBeforeFill = true;
        }

        private void btnEditDataTypeList_Click(object sender, EventArgs e)
        {
            frmDataType dataType = new frmDataType();
            dataType.ShowDialog();
            this.datatypeTableAdapter.Fill(this.esme_environmentDataSet.datatype);
        }

        private void cboDataType_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cboDataType.SelectedValue == null)
                return;
            curDataTypeID = int.Parse(((DataRowView)(cboDataType.SelectedValue)).Row[0].ToString());
            this.datasetTableAdapter.FillByDataType(this.esme_environmentDataSet.dataset, curDataTypeID);
            this.datasetTableAdapter.GetDataByDataType(curDataTypeID);
        }

        private void cboDataSet_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cboDataSet.SelectedValue == null)
                return;
            curDataSetID = int.Parse(((DataRowView)(cboDataSet.SelectedValue)).Row[0].ToString());
            this.datasubsetTableAdapter.FillByDataSet(this.esme_environmentDataSet.datasubset, curDataSetID);
            this.datasubsetTableAdapter.GetDataByDataSet(curDataSetID);
        }

        private void cboSubset_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cboSubset.SelectedValue == null)
                return;
            CurDataSubsetID = int.Parse(((DataRowView)(cboSubset.SelectedValue)).Row[0].ToString());
            //this.datasubsetTableAdapter.FillByDataSet(this.esme_environmentDataSet.datasubset, curDataSetID);
            //this.datasubsetTableAdapter.GetDataByDataSet(curDataSetID);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            frmDataSet dataSet = new frmDataSet();
            dataSet.ShowDialog("Salinity");
        }

    }
}