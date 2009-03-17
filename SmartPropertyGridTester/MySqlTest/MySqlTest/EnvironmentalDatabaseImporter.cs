using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using MySql.Data.MySqlClient;
using CustomControls;

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
            DataType.DisplayColumn = "Name";
            DataType.ValueColumn = "idDataType";

            DataSet.MySqlConnection = DataType.MySqlConnection;
            DataSet.Table = "DataSet";
            DataSet.DisplayColumn = "Name";
            DataSet.ValueColumn = "idDataSet";
            DataSet.ForeignKey = "idDataType";

            DataSubset.MySqlConnection = DataSet.MySqlConnection;
            DataSubset.Table = "DataSubSet";
            DataSubset.DisplayColumn = "Name";
            DataSubset.ValueColumn = "idDataSubSet";
            DataSubset.ForeignKey = "idDataSet";

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

        #region Dropdown KeyChanged event handlers
        private void DataType_KeyChanged(object sender, KeyChangedEventArgs e)
        {
            DataSet.ForeignKeyValue = e.NewKey;
            DataSet.Fill();
        }

        private void DataSet_KeyChanged(object sender, KeyChangedEventArgs e)
        {
            DataSubset.ForeignKeyValue = e.NewKey;
            DataSubset.Fill();
        }

        private void DataSubset_KeyChanged(object sender, KeyChangedEventArgs e)
        {

        }
        #endregion

        private void btnEditTypes_Click(object sender, EventArgs e)
        {
            frmDataType dataType = new frmDataType(connection);
            dataType.ShowDialog();
        }

        private void btnEditSets_Click(object sender, EventArgs e)
        {
            frmDataSet dataSet = new frmDataSet(connection);
            dataSet.ShowDialog();
        }

        private void btnEditSubsets_Click(object sender, EventArgs e)
        {
            frmDataType2 dataType2 = new frmDataType2(connection);
            dataType2.ShowDialog();
        }


    }
}