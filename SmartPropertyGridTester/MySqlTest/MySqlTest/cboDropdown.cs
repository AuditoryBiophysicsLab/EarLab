using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using MySql.Data.MySqlClient;

namespace MySqlTest
{
    public partial class cboDropdown : UserControl
    {
        MySqlConnection mDB;
        string mHostname, mUsername, mPassword;

        public cboDropdown(string Hostname, string Username, string Password)
        {
            InitializeComponent();
            mHostname = Hostname;
            mUsername = Username;
            mPassword = Password;
            ConnectToDatabase();
        }

        public cboDropdown(MySqlConnection SqlConnection)
        {
            if (SqlConnection.State != ConnectionState.Open)
            {
                SqlConnection.Open();
                if (SqlConnection.State != ConnectionState.Open)
                    throw new Exception("Supplied MySqlConnection is not connected to any database");
            }
        }

        private void ConnectToDatabase()
        {
            string ConnectString = "Database=esme_environment;Data Source=" + mHostname + ";User Id=" + mUsername + ";Password=" + mPassword;
            mDB = new MySqlConnection(ConnectString);
        }

        private void PopulateDatatype()
        {
            //datasetBindingSource.
        }
    }
}
