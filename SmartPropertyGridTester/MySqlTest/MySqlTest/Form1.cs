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
        }

        private void ConnectToDatabase(string Hostname, string Username, string Password)
        {
            string ConnectString = "Database=esme_environment;Data Source=" + Hostname + ";User Id=" + Username + ";Password=" + Password;
            dbConnection = new MySqlConnection(ConnectString);
        }

        private void PopulateDatatype()
        {
            MySqlDataAdapter dataAdapter = new MySqlDataAdapter("SELECT * FROM datatype", dbConnection);
            MySqlCommandBuilder commandBuilder = new MySqlCommandBuilder(dataAdapter);
            DataSet ds = new DataSet();
            cboDataType.Items.Ad
            foreach (DataRow curRow in ds.Tables[0].Rows)
            {
                cboDataType.Items.Add()
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            dbConnection = new MySqlConnection("Database=esme_environment;Data Source=nikon.bu.edu;User Id=da;Password=AmySophie17");

            MySqlDataAdapter dataAdapter = new MySqlDataAdapter("SELECT * FROM datatype", dbConnection);
            MySqlCommandBuilder commandBuilder = new MySqlCommandBuilder(dataAdapter);
            DataSet ds = new DataSet();
            dataAdapter.Fill(ds);
            DataRow dr = ds.Tables[0].NewRow();
            dr["Name"] = "New Type";
            ds.Tables[0].Rows.Add(dr);
            dataAdapter.Update(ds);
        }

        private void btnEditDataTypeList_Click(object sender, EventArgs e)
        {
        }

        private void cboDataType_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}