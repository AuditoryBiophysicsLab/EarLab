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
        private MySqlConnection connection;

        public frmDatabaseEditor()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            connection = ConnectToDatabase("nikon.bu.edu", "esme_environment", "da", "AmySophie17");

            cboDataType.SelectedValueChanged += new EventHandler(cboDropdown1_SelectedValueChanged);

            cboDataType.MySqlConnection = connection;
            cboDataType.Table = "DataType";
            cboDataType.DisplayColumnName = "Name";
            cboDataType.ValueColumnName = "idDataType";

            cboDataSet.MySqlConnection = connection;
            cboDataSet.Table = "DataSet";
            cboDataSet.DisplayColumnName = "Name";
            cboDataSet.ValueColumnName = "idDataSet";
            cboDataSet.ForeignKeyName = "idDataType";

            cboDataType.Fill();
        }

        private void btnEditDataTypeList_Click(object sender, EventArgs e)
        {
            frmDataType dataType = new frmDataType();
            dataType.ShowDialog();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            frmDataSet dataSet = new frmDataSet();
            dataSet.ShowDialog("Salinity");
        }

        void cboDropdown1_SelectedValueChanged(object sender, EventArgs e)
        {
            cboDataSet.ForeignKeyValue = cboDataType.SelectedValue;
            cboDataSet.Fill();
        }

        private MySqlConnection ConnectToDatabase(string Server, string Database, string Username, string Password)
        {
            return ConnectToDatabase("Server=" + Server + ";Database=" + Database + ";User Id=" + Username + ";Password=" + Password);
        }

        private MySqlConnection ConnectToDatabase(string Server, string Database, string Username, string Password, int Port)
        {
            return ConnectToDatabase("Server=" + Server + ";Database=" + Database + ";Port=" + Port + ";User Id=" + Username + ";Password=" + Password);
        }

        private MySqlConnection ConnectToDatabase(string ConnectString)
        {
            MySqlConnection connection;
            connection = new MySqlConnection(ConnectString);
            connection.Open();
            if (connection.State != ConnectionState.Open)
            {
                if (connection.State != ConnectionState.Open)
                    throw new Exception("Cannot connect to requested database with supplied credentials");
            }
            connection.Close();
            return connection;
        }
    }
}