using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using MySql.Data.MySqlClient;
using CustomControls;
using ESME.Environment;

namespace MySqlTest
{
    public partial class EnvironmentalDatabaseImporter : Form
    {
        private MySqlConnection connection;
        private ESME.Environment.Database myEnvironmentDB = new Database("nikon.bu.edu", "test_environment", "da", "AmySophie17");

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

        private void button1_Click(object sender, EventArgs e)
        {
#if false
            int PointID;
            myEnvironmentDB.AddDataType("Test");
            myEnvironmentDB.AddDataSubset("Salinity", "GDEM-V 3.0", "January");
		    myEnvironmentDB.AddDataSubset("Salinity", "GDEM-V 3.0", "February");
            myEnvironmentDB.AddDataSubset("Salinity", "GDEM-V 3.0", "March");
            myEnvironmentDB.AddDataSubset("Salinity", "GDEM-V 3.0", "April");
            myEnvironmentDB.AddDataSubset("Salinity", "GDEM-V 3.0", "May");
            myEnvironmentDB.AddDataSubset("Salinity", "GDEM-V 3.0", "June");
            myEnvironmentDB.AddDataSubset("Salinity", "GDEM-V 3.0", "July");
            myEnvironmentDB.AddDataSubset("Salinity", "GDEM-V 3.0", "August");
            myEnvironmentDB.AddDataSubset("Salinity", "GDEM-V 3.0", "September");
            myEnvironmentDB.AddDataSubset("Salinity", "GDEM-V 3.0", "October");
            myEnvironmentDB.AddDataSubset("Salinity", "GDEM-V 3.0", "November");
            myEnvironmentDB.AddDataSubset("Salinity", "GDEM-V 3.0", "December");
            PointID = myEnvironmentDB.AddDataPoint(2, 42.802101f, -70.875807f);
            myEnvironmentDB.AddDatum(PointID, 10, 100);
            myEnvironmentDB.AddDatum(PointID, 20, 90);
            myEnvironmentDB.AddDatum(PointID, 30, 80);
            myEnvironmentDB.AddDatum(PointID, 40, 70);
            myEnvironmentDB.AddDatum(PointID, 50, 60);
            myEnvironmentDB.AddDatum(PointID, 60, 50);
            myEnvironmentDB.AddDatum(PointID, 70, 40);
            myEnvironmentDB.Create("test_environment");
#else
            DataPoint dp = new DataPoint(42.802101f, -70.875807f);
            dp.AddDatum(11, 101);
            dp.AddDatum(21, 91);
            dp.AddDatum(31, 81);
            dp.AddDatum(41, 71);
            dp.AddDatum(51, 61);
            dp.AddDatum(61, 51);
            dp.AddDatum(71, 41);
            myEnvironmentDB.Add("Salinity", "GDEM-V 3.0", "January", dp);
            int mySubsetID = myEnvironmentDB.GetDataSubsetID("Salinity", "GDEM-V 3.0", "January");
            //myEnvironmentDB.AddDataPoint();
#endif
        }
    }
}