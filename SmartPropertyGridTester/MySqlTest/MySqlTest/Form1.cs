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
        MySqlConnection dbConnection;

        public frmDatabaseEditor()
        {
            InitializeComponent();
            ConnectToDatabase("nikon.bu.edu", "da", "AmySophie17");
            PopulateDatatype();
        }

        private void ConnectToDatabase(string Hostname, string Username, string Password)
        {
            string ConnectString = "Database=esme_environment;Data Source=" + Hostname + ";User Id=" + Username + ";Password=" + Password;
            dbConnection = new MySqlConnection(ConnectString);
        }

        private void PopulateDatatype()
        {
            //datasetBindingSource.
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'esme_environmentDataSet.datasubset' table. You can move, or remove it, as needed.
            //this.datasubsetTableAdapter.Fill(this.esme_environmentDataSet.datasubset);
            // TODO: This line of code loads data into the 'esme_environmentDataSet.dataset' table. You can move, or remove it, as needed.
            // this.datasetTableAdapter.Fill(this.esme_environmentDataSet.dataset);
            // TODO: This line of code loads data into the 'esme_environmentDataSet.datatype' table. You can move, or remove it, as needed.
            this.datatypeTableAdapter.Fill(this.esme_environmentDataSet.datatype);
        }

        private void btnEditDataTypeList_Click(object sender, EventArgs e)
        {
        }

        private void cboDataType_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.datasetTableAdapter.FillByDataType(this.esme_environmentDataSet.dataset, int.Parse(((DataRowView)(cboDataType.SelectedValue)).Row[0].ToString()));
        }
    }
}