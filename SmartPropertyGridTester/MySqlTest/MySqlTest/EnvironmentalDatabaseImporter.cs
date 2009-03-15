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
    public partial class EnvironmentalDatabaseImporter : Form
    {
        private MySqlConnection connection;

        public EnvironmentalDatabaseImporter()
        {
            InitializeComponent();
        }

        private void EnvironmentalDatabaseImporter_Load(object sender, EventArgs e)
        {
            connection = ConnectToDatabase("nikon.bu.edu", "esme_environment", "da", "AmySophie17");

            DataType.MySqlConnection = connection;
            DataType.Table = "DataType";
            DataType.DisplayColumnName = "Name";
            DataType.ValueColumnName = "idDataType";

            DataSet.MySqlConnection = DataType.MySqlConnection;
            DataSet.Table = "DataSet";
            DataSet.DisplayColumnName = "Name";
            DataSet.ValueColumnName = "idDataSet";
            DataSet.ForeignKeyName = "idDataType";

            DataSubset.MySqlConnection = DataSet.MySqlConnection;
            DataSubset.Table = "DataSubSet";
            DataSubset.DisplayColumnName = "Name";
            DataSubset.ValueColumnName = "idDataSubSet";
            DataSubset.ForeignKeyName = "idDataSet";

            DataType.Fill();
        }

        #region Database connections
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
        #endregion

        #region Dropdown selection changed event handlers
        private void DataType_SelectedValueChanged(object sender, EventArgs e)
        {
            DataSet.ForeignKeyValue = DataType.SelectedValue;
            DataSet.Fill();
        }

        private void DataSet_SelectedValueChanged(object sender, EventArgs e)
        {
            DataSubset.ForeignKeyValue = DataSet.SelectedValue;
            DataSubset.Fill();
        }

        private void DataSubset_SelectedValueChanged(object sender, EventArgs e)
        {

        }
        #endregion

        private void btnEditTypes_Click(object sender, EventArgs e)
        {
            frmDataType dataType = new frmDataType();
            dataType.ShowDialog();
        }

        private void btnEditSets_Click(object sender, EventArgs e)
        {
            frmDataSet dataSet = new frmDataSet(connection);
            dataSet.ShowDialog(DataType.SelectedValue);
        }
    }
}